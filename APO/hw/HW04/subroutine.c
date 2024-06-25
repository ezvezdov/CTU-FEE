/* Implementation of the subroutine subroutine_fnc */

#include "subroutine.h"
#include<string.h>
int subroutine_fnc(char* str, int a)
{
//   int a3;
//   int a4;

//   *a5 = *a0; //mv	a5,a0
//   a4 = 32; //li	a4,32
  
//   if(*a1 == a4){goto subroutine_fnc_plus_0x20;} //beq	a1,a4,10124 <subroutine_fnc+0x20>
//   a4 = 9; //li	a4,9
//   if(*a1 == a4){goto subroutine_fnc_plus_0x20;} //beq	a1,a4,10124 <subroutine_fnc+0x20>
//   str = 0x11; //lui	a4,0x11
//   str = "...."; //lw	a4,492(a4) # 111ec <optional_char_to_skip> 

  

//   if(a4 != *a1){goto subroutine_fnc_plus_0x40;}
// subroutine_fnc_plus_0x20:
//     a4 = *a5; //lw	a4,0(a5)
//     a3 = a4 + 1; //addi	a3,a4,1
//     if(a4 == 0){goto subroutine_fnc_plus_0x38;} //beqz	a4,1013c <subroutine_fnc+0x38>
//     *a0 = 0; //li	a0,0
// subroutine_fnc_plus_0x30:
//     *a5 = a3;//sw	a3,0(a5)
//     return; //ret //jalr x0,0(x1)
   
// subroutine_fnc_plus_0x38:   
//     *a0 = *a1; //mv	a0,a1
//     goto subroutine_fnc_plus_0x30; //j	10134 <subroutine_fnc+0x30>
// subroutine_fnc_plus_0x40:
//     *a0 = *a1; //mv	a0,a1
//     a3 = 0;//li	a3,0
//     goto subroutine_fnc_plus_0x30;//j	10134 <subroutine_fnc+0x30>
    return a;
}
