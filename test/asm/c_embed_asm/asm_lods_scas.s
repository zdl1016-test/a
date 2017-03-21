	.file	"asm_lods_scas.c"
	.section	.rodata
.LC0:
	.string	"abcd"
.LC1:
	.string	"buf:[%s]\n"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ecx
	subl	$172, %esp
	movl	$.LC0, -20(%ebp)
	leal	-148(%ebp), %edx
	movl	$128, %eax
	movl	%eax, 8(%esp)
	movl	$0, 4(%esp)
	movl	%edx, (%esp)
	call	memset
	movl	-20(%ebp), %eax
	movl	$-1, %ecx
	movl	%eax, -160(%ebp)
	movl	$0, %eax
	cld
	movl	-160(%ebp), %edi
	repnz
	scasb
	movl	%ecx, %eax
	notl	%eax
	subl	$1, %eax
	movl	%eax, -16(%ebp)
	movl	-20(%ebp), %esi
	leal	-148(%ebp), %edi
#APP
	1:cld;	
decl %ecx;	
jz 2;	
lodsb;	
scasb;	
jmp 1;	
2:	

#NO_APP
	movl	%ecx, -16(%ebp)
	leal	-148(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC1, (%esp)
	call	printf
	movl	$0, %eax
	addl	$172, %esp
	popl	%ecx
	popl	%esi
	popl	%edi
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.0 (SUSE Linux)"
	.section	.note.GNU-stack,"",@progbits
