#ifndef BLACKJACK_H_
#define BLACKJACK_H_

#define NUMCARDS  52
#define WIN   21

struct Card{
    char suit;
    int num;
};

int jackMain(struct player player[], int playerNumber);
void printJackLogo();
int jackMenu();
void playJack(struct player player[], int playerNumber,int bet);
int checkwin(struct Card hand[]);
int jGetBet(struct player player[], int playerNumber);
void printcard(struct Card card);
void drawCards(struct Card deck[], struct Card hand[], int num);
void dealCards(struct Card deck[], struct Card hand[], int num);
void makeDeck(struct Card deck[]);
void printHand(struct Card hand[], int player);
void makeHand(struct Card hand[], int big);

#endif // BLACKJACK_H_
