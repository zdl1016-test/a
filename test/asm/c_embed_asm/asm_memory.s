	.file	"asm_memory.c"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$20, %esp
	movl	$0, -12(%ebp)
	leal	-12(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	movl	$9999, (%eax)
	movl	-8(%ebp), %eax
	movl	(%eax), %eax
	cmpl	$9999, %eax
	jne	.L2
	movl	$2, -24(%ebp)
	jmp	.L4
.L2:
	movl	$0, -24(%ebp)
.L4:
	movl	-24(%ebp), %eax
	addl	$20, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.0 (SUSE Linux)"
	.section	.note.GNU-stack,"",@progbits
