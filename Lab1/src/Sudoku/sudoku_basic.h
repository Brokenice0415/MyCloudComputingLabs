#ifndef SUDOKU_BASIC_H
#define SUDOKU_BASIC_H

#include <map>
#include <vector>


using namespace std;

enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

class Basic
{
	private:
	int neighbors[N][NEIGHBOR];
    int* board;
	int spaces[N];
	int nspaces;
	int (*chess)[COL] = (int (*)[COL])board;
    
  	public:
    Basic(int inout[128]);
    void init_neighbors();
    void mark_adjacent(bool adjacent[ROW][COL], int row, int col);
    void collect_neighbors(bool adjacent[ROW][COL], int row, int col, int myneighbors[NEIGHBOR]);
    void find_spaces();
    bool available(int gess, int cell);
    bool solve(int white_space);
    bool solved();
};

#endif
