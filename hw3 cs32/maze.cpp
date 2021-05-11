//using namespace std;
//#include <iostream>


bool pathExists(char maze[][10], int sr, int sc, int er, int ec) {
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = '@';
    //can move south
    if (maze[sr + 1][sc] == '.' && pathExists(maze, sr + 1, sc, er, ec))
        return true;

    //can move west
    if (maze[sr][sc - 1] == '.' && pathExists(maze, sr, sc - 1, er, ec))
        return true;

    //can move east
    if (maze[sr][sc + 1] == '.' && pathExists(maze, sr, sc + 1, er, ec))
        return true;

    //can move north
    if (maze[sr - 1][sc] == '.' && pathExists(maze, sr - 1, sc, er, ec))
        return true;

    return false;
};
/*
int main()
{
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
}*/