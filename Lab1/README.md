# Lab 1: “Super-fast” Sudoku Solving

Our team have managed to finish **all** implementation requirements. 

The **algorithm** we choose is **dance_links** .

## Structure

```shell
.
├── Makefile
├── src						# current version code
│   ├── main.cc
│   ├── pool.cc 				# threadpool
│   ├── pool.h
│   ├── sthread.cc 				# thread functions
│   ├── sthread.h
│   ├── sudoku_arity.cc 		# entity to solve sudoku problems
│   └── sudoku_arity.h
├── src						# code which use arity algorithm
│   ├── main.cc
│   ├── pool.cc 				# threadpool
│   ├── pool.h
│   ├── sthread.cc 				# thread functions
│   ├── sthread.h
│   ├── sudoku_arity.cc 		# entity to solve sudoku problems
│   └── sudoku_arity.h
└── test 					# some sample sudoku problems
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

We provide `-t` `-m` `-M` `-q` commands to control the performance of program

- -t :	enable program to print information of running time
- -m :  set the thread pool's **min** alive thread number
- -M :  set the thread pool's **max** alive thread number
- -q :   set the thread pool's **max** tasks waiting on queue

### Example input

```shell
>./sudoku_solve
./test/test1
693784512487512936125963874932651487568247391741398625319475268856129743274836159
./test/test1
693784512487512936125963874932651487568247391741398625319475268856129743274836159
```

Then you would get your result in file `solved` , in which all your need would be placed **in the same order** as which you have typed in.

```shell
>./sudoku_solve -t -m 10 -M 20 -q 100
./test/test1
0.000931 sec
693784512487512936125963874932651487568247391741398625319475268856129743274836159
```

In this example, we set the thread pool with **10** min alive threads, **20** max alive threads and at most **100** tasks waiting on queue, we use `-t` to enable program to print `0.000931 sec` as well

## Contrast

The code in `./arity_src`  performs the same with that in `./src` except the algorithm it uses is **min_arity**

You can type  command below to compile it

```shell
> make sudoku_solve_arity
```

Then everything is the same with all above, you can compare the differences brought by two different algorithm in running time.