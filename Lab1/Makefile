CC 		  = g++
FLAGS += -std=c++11 -O2 -g -pthread

SRC 	 += src/
ARITYSRC += arity_src/

all: sudoku_solve

sudoku_solve: $(SRC)*.cc
	$(CC) $(FLAGS) -o $@ $^

sudoku_solve_arity : $(ARITYSRC)*.cc
	$(CC) $(FLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf ./sudoku_solve
	rm -rf ./sudoku_solve_arity
