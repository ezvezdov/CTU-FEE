CC = gcc
CFLAGS+=  -mssse3 -g -O1 -Wall -std=c99 -lm -lrt
HW=main
ZIP=zip



$(HW): main.c
	$(CC) $(CFLAGS) main.c -o $(HW)

zip:
	$(ZIP) $(HW).zip main.c

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW).zip

.PHONY: clean zip
