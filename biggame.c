// Luke Fallon
// 1 November 2013
// Lucky 7 Casino

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "saveload.c"
#include "slot.c"
#include "roulette.c"
#include "blackjack.c"

void gamesMenu();
void printMainLogo();
int mainMenu();
void checkStats(struct player player[], int playerNumber);

int main(){

    printMainLogo();

    // System can have a max of three saved games.
    struct player players[MAXPLAYERS];
    int playerNumber;

    // Open location where saves are written.

    while(1){

        int choice = mainMenu();
        if(choice == 1){
            // Create new profile
            playerNumber = createNewProfile(players);
            if(playerNumber == -1)
                continue;
            gamesMenu(players, playerNumber);
        }
        else if(choice == 2){
                // Load Profile
            int temp = loadProfile(players);
            if(temp == -1)
                // -1 is the flag to go back.
                continue;
            else
                playerNumber = temp;
            gamesMenu(players, playerNumber);
        }
        else if(choice == 3){
            deleteProfile(players);
        }
        else if(choice == 4){
            resetGame();
        }
        else if(choice == 5){
            printf("Thanks for playing!\n");
            updateSaves(players);
            return 0;
        }

    }

    return 0;
}
// Task:    Prints the logo.
void printMainLogo(){
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("###                                   ###\n");
    printf("###      Welcome to Lucky 7 Casino!   ###\n");
    printf("###                                   ###\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("\n");
}
// Task:    Prints the main menu and prompts user to input choice.
//
// Returns: User choice. (Int [1,5])
int mainMenu(){
    int choice;

    printf("\n//            Main Menu            \\\\\n");
    printf("Select an option from the menu below!\n");
    printf("1. New Profile\n");
    printf("2. Change/Load Profile\n");
    printf("3. Delete Profile\n");
    printf("4. Reset Game\n");
    printf("5. Quit\n");

    printf(">> ");
    scanf("%d", &choice);

    int valid = 0;
    while(!valid){
        if((choice < 1) || (choice > 5)){
            printf("Sorry! That option is invalid. Try again.\n");
            printf(">> ");
            scanf("%d", &choice);
        }
        else
            valid = 1;
    }
    return choice;
}

// Task:     Prompts the user to enter a choice from a list of games,
//           then calls the main function for that game.
//
// Returns:  the integer value of the choice.
void gamesMenu(struct player player[], int playerNumber){

    int choice;
    while(1){
    // Print the starting menu
    printf("\n//            Game Menu            \\\\\n");
    printf("Select an game from the menu below!\n");
    printf("1. Play Slots\n");
    printf("2. Play Roulette\n");
    printf("3. Play Blackjack\n");
    printf("4. Check Stats\n");
    printf("5. Back\n");

    // Get and validate choice
    int valid = 0;
    while(!valid){
        printf(">> ");
        scanf("%d", &choice);
        if((choice < 1) || (choice > 5))
            printf("That is not a valid choice. Try again.\n");
        else
            valid = 1;
    }

    if(choice == 1)
        slotMain(player, playerNumber);
    else if(choice == 2)
        rouletteMain(player, playerNumber);
    else if(choice == 3)
        jackMain(player, playerNumber);
    else if(choice == 4)
        checkStats(player, playerNumber);
    else if(choice == 5)
        break;
    }
}
// Input:   The player array, the player's number who wants to check stats
//
// Output:  Displays user profile data.
void checkStats(struct player player[], int playerNumber){

    printf("\nCurrent profile:\n");
    printf("Name: \t\t%s\n", player[playerNumber].name);
    printf("Balance: \t$%d\n", player[playerNumber].balance);
    printf("Total Winnings: $%d\n", player[playerNumber].winnings);
    printf("Total Loses: \t$%d\n", player[playerNumber].losses);

    if(player[playerNumber].winnings >= player[playerNumber].losses)
        printf("Net Profit: \t$%d\n", player[playerNumber].winnings - player[playerNumber].losses);
    else
        printf("Net Loss: \t$%d\n", player[playerNumber].losses - player[playerNumber].winnings);

}
