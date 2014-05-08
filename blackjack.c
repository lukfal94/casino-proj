// John Barber
// 26 October 2013
// Black Jack

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "saveload.h"
#include "blackjack.h"


int jackMain(struct player player[], int playerNumber){

    //Prints the logo and makes sure that every runthrough will have different random numbers
    srand(time(NULL));
    printJackLogo();
    int bet = 0;

    // Main loop of the game
    int choice = 1;
    while(choice!=3){
        // Get user input
        choice = jackMenu();
        if(choice == 1)
        {
            // Gets the users bet, then plays the game
            bet = jGetBet(player, playerNumber);
            if(bet > 0)
                playJack(player, playerNumber, bet);
        }

        else if(choice == 2)
            // Look at your current winnings
            printf("\nYour current balance is %d.  You have won %d dollars so far. \n",  player[playerNumber].balance,  player[playerNumber].winnings);

        else if(choice == 3)
            // Quit
            printf("Thanks for playing!\n");

        else
            // Only executes if user did not choose a valid option
            printf("Sorry, that is not a valid input! Try again.\n\n");
    }

    return 0;

}

// Prints the "logo" of the blackjack place at the beginning of the game
void printJackLogo(){
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("###                                   ###\n");
    printf("###  Tricky Dick's Blackjack Tables   ###\n");
    printf("###                                   ###\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("\n");
}

// Prompts the user to enter a choice from a printed menu,
// returns the integer value of their choice.
int jackMenu(){

    int choice;

    // Print the starting menu
    printf("\n//         Blackjack Menu          \\\\\n");
    printf("Select an option from the menu below!\n");
    printf("1. Play some blackjack!\n");
    printf("2. Check your current balance.\n");
    printf("3. Quit\n");

    // Get user input

    printf(">> ");
    scanf("%d", &choice);

    return choice;
}


// Requires a struct of type player, the main loop of the Blackjack game.
// Generates a random array of cards, deals the player the first 2 and the dealer the second 2, asks the player if he wants more cards.  If he does, it gives them to him and checks for a break, constantly repeating this loop.
// Repeats the same with a house hand, constantly hitting until 17 is reached
//compares the two values

void playJack(struct player player[], int playerNumber, int bet){

    int winnings = bet;
    int winnertemp = 0;

    //All Deck and Hand setup is here
    struct Card deck[52];
    makeDeck(deck);
    struct Card yourHand[10];
    struct Card houseHand[10];
    makeHand(yourHand,10);
    makeHand(houseHand,10);
    dealCards(deck,yourHand,2);
    dealCards(deck,houseHand,2);
    printHand(yourHand,1);
    printHand(houseHand,0);

    int choice = 0;
    int winner = checkwin(yourHand);

    //Quickly checks for a blackjack, which indicates an instant win
    if(winner == 21)
    {
        printf("Blackjack!  You win 2.5 times your bet!\n");
        winnings = bet * 2.5;
        choice = -1;
    }

    //If no blackjack, starts the main loop for the hits
    while(choice == 0)
    {
        //gets user input
        printf("Would you like to take a hit? \n1)Take Hit\n2)Stand\n>>");
        scanf("%d", &choice);

        //Draws the user another card, calculates total, and if it is over 21, ends turn.
        if(choice == 1)
        {
            drawCards(deck, yourHand, 1);
            printHand(yourHand,1);
            winner = checkwin(yourHand);
            if(winner > 21)
            {
                printf("You broke...\n");
                choice = -1;
                winnings = 0;
            }
            else
                choice = 0;
        }

    }

    winner = checkwin(houseHand);
    printHand(yourHand,1);

    //This does the exact same thing as above, but for the dealers hand.  It's a bit simpler since instead of taking user input
    //It just goes up to 21
    if(choice > -1)
    {
        while(winner < 17)
        {
            drawCards(deck, houseHand,1);
            winner = checkwin(houseHand);
        }

        if(winner > 21)
        {
            printHand(houseHand,-1);
            printf("The Dealer Broke! \n");
            winnings = bet * 2;
        }
        //Once the house's hand is greater than 21, it either kills it (option right above) or does this
        //Compares the two values, and calculates winnings based on who's is greater
        else
        {
            printHand(houseHand,-1);
            winner = checkwin(houseHand);
            winnertemp = checkwin(yourHand);

            if(winner < winnertemp)
            {
                printf("You won! \n");
                winnings = bet * 2;
            }

            else if(winner > winnertemp)
            {
                printf("You lost...\n");
                winnings = 0;
            }

            else if(winner == winnertemp)
            {
                printf("You tied.\n");
                winnings = bet;
            }

        }

    }
    //Prints out winning data, and changes the players winnings to reflect the new score
    printf("You made %d in the game.\n\n", winnings);
    player[playerNumber].balance += winnings;
    player[playerNumber].winnings += winnings;
}

//This simply finds the "blackjack value" of a set of cards.  This means the total sum value of the cards, with face cards equaling 10 and aces equaling 11
//Unless the value is over 21, in which case aces equal 1
int checkwin(struct Card hand[])
{
    int i = 0;
    int x, y;
    int tempHand[10];
    //This stores the values of the cards in a simple int array, making them easier to work with.
    //It uses some simple if statements to turn face cards into 10s and aces into 11s
    for(i = 0; i < 10; i ++)
    {
        x = hand[i].num;
        if(x > 10)
            x = 10;
        if(x == 1)
            x = 11;
        tempHand[i] = x;
    }

    int total = 0;
    i = 0;

    //This loop adds up the array, before dealing with the aces  It will keep repeating so long as both the sum is over 21 and there are aces in the hand
    while(i == 0)
    {
        for(i = 0; i < 10; i ++)
        {
            total = total + tempHand[i];
        }

        //If the total value is over 21, this checks if there are any aces.  If there are, than it takes said ace, turns it into a 1, and then restarts the whole loop
        //This way, every ace will be checked against the 21 value, and you could potentially have 2 aces, one equaling 11 and one equalling 1
        //if it checks more than 7 cards and finds that there are no aces, though, it gives up and returns the value
        if(total > 21)
        {
            y = 0;
            i = -1;
            while(y == 0)
            {
                i ++;
                if(i > 8)
                {
                    y = 1;
                }
                if(tempHand[i] == 11)
                {
                    tempHand[i] = 1;
                    y = 1;
                    i = 0;
                }
            }
        }

    }

    total = 0;
    //It has to recalculate the value to account for any aces that were changed.
    for(i = 0; i < 10; i ++)
    {
        total = total + tempHand[i];
    }

    //By simply returning the added total, I can now just used an integer to compare instead of having to make that it's own method
    return total;
}


// Simple function: asks the user for input, checks if it is valid, then returns it
int jGetBet(struct player player[], int playerNumber){

    int wager;

    //user input
    printf("\nYour current balance is: $%d .\n", player[playerNumber].balance);
    printf("How much would you like to wager?  You can win up to 150%% of what you wager!\n");
    printf("Note: To quit, enter 0.\n");
    printf(">> ");

    scanf("%d", &wager);

    //quit/escape option
    if(wager == 0)
        return 0;

    int valid = 0;

    //checks a few failure conditions
    while(!valid){
        // The user can not make a wager if they are out of money.
        if(player[playerNumber].balance == 0){
            printf("Uh oh! Looks like you're out of money!\n");
            break; // Will enter a getMoreChips() function
            printf(">> ");
            scanf("%d", &wager);
        }
        // Wager cannot exceed present balance.
        else if(player[playerNumber].balance - wager < 0){
            printf("Uh oh! Looks like you don't have enough money for that wager!\n");
            printf("Please enter a smaller wager:\n");
            printf(">> ");
            scanf("%d", &wager);
        }
        //Wager can not be negative
        else if(wager < 0)
        {
            printf("You can't wager a negative wager, silly");
            printf("Please enter a positive wager:\n");
            printf(">> ");
            scanf("%d", &wager);
        }
        // If non of these conditions flag, bet is valid
        else
            valid = 1;
    }
    // Update the balance
    player[playerNumber].balance -= wager;
    player[playerNumber].losses += wager;
    return wager;
}


//This function makes the deck of cards by taking an empty array and filling it up in a random order.
void makeDeck(struct Card deck[])
{

    //DOUBLE check that everything is random
    srand(time(NULL));

    int i,x,y,z;

    //fills the array with zeros, so that the test condition to see if a spot in the deck is full later will work
    for(i = 0; i <= 51; i ++)
    {
        deck[i].num = 0;
        x = deck[i].num;
    }

    //This fills each card in the deck with a value (1-13) and suit (S,C,D,H).  It does so by going through each card in order and choosing a random spot in the array to put it
    for(i = 0; i <=51; i ++)
    {

        //Tries to insert a card at spot Y; if y is full (does not equal zero, as outlined above) adds one to Y; if Y reaches the end (52) resets Y to zero
        y = rand()%NUMCARDS;
        while(deck[y].num > 0)

        {
            y ++;
            if(y == 52)
                y = 0;
        }

        //Simply assigns value by the number divisible by four (incrementing once every four cards and suit by the modulus 4 (incrementing to 4 and then reseting each time)
        deck[y].num = (i / 4) + 1;
        x = i % 4;

        if (x == 0)
            deck[y].suit = 'S';
        else if (x == 1)
            deck[y].suit = 'H';
        else if (x == 2)
            deck[y].suit = 'C';
        else if (x == 3)
            deck[y].suit = 'D';
    }
}

//This takes the top cards out of the deck and places them in a different card array, changing the cards in the card array to 0 (non-existent)
void dealCards(struct Card deck[], struct Card hand[], int num)
{

    int x = 0;
    int i = -1;

    //This finds the first open card in the deck.  If 3 cards have been dealt, then it will increase I to 3 so that new cards are always dealt
    while(x == 0)
    {
        i ++;
        x = deck[i].num;
    }

    //Adds the correct number of cards to the suit, then changes the value of those cards in the deck to 0 so that they are not drawn again
    for(x = 0; x < num; x ++)
    {
        hand[x].num = deck[i+x].num;
        hand[x].suit = deck[i+x].suit;
        deck[i+x].num = 0;
        deck[i+x].suit = 'n';
    }
}

//This function won't be commented because it is nearly identical to the array above, except in starts adding cards to the hand at the first available space, instead of simple at the top.
void drawCards(struct Card deck[], struct Card hand[], int num)
{
    int x = 0;
    int ideck = -1;
    int ihand = -1;
    while(x == 0)
    {
        ideck ++;
        x = deck[ideck].num;
    }
    x = 1;

    while(x > 0)
    {
        ihand ++;
        x = hand[ihand].num;
    }

    for(x = 0; x < num; x ++)
    {
        hand[ihand+x].num = deck[ideck+x].num;
        hand[ihand+x].suit = deck[ideck+x].suit;
        deck[ideck+x].num = 0;
        deck[ideck+x].suit = 'n';
    }
}

// Prints out a cards value.  Just a series of if statements that translate values to Card values (1 - Ace, 11 - Jack, etc.)
// Also makes them look pretty.
void printcard(struct Card card)
{
    int x = card.num;
    char c = card.suit;
    if(x==1)
    {
        printf("   Ace-");
    }
    else if(x<10)
    {
        printf("     %d-", x);
    }
    else if(x==10)
    {
        printf("    %d-", x);
    }
    else if(x==11)
        printf("  Jack-");
    else if(x==12)
        printf(" Queen-");
    else if(x==13)
        printf("  King-");
    else
        printf("- %d -", x);
    printf("%c", c);
}


//This array prints a players hand.  It's rather simple; it prints all cards up into a 0 value after printing a description of the persons hand, determined by a variable input
//If a special input is given (-1), the program prints the house's early hand, skipping over the first card and printing "FACEDOWN" instead.
void printHand(struct Card hand[], int player)
{
    int cardNum = 0;
    if(player == 0)
    {
        printf("The House's hand is:\n");
        printf("| FACEDOWN |\n");
        cardNum = 1;
    }
    else if(player == -1)
    {
        printf("The House's hand was:\n");
    }
    else
    {
        printf("Player %d's hand was:\n",player);
    }
    while(cardNum >= 0)
    {
        printf("| ");
        printcard(hand[cardNum]);
        printf(" |\n");
        cardNum ++;
        if(hand[cardNum].num == 0)
            cardNum = -1;
    }
    printf("------------------\n");

}

//Takes an array of cards the size given, and resets all values to 0 (no card).  Since I need all hands to start empty,
//I added this function as a reminder
void makeHand(struct Card hand[], int big)
{
    int i = 0;
    for(i = 0; i < big; i ++)
    {
        hand[i].num = 0;
        hand[i].suit = 'n';
    }
}
