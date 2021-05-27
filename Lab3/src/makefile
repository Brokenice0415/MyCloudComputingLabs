CC = g++
FLAGS += -O2 -g

SRC += util/
SRC += ./
SRC += participant/

all: kvstore2pcsystem myshell

kvstore2pcsystem: kvstore2pcsystem.cpp
	$(CC) $(FLAGS) -o $@ $^
	
myshell: myshell.cpp
	$(CC) $(FLAGS) -o $@ $^


.PHONY: clean
clean:
	rm -rf kvstore2pcsystem
	rm -rf myshell
