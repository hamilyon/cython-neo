	.file	"hello_world"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.Lhello_str, %edi
	callq	puts@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.Lhello_str,@object             # @hello_str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lhello_str:
	.asciz	"Hello, World!\n"
	.size	.Lhello_str, 15

	.section	".note.GNU-stack","",@progbits
