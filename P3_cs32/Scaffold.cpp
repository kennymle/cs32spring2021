// Scaffold.cpp

#include "provided.h"
#include <iostream>           //added
#include <list>               //added
#include <vector>             //added
using namespace std;

class ScaffoldImpl
{
public:
    ScaffoldImpl(int nColumns, int nLevels);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const;
    void display() const;
    bool makeMove(int column, int color);
    int undoMove();
private:
    vector<vector<int>> vi;  //vector of int
    list<int> undolist;
    int m_col, m_level;
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
    :m_col(nColumns), m_level(nLevels)
{
    if (m_col <= 0 || m_level <= 0) {  //checks if positive value parameters were used
        cerr << "Error" << endl;       //terminate after writing msg to cerr 
        exit(0);                       //successful termination of program
    }
    
    vi.resize(m_level);          // creates a grid which now has m_level empty rows
    for (int i = 0; i < m_level; i++)
        vi[i].resize(m_col);  // row i now has m_col columns

    // Now we have an m_level by m_col "array" of ints.
    for (int i = 0; i < m_level; i++)
        for (int j = 0; j < m_col; j++)
            vi[i][j] = VACANT;              //VACANT = -1
}

int ScaffoldImpl::cols() const
{
    return m_col;  
}

int ScaffoldImpl::levels() const
{
    return m_level;  
}

int ScaffoldImpl::numberEmpty() const
{
    int mcounter = 0;                       //counter for how many empty or vacant spots are on the grid
    for (int i = 0; i < m_level; i++)       //nested forloop to go through all variables of the grid
        for (int j = 0; j < m_col; j++)
            if (vi[i][j] == VACANT)         //if the index of the current grid is vacant, increment the counter
                mcounter++;
    return mcounter;  
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    int row = m_level - level;          //variable conversion such that (1,1) will be bottom left position of grid
    if (vi[row][column-1] == RED)       //RED = 0
        return RED;                     //if the position is 0, return RED
    if (vi[row][column-1] == BLACK)     //BLACK = 1
        return BLACK;                   //if the position is 0, return BLACK
    else return VACANT;                 //VACANT = -1
}

void ScaffoldImpl::display() const
{
    for (int i = 0; i < m_level; i++) {         //nested forloop to print out the grid
        for (int j = 0; j < m_col; j++) {
            cout << "|";                        //starts with divider "|"
            if (vi[i][j] == RED)   //RED = 0
                cout << "R";
            if (vi[i][j] == BLACK)   //BLACK = 1
                cout << "B";
            if (vi[i][j] == VACANT)  //VACANT = -1
             cout << " ";        
        }
        cout << "|" << endl;                    //last element in the row is the divider "|"
    }
    for (int i = 0; i < m_col; i++)             //last row will have "+" and "-" alternating for divider and slots respectively
        cout << "+-";
    cout << "+" << endl;
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    if (column <= 0 || column > m_col)          //numbering starts at (1,1) bottom left
        return false;                           //if column is out of bounds, return false
    for (int i = m_level; i != 0; i--) {        //going through the levels of that column
        if (vi[i - 1][column - 1] == VACANT) {  
            vi[i - 1][column - 1] = color;      //if move is valid, put a "cheker" of the color in that column
            undolist.push_front(i - 1);         //store index of this last move in a list
            undolist.push_front(column - 1);    //the first parameter, and then the second parameter are added in the list
            return true;
        }
    }
    return false;
}

int ScaffoldImpl::undoMove()
{

    if (undolist.empty())               //if list and consequently scaffold is empty, then do nothing
        return 0;
    int column = undolist.front();      //stores front value of list into column integer variable
    undolist.pop_front();               
    int level = undolist.front();       //stores next value of list into column integer variable
    undolist.pop_front();           

    vi[level][column] = VACANT;        //makes that slot at that index VACANT
    return column + 1;                  //returns the column number of the undoed action
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code. 

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
    delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}

int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
    m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
