// Luke Fallon
// 26 October 2013
// Slot

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "saveload.h"
#include "slot.h"

//
int slotMain(struct player player[], int playerNumber){

    srand(time(NULL));

    printSlotLogo();

    // Main loop of the game
    int choice = 1;
    while(choice!=3){
        // Get user input
        choice = slotMenu();

        if(choice == 1){
            // Play the slot game
            playSlots(player);
            updateSaves(player);
        }
        else if(choice == 2)
            // Look at the pay out table
            showTable();
        else if(choice == 3)
            // Quit
            printf("Thanks for playing!\n");
        else
            // Only executes if user did not choose a valid option
            printf("Sorry, that is not a valid input! Try again.\n\n");
    }
    return 0;
}
// Prints the "logo" of the Lucky Seven Slots parlor at the beginning of the game
void printSlotLogo(){
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("###                                   ###\n");
    printf("### Welcome to Lucky 7's Slot Parlor! ###\n");
    printf("###                                   ###\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("\n");
}
// Prompts the user to enter a choice from a printed menu,
// returns the integer value of the choice.
int slotMenu(){

    int choice;

    // Print the starting menu
    printf("Select an option from the menu below!\n");
    printf("1. Play some slots!\n");
    printf("2. View pay table.\n");
    printf("3. Back\n");

    // Get user input
    printf(">> ");
    scanf("%d", &choice);

    return choice;
}
// Requires a struct of type player, the main loop of the slots game.
// Generates a random payline, sees if the user won anything, and
// display the net gain at the end of the session.
void playSlots(struct player *info){

    int payline[NUMWHEELS];
    char paytable[NUMWHEELS][7];
    int i,j;
    int wager;

    while(1){

        wager = sGetBet(info);
        // If user choice to quit, exit immediately.
        if(wager == 0)
            break;
        // Pull the lever! (Generate 3 random numbers, 1 for each wheel)
        for(i = 0; i < NUMWHEELS; i++)
            payline[i] = (rand()%WHEELSIZE) + 1;

        createPayline(payline);
        createPaytable(payline, paytable);
        printPayline(paytable);
        payOut(wager, payline, info);
    }
    if(info->winnings >= info->losses)
        printf("\nIn this session you had a net gain of $%d.\n\n", info->winnings - info->losses);
    else
        printf("\nIn this session you had a net loss of $%d.\n\n", info->losses - info->winnings);
}
// Displays the user's account balance. Gets a wager from the user (1,2 or 3) and checks if valid.
int sGetBet(struct player *a){

    int wager;

    printf("\nYour current balance is: $%d.\n", a->balance);
    printf("You may bet 1, 2, or 3 dollars. The more you wager, the more you win!\n");


    int valid = 0;
    while(!valid){

        printf("How much would you like to wager? Note: To quit, enter 0.\n");
        printf(">> ");
        scanf("%d", &wager);

        if(wager == 0)
            return 0;
        // The user can not make a wager if they are out of money.
        else if(a->balance == 0){
            printf("\nLooks like you don't have enough money to make a bet!\n");
            printf("Here's some to help you out! (Balance + $500)\n");
            a->balance += 500;
        }
        // Wager cannot exceed present balance.
        else if(a->balance - wager < 0){
            printf("Uh oh! Looks like you don't have enough money for that wager!\n");
            printf("Please enter a smaller wager:\n");
        }
        // Wager cannot be more than 3 or less than 1
        else if(wager < 1 || wager > 3){
            printf("Sorry, that is not a valid wager!\n");
        }
        // If non of these conditions flag, bet is valid
        else
            valid = 1;
    }
    // Update the balance
    a->balance -= wager;
    a->losses += wager;
    return wager;
}
// Convert the random numbers into numbers 0-6 relating to symbols
void createPayline(int array[]){
    int i;
    for(i = 0; i < NUMWHEELS; i++){
        if(array[i] > 21)
            array[i] = 0;
        else if(array[i] >= 21)
            array[i] = TWOX;
        else if(array[i] >= 19)
            array[i] = RED7;
        else if(array[i] >= 16)
            array[i] = PINK7;
        else if(array[i] >= 12)
            array[i] = BLUE7;
        else if(array[i] >= 7)
            array[i] = BAR5;
        else if(array[i] >= 1)
            array[i] = BAR;
    }
}
// Creates an array with the string representationg of the
// numberic values in createPayline
void createPaytable(int array[], char payTable[][7]){
    int i;
    for(i = 0; i < NUMWHEELS; i++){
        if(array[i] == TWOX)
            strcpy(payTable[i], "  2x  ");
        else if(array[i] == RED7)
            strcpy(payTable[i], "Red  7");
        else if(array[i] == PINK7)
            strcpy(payTable[i], "Pink 7");
        else if(array[i] == BLUE7)
            strcpy(payTable[i], "Blue 7");
        else if(array[i] == BAR5)
            strcpy(payTable[i], "5 BAR ");
        else if(array[i] == BAR)
            strcpy(payTable[i], "  BAR ");
        else if(array[i] == 0)
            strcpy(payTable[i], "  --- ");
    }
}
// Prints the spin given a char array from createPaytable
void printPayline(char payTable[][7]){
    printf("\nHere's the spin:");
    printf("\n**********************************\n");
    printf("*          *          *          *\n");
    printf("* %s   *  %s  *  %s  *\n", payTable[0], payTable[1], payTable[2]);
    printf("*          *          *          *\n");
    printf("**********************************\n\n");
}
// Given the wager, player struct, and numeric representation
// of payline, calculates winnings (if any) and updates the
// balance and total winnings.
void payOut(int wager, int array[], struct player *a){

    int winnings = 0;

    if(array[0] == TWOX && array[1] == TWOX && array[2] == TWOX){
        winnings = 1200 * wager;
        printf("JACKPOT!! You won $%d!\n", winnings);
    }
    else if(array[0] == RED7 && array[1] == RED7 && array[2] == RED7){
        winnings = 200 * wager;
        printf("WOAH!! You won $%d!\n", winnings);
    }
    else if(array[0] == PINK7 && array[1] == PINK7 && array[2] == PINK7){
        winnings = 100 * wager;
        printf("WOAH!! You won $%d!\n", winnings);
    }
    else if(array[0] == BLUE7 && array[1] == BLUE7 && array[2] == BLUE7){
        winnings = 60 * wager;
        printf("WOAH!! You won $%d!\n", winnings);
    }
    else if(array[0] == BAR5 && array[1] == BAR5 && array[2] == BAR5){
        winnings = 50 * wager;
        printf("Nice! You won $%d!\n", winnings);
    }
    else if(array[0] == BAR && array[1] == BAR && array[2] == BAR){
        winnings = 10 * wager;
        printf("You won $%d!\n", winnings);
    }
    else if((array[0] == RED7 || array[0] == BLUE7 || array[0] == PINK7) && (array[1] == RED7 || array[1] == BLUE7 || array[1] == PINK7) && (array[2] == RED7 || array[2] == BLUE7 || array[2] == PINK7)){
        winnings = 30 * wager;
        printf("Nice! You won $%d!\n", winnings);
    }
    else if((array[0] == BAR5 || array[0] == BAR) && (array[1] == BAR5 || array[1] == BAR) && (array[2] == BAR5 || array[2] == BAR)){
        winnings = 5 * wager;
        printf("You won $%d!\n", winnings);
    }
    else if(array[0] == TWOX || array[1] == TWOX || array[2] == TWOX){
        winnings = 2 * wager;
        printf("You won $%d!\n", winnings);
    }
    else
        printf("Sorry, you didn't win anything on that spin!\n");
    printf("\n");
    // Add the winnings to the balance
    a->balance += winnings;
    a->winnings += winnings;
}
// Prints a visual representation of the payout for
// each winning spin combination.
void showTable(){
    printf("\n****************************************************");
    printf("\n*    Wager   *    1   *   2   *   3   *    Wager   *");
    printf("\n****************************************************");
    printf("\n*   3 of 2x  *  1,200 * 2,400 * 3,600 *   3 of 2x  *");
    printf("\n*  3 Red 7's *   200  *  400  *  600  *  3 Red 7's *");
    printf("\n* 3 Pink 7's *   100  *  200  *  300  * 3 Pink 7's *");
    printf("\n* 3 Blue 7's *    60  *  120  *  180  * 3 Blue 7's *");
    printf("\n*  3 5Bar's  *    50  *  100  *  150  *  3 5Bar's  *");
    printf("\n*  Any 3 7's *    30  *   60  *   90  *  Any 3 7's *");
    printf("\n*   3 Bar's  *    10  *   20  *   30  *   3 Bar's  *");
    printf("\n*  Any 3 Bar *     5  *   10  *   15  *  Any 3 Bar *");
    printf("\n*   1 of 2x  *     2  *    4  *    6  *   1 of 2x  *");
    printf("\n****************************************************\n\n");
}
