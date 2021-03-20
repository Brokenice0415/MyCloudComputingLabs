CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku_solve

sudoku_solve: main.cc thread.cc sudoku_arity.cc pool.cc
	g++ -O2 -g -pthread -o $@ $^
