	.file	"map.c"
	.text
	.globl	foo
	.bss
	.align 4
	.type	foo, @object
	.size	foo, 4
foo:
	.zero	4
	.globl	stuff
	.data
	.align 4
	.type	stuff, @object
	.size	stuff, 4
stuff:
	.long	7
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movl	$0, -20(%rbp)
	movl	$100, %edi
	call	malloc@PLT
	movq	%rax, -16(%rbp)
	movl	$100, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movl	$3, %edi
	call	recur@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04.1) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
