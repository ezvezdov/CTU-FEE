	.file	"sum2var.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"sum %d + %d -> %d\n"
.LC1:
	.string	"sum 0x%x + 0x%x -> 0x%x\n"
	.section	.text.startup,"ax",@progbits
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	var_a(%rip), %edx
	leaq	.LC0(%rip), %rsi
	xorl	%eax, %eax
	movl	var_b(%rip), %ecx
	movl	$1, %edi
	leal	(%rdx,%rcx), %r8d
	movl	%r8d, var_c(%rip)
	call	__printf_chk@PLT
	movl	var_a(%rip), %edx
	xorl	%eax, %eax
	movl	var_c(%rip), %r8d
	movl	var_b(%rip), %ecx
	leaq	.LC1(%rip), %rsi
	movl	$1, %edi
	call	__printf_chk@PLT
	xorl	%eax, %eax
	popq	%rdx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.globl	var_c
	.data
	.align 4
	.type	var_c, @object
	.size	var_c, 4
var_c:
	.long	13107
	.globl	var_b
	.align 4
	.type	var_b, @object
	.size	var_b, 4
var_b:
	.long	8738
	.globl	var_a
	.align 4
	.type	var_a, @object
	.size	var_a, 4
var_a:
	.long	4660
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
