#ifndef ROULETTE_H_
#define ROULETTE_H_

#define BETLEN      16
#define MAXBETS     10
#define MAXNUMWIN   18


struct rBet{
    char type[BETLEN];
    int bet;
    int mult;
    int win_nums[MAXNUMWIN];
};

int rouletteMain(struct player player[], int playerNumber);
int roulMenu();
void playRoul(struct player player[], int playerNumber);
int rGetBet(struct rBet bets[], struct player player[], int playerNumber);
void insideBets(struct rBet bets[], int num_bets);
void outsideBets(struct rBet bets[], int num_bets);
int spinWheel();
void processSpin(struct rBet bets[], int win_num, int num_bets, struct player player[], int player_num);
void roulHelp();
void printRouletteLogo();
void printBoard();

#endif // ROULETTE_H_
