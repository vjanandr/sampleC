	.file	"bubble.c"
	.text
	.globl	a_
	.data
	.align 32
	.type	a_, @object
	.size	a_, 40
a_:
	.long	54
	.long	81
	.long	37
	.long	44
	.long	29
	.long	47
	.long	59
	.long	83
	.long	51
	.long	45
	.section	.rodata
.LC0:
	.string	"\n%s\n"
.LC1:
	.string	"%d "
	.text
	.globl	print
	.type	print, @function
print:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movl	$0, -4(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -4(%rbp)
.L2:
	movl	-4(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jl	.L3
	movl	$10, %edi
	call	putchar@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	print, .-print
	.section	.rodata
.LC2:
	.string	"Before sort"
.LC3:
	.string	"After sort"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$10, %edx
	leaq	.LC2(%rip), %rsi
	leaq	a_(%rip), %rdi
	call	print
	movl	$0, -12(%rbp)
	jmp	.L5
.L9:
	movl	$0, -8(%rbp)
	jmp	.L6
.L8:
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	a_(%rip), %rax
	movl	(%rdx,%rax), %edx
	movl	-8(%rbp), %eax
	addl	$1, %eax
	cltq
	leaq	0(,%rax,4), %rcx
	leaq	a_(%rip), %rax
	movl	(%rcx,%rax), %eax
	cmpl	%eax, %edx
	jle	.L7
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	a_(%rip), %rax
	movl	(%rdx,%rax), %eax
	movl	%eax, -4(%rbp)
	movl	-8(%rbp), %eax
	addl	$1, %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	a_(%rip), %rax
	movl	(%rdx,%rax), %edx
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	leaq	a_(%rip), %rax
	movl	%edx, (%rcx,%rax)
	movl	-8(%rbp), %eax
	addl	$1, %eax
	cltq
	leaq	0(,%rax,4), %rcx
	leaq	a_(%rip), %rax
	movl	-4(%rbp), %edx
	movl	%edx, (%rcx,%rax)
.L7:
	addl	$1, -8(%rbp)
.L6:
	movl	$9, %eax
	subl	-12(%rbp), %eax
	cmpl	%eax, -8(%rbp)
	jl	.L8
	addl	$1, -12(%rbp)
.L5:
	cmpl	$8, -12(%rbp)
	jle	.L9
	movl	$10, %edx
	leaq	.LC3(%rip), %rsi
	leaq	a_(%rip), %rdi
	call	print
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
