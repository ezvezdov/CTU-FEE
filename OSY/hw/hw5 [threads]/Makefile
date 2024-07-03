CFLAGS+=  -Wall -g -O2
EXTRA_CFLAGS?= -fsanitize=address -fno-omit-frame-pointer
HW=prod-cons
ZIP=zip

$(HW): main.c 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) main.c -o $(HW)

zip:
	$(ZIP) $(HW)-brute.zip main.c Makefile

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-brute.zip

.PHONY: clean zip
