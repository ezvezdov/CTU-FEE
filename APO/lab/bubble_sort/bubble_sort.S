.globl _start
.option norelax

.texts

_start:

	lui x1, x0 //int i = 0;
	addi x1, x1, -4 // i = -1

	
	lw x3, array_size(x0) // array size for i
	addi t6,t6, 4  //load 4 for mult 
	mul x3, x3, t6 //mult array_size by 4 

	lw x4, array_size(x0) // array size for j
	addi x4,x3, -4 // n-1

for1:
	addi x1,x1, 4 //i++
	bge x1,x3, for1_end //for1
	lui x2, x0 // int j = 0 index	
	addi x2, x2, -4 // j = -1
for2:
	addi x2, x2, 4 //j++
	bge x2,x4,for1 //for2

	addi x5, x2, 4 // tmp for j+1

	lw x6, array_data(x2) // load array_data(j)
	lw x7, array_data(x5) // load array_data(j+1)
	blt x6,x7,for2
	sw x6, array_data(x5) //swap1
	sw x7, array_data(x2) //swap2
	j for2

for1_end

end_loop:
	ebreak           //stop the simulator
	j end_loop
	nop

.data

array_size:
.word 15

array_data:
.word 3, 15, 5, 6, 2 ,9, 10, 13, 1, 4, 7, 8, 11, 14, 12

