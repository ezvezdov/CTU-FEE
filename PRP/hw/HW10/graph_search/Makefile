
CC:=clang
CFLAGS+=-O2 -g
LDFLAGS+=-lm
#clang -c tgraph_search.c -std=c99 -O3 -march=native -pedantic -Wall -Werror -D_POSIX_C_SOURCE=200809L -Wnewline-eof -fcolor-diagnostics  -o tgraph_search.o



OS:=$(shell uname)

ifeq ($(OS),Linux)
   CFLAGS+=-std=gnu99
   LDFLAGS+=-lrt
endif
CFLAGS+=-march=native -pedantic -Wall -Werror -D_POSIX_C_SOURCE=200809L -Wnewline-eof -fcolor-diagnostics 


#OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
OBJS=\
     dijkstra.o\
     graph.o\
     my_malloc.o\
     graph_utils.o\
     pq_heap-no_update.o\

TARGET=tgraph_search

TARGET_TIME=tgraph_search-time

OBJS_ALL=$(OBJS)
OBJS_ALL+=$(TARGET).o 
OBJS_ALL+=$(TARGET_TIME).o

all: bin time

bin: $(TARGET)

time: $(TARGET_TIME)

$(OBJS_ALL): %.o: %.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

$(TARGET): $(OBJS_ALL)
	$(CC) $(OBJS) $@.o $(LDFLAGS) -o $@ 

$(TARGET_TIME): $(OBJS_ALL)
	$(CC) $(OBJS) $@.o $(LDFLAGS) -o $@ 

clean:
	$(RM) $(OBJS_ALL) $(TARGET) $(TARGET_TIME)

