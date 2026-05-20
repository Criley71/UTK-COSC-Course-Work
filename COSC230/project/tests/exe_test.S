.section .text
.global _start
.option norvc
_start:
arithmetic:
	addi sp, sp, -8
	addi t0, tp, 192
	add  a0, t0, sp
	sub  t4, s3, sp
        mul  s1, s2, s3
        div  t2, t3, t4
	rem  t5, t5, t5
bitwise:
	and  a0, a1, a2
	andi t0, t1, 13
	or   s0, s1, s2
	ori  t0, t1, 13
	xor  a0, a1, a2
	xori t0, t1, 13
	srl  t1, t2, t3
	srli t1, t2, 3
	sra  a0, a1, a2
	srai t0, t1, 3
	sll  a1, a2, a3
	slli a2, a3, 3
loads:
	lb   a0, -8(sp)
	lbu  a1, -16(sp)
	lh   a2, 0(sp)
	lhu  a3, 3(sp)
	lw   a4, 16(sp)
	lwu  a5, 32(sp)
	ld   a6, 256(sp)
stores:
	sb   s0, -8(s3)
        sh   s2, -16(s3)
        sw   s4, -32(s3)
	sd   s5, 128(s3)
utypes:
	auipc t5, 0x3124
	lui   s5, 0x9344
	auipc t6, 0
jumps:
	jalr   ra, 8(t6)
	jal    ra, branches
branches:
	beq    s0, s1, 1f
1:	blt    s5, t5, 1f
1:	ble    zero, zero, 1f
1:	bgt    t0, t1, 1f
1:	bge    gp, sp, 1f
1:	bltu   a0, a1, 1f
1:	bgtu   t0, t5, 1f
1:	bne    zero, zero, branches

system:
	ecall

illegal:
.word 0xdeadbeef
