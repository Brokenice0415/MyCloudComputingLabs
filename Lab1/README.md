# Lab 1: “Super-fast” Sudoku Solving

Our team have managed to finish **all** implementation requirements. 

The **algorithm** we choose is **min_arity** .

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
├── v1						# last version code
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

### Example input

```shell
>./sudoku_solve
./test/test1000
./test/test100
```

Then you would get your result in file `solved` , in which all your need would be placed **in the same order** as which you have typed in.

## Modified 2021 Mar

The sudoku results would be output **on Shell** instead of the file `solved` .

Provided that you insist to get sudoku results in file `solved`, locate code in `src/sthread.cc` line 79

```c++

	FILE *fp = fopen(output, "a+");

	fprintf(fp, "%s\r\n", result->p);
	
	fclose(fp);
	
	/*
	printf("%s\r\n", result->p);		//annotate this line and disannotate lines above
	*/

```
Then you can run `make` again to recompile all codes.

## Modified 2021 Apr

**ACCELERATE 3 TIMES FASTER THAN BEFORE**

The the places where origin sudoku problems are **would be replaced by **solutions we get.

**For example**

You type in `./test100`

Then after the program processing, you would get your solutions **not only** on screen **but also** in `./test100`, **all zeros would be filled by correct solution numbers**



You can get the code in **last** version in fold `v1`