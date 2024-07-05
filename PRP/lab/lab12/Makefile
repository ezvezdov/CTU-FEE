CFLAGS+=--std=c99 
CFLAGS+=-g
CFLAGS+=-O2 -Wall -Werror -pedantic
LFGLAGS+=-lm
CC=clang

all: main bin_heap

main: main.c binary_heap.o
	$(CC) $(CFLAGS) main.c binary_heap.o -o main 

bin_heap: binary_heap.c binary_heap.h
	$(CC) $(CFLAGS) -c binary_heap.c  -o binary_heap.o 

clean:
	rm -f *.o
	rm -f main
  
