CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku sudoku_thread sudoku_nothread

sudoku: main_pool.cc sudoku_basic.cc pool.cc
	g++ -O2 -g -pthread -o $@ $^

sudoku_thread: main_thread.cc sudoku_basic.cc
	g++ -O2 -g -pthread -o $@ $^

sudoku_nothread: main_nothread.cc sudoku_basic.cc
	g++ -O2 -g -o $@ $^
