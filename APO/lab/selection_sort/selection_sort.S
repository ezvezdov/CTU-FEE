// Simple sorting algorithm - selection sort

// Directives to make interesting windows visible
#pragma qtrvsim show registers
#pragma qtrvsim show memory

.option norelax

.globl  array
.globl  _start

.text

_start:

la   a0, array
addi s0, zero, 0  //Minimum value from the rest of the array will be placed here. (Offset in the array, increasing by 4 bytes).
addi s1, zero, 60 // Maximal index/offset value. Used for cycle termination = number of values in array * 4.
add  s2, zero, s0 //Working position (offset)
// s3 - offset of the smallest value found so far in given run
// s4 - value of the smallest value found so far in  given run
// s5 - temporary
main_cycle:
        beq  s0, s1, main_cycle_end

        add  t0, a0, s0
        lw   s4, 0(t0)   // lw  s4, array(s0)
        add  s3, s0, zero
        add  s2, s0, zero

inner_cycle:
        beq  s2, s1, inner_cycle_end
                add  t0, a0, s2
                lw   s5, 0(t0) // lw s5, array(s2)

                // expand bgt s5, s4, not_minimum
                slt  t0, s4, s5
                bne  t0, zero, not_minimum

                        addi s3, s2, 0
                        addi s4, s5, 0
not_minimum:
                addi s2, s2, 4
                j inner_cycle
inner_cycle_end:
        add  t0, a0, s0
        lw   s5, 0(t0)  // lw s5, array(s0)
        sw   s4, 0(t0)  // sw s4, array(s0)
        add  t0, a0, s3
        sw   s5, 0(t0)  // sw s5, array(s3)

        addi s0, s0, 4
        j main_cycle
main_cycle_end:

//Final infinite loop
end_loop:
        fence            // flush cache memory
        ebreak           // stop the simulator
        j end_loop

.org 0x400

.data
// .align    2 // not supported by QtRVSsim

array:
.word    5, 3, 4, 1, 15, 8, 9, 2, 10, 6, 11, 1, 6, 9, 12

// Specify location to show in memory window
#pragma qtrvsim focus memory array