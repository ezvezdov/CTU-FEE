CC=gcc
CFLAGS+= -Wall
EXTRA_CFLAGS=

FORKPIPE=forkpipe
NSD=nsd
TARGET=osy04
TAR=tar

NSD_SOURCES = forkpipe.c nd.c nd.h nsd_main.c nsd.c
OBJS= nd.o nsd.o nsd_main.o

all: $(NSD) $(FORKPIPE) depend

$(OBJS): %.o: %.c
	$(CC) -c $< $(CFLAGS) $(EXTRA_CFLAGS) -o $@
$(NSD): $(OBJS)
	$(CC) $(OBJS) $(EXTRA_CFLAGS) -o $@ 

$(FORKPIPE): forkpipe.c
	$(CC) $(CFLAGS) forkpipe.c -o $(FORKPIPE)

depend: $(NSD_SOURCES) *.h
	$(CC) $(CFLAGS) -w -E -M $(filter %.c,$(NSD_SOURCES)) >> depend

tar:
	$(TAR) czf $(TARGET).tgz Makefile *.[ch]

clean:
	$(RM) *.o depend
	$(RM) $(TARGET).tgz

.PHONY: clean tar depend

-include depend
