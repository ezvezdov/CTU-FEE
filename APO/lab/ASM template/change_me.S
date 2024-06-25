/* template for own QtRvSim program development */

.globl _start
.globl __start

.option norelax

.text

__start:
_start:
	la t0, var1
	lw a2,0x0(t0)
	lw a3,0x0(t0)
	jal ra, mult
	j loop

mult:
	add a1,zero,zero

multiloop:
	beq a3, zero, multend
	add a1,a1, a2
	addi a3, a3, -1
	j multiloop
multend:
	sw a1, 0x0(t0)
	jr ra





loop:	ebreak
        beq     zero, zero, loop

.data

var1:	.word 7,6
