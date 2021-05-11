
#include <queue>
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
    queue<Coord> coordQueue;
    coordQueue.push(Coord(sr, sc));
    maze[sr][sc] = '@';

    while (!coordQueue.empty()) {


        Coord curr = coordQueue.front();  //stores r() and c() b4 poppin
        coordQueue.pop();               //returns void, not value? [r][c]

        if (curr.r() == er && curr.c() == ec)
            return true;

        //can move south
        if (maze[curr.r() + 1][curr.c()] == '.') {
            coordQueue.push(Coord(curr.r() + 1, curr.c()));
            maze[curr.r() + 1][curr.c()] = '@';
        }
        //can move west
        if (maze[curr.r()][curr.c() - 1] == '.') {
            coordQueue.push(Coord(curr.r(), curr.c() - 1));
            maze[curr.r()][curr.c() - 1] = '@';
        }
        //can move north
        if (maze[curr.r() - 1][curr.c()] == '.') {
            coordQueue.push(Coord(curr.r() - 1, curr.c()));
            maze[curr.r() - 1][curr.c()] = '@';
        }
        //can move east
        if (maze[curr.r()][curr.c() + 1] == '.') {
            coordQueue.push(Coord(curr.r(), curr.c() + 1));
            maze[curr.r()][curr.c() + 1] = '@';
        }

    }

    return false;   // Return true if there is a path from (sr,sc) to (er,ec)
                    // through the maze; return false otherwise
};




int main()
{
    queue<Coord> coordQueue;    // declare a queue of Coords

    Coord a(5, 6);
    coordQueue.push(a);            // enqueue item at back of queue
    coordQueue.push(Coord(3, 4));   // enqueue item at back of queue
        //Coord b = coordQueue.front();  // look at front item
    coordQueue.pop();              // remove the front item from queue
    if (coordQueue.empty())        // Is the queue empty?
        cout << "empty!" << endl;
    cout << coordQueue.size() << endl;  // num of elements

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
