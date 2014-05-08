#ifndef SLOT_H_
#define SLOT_H_

#define WHEELSIZE  30
#define NUMWHEELS   3
#define TWOX        1
#define RED7        2
#define PINK7       3
#define BLUE7       4
#define BAR5        5
#define BAR         6

int slotMain(struct player player[], int playerNumber);
void printSlotLogo();
int slotMenu();
void playSlots(struct player *a);
int sGetBet(struct player *a);
void createPayline(int array[]);
void createPaytable(int array[], char payTable[][7]);
void printPayline(char payTable[][7]);
void payOut(int wager, int array[], struct player *a);
void showTable();

#endif // SLOT_H_
