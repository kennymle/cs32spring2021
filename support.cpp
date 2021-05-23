////////////////////////////////////////////////////////////////////
//////////////additional function used in "completed"///////////////
////////////////////////////////////////////////////////////////////
#include "support.h"

int connected(Scaffold* s, int level, int col, int N) {
    //checks if there is a connection for N in a row horizontally or orthogonally
    int rowlimit = s->levels() - level + 1;     //limit variables to ensure the function doesn't check out of bounds of vector
    int columnlimit = s->cols() - col + 1;

    //check connection in North direction           //should check in both directions north & south
    int redcounter = 0, blackcounter = 0;
    for (int i = 0; i < N; i++) {
        if (N <= rowlimit && s->checkerAt(col, level + i) == RED)
            redcounter++;
        if (N <= rowlimit && s->checkerAt(col, level + i) == BLACK)
            blackcounter++;
    }
    if (redcounter == N)
        return RED;
    if (blackcounter == N)
        return BLACK;

    //check connection in South direction
    redcounter = 0, blackcounter = 0;
    for (int i = 0; i < N; i++) {
        if (N <= level && s->checkerAt(col, level - i) == RED)
            redcounter++;
        if (N <= level && s->checkerAt(col, level - i) == BLACK)
            blackcounter++;
    }
    if (redcounter == N)
        return RED;
    if (blackcounter == N)
        return BLACK;

    //check connection in East direction
    redcounter = 0, blackcounter = 0;
    for (int i = 0; i < N; i++) {
        if (N <= columnlimit && s->checkerAt(col + i, level) == RED)
            redcounter++;
        if (N <= columnlimit && s->checkerAt(col + i, level) == BLACK)
            blackcounter++;
    }
    if (redcounter == N)
        return RED;
    if (blackcounter == N)
        return BLACK;

    //check connection in West direction
    redcounter = 0, blackcounter = 0;
    for (int i = 0; i < N; i++) {
        if (N <= col && s->checkerAt(col - i, level) == RED)
            redcounter++;
        if (N <= col && s->checkerAt(col - i, level) == BLACK)
            blackcounter++;
    }
    if (redcounter == N)
        return RED;
    if (blackcounter == N)
        return BLACK;

    //check connection in NorthEast direction
    redcounter = 0, blackcounter = 0;
    for (int i = 0; i < N; i++) {
        if (N <= rowlimit && N <= columnlimit && s->checkerAt(col + i, level + i) == RED)
            redcounter++;
        if (N <= rowlimit && N <= columnlimit && s->checkerAt(col + i, level + i) == BLACK)
            blackcounter++;
    }
    if (redcounter == N)
        return RED;
    if (blackcounter == N)
        return BLACK;

    //check connection in NorthWest direction
    redcounter = 0, blackcounter = 0;
    for (int i = 0; i < N; i++) {
        if (N <= rowlimit && N <= col && s->checkerAt(col - i, level + i) == RED)
            redcounter++;
        if (N <= rowlimit && N <= col && s->checkerAt(col - i, level + i) == BLACK)
            blackcounter++;
    }
    if (redcounter == N)
        return RED;
    if (blackcounter == N)
        return BLACK;

    //check connection in SouthEast direction
    redcounter = 0, blackcounter = 0;
    for (int i = 0; i < N; i++) {
        if (N <= level && N <= columnlimit && s->checkerAt(col + i, level - i) == RED)
            redcounter++;
        if (N <= level && N <= columnlimit && s->checkerAt(col + i, level - i) == BLACK)
            blackcounter++;
    }
    if (redcounter == N)
        return RED;
    if (blackcounter == N)
        return BLACK;

    //check connection in SouthWest direction
    redcounter = 0, blackcounter = 0;
    for (int i = 0; i < N; i++) {
        if (N <= level && N <= col && s->checkerAt(col - i, level - i) == RED)
            redcounter++;
        if (N <= level && N <= col && s->checkerAt(col - i, level - i) == BLACK)
            blackcounter++;
    }
    if (redcounter == N)
        return RED;
    if (blackcounter == N)
        return BLACK;

    return VACANT;              //no connections 
}