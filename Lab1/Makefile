CC 		  = g++
CXXFLAGS += -std=c++11 -O2 -g -pthread

CXXSRC 	 += src/

all: sudoku_solve

sudoku_solve: $(CXXSRC)*.cc
	$(CC) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf ./sudoku_solve
	rm -rf ./solved
