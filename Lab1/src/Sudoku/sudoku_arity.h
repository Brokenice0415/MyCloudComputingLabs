#ifndef SUDOKU_ARITY_H
#define SUDOKU_ARITY_H

#include <map>
#include <vector>


using namespace std;

enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

struct puzzle_t{
	char name[100];
	char p[128];
};

class Arity
{
	private:
	int neighbors[N][NEIGHBOR];
    int* board;
	int spaces[N];
	int nspaces;
	int (*chess)[COL] = (int (*)[COL])board;
    
  	public:
    Arity(int inout[128]);
    void init_neighbors();
    void mark_adjacent(bool adjacent[ROW][COL], int row, int col);
    void collect_neighbors(bool adjacent[ROW][COL], int row, int col, int myneighbors[NEIGHBOR]);
    void find_spaces();
    bool available(int gess, int cell);
    
    int arity(int cell);
    void find_min_arity(int space);
    
    bool solve(int white_space);
    bool solved();
    
    puzzle_t* get_result();
};

#endif
