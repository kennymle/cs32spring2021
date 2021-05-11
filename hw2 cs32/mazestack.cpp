
#include<stack>
using namespace std;
#include <iostream>
class Coord
{
public:
    Coord(int r, int c) : m_row(r), m_col(c) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
private:
    int m_row;
    int m_col;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec) {
    stack<Coord> coordStack;
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = '@';

    while (!coordStack.empty()) {


        Coord curr = coordStack.top();  //stores r() and c() b4 poppin
        coordStack.pop();               //returns void, not value? [r][c]

        if (curr.r() == er && curr.c() == ec)
            return true;

        //can move south
        if (maze[curr.r() + 1][curr.c()] == '.') {
            coordStack.push(Coord(curr.r() + 1, curr.c()));
            maze[curr.r() + 1][curr.c()] = '@';
        }
        //can move west
        if (maze[curr.r()][curr.c() - 1] == '.') {
            coordStack.push(Coord(curr.r(), curr.c() - 1));
            maze[curr.r()][curr.c() - 1] = '@';
        }
        //can move north
        if (maze[curr.r() - 1][curr.c()] == '.') {
            coordStack.push(Coord(curr.r() - 1, curr.c()));
            maze[curr.r() - 1][curr.c()] = '@';
        }
        //can move east
        if (maze[curr.r()][curr.c() + 1] == '.') {
            coordStack.push(Coord(curr.r(), curr.c() + 1));
            maze[curr.r()][curr.c() + 1] = '@';
        }

    }

    return false;   // Return true if there is a path from (sr,sc) to (er,ec)
                    // through the maze; return false otherwise
};



int main()
{
    stack<Coord> coordStack;     // declare a stack of Coords

    Coord a(5, 6);
    coordStack.push(a);
    coordStack.push(a);          // push the coordinate (5,6)
    coordStack.push(Coord(3, 4)); // push the coordinate (3,4)
    //Coord b = coordStack.top();  // look at top item in the stack
    coordStack.pop();            // remove the top item from stack
    if (coordStack.empty())      // Is the stack empty?
        cout << "empty!" << endl;
    cout << coordStack.size() << endl;  // num of elements

    char maze[10][10] = {
        { 'X','X','X','X','X','X','X','X','X','X' },
        { 'X','.','.','.','.','X','.','.','X','X' },
        { 'X','.','X','X','.','X','X','.','.','X' },
        { 'X','X','X','.','.','.','.','X','.','X' },
        { 'X','.','X','X','X','.','X','X','X','X' },
        { 'X','.','X','.','.','.','X','.','.','X' },
        { 'X','.','.','.','X','.','X','.','.','X' },
        { 'X','X','X','X','X','.','X','.','X','X' },
        { 'X','.','.','.','.','.','.','.','.','X' },
        { 'X','X','X','X','X','X','X','X','X','X' }
    };

    if (pathExists(maze, 3, 1, 8, 8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}