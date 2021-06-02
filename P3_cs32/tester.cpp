// tester.cpp

#include "provided.h"
#include <iostream>
#include <sstream>
#include <streambuf>
#include <cstdlib>
#include <cassert>
#include <string>
using namespace std;

class StreambufSwitcher
{
public:
    StreambufSwitcher(ios& dest, streambuf* sb,
        ios::iostate exceptions = ios::goodbit)
        : dest_stream(dest), oldsb(dest.rdbuf(sb)), oldex(dest.exceptions())
    {
        dest_stream.exceptions(exceptions);
    }
    StreambufSwitcher(ios& dest, ios& src)
        : StreambufSwitcher(dest, src.rdbuf(), src.exceptions())
    {}
    ~StreambufSwitcher()
    {
        dest_stream.rdbuf(oldsb); dest_stream.exceptions(oldex);
    }
private:
    ios& dest_stream;
    streambuf* oldsb;
    ios::iostate oldex;
};

class ForcedPlayer : public Player
{
public:
    ForcedPlayer(string name, string moves)
        : Player(name), m_moves(moves)
    {}
    int chooseMove(const Scaffold& s, int N, int color)
    {
        if (s.numberEmpty() == 0)
            return -1;
        int nfilled = s.levels() * s.cols() - s.numberEmpty();
        int pos = nfilled / 2;
        if (pos >= m_moves.size())
            return -1;
        return m_moves[pos] - '0';
    }
private:
    string m_moves;
};

void testone(int n)
{
    StreambufSwitcher ssgo(cout, cerr);
    istringstream issnl("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    StreambufSwitcher ssgi(cin, issnl);

    switch (n)
    {
    default: {
        cout << "Bad argument" << endl;

        // Scaffold tests

    } break; case  1: {
        Scaffold s(10, 7);
        assert(s.cols() == 10 && s.levels() == 7 &&
            s.numberEmpty() == s.cols() * s.levels());
    } break; case  2: {
        Scaffold s(3, 2);
        ostringstream oss;
        {
            StreambufSwitcher sso(cout, oss);
            s.display();
        }
        string res = oss.str();
        if (!res.empty() && res[0] == '\n')
            res.erase(0, 1);
        assert(res == "| | | |\n| | | |\n+-+-+-+\n");
    } break; case  3: {
        Scaffold s(3, 2);
        assert(!s.makeMove(0, RED) && !s.makeMove(4, RED));
    } break; case  4: {
        Scaffold s(3, 2);
        assert(!s.makeMove(1, VACANT) && !s.makeMove(1, 42));
        assert(s.checkerAt(1, 1) == VACANT && s.checkerAt(1, 2) == VACANT);
        assert(s.numberEmpty() == 6);
    } break; case  5: {
        Scaffold s(3, 2);
        assert(s.makeMove(1, RED));
    } break; case  6: {
        Scaffold s(3, 2);
        s.makeMove(1, RED);
        assert(s.checkerAt(1, 1) == RED && s.checkerAt(1, 2) == VACANT);
    } break; case  7: {
        Scaffold s(3, 2);
        s.makeMove(1, RED);
        assert(s.numberEmpty() == 5);
    } break; case  8: {
        Scaffold s(3, 2);
        s.makeMove(1, RED);
        assert(s.makeMove(1, BLACK));
        assert(s.checkerAt(1, 1) == RED && s.checkerAt(1, 2) == BLACK);
    } break; case  9: {
        Scaffold s(3, 2);
        s.makeMove(1, RED);
        s.makeMove(1, BLACK);
        assert(!s.makeMove(1, RED));
    } break; case 10: {
        Scaffold s(3, 2);
        s.makeMove(1, RED);
        s.makeMove(1, BLACK);
        s.makeMove(2, RED);
        s.makeMove(2, BLACK);
        s.makeMove(3, RED);
        s.makeMove(3, BLACK);
        assert(s.numberEmpty() == 0);
        assert(s.checkerAt(0, 1) == VACANT && s.checkerAt(4, 1) == VACANT);
        assert(s.checkerAt(1, 0) == VACANT && s.checkerAt(1, 3) == VACANT);
    } break; case 11: {
        Scaffold s(3, 2);
        assert(s.undoMove() == 0 && s.numberEmpty() == 6);
    } break; case 12: {
        Scaffold s(10, 3);
        s.makeMove(7, RED);
        assert(s.numberEmpty() == 29);
        assert(s.undoMove() == 7 && s.undoMove() == 0);
        assert(s.numberEmpty() == 30);
    } break; case 13: {
        Scaffold s(10, 3);
        s.makeMove(4, RED);
        s.makeMove(4, BLACK);
        s.makeMove(4, RED);
        assert(s.numberEmpty() == 27 && s.checkerAt(4, 3) == RED);
        assert(s.undoMove() == 4 && s.numberEmpty() == 28);
        assert(s.checkerAt(4, 3) == VACANT && s.checkerAt(4, 2) == BLACK);
        assert(s.undoMove() == 4 && s.numberEmpty() == 29);
        assert(s.checkerAt(4, 2) == VACANT && s.checkerAt(4, 1) == RED);
        assert(s.undoMove() == 4 && s.numberEmpty() == 30);
        assert(s.checkerAt(4, 1) == VACANT);
    } break; case 14: {
        Scaffold s(10, 3);
        assert(s.makeMove(3, RED));
        assert(s.makeMove(8, BLACK));
        assert(s.makeMove(7, RED));
        assert(s.makeMove(3, BLACK));
        assert(s.makeMove(7, RED));
        assert(s.makeMove(7, BLACK));
        assert(s.makeMove(3, RED) && s.checkerAt(3, 3) == RED);
        s.undoMove();
        assert(s.checkerAt(3, 3) == VACANT);
        s.undoMove();
        assert(s.checkerAt(7, 3) == VACANT);
        s.undoMove();
        assert(s.checkerAt(7, 2) == VACANT);
        assert(s.makeMove(7, RED) && s.checkerAt(7, 2) == RED);
        assert(s.makeMove(3, BLACK) && s.checkerAt(3, 3) == BLACK);
        assert(s.makeMove(7, RED) && s.checkerAt(7, 3) == RED);
    } break; case 15: {
        //               R  B  R  B  R  B  R  B  R  B  R  B  R  B  R
        int cols[15] = { 3, 2, 2, 3, 2, 1, 2, 3, 1, 1, 2, 3, 1, 3, 1 };
        Scaffold s(3, 5);
        assert(s.numberEmpty() == 15);
        for (int k = 0; k < 15; k++)
            assert(s.makeMove(cols[k], (k % 2 == 0 ? RED : BLACK)));
        assert(s.numberEmpty() == 0);
        assert(s.checkerAt(1, 5) == RED && s.checkerAt(2, 5) == RED);
        assert(s.checkerAt(3, 5) == BLACK);
        for (int k = 15 - 1; k >= 0; k--)
            assert(s.undoMove() == cols[k]);
    } break; case 16: {
        Scaffold s(3, 2);
        s.makeMove(1, RED);
        s.makeMove(2, BLACK);
        s.makeMove(2, RED);
        ostringstream oss;
        {
            StreambufSwitcher sso(cout, oss);
            s.display();
        }
        string res = oss.str();
        size_t pos = res.find('|');
        assert(pos != string::npos);
        res.erase(0, pos);
        pos = res.find_last_not_of('\n');
        res.erase(pos + 1);
        assert(res == "| |R| |\n|R|B| |\n+-+-+-+");

        // Game tests

    } break; case 17: {
        ForcedPlayer p1("P1", "12");
        ForcedPlayer p2("P2", "3");
        Game g(3, 1, 2, &p1, &p2);
        int w = 999;
        assert(!g.completed(w) && w == 999);
    } break; case 18: {
        ForcedPlayer p1("P1", "21");
        ForcedPlayer p2("P2", "3");
        Game g(3, 1, 2, &p1, &p2);
        assert(g.takeTurn());
        assert(g.checkerAt(2, 1) == RED);
    } break; case 19: {
        ForcedPlayer p1("P1", "21");
        ForcedPlayer p2("P2", "3");
        Game g(3, 1, 2, &p1, &p2);
        g.takeTurn();
        assert(g.takeTurn());
        int w = 999;
        assert(!g.completed(w) && g.checkerAt(3, 1) == BLACK);
    } break; case 20: {
        ForcedPlayer p1("P1", "21");
        ForcedPlayer p2("P2", "3");
        Game g(3, 1, 2, &p1, &p2);
        g.takeTurn();
        g.takeTurn();
        assert(g.takeTurn());
        int w = 999;
        assert(g.completed(w) && w == RED);
    } break; case 21: {
        ForcedPlayer p1("P1", "13");
        ForcedPlayer p2("P2", "2");
        Game g(3, 1, 2, &p1, &p2);
        g.takeTurn();
        g.takeTurn();
        g.takeTurn();
        assert(!g.takeTurn());
        int w;
        assert(g.completed(w) && w == TIE_GAME);
    } break; case 22: {
        ForcedPlayer p1("P1", "132132132132");
        ForcedPlayer p2("P2", "213213213213");
        Game g(3, 1, 2, &p1, &p2);
        g.play();
        int w;
        assert(g.completed(w));
    } break; case 23: {
        ForcedPlayer p1("P1", "569:129:12");  // : is column 10
        ForcedPlayer p2("P2", "12485659:6");
        Game g(10, 3, 3, &p1, &p2);
        for (int k = 0; k < 20; k++)
            g.takeTurn();
        int w = 999;
        assert(g.completed(w) && w == BLACK);
        // RR..BB..BB
        // RR..BB..RR
        // BB.BRR.BRR

            // Player tests

    } break; case 24: {
        Scaffold s(10, 2);
        s.makeMove(2, RED);
        s.makeMove(6, BLACK);
        s.makeMove(6, RED);
        HumanPlayer hp("Hugh");
        ostringstream oss;
        istringstream iss("11\n0\n6\n2\n1\n3\n4\n5\n7\n8\n9\n10\n");
        {
            StreambufSwitcher sso(cout, oss);
            StreambufSwitcher ssi(cin, iss);
            assert(hp.chooseMove(s, 4, BLACK) == 2);
        }
    } break; case 25: {
        Scaffold s(3, 2);
        s.makeMove(1, RED);
        s.makeMove(1, BLACK);
        s.makeMove(2, BLACK);
        s.makeMove(2, RED);
        s.makeMove(3, RED);
        s.makeMove(3, BLACK);
        HumanPlayer hp("Hugh");
        ostringstream oss;
        istringstream iss("1\n2\n3\n");
        {
            StreambufSwitcher sso(cout, oss);
            StreambufSwitcher ssi(cin, iss);
            assert(hp.chooseMove(s, 3, RED) == 0);
        }
    } break; case 26: {
        Scaffold s(10, 2);
        s.makeMove(1, RED);
        s.makeMove(1, BLACK);
        s.makeMove(2, BLACK);
        s.makeMove(2, RED);
        s.makeMove(3, RED);
        s.makeMove(3, BLACK);
        s.makeMove(4, BLACK);
        s.makeMove(4, RED);
        s.makeMove(5, RED);
        s.makeMove(5, BLACK);
        s.makeMove(6, BLACK);
        s.makeMove(6, RED);
        s.makeMove(7, RED);
        s.makeMove(8, BLACK);
        s.makeMove(8, RED);
        s.makeMove(9, RED);
        s.makeMove(9, BLACK);
        s.makeMove(10, BLACK);
        s.makeMove(10, RED);
        BadPlayer bp("Brad");
        assert(bp.chooseMove(s, 4, BLACK) == 7);
    } break; case 27: {
        Scaffold s(10, 2);
        s.makeMove(1, RED);
        s.makeMove(1, BLACK);
        s.makeMove(2, BLACK);
        s.makeMove(2, RED);
        s.makeMove(3, RED);
        s.makeMove(3, BLACK);
        s.makeMove(4, BLACK);
        s.makeMove(4, RED);
        s.makeMove(5, RED);
        s.makeMove(5, BLACK);
        s.makeMove(6, BLACK);
        s.makeMove(6, RED);
        s.makeMove(7, RED);
        s.makeMove(7, BLACK);
        s.makeMove(8, BLACK);
        s.makeMove(8, RED);
        s.makeMove(9, RED);
        s.makeMove(9, BLACK);
        s.makeMove(10, BLACK);
        s.makeMove(10, RED);
        BadPlayer bp("Brad");
        assert(bp.chooseMove(s, 4, BLACK) == 0);
    } break; case 28: {
        Scaffold s(10, 2);
        s.makeMove(1, RED);
        s.makeMove(1, BLACK);
        s.makeMove(2, BLACK);
        s.makeMove(2, RED);
        s.makeMove(3, RED);
        s.makeMove(3, BLACK);
        s.makeMove(4, BLACK);
        s.makeMove(4, RED);
        s.makeMove(5, RED);
        s.makeMove(5, BLACK);
        s.makeMove(6, BLACK);
        s.makeMove(6, RED);
        s.makeMove(7, RED);
        s.makeMove(8, BLACK);
        s.makeMove(8, RED);
        s.makeMove(9, RED);
        s.makeMove(9, BLACK);
        s.makeMove(10, BLACK);
        s.makeMove(10, RED);
        SmartPlayer sp("Sam");
        assert(sp.chooseMove(s, 4, BLACK) == 7);
    } break; case 29: {
        Scaffold s(10, 2);
        s.makeMove(1, RED);
        s.makeMove(1, BLACK);
        s.makeMove(2, BLACK);
        s.makeMove(2, RED);
        s.makeMove(3, RED);
        s.makeMove(3, BLACK);
        s.makeMove(4, BLACK);
        s.makeMove(4, RED);
        s.makeMove(5, RED);
        s.makeMove(5, BLACK);
        s.makeMove(6, BLACK);
        s.makeMove(6, RED);
        s.makeMove(7, RED);
        s.makeMove(7, BLACK);
        s.makeMove(8, BLACK);
        s.makeMove(8, RED);
        s.makeMove(9, RED);
        s.makeMove(9, BLACK);
        s.makeMove(10, BLACK);
        s.makeMove(10, RED);
        SmartPlayer sp("Sam");
        assert(sp.chooseMove(s, 4, BLACK) == 0);
    } break; case 30: {
        Scaffold s(10, 1);
        s.makeMove(3, RED);
        s.makeMove(7, BLACK);
        s.makeMove(6, RED);
        s.makeMove(9, BLACK);
        s.makeMove(5, RED);
        SmartPlayer sp("Sam");
        assert(sp.chooseMove(s, 3, BLACK) == 8);
        //  ..R.RRB.B.
    } break; case 31: {
        Scaffold s(10, 1);
        s.makeMove(3, RED);
        s.makeMove(7, BLACK);
        s.makeMove(10, RED);
        s.makeMove(2, BLACK);
        s.makeMove(5, RED);
        SmartPlayer sp("Sam");
        assert(sp.chooseMove(s, 3, BLACK) == 4);
        //  .BR.R.B..R
    } break; case 32: {
        Scaffold s(10, 1);
        s.makeMove(9, RED);
        s.makeMove(10, BLACK);
        s.makeMove(4, RED);
        s.makeMove(2, BLACK);
        SmartPlayer sp("Sam");
        assert(sp.chooseMove(s, 3, RED) == 5);
        //  .B.R....RB
    } break; case 33: {
        Scaffold s(7, 2);
        s.makeMove(2, RED);
        s.makeMove(2, BLACK);
        s.makeMove(6, RED);
        SmartPlayer sp("Sam");
        assert(sp.chooseMove(s, 3, BLACK) == 4);
        //  .B.....
        //  .R...R.
    }
    }
}

int main()
{
    cout << "Enter test number: ";
    int n;
    cin >> n;
    testone(n);
    cout << "Passed" << endl;
}