/* template for own QtRvSim program development */

.globl _start
.globl __start

.option norelax

.text

nop
nop
nop

__start:
_start:
	addi t0, zero, 6
	la t1, v1
	la t2, v2
	la t3, v3

loop:
	beq t0, zero, endloop

	lw a2, 0(t1)
	lw a3, 0(t2)
	add a4, a2, a3
	sw a4, 0(t3)

	addi t0, t0, -1; # t0 = t0 + -1
	addi t1, t1, 4; # t1 = t1 + 4
	addi t2, t2, 4; # t2 = t2 + 4
	addi t3, t3, 4; # t3 = t3 + 4
	j loop  # jump to loop
	


endloop:	ebreak
        beq     zero, zero, endloop

.data

.word 42,42,42

v1:
.word 1,2,3,4,5,
v2:
.word 1,2,3,4,5,

