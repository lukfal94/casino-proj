// Luke Fallon
// 4 December 2013
// Save Load Functions


#include <stdio.h>
#include <string.h>
#include "saveload.h"

// Input:   array of players.
//
// Output:  creates new player profile in array,
//          writes updated array to file.
//
// Returns: the index of the new player in the array.
int createNewProfile(struct player p[]){

    // Store previous profile information
    readSaves(p);

    int i;
    int open = -1;
    // Find first open slot
    for(i = 0; i < MAXPLAYERS; i++){
        if(p[i].used == 0){
            open = i;
            break;
        }
    }

    if(open == -1){
        printf("Sorry, there are no empty profile slots.\n\n");
        return -1;
    }

    // Initialize new user profile.
    printf("\nEnter your name:\n");
    scanf("%s", p[open].name);
    p[open].balance = STARTINGBAL;
    p[open].losses = 0;
    p[open].winnings = 0;
    p[open].used = 1;

    printf("\nWelcome, %s!\n", p[open].name);

    // Update save file
    updateSaves(p);

    // Return the player number
    return open;
}
// Reads the profiles stored in a txt file and asks the user
// to choose one to load and play as.
//
// Returns: Index of player in the array.
int loadProfile(struct player p[]){
    // Read in profiles from save file
    readSaves(p);

    // List profiles
    printf("\nSelect a profile from the menu below:\n");
    printf("1. %s\n", p[0].name);
    printf("2. %s\n", p[1].name);
    printf("3. %s\n", p[2].name);
    printf("4. Back\n");

    int choice;

    // Validate choice
    int valid = 0;
    while(!valid){
        printf(">> ");
        scanf("%d", &choice);
        if(choice == 4)
            return -1;
        else if((choice < 1) || (choice > 3))
            printf("That is not a valid profile. Try again.\n");
        else if(p[choice-1].used == 0)
            // Check that the profile has been initialized.
            printf("That is not a valid profile. Try again.\n");
        else
            valid = 1;
    }

    printf("\nWelcome back, %s!\n", p[choice-1].name);
    // Return the index of the players location in the array.
    return choice-1;
}
// Input:   player array
//
// Output:  resets a player struct in the array, writes to file.
int deleteProfile(struct player p[]){
    int choice;

    printf("\nSelect the profile to delete: \n");
    printf("1. %s\n", p[0].name);
    printf("2. %s\n", p[1].name);
    printf("3. %s\n", p[2].name);
    printf("4. Back\n");

    // Validate choice
    int valid = 0;
    while(!valid){
        printf(">> ");
        scanf("%d", &choice);
        if(choice == 4)
            return 0;
        else if((choice < 1) || (choice > MAXPLAYERS))
            printf("That is not a valid profile. Try again.\n");
        else if(p[choice-1].used == 0)
            // Check that the profile has been initialized.
            printf("That is not a valid profile. Try again.\n");
        else
            valid = 1;
    }

    // Reset the profile
    strcpy(p[choice-1].name, "Empty");
    p[choice-1].balance = 0;
    p[choice-1].winnings = 0;
    p[choice-1].losses = 0;
    p[choice-1].used = 0;

    // Write to file
    updateSaves(p);

    printf("Profile successfully deleted!\n");
}
// Reads contents of file to player array
void readSaves(struct player p[]){
    FILE* ifp = fopen("playerdata.txt", "r");

    int i;

    for(i = 0; i < MAXPLAYERS; i++){
        fscanf(ifp, "%s", p[i].name);
        fscanf(ifp, "%d", &(p[i].balance));
        fscanf(ifp, "%d", &(p[i].winnings));
        fscanf(ifp, "%d", &(p[i].losses));
        fscanf(ifp, "%d", &(p[i].used));
    }
    fclose(ifp);
}
// Writes contents of player array to file
void updateSaves(struct player p[]){
    int i;
    // Update save file
    FILE* ofp = fopen("playerdata.txt", "w");
    for(i=0; i < MAXPLAYERS; i++){
        fprintf(ofp, "%s\n%d\n%d\n%d\n%d\n", p[i].name, p[i].balance, p[i].winnings, p[i].losses, p[i].used);
    }
    fclose(ofp);
}

// Resets all player profiles of the game.
void resetGame(){
    printf("\nAre you sure you want to reset all profiles?\n");
    printf("1. Yes\n");
    printf("2. No\n");

    int choice;
    choice = menuInput(choice, 1, 2);
    if(choice == 1){
        FILE* ofp = fopen("playerdata.txt", "w");

        int i;
        for(i = 0; i < 3; i++){
            fprintf(ofp, "Empty\n0\n0\n0\n0\n");
        }
        fclose(ofp);
        printf("\nGame successfully reset!\n");
    }
}
// Input:   a variable to scan user input to, the menu minimum,
//          the menu maximum.
//
// Returns: the user's menu choice
int menuInput(int choice, int menuMin, int menuMax){
    int valid = 0;
    while(!valid){
        printf(">> ");
        scanf("%d", &choice);
        if(choice < menuMin || choice > menuMax)
            printf("\nSorry, that is not a valid input. Try again.\n");
        else
            valid = 1;
    }
    return choice;
}
