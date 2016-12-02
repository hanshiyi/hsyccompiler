.data
glo	:	.space	4
stringalign0	:	.asciiz	"space"
string_0	:	.asciiz	"greatk:"
string_1	:	.asciiz	"wonderful"
string_2	:	.asciiz	"test succeed!"
.text 
j	main

	$$return:	
	lw	$fp,	52($sp)
	lw	$a3,	48($sp)
	lw	$a2,	44($sp)
	lw	$a1,	40($sp)
	lw	$a0,	36($sp)
	lw	$s7,	32($sp)
	lw	$s6,	28($sp)
	lw	$s5,	24($sp)
	lw	$s4,	20($sp)
	lw	$s3,	16($sp)
	lw	$s2,	12($sp)
	lw	$s1,	8($sp)
	lw	$s0,	4($sp)
	lw	$ra,	0($sp)
	add	$sp,	$sp,	56
	jr	$ra
	nop
test1:
	add	$sp,	$sp,	-56
	sw	$fp,	52($sp)
	sw	$a3,	48($sp)
	sw	$a2,	44($sp)
	sw	$a1,	40($sp)
	sw	$a0,	36($sp)
	sw	$s7,	32($sp)
	sw	$s6,	28($sp)
	sw	$s5,	24($sp)
	sw	$s4,	20($sp)
	sw	$s3,	16($sp)
	sw	$s2,	12($sp)
	sw	$s1,	8($sp)
	sw	$s0,	4($sp)
	sw	$ra,	0($sp)
	add	$sp,	$sp,	0
	add	$fp,	$sp,	$0
	addi	$sp,	$sp,	-4
	sw	$a2,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	addi	$a0,	$s0,	0
	li	$v0	1
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$a1,	0($sp)
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	lw	$s0,	60($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	mult	$s1,	$s2
	addi	$sp,	$sp,	-4
	mflo	$s0
	sw	$s0,	0($sp)
	lw	$s0,	56($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	sub	$s0,	$s1,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	mult	$s1,	$s2
	addi	$sp,	$sp,	-4
	mflo	$s0
	sw	$s0,	0($sp)
	addi	$s0,	$0,	2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	div	$s1,	$s2
	addi	$sp,	$sp,	-4
	mflo	$s0
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	add	$v0,	$s0,	$0
	j	test1_end
test1_end:
	add	$sp,	$sp,	0
	j	$$return
main:
	add	$sp,	$sp,	-12
	add	$fp,	$sp,	$0
	addi	$t4,	$0,	10
	sw	$t4,	0($fp)
	li	$v0	5
	syscall
	sw	$v0,	8($fp)
	li	$v0	5
	syscall
	sw	$v0,	4($fp)
	addi	$s0,	$0,	7
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$s0,	$0,	5
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	mult	$s1,	$s2
	addi	$sp,	$sp,	-4
	mflo	$s0
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$s0,	$0,	1
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$s0,	$0,	2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$s0,	$0,	3
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$s0,	$0,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$a0,	20($sp)
	lw	$a1,	16($sp)
	lw	$a2,	12($sp)
	lw	$a3,	8($sp)
	jal	test1
	nop	
	addi	$sp,	$sp,	24
	lw	$a3,	60($fp)
	lw	$a2,	56($fp)
	lw	$a1,	52($fp)
	lw	$a0,	48($fp)
	addi	$sp,	$sp,	-4
	sw	$v0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$s0,	$0,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	div	$s1,	$s2
	addi	$sp,	$sp,	-4
	mflo	$s0
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	sw	$s0,	4($fp)
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$s0,	$0,	40
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	sub	$s0,	$s1,	$s2
	sw	$s0,	glo
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	la	$a0	string_0
	li	$v0	4
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	addi	$a0,	$s0,	0
	li	$v0	1
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	la	$a0	string_1
	li	$v0	4
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s0,	glo
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	addi	$a0,	$s0,	0
	li	$v0	1
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	la	$a0	string_2
	li	$v0	4
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	main_end:

