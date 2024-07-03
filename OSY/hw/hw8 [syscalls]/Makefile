CC=gcc
CFLAGS+=  -ffreestanding -nostdlib -nostdinc -static -m32 -Wall -g -O2
HW=hexconv
TEST=original
ZIP=zip

$(HW): hexconv.c 
	$(CC) $(CFLAGS) $(HW).c -o $(HW)

original: original.c
	$(CC) original.c -o $(TEST)

zip:
	$(ZIP) $(HW)-brute.zip $(HW).c 

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-brute.zip

.PHONY: clean zip
