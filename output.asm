.data
string_0	:	.asciiz	"dowhile: "
string_1	:	.asciiz	"arrayvalue:"
string_2	:	.asciiz	"The end!!"
globala	:	.space	40
.text 
j	main
fib:
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
	sw	$a0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	1
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	beq	$s1,	$s2,	ifbegin_0
	j	ifend_0
	ifbegin_0:
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	1
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	add	$v0,	$s0,	$0
	j	elseend_0
	ifend_0:
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	0
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	beq	$s1,	$s2,	ifbegin_1
	j	ifend_1
	ifbegin_1:
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	1
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	add	$v0,	$s0,	$0
	j	elseend_1
	ifend_1:
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	1
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	sub	$s0,	$s1,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	add	$a0,	$s0,	$0
	jal	fib
	nop	
	addi	$sp,	$sp,	4
	add	$sp,	$sp,	0
	lw	$a3,	48($fp)
	lw	$a2,	44($fp)
	lw	$a1,	40($fp)
	lw	$a0,	36($fp)
	addi	$sp,	$sp,	-4
	sw	$v0,	0($sp)
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	2
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	sub	$s0,	$s1,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	add	$a0,	$s0,	$0
	jal	fib
	nop	
	addi	$sp,	$sp,	4
	add	$sp,	$sp,	0
	lw	$a3,	48($fp)
	lw	$a2,	44($fp)
	lw	$a1,	40($fp)
	lw	$a0,	36($fp)
	addi	$sp,	$sp,	-4
	sw	$v0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	add	$s0,	$s1,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	add	$v0,	$s0,	$0
	elseend_1:
	elseend_0:
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
main:
	add	$sp,	$sp,	-88
	add	$fp,	$sp,	$0
	addi	$t4,	$0,	5
	sw	$t4,	0($fp)
	li	$v0	5
	syscall
	sw	$v0,	4($fp)
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	add	$a0,	$s0,	$0
	jal	fib
	nop	
	addi	$sp,	$sp,	4
	add	$sp,	$sp,	-88
	lw	$a3,	136($fp)
	lw	$a2,	132($fp)
	lw	$a1,	128($fp)
	lw	$a0,	124($fp)
	addi	$sp,	$sp,	-4
	sw	$v0,	0($sp)
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
	addi	$s0,	$0,	1
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	3
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	sub	$s0,	$0,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	lw	$t0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$t1	$fp,	8
	sll	$t0,	$t0,	2
	add	$t0,	$t0,	$t1
	sw	$s0,	0($t0)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	2
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	1
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	lw	$t0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$t1	$fp,	8
	sll	$t0,	$t0,	2
	add	$t0,	$t0,	$t1
	sw	$s0,	0($t0)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	3
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	2
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	lw	$t0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$t1	$fp,	8
	sll	$t0,	$t0,	2
	add	$t0,	$t0,	$t1
	sw	$s0,	0($t0)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	3
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$t1,	$fp,	8
	sll	$s0,	$s0,	2
	add	$t0,	$t1,	$s0
	lw	$s0,	0($t0)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$t1,	$fp,	8
	sll	$s0,	$s0,	2
	add	$t0,	$t1,	$s0
	lw	$s0,	0($t0)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$t1,	$fp,	8
	sll	$s0,	$s0,	2
	add	$t0,	$t1,	$s0
	lw	$s0,	0($t0)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
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
	addi	$s0,	$0,	0
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	sw	$s0,	4($fp)
	forloopcondition_0:
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	blt	$s1,	$s2,	forloopbegin_0
	j	forloopend_0
	forloopbegin_0:
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	6
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	add	$s0,	$s1,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	10
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	add	$s0,	$s1,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	la	$t1,	globala
	lw	$t0,	0($sp)
	addi	$sp,	$sp,	4
	sll	$t0,	$t0,	2
	add	$t0,	$t0,	$t1
	sw	$s0,	0($t0)
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
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
	addi	$s0,	$0,	1
	sw	$s0,	0($sp)
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	add	$s0,	$s1,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	sw	$s0,	4($fp)
	j	forloopcondition_0
	forloopend_0:
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	10
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	sw	$s0,	4($fp)
	dowhilebegin_0:
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	la	$a0	string_0
	li	$v0	4
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
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
	sw	$a0,	0($sp)
	la	$a0	string_1
	li	$v0	4
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	la	$t1,	globala
	sll	$s0,	$s0,	2
	add	$t0,	$t1,	$s0
	lw	$s0,	0($t0)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	addi	$a0,	$s0,	0
	li	$v0	1
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	addi	$sp,	$sp,	-4
	addi	$s0,	$0,	2
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	sub	$s0,	$s1,	$s2
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	sw	$s0,	4($fp)
	lw	$s0,	4($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($fp)
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s0,	0($sp)
	addi	$sp,	$sp,	4
	addi	$sp,	$sp,	-4
	sw	$s0,	0($sp)
	lw	$s2,	0($sp)
	addi	$sp,	$sp,	4
	lw	$s1,	0($sp)
	addi	$sp,	$sp,	4
	bgt	$s1,	$s2,	dowhilebegin_0
	j	dowhileend_0
	dowhileend_0:
	addi	$sp,	$sp,	-4
	sw	$a0,	0($sp)
	la	$a0	string_2
	li	$v0	4
	syscall
	lw	$a0,	0($sp)
	addi	$sp,	$sp,	4
