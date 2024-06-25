.option norelax

.globl _start

.text
_start:

main:

    addi  x2,  x0, 10
	nop
	nop
    add   x11, x0, x2   // A : $11<-$2
    add   x12, x0, x2   // B : $12<-$2
    add   x13, x0, x2   // C : $13<-$2

    //la x5, varx  // $5 = (byte*) &varx; 
    // The macro-instruction la is compiled as two following instructions:
    //addi  x5,  x0, 0x400
	auipc x5, 0(x1)
	nop
	nop 
	addi x5, x5, 36
	nop
	nop

    lw    x1, 0(x5)     // $1 = *((int*)$5);
    add   x15, x0, x1   // D : $15<-$1
    add   x16, x0, x1   // E : $16<-$1
    add   x17, x0, x1   // F : $17<-$1
loop:
    ebreak
    beq    x0, x0, loop
    nop

.data

varx:
	.word  0x1234