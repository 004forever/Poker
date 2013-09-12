

#ifndef poker_h
#define poker_h
#include <iostream>
#include <fstream>
using namespace std;


class Poker
{
    private:
        struct card
        {
            int value;
            string suit;
            bool used;
        };
        card *deck;
        card *hand;
        int sum;
        int count;
        int payOff[9];
        void setHand();
        void setPayoff();
        void loadDeck();
        int calcMax();
        void getExpected(int, int);
        int getPayoff();
        void displayMove(int);
        void calcAdv();
    public:
        Poker();
        ~Poker();
        
        
};

#endif
