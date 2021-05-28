CC = g++
FLAGS += -O2 -g

all: kvstore2pcsystem myshell

kvstore2pcsystem: src/kvstore2pcsystem.cpp
	$(CC) $(FLAGS) -o $@ $^
	
myshell: src/myshell.cpp
	$(CC) $(FLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf kvstore2pcsystem
	rm -rf myshell
