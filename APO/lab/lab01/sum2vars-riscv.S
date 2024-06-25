.globl _start

.text
_start:
	lw   x4, var_a(x0)
	lw   x5, var_b(x0)
	add  x6, x4, x5
	sw   x6, var_c(x0)
	
	addi x2, x0, 0
	jr   ra

.data
.org 400
var_a:	.word 0x1234
var_b:	.word 0x2222
var_c:	.word 0x3333

#pragma qtrvsim show registers
#pragma qtrvsim show memory
#pragma qtrvsim focus memory var_a
#pragma qtrvsim tab core