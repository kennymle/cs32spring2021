// Game.cpp

#include "provided.h"
#include "support.h"
#include <iostream>           //added

using namespace std;



class GameImpl
{
public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
    bool completed(int& winner) const;
    bool takeTurn();
    void play();
    int checkerAt(int c, int r) const;
private:
    Scaffold* m_s;
    Player* redplayer;
    Player* blackplayer;
    bool redmove, blackmove;
    int m_N, m_col, m_level;
    //int m_winner;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
    :m_N(N), redmove(true), blackmove(false), redplayer(red), blackplayer(black), m_col(nColumns), m_level(nLevels)
{
    //member initialization list for all the variables that are inputted in parameters accordingly
    m_s = new Scaffold(nColumns, nLevels);  //creates a new scaffold with the given columns and levels
}

bool GameImpl::completed(int& winner) const
{
    for (int i = 1; i <= m_s->levels(); i++) {   //check starting at top left; i is levels, j is columns
        for (int j = 1; j <= m_s->cols(); j++) {
            if (connected(m_s, i, j, m_N) == RED) {
                winner = RED;
                delete m_s;                                         //deletes the allocated grid once game over
                return true;
            }
            if (connected(m_s, i, j, m_N) == BLACK) {
                winner = BLACK;
                delete m_s;                                         //deletes the allocated grid once game over
                return true;
            }
        }
    }

    if (m_s->numberEmpty() == 0) {      //if its a filled scaffold, winner is set to tiegame
        winner = TIE_GAME;
        delete m_s;                                         //deletes the allocated grid once game over
        return true;                   
    }

    return false;       //the game must go on
}

bool GameImpl::takeTurn()
{
    if (m_s->numberEmpty() != 0) {
        if (redmove) {      //if its red's turn to move, make move
            m_s->makeMove(redplayer->chooseMove(*m_s, m_N, RED), RED);  //make move takes parameter (column,color)
            redmove = false; blackmove = true;
            m_s->display();
            return true;
        }
        if (blackmove) {      //if its black's turn to move, make move
            m_s->makeMove(blackplayer->chooseMove(*m_s, m_N, BLACK), BLACK);  //make move takes parameter (column,color)
            redmove = true; blackmove = false;
            m_s->display();
            cout << "Press Enter to continue." << endl;
            cin.ignore(256, '\n');
            return true;
        }
    }
        return false;  //  if game is over return false
}

void GameImpl::play()
{
    int m_winner;
    cout << "Press Enter to continue." << endl;
    cin.ignore(256, '\n');
    m_s->display();

    while (!completed(m_winner)) {
        takeTurn();
    }


    if (m_winner == RED)                                //if m_winner == 0
        cout << redplayer->name() << " won!" << endl;
    if (m_winner == BLACK)                              //if m_winner == 1
        cout << blackplayer->name() << " won!" << endl;
    if (m_winner == TIE_GAME)                           //if m_winner == -1
        cout << "Tie!" << endl;
}

int GameImpl::checkerAt(int c, int r) const
{
    return m_s->checkerAt(c, r);
}

//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code. 

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}

Game::~Game()
{
    delete m_impl;
}

bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}

int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}