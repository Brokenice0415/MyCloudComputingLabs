#include <assert.h>
#include <memory.h>
#include <strings.h>
#include <map>
#include <vector>

#include <algorithm>

#include "sudoku_arity.h"

using namespace std;

Arity::Arity(int inout[81]): board(inout)
{
	init_neighbors();
	find_spaces();
}

void Arity::init_neighbors()
{
  for (int row = 0; row < ROW; ++row) {
    for (int col = 0; col < COL; ++col) {
      bool adjacent[ROW][COL];
      bzero(adjacent, sizeof adjacent);
      mark_adjacent(adjacent, row, col);

      int me = row*COL + col;
      collect_neighbors(adjacent, row, col, neighbors[me]);
    }
  }
}

void Arity::mark_adjacent(bool adjacent[ROW][COL], int row, int col)
{
  for (int i = 0; i < NUM; ++i) {
    adjacent[row][i] = true;
    adjacent[i][col] = true;
  }
  int top = (row/3)*3;
  int left = (col/3)*3;
  adjacent[top][left] = true;
  adjacent[top][left+1] = true;
  adjacent[top][left+2] = true;
  adjacent[top+1][left] = true;
  adjacent[top+1][left+1] = true;
  adjacent[top+1][left+2] = true;
  adjacent[top+2][left] = true;
  adjacent[top+2][left+1] = true;
  adjacent[top+2][left+2] = true;
}

void Arity::collect_neighbors(bool adjacent[ROW][COL], int row, int col, int myneighbors[NEIGHBOR])
{
  int n = 0;
  for (int y = 0; y < ROW; ++y) {
    for (int x = 0; x < COL; ++x) {
      if (adjacent[y][x] && !(y == row && x == col)) {
        assert(n < NEIGHBOR);
        myneighbors[n++] = y*COL + x;
      }
    }
  }
  assert(n == NEIGHBOR);
}

void Arity::find_spaces()
{
  nspaces = 0;
  for (int cell = 0; cell < N; ++cell) {
    if (board[cell] == 0)
      spaces[nspaces++] = cell;
  }
}

bool Arity::available(int guess, int cell)
{
  for (int i = 0; i < NEIGHBOR; ++i) {
    int neighbor = neighbors[cell][i];
    if (board[neighbor] == guess) {
      return false;
    }
  }
  return true;
}

int Arity::arity(int cell)
{
  bool occupied[10] = {false};
  for (int i = 0; i < NEIGHBOR; ++i) {
    int neighbor = neighbors[cell][i];
    occupied[board[neighbor]] = true;
  }
  return std::count(occupied+1, occupied+10, false);
}

void Arity::find_min_arity(int space)
{
  int cell = spaces[space];
  int min_space = space;
  int min_arity = arity(cell);

  for (int sp = space+1; sp < nspaces && min_arity > 1; ++sp) {
    int cur_arity = arity(spaces[sp]);
    if (cur_arity < min_arity) {
      min_arity = cur_arity;
      min_space = sp;
    }
  }

  if (space != min_space) {
    std::swap(spaces[min_space], spaces[space]);
  }
}

bool Arity::solve(int which_space)
{
  if (which_space >= nspaces) {
    return true;
  }

  find_min_arity(which_space);
  int cell = spaces[which_space];

  for (int guess = 1; guess <= NUM; ++guess) {
    if (available(guess, cell)) {
      // hold
      assert(board[cell] == 0);
      board[cell] = guess;

      // try
      if (solve(which_space+1)) {
        return true;
      }

      // unhold
      assert(board[cell] == guess);
      board[cell] = 0;
    }
  }
  return false;
}

bool Arity::solved()
{
  for (int row = 0; row < ROW; ++row) {
    // check row
    int occurs[10] = { 0 };
    for (int col = 0; col < COL; ++col) {
      int val = chess[row][col];
      assert(1 <= val && val <= NUM);
      ++occurs[val];
    }

    if (std::count(occurs, occurs+10, 1) != NUM)
      return false;
  }

  for (int col = 0; col < COL; ++col) {
    int occurs[10] = { 0 };
    for (int row = 0; row < ROW; ++row) {
      int val = chess[row][col];
      // assert(1 <= val && val <= NUM);
      ++occurs[val];
    }

    if (std::count(occurs, occurs+10, 1) != NUM)
      return false;
  }

  for (int row = 0; row < ROW; row += 3) {
    for (int col = 0; col < COL; col += 3) {
      int occurs[10] = { 0 };
      ++occurs[chess[row  ][col]];
      ++occurs[chess[row  ][col+1]];
      ++occurs[chess[row  ][col+2]];
      ++occurs[chess[row+1][col]];
      ++occurs[chess[row+1][col+1]];
      ++occurs[chess[row+1][col+2]];
      ++occurs[chess[row+2][col]];
      ++occurs[chess[row+2][col+1]];
      ++occurs[chess[row+2][col+2]];

      if (std::count(occurs, occurs+10, 1) != NUM)
        return false;
    }
  }
  return true;
}

puzzle_t* Arity::get_result(){
	puzzle_t *puzzle = (puzzle_t *)malloc(sizeof(puzzle_t));
	for(int i = 0; i < N; i ++) {
		puzzle->p[i] = '0' + board[i];
	}
	puzzle->p[81] = 0;
	return puzzle;
}
