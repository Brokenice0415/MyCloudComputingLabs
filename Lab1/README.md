# Lab 1: “Super-fast” Sudoku Solving

Our team have managed to finish **all** implementation requirements. 

The **algorithm** we choose is **min_arity** .

## Structure

```shell
.
├── Makefile
├── src
│   ├── main.cc
│   ├── pool.cc # threadpool
│   ├── pool.h
│   ├── sthread.cc # thread functions
│   ├── sthread.h
│   ├── sudoku_arity.cc # entity to solve sudoku problems
│   └── sudoku_arity.h
└── test # some sample sudoku problems
    ├── test1
    ├── test100
    └── test1000

```

## Helper

Type the makefile instruction below to compile all codes.

```shell
make
```

Then you will get `sudoku_solve` by which you can solve sudoku problems.

### Example input

```shell
>./sudoku_solve
./test/test1000
./test/test100
```

Then you would get your result in file `solved` , in which all your need would be placed **in the same order** as which you have typed in.

