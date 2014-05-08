// Luke Fallon
// 1 November 2013
// Roulette

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "saveload.h"
#include "roulette.h"

int rouletteMain(struct player player[], int playerNumber);
int roulMenu();
void playRoul(struct player player[], int playerNumber);
void getBet(struct rBet bets[]);
void insideBets(struct rBet bets[], int num_bets);
void outsideBets(struct rBet bets[], int num_bets);
void roulHelp();
void printBoard();

// Input:   player array, current player's number in array
//
// Output:  the main loop of the roulette game. returns 0 when
//          finished.
int rouletteMain(struct player player[], int playerNumber){
    srand(time(NULL));
    printRouletteLogo();

    // Main loop of the game
    int choice = 1;
    while(choice!=4){
        // Get user input
        choice = roulMenu();

        if(choice == 1)
            // Play the slot game
            playRoul(player, playerNumber);
        else if(choice == 2)
            // Look at the pay out table
            roulHelp();
        else if(choice == 3)
            printBoard();
        else if(choice == 4)
            // Quit
            printf("Thanks for playing!\n");
        else
            // Only executes if user did not choose a valid option
            printf("Sorry, that is not a valid input! Try again.\n\n");
    }
    return 0;
}
// Input:   none
//
// Output:  Prints a menu and returns user's choice.
int roulMenu(){

    int choice;

    // Print the starting menu
    printf("\n//        Roulette Menu            \\\\\n");
    printf("Select an option from the menu below!\n");
    printf("1. Play roulette!\n");
    printf("2. How to play\n");
    printf("3. View board\n");
    printf("4. Back\n");

    // Get user input
    choice = menuInput(choice, 1, 4);

    return choice;
}
// Input:   player struct array, the current player's number.
//
// Output:  none
void playRoul(struct player player[], int playerNumber){

    // Create array to hold up to 10 bets
    struct rBet bets[MAXBETS];
    // Initialize all values to -1 (-1 will never be the result of a spin)
    int i, j;
    for(i = 0; i < MAXBETS; i++){
        for(j = 0; j < MAXNUMWIN; j++){
            bets[i].win_nums[j] = -1;
        }
    }

    printBoard();

    // Get the bets and generate random spin
    int num_bets = rGetBet(bets, player, playerNumber);
    int spinNum = spinWheel();

    processSpin(bets, spinNum, num_bets, player, playerNumber);
}
// Input:   initialized bet array, player array, current player number
//
// Output:  gets and verifies the user's desired bet, returns the number of
//          bets made.
int rGetBet(struct rBet bets[], struct player player[], int playerNumber){

    int num_bets = 0; //To keep track of the num of bets
    int flag = 0; //Flag if user wants to quit before max bets

    while(!flag && num_bets < 10){

        printf("\nWhat type of bet would you like to place?\n");
        printf("1. Inside Bet\n");
        printf("2. Outside Bet\n");

        int choice;
        int bet;
        choice = menuInput(choice, 1, 2);

        // For inside bets
        if(choice == 1){

            int valid = 0;
            while(!valid){
                // Check if player's balance is less than the minimum bet
                if(player[playerNumber].balance <= 5){
                    printf("\nLooks like you don't have enough money to make a bet!\n");
                    printf("Here's some to help you out!\n");
                    player[playerNumber].balance += 500;
                }
                printf("\nYour current balance is: $%d.\n", player[playerNumber].balance);
                printf("How much would you like to bet? (min bet: $5, max: $100)\n");
                int tempBet = menuInput(bet, 5, 100);

                // Check the bet isn't greater than the player's balance.
                if(player[playerNumber].balance - tempBet < 0){
                    printf("Sorry, you don't have enough money to make that bet.\n");
                }
                else{
                    valid = 1;
                    bets[num_bets].bet = tempBet; // Store the bet
                    player[playerNumber].balance -= tempBet;
                    insideBets(bets, num_bets); // Get the specific type of bet
                }
            }
        }
        // For outside bets
        else if(choice == 2){

            int valid = 0;
            while(!valid){
                if(player[playerNumber].balance <= 5){
                    printf("\nLooks like you don't have enough money to make a bet!\n");
                    printf("Here's some to help you out!\n");
                    player[playerNumber].balance += 500;
                }

                printf("\nYour current balance is: $%d.\n", player[playerNumber].balance);
                printf("How much would you like to bet? (min bet: $5, max: $1000)\n");
                int tempBet = menuInput(bet, 5, 1000);

                if(player[playerNumber].balance - tempBet <= 0){
                    printf("Sorry, you don't have enough money to make that bet.\n");
                }
                else{
                    valid = 1;
                    bets[num_bets].bet = tempBet; // Store the bet
                    player[playerNumber].balance -= tempBet;
                    outsideBets(bets, num_bets); // Get the specifc type of bet
                }
            }
        }

        // Print out a summary of the bet.
        printf("\nBet %d:\n", num_bets+1);
        printf("Amount: %d\n", bets[num_bets].bet);
        printf("Type: %s\n", bets[num_bets].type);
        printf("Winning Numbers: ");

        int i;
        for(i = 0; i < MAXNUMWIN; i++){
            if(bets[num_bets].win_nums[i] > -1)
                printf("%d ", bets[num_bets].win_nums[i]);
        }
        printf("\n");

        // Check the number of bets is not ten.
        num_bets++;
        if(num_bets == 10)
            flag = 1;
        else{
            int cont;
            printf("\nWould you like to place another bet?\n");
            printf("1. Yes\n");
            printf("2. No\n");
            flag = menuInput(cont, 1, 2) - 1;
        }
    }
    return num_bets;
}
// Input:   the bet array, the current bet.
//
// Output:  stores the user's bet in the bet array.
void insideBets(struct rBet bets[], int num_bets){
    /*Inside Bets
        *Single number
            35-1 (bet * 36)
        *Split(2 adjacent numbers)
            17-1 (bet * 18)
        *Street(Row)
            13-1 (bet * 14)
        *Corner(4 adj numbers)
            8-1 (bet * 9)
        *Double street
            5-1 (bet *6)
    */
    int choice;

    // Initialize the bet array to a none winning value.
    int k;
    for(k = 0; k < MAXNUMWIN; k++)
        bets[num_bets].win_nums[k] = -1;

    printf("\nWhat type of inside bet would you like to make?\n");
    printf("1. Single Number\n");
    printf("2. Split\n");
    printf("3. Street\n");
    printf("4. Corner\n");
    printf("5. Double Street\n");

    choice = menuInput(choice, 1, 5);

    // Each of the options below validate the desired bet, and store
    // the winning numbers and multiplier for that bet.

    // Bet Type: Single Number
    if(choice == 1){
        int numb;
        printf("\nEnter the number you want to bet on.\n");

        bets[num_bets].win_nums[0] = menuInput(numb, 0, 36);
        bets[num_bets].mult = 36;
        strcpy(bets[num_bets].type, "Single Num");
    }
    // Bet Type: Split
    else if(choice == 2){
        int numb1, numb2;
        printf("\nEnter the two consecutive numbers you would like to bet on:\n");

        int valid = 0;
        while(!valid){
            printf(">> ");
            scanf("%d %d", &numb1, &numb2);

            // Swap numbers so numb1 is smaller
            if(numb1 > numb2){
                int temp = numb2;
                numb1 = numb2;
                numb2 = temp;
            }
            if(numb1 % 3 == 0)
                printf("Sorry, this is not a valid split. Try again.\n");
            else if(numb2 - numb1 != 1)
                printf("Sorry, these numbers are not consecutive. Try again.\n");
            else
                valid = 1;
        }

        bets[num_bets].win_nums[0] = numb1;
        bets[num_bets].win_nums[1] = numb2;
        bets[num_bets].mult = 18;
        strcpy(bets[num_bets].type, "Split");
    }
    // Bet Type: Street
    else if(choice == 3){
        int num;
        printf("\nEnter the first number of the street (row) you would like to bet on:\n");

        int valid = 0;
        while(!valid){
            printf(">> ");
            scanf("%d", &num);
            if((num % 3 != 1) || (num > 34)){
                printf("Sorry, that is not the first number of a street. Try again.\n");
            }
            else
                valid = 1;
        }

        int i;
        for(i = 0; i < 3; i++)
            bets[num_bets].win_nums[i] = num+i;
        bets[num_bets].mult = 14;
        strcpy(bets[num_bets].type, "Street");
    }
    // Bet Type: Corner
    else if(choice == 4){
        int num;
        printf("\nEnter the number in the top left of the four numbers sharing the corners:\n");

        int valid = 0;
        while(!valid){
            printf(">> ");
            scanf("%d", &num);
            if(num % 3 == 0)
                printf("Sorry, that is not the top left part of a corner. Try again.\n");
            else if(num < 1 || num > 32)
                printf("Sorry, that is not the top left part of a corner. Try again.\n");
            else
                valid = 1;
        }

        bets[num_bets].win_nums[0] = num;
        bets[num_bets].win_nums[1] = num+1;
        bets[num_bets].win_nums[2] = num+3;
        bets[num_bets].win_nums[3] = num+4;
        bets[num_bets].mult = 6;
        strcpy(bets[num_bets].type, "Corner");
    }
    // Bet Type: Double Street
    else if(choice == 5){
        int num;
        printf("\nEnter the first number of the top street in your double street:\n");

        int valid = 0;
        while(!valid){
            printf(">> ");
            scanf("%d", &num);
            if((num % 3 != 1) || (num > 31)){
                printf("Sorry, that is not the first number of a double street. Try again.\n");
            }
            else
                valid = 1;
        }

        int i;
        for(i = 0; i < 6; i++)
            bets[num_bets].win_nums[i] = num+i;
        bets[num_bets].mult = 6;
        strcpy(bets[num_bets].type, "Double Street");
    }
}
// Input:   the bet array, the current bet.
//
// Output:  stores the user's bet in the bet array.
void outsideBets(struct rBet bets[], int num_bets){
    /*Outside Bets
        *Red or Black
            1-1 (bet * 2)
        *Odd or Even (bet * 2)
            1-1 (bet * 2)
        *1 - 18/ 19- 36
            1-1 (bet * 2)
        *Dozens
            2-1 (bet * 3)
        *Columns
            2-1 (bet *3)
    */
    int choice;
    int k;
    for(k = 0; k < MAXNUMWIN; k++)
        bets[num_bets].win_nums[k] = -1;

    printf("\nWhat type of outside bet would you like to make?\n");
    printf("1. Red or Black\n");
    printf("2. Odd or Even\n");
    printf("3. 1-18/19-36\n");
    printf("4. Dozens\n");
    printf("5. Columns\n");

    choice = menuInput(choice, 1, 5);

    // Each of the options below validate the desired bet, and store
    // the winning numbers and multiplier for that bet.

    // Bet Type: Red/Black
    if(choice == 1){
        int numb;
        printf("\nWould you like to bet on red or black? (Enter 1 for red, 2 for black)\n");

        int redWin[18] = {2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35};
        int blkWin[18] = {1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36};
        numb = menuInput(numb, 1, 2);
        if(numb == 1){
            int i;
            for(i = 0; i < 18; i++){
                bets[num_bets].win_nums[i] = redWin[i];
                printf("%d ", bets[num_bets].win_nums[i]);
            }
            strcpy(bets[num_bets].type, "Red");
        }
        else if(numb == 2){
            int i;
            for(i = 0; i < 18; i++){
                bets[num_bets].win_nums[i] = blkWin[i];
                printf("%d ", bets[num_bets].win_nums[i]);
            }
            strcpy(bets[num_bets].type, "Black");
        }
        bets[num_bets].mult = 2;
    }
    // Bet Type: Odd/Even
    else if(choice == 2){
        int numb;
        printf("\nWould you like to bet on odd or even? (Enter 1 for odd, 2 for even)\n");

        numb = menuInput(numb, 1, 2);
        // Odds
        if(numb == 1){
            int i;
            for(i = 0; i < MAXNUMWIN; i++){
                bets[num_bets].win_nums[i] = (i*2)+1;
            }
            strcpy(bets[num_bets].type, "Odds");
        }
        // Evens
        else if(numb == 2){
            int i;
            for(i = 0; i < MAXNUMWIN; i++){
                bets[num_bets].win_nums[i] = (i*2)+2;
            }
            strcpy(bets[num_bets].type, "Evens");
        }
        bets[num_bets].mult = 2;
    }
    // Bet Type: 1-18/19-36
    else if(choice == 3){
        int numb;
        printf("\nWould you like to bet on 1-18, or 19-36? (Enter 1 for 1-18, 2 for 19-36)\n");


        numb = menuInput(numb, 1, 2);
        if(numb == 1){
            int i;
            for(i = 0; i < MAXNUMWIN; i++)
                bets[num_bets].win_nums[i] = i+1;
            strcpy(bets[num_bets].type, "1-18");
        }
        else if(numb == 2){
            int i;
            for(i = 0; i < MAXNUMWIN; i++)
                bets[num_bets].win_nums[i] = i+19;
            strcpy(bets[num_bets].type, "19-36");
        }
        bets[num_bets].mult = 2;
    }
    // Bet Type: Dozens
    else if(choice == 4){
        int num;
        printf("\nWhich of the three dozens would you like to bet on? (1 for 1-12, 2 for 13-24, 3 for 25-36)\n");
        num = menuInput(num, 1,3);
        if(num == 1){
            int i;
            for(i = 0; i < 12; i++)
                bets[num_bets].win_nums[i] = i+1;
            strcpy(bets[num_bets].type, "1st Dozen");
        }
        else if(num == 2){
            int i;
            for(i = 0; i < 12; i++)
                bets[num_bets].win_nums[i] = i+13;
            strcpy(bets[num_bets].type, "2nd Dozen");
        }
        else if(num == 3){
            int i;
            for(i = 0; i < 12; i++)
                bets[num_bets].win_nums[i] = i+25;
            strcpy(bets[num_bets].type, "3rd Dozen");
        }
        bets[num_bets].mult = 3;

    }
    // Bet Type: Columns
    else if(choice == 5){
        int num;
        printf("\nWhich of the three columns would you like to bet on? (1, 2, or 3)\n");
        num = menuInput(num, 1,3);
        if(num == 1){
            int i;
            for(i = 0; i < 12; i++)
                bets[num_bets].win_nums[i] = (i*3)+num;
            strcpy(bets[num_bets].type, "1st Column");
        }
        else if(num == 2){
            int i;
            for(i = 0; i < 12; i++)
                bets[num_bets].win_nums[i] = (i*3)+num;
            strcpy(bets[num_bets].type, "2nd Column");
        }
        else if(num == 3){
            int i;
            for(i = 0; i < 12; i++)
                bets[num_bets].win_nums[i] = (i*3)+num;
            strcpy(bets[num_bets].type, "3rd Column");
        }
        bets[num_bets].mult = 3;
    }
}
// Input:   none
//
// Output:  returns a random number 0 - 36
int spinWheel(){
    int num = rand() % 37;

    printf("\nThe ball landed on %d!\n", num);

    return num;
}
// Input:   the bet array, the winning number, the number of bets placed,
//          the player array, the current player number.
//
// Output:  checks bet array for a winner, updates player's information.
void processSpin(struct rBet bets[], int win_num, int num_bets, struct player player[], int playerNumber){

    int i, j;

    int winnings = 0; // Total winnings for round
    int tBets = 0; // Total spent on bets for round
    int winner = 0; // Winner flag

    // Loops through each bet
    for(i = 0; i < num_bets; i++){
        tBets += bets[i].bet;
        // Loops through each number in the current bet
        for(j = 0; j < MAXNUMWIN; j++){
            // If the winning number is in the bet's winners
            if(bets[i].win_nums[j] == win_num){
                winner = 1;
                winnings += bets[i].bet * bets[i].mult;
                printf("\nYour '%s' bet won $%d!\n", bets[i].type, winnings);
                break;
            }
        }
    }

    if(winner == 0)
            printf("\nSorry, you didn't win this round.\n");
        else
            printf("You made $%d this round.\n", winnings - tBets);

        // Update player profile
        player[playerNumber].balance += winnings;
        player[playerNumber].winnings += winnings;
        player[playerNumber].losses += tBets;
        updateSaves(player);
}
// Output:  Prints a help dialog.
void roulHelp(){
    printf("\nSorry, help is not yet implemented.\n");
    printf("Check out this site for some useful information about roulette:\n");
    printf("http://en.wikipedia.org/wiki/Roulette\n");
}
// Output:  Prints roulette logo.
void printRouletteLogo(){
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("###                                   ###\n");
    printf("###       Garen's \"Spin to Win\"       ###\n");
    printf("###            Roulette Room          ###\n");
    printf("###                                   ###\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("\n");
}
// Output:  Prints a representation of a blackjack board.
void printBoard(){
    printf("Note: Numbers surrounded by # signs are on a\n\"black\" space. Otherwise, the number is red. \n  Ex: \"# 2 #\" is black. \"  3  \" is red.\n");
    printf("\n");
    printf("                  /            0            \\ \n");
    printf("*********************************************\n");
    printf("*  1-  * ******* *    1   | #  2 # |    3   *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("*  18  * * 1st * * #  4 # |    5   | #  6 # *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("******** * 12  * *    7   | #  8 # |    9   *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("* Even * ******* * # 10 # | # 11 # |   12   *\n");
    printf("******************--------------------------*\n");
    printf("*  Red * ******* * # 13 # |   14   | # 15 # *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("******** * 2nd * *   16   | # 17 # |   18   *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("******** * 12  * *   19   | # 20 # |   21   *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("* Black* ******* * # 22 # |   23   | # 24 # *\n");
    printf("******************--------------------------*\n");
    printf("* Odd  * ******* *   25   | # 26 # |   27   *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("******** * 3rd * * # 28 # | # 29 # |   30   *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("* 19-  * * 12  * * # 31 # |   32   | # 33 # *\n");
    printf("*      * *     * *--------------------------*\n");
    printf("*  36  * ******* *   34   | # 35 # |   36   *\n");
    printf("*********************************************\n");
    printf("//      Scroll Up to View Entire Board     //\n");
}
