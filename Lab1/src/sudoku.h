#ifndef SUDOKU_H
#define SUDOKU_H

#include <map>
#include <vector>


using namespace std;

enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

struct puzzle_t{
	char p[128];
};

struct file_t{
	int pos;
	char name[100];
};


struct Node;

typedef Node Column;

struct Node{
    Node* left;
    Node* right;
    Node* up;
    Node* down;
    Column* col;
    int name;
    int size;
};

const int kMaxNodes = 1 + 81*4 + 9*9*9*4;
const int kMaxColumns = 400;
const int kRow = 100, kCol = 200, kBox = 300;

struct Dance
{
    Column* root_;
    int*    inout_;
    Column* columns_[400];
    vector<Node*> stack_;
    Node    nodes_[kMaxNodes];
    int     cur_node_;
  
    Column* new_column(int n);
    void append_column(int n);

    Node* new_row(int col);

    int get_row_col(int row, int val);

    int get_col_col(int col, int val);

    int get_box_col(int box, int val);

    Dance(int inout[81]);

    Column* get_min_column();

    void cover(Column* c);

    void uncover(Column* c);

    bool solve();

    void put_left(Column* old, Column* nnew);

    void put_up(Column* old, Node* nnew);
    
    puzzle_t* get_result();
};


#endif
