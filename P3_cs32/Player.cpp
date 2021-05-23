// Player.cpp

#include "provided.h"
#include "support.h"
#include <iostream>           //added
#include <list>               //added
#include <cmath>              //added
#include <vector>             //added
#include <string>


using namespace std;

int chooseBetterMove(Scaffold& s, int N, int color, int firstcolor, int currentcolumn, int depth, vector<int>& ratings, vector<int>& columnchoice);
bool completed(Scaffold* s, int& winner, int column, int N);

class HumanPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};


class BadPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int col = 0;
    while (col < 1 || col > s.cols())
    {
        cout << "Pick a column between 1 and " + to_string(s.cols()) + " that is vacant." << endl;
        cin >> col;             //waits for user to input a valid integer value
        cout << endl;
        
        if (s.checkerAt(col, s.levels()) != VACANT) {       //if the number inputted corresponds to a filled column, set col = 0
            col = 0;                                        //col = 0 will cause the program to stay in the while loop
        }
    }
    return col;
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    //fills moves left to right using a nested forloop
    for (int i = 1; i <= s.cols(); i++) {
        for (int j = 1; j <= s.levels(); j++) {
            if (s.checkerAt(i, s.levels()) == VACANT)  //from left to right column, check if there is any spots that are vacant
                return i;                         //if there is a vacant spot in that column, return that column number
        }
    }
    return 0;  //  no move is possible
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (s.numberEmpty() == 0)       //  no move is possible
        return 0;
    int depth = 1;
    int currentcolumn = 1;
    vector<int> columnchoice;
    vector<int> ratings;            //vector filled with 0, positive value, or negative value
    vector<bool> columnchecked;
    Scaffold copy(s);
    int choice = chooseBetterMove(copy, N, color, color, currentcolumn, depth, ratings, columnchoice);
    return  choice;
}


//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code. 

HumanPlayer::HumanPlayer(string nm)
    : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
    : Player(nm)
{
    m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
    delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
    : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}


////////////////////////////////////////////////////////////////////
///////additional functions used in SmartPlayer's "chooseMove"//////
////////////////////////////////////////////////////////////////////
                                                            //caller        col                            move                          checked
int chooseBetterMove(Scaffold& s, int N, int color, int firstcolor, int currentcolumn, int depth, vector<int>& ratings, vector<int>& columnchoice)
{
    if (ratings.size() < 25000) {       //time limit restraint
        
        int c = 1;
        if (color == RED)       //alternate the colors for the next recursive call on chooseBetterMove
            c = BLACK;          //such that it will be the other player's move to choose
        if (color == BLACK)     //note: RED = 0; BLACK = 1; VACANT = -1;
            c = RED;

        int winner;
        if (completed(&s, winner, currentcolumn, N)) {
            columnchoice.push_back(currentcolumn);
            if (winner == firstcolor)
                ratings.push_back(1000000000 - depth);       //pushback(positive value of respective depth)
            else if (winner == TIE_GAME)
                ratings.push_back(0);                        //pushback(0)
            else ratings.push_back(-1000000000 + depth);     //pushback(negative value of respective depth)
            //cout << "Column size: " << columnchoice.size();
            //cout << "; Rating size: " << ratings.size() << endl;
            //cout << "Column Value: " << columnchoice.at(columnchoice.size()-1);
            //cout << ";  Rating Value: " << ratings.at(ratings.size() - 1) << endl;
        }
        else {
            for (int i = 1; i <= s.cols(); i++) {   //iterate through columns
                if (s.makeMove(i, color)) {     //tries to make move in current column. If valid, enter if statement
                    //s.display();
                    chooseBetterMove(s, N, c, firstcolor, i, depth + 1, ratings, columnchoice);
                    s.undoMove();
                }
            }
        }
    }

    int indexofmax = 0;
    int indexofmin = 0;
    int max = 0;
    for (int k = 0; k < (int)ratings.size(); k++)
        if (max < ratings.at(k)) {
            max = ratings.at(k);
            indexofmax = k;             //index of greatest positive number
        }
    int min = 0;
    for (int k = 0; k < (int)ratings.size(); k++)
       if (min > ratings.at(k)) {
           min = ratings.at(k);
           indexofmin = k;              //index of greatest negative number
       }

    int index = 0;                      
    if (max >= abs(min))                //index of greatest magnitude number
        index = indexofmax;
    else index = indexofmin;

    if (depth == 1)
        return columnchoice.at(index);       //return column number if depth is 1
    return ratings.at(index);                //if depth is not 1, return the rating
    return 0;
}



bool completed(Scaffold* s, int& winner, int column, int N)     //from Game.cpp with extra parameters due to lack of private data members
{
    if (s->numberEmpty() == 0) {      //if its a filled scaffold, winner is set to tiegame
        winner = TIE_GAME;
        return true;
    }

    for (int i = 1; i <= s->levels(); i++) {   //check starting at top left; i is levels, j is columns
        for (int j = 1; j <= s->cols(); j++) {
            if (connected(s, i, j, N) == RED) {
                winner = RED;
                return true;
            }
            if (connected(s, i, j, N) == BLACK) {
                winner = BLACK;
                return true;
            }
        }
    }
    return false;       //no winner
}