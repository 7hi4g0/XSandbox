include ../Makefile

all: main.c window.o
	gcc -g -o window main.c window.o $(CFLAGS) -I./ -lX11 -lfontconfig -lXft

window.o: window.c window.h
	gcc -g -c -o window.o window.c $(CFLAGS) -I./
