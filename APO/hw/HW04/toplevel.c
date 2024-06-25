/* Implementation of the subroutine toplevel_fnc */

#include "subroutine.h"
#include <unistd.h>

int toplevel_fnc(void)
{
  //addi	sp,sp,-32
  //sw	ra,28(sp)
  //sw	s0,24(sp)

  // int sp_12 = 0; //sw zero,12(sp)
  // int a7 = 63;
  // int a0 = 0;

  // read(,,1)

  //sw	zero,12(sp)






  /*
   * fill in code with equivalent function
   * to the toplevel_fnc function from
   * the assembly program listing.
   */

  /*
   * The code calls function subroutine_fnc
   * and processes its return value.
   * 
   * Then it returns final result.
   */
}

// read()

// 080ac177 <toplevel_fnc>:
//  80ac177:	55                   	push   %ebp
//  80ac178:	57                   	push   %edi
//  80ac179:	56                   	push   %esi
//  80ac17a:	53                   	push   %ebx
//  80ac17b:	83 ec 10             	sub    $0x10,%esp
//  80ac17e:	c7 44 24 0c 00 00 00 	movl   $0x0,0xc(%esp)
//  80ac185:	00 
//  80ac186:	bf 00 00 00 00       	mov    $0x0,%edi
//  80ac18b:	bd 00 00 00 00       	mov    $0x0,%ebp
//  80ac190:	8d 74 24 0b          	lea    0xb(%esp),%esi
//  80ac194:	eb 03                	jmp    80ac199 <toplevel_fnc+0x22>
//  80ac196:	83 c7 01             	add    $0x1,%edi
//  80ac199:	ba 01 00 00 00       	mov    $0x1,%edx
//  80ac19e:	89 eb                	mov    %ebp,%ebx
//  80ac1a0:	89 f1                	mov    %esi,%ecx
//  80ac1a2:	b8 03 00 00 00       	mov    $0x3,%eax
//  80ac1a7:	cd 80                	int    $0x80
//  80ac1a9:	83 f8 01             	cmp    $0x1,%eax
//  80ac1ac:	75 2d                	jne    80ac1db <toplevel_fnc+0x64>
//  80ac1ae:	0f be 44 24 0b       	movsbl 0xb(%esp),%eax
//  80ac1b3:	50                   	push   %eax
//  80ac1b4:	8d 44 24 10          	lea    0x10(%esp),%eax
//  80ac1b8:	50                   	push   %eax
//  80ac1b9:	e8 83 ff ff ff       	call   80ac141 <subroutine_fnc>
//  80ac1be:	88 44 24 13          	mov    %al,0x13(%esp)
//  80ac1c2:	83 c4 08             	add    $0x8,%esp
//  80ac1c5:	84 c0                	test   %al,%al
//  80ac1c7:	74 cd                	je     80ac196 <toplevel_fnc+0x1f>
//  80ac1c9:	ba 01 00 00 00       	mov    $0x1,%edx
//  80ac1ce:	89 d3                	mov    %edx,%ebx
//  80ac1d0:	89 f1                	mov    %esi,%ecx
//  80ac1d2:	b8 04 00 00 00       	mov    $0x4,%eax
//  80ac1d7:	cd 80                	int    $0x80
//  80ac1d9:	eb be                	jmp    80ac199 <toplevel_fnc+0x22>
//  80ac1db:	89 f8                	mov    %edi,%eax
//  80ac1dd:	83 c4 10             	add    $0x10,%esp
//  80ac1e0:	5b                   	pop    %ebx
//  80ac1e1:	5e                   	pop    %esi
//  80ac1e2:	5f                   	pop    %edi
//  80ac1e3:	5d                   	pop    %ebp
//  80ac1e4:	c3                   	ret    