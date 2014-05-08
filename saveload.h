#ifndef SAVELOAD_H_
#define SAVELOAD_H_

#define MAXPLAYERS  3
#define STARTINGBAL 500

struct player{
    char name[20];
    int balance;
    int winnings;
    int losses;
    int used;
};

int createNewProfile(struct player p[]);
int loadProfile(struct player p[]);
int deleteProfile(struct player p[]);
void readSaves(struct player p[]);
void updateSaves(struct player p[]);
void resetGame();
int menuInput(int choice, int menuMin, int menuMax);

#endif // SAVELOAD_H_
