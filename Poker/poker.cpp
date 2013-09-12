#include "poker.h"


//This function initializes the class
Poker::Poker()
{
    int value;
    loadDeck();
    setPayoff();
    calcOdds();
    //calcAdv();
    //setHand();
    value = calcMax();
    displayMove(value);
}

void Poker::calcAdv()
{
    float expect;
    float values[] = {649740, 72193, 4165, 694, 509, 255, 47, 21, 7.69};
    expect = 0.0;
    for(int test = 0; test < 9;test++)
    {
        expect += (float) payOff[test] / values[test];
    }
    cout<<expect<<endl;
    expect = 1 - expect;
    expect *= 100;
    cout<<endl<<"House edge: "<<expect<<"%"<<endl;
}

//This function turns the number encoding for the hand into a list of moves.  
void Poker::displayMove(int move)
{
    if(move == 0)
        cout<<"Keep all"<<endl;
    else 
    {
        cout<<"Throw ";
        if(move >= 16)
        {
            move -= 16;
            cout<< "1 ";
        }
        if(move >= 8)
        {
            move -= 8;
            cout<<"2 ";
        }
        if(move >= 4)
        {
            move -=4;
            cout<<"3 ";
        }
        if(move >= 2)
        {
            move -= 2;
            cout<<"4 ";
        }
        if(move >= 1)
        {
            move -= 1;
            cout<<"5 ";
        }
        cout<<endl;
    }
}

void Poker::loadDeck()
{
    deck = new card[52];
    
	int values[13] = {2,3,4,5,6,7,8,9,10,11,12,13,14};
	string suits[4] = {"clubs", "diamonds", "spades", "hearts"};
    
	int c = 0;	
    
	for(int a = 0; a < 4; a++)
	{
		for(int b = 0; b < 13; b++)
		{
			deck[c].suit = suits[a];
			c++;
		}
	}
	
	c = 0;
    
	for(int a = 0; a < 4; a++)
	{
		for(int b = 0; b < 13; b++)
		{
			deck[c].value = values[b];
			deck[c].used = false;
			c++;
		}
	}
    
}

//This function prompts the user for the hand.  
//It sets the five cards in the hand and detects and resolves potential errors
void Poker::setHand()
{
    hand = new card[5];	
    
    for(int a = 0; a < 5; a++)
    {
        int tempn = -1;
        string temps = "blank";
            
        while(tempn < 2 || tempn > 14)
        {
            cout << endl<<"Please enter value of card #" << (a+1) << " in your hand: ";
            cin >> tempn;
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(1000);
                tempn = -1;
            }
        }
        while(temps != "spades" && temps != "hearts" && temps != "clubs" && temps != "diamonds")
        {
            cout << "Please enter suit of card #" << (a+1) << " in your hand: ";
            cin >> temps;
        }
        
        for(int x = 0; x < 52; x++)
        {
            if(deck[x].value == tempn && deck[x].suit == temps)
            {
                if(!deck[x].used)
                {
                    hand[a] = deck[x];
                    deck[x].used = true;
                }
                else {
                    a--;
                }
            }
        }
            
    }

}

//This function prompts the user for an input file that contains the pay-out table.
//These values are set to the pay-out array
void Poker::setPayoff()
{
    
	string temps;
	ifstream myfile;
    
	cout << "Please enter filename of pay-out table: ";
	cin >> temps;
    
	myfile.open(temps.c_str());
	if (myfile.is_open())
	{
        int a = 0;
        int tempn;
        while (myfile.good() && a < 9)
        {
            myfile>>tempn;
            payOff[a] = tempn;
            a++;
        }
    }
	else
	{
		cout << "Couldn't open file";
	}
	myfile.close();

}

//This function finds the best play combination and returns it. 
//It does this by checking all 32 play combinations with the expected payoff function
int Poker::calcMax()
{
    int best = -1;
    float tempValue, bestValue = (float) -1;
    sum = 0;
    count = 0;
    for(int test = 0;test < 32;test++)
    {
        cout<<"Checked "<<test<<" ";
        getExpected(test, 0);
        tempValue = (float) sum / count;
        sum = 0;
        count = 0;
        cout<<tempValue<<endl;
        if(tempValue > bestValue)
        {
            bestValue = tempValue;
            best = test;
        }
    }
    cout<<bestValue<<endl;
    return best;
}

//This function calculates the expected payoff for a play combination.
//This is a recursive function that runs all available combinations for one hand
//and calls itself to check a smaller combination(I'm writing this one)
void Poker::getExpected(int move, int start)
{
    card temp;
    int spot, value;
    
    if(move == 0)
    {
        sum += getPayoff();
        count++;
    }
    if(start != 52  && move > 0 && move < 32)
    {
        if(move >= 16)
        {
            spot = 0;
            value = 16;
        }
        else if(move >= 8)
        {
            spot = 1;
            value = 8;
        }
        else if(move >= 4)
        {
            spot = 2;
            value = 4;
        }
        else if(move >= 2)
        {
            spot = 3;
            value = 2;
        }
        else if(move >= 1)
        {
            spot = 4;
            value = 1;
        }
    
        temp = hand[spot];
        for(int test = start; test < 52;test++)
        {
            if(!deck[test].used)
            {
                hand[spot] = deck[test];
                deck[test].used = true;
                getExpected(move - value, test+1);
                deck[test].used = false;
            }
        }
        hand[spot] = temp;
    }
}
//This function determines what hand you have and returns the correct payoff
int Poker::getPayoff()
{
    bool four = false, flush, straight = false, jacks = false;
    int sets = 0, run = 0, count;
    flush = hand[0].suit == hand[1].suit && hand[0].suit == hand[2].suit && hand[0].suit == hand[3].suit && hand[0].suit == hand[4].suit;
    for(int test = 2; test < 15; test++)
    {
        count = 0;
        for(int h = 0; h < 5;h++)
        {
            if(hand[h].value == test)
                count++;
        }
        if(count == 0)
            run = 0;
        else if(count == 1)
            run++;
        else if(count == 2 && test > 10)
            jacks = true;
        else if(count == 4)
            four = true;
        if(count > 1)
            sets += count;
        if(test == 14 && run == 5 && flush)
            return payOff[0];
        if(run == 5)
            straight = true;
    }
    if(flush && straight)
        return payOff[1];
    if(four)
        return payOff[2];
    if(sets == 5)
        return payOff[3];
    if(flush)
        return payOff[4];
    if(straight)
        return payOff[5];
    if(sets == 3)
        return payOff[6];
    if(sets == 4)
        return payOff[7];
    if(sets == 2 && jacks)
        return payOff[8];
    return 0; 
        
}



//the deconstructor class
Poker::~Poker()
{
    delete[] deck;
    delete[] hand;
}