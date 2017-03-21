	.file	"hello.c"
	.section	.rodata
.LC0:
	.string	"Result: %d, %d\\n"
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
	subl	$36, %esp
	movl	$10, -12(%ebp)
	movl	$0, -8(%ebp)
	movl	-12(%ebp), %edx
#APP
	movl %edx, %eax;movl %eax, %edx;
#NO_APP
	movl	%edx, -8(%ebp)
	movl	-8(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
	addl	$36, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.0 (SUSE Linux)"
	.section	.note.GNU-stack,"",@progbits
