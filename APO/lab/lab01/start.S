.globl   _start
.text
.option norelax

_start:
     la x2, _end+0x4000
     la x3, __global_pointer$
     jal  main
     ebreak