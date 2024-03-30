	.file               "tls_test.c"
	.option             pic
	.attribute          arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute          unaligned_access, 0
	.attribute          stack_align, 16

	.text
	.globl              counter
	.section            .tbss,"awT",@nobits
	.align              2
	.type               counter, @object
	.size               counter, 4
counter:
	.zero               4

	.section            .rodata
	.align              3
.LC0:
.string "Thread %d, Counter:
	%d\n"
	.text
	.align              1
	.globl              thread_function
	.type               thread_function, @function
thread_function:
.LFB6:
	.cfi_startproc
	addi                sp,sp,-32
	.cfi_def_cfa_offset 32
	sd                  ra,24(sp)
	sd                  s0,16(sp)
	.cfi_offset         1, -8
	.cfi_offset         8, -16
	addi                s0,sp,32
	.cfi_def_cfa        8, 0
	sd                  a0,-24(s0)
	ld                  a5,-24(s0)
	sext.w              a4,a5
	lui                 a5,%tprel_hi(counter)
	add                 a5,a5,tp,%tprel_add(counter)
	lw                  a5,%tprel_lo(counter)(a5)
	addw                a5,a4,a5
	sext.w              a4,a5
	lui                 a5,%tprel_hi(counter)
	add                 a5,a5,tp,%tprel_add(counter)
	sw                  a4,%tprel_lo(counter)(a5)
	ld                  a5,-24(s0)
	sext.w              a4,a5
	lui                 a5,%tprel_hi(counter)
	add                 a5,a5,tp,%tprel_add(counter)
	lw                  a5,%tprel_lo(counter)(a5)
	mv                  a2,a5
	mv                  a1,a4
	lla                 a0,.LC0
	call                printf@plt
	li                  a5,0
	mv                  a0,a5
	ld                  ra,24(sp)
	.cfi_restore        1
	ld                  s0,16(sp)
	.cfi_restore        8
	.cfi_def_cfa        2, 32
	addi                sp,sp,32
	.cfi_def_cfa_offset 0
	jr                  ra
	.cfi_endproc
.LFE6:
	.size               thread_function, .-thread_function

	.align              1
	.globl              main
	.type               main, @function
main:
.LFB7:
	.cfi_startproc
	addi                sp,sp,-32
	.cfi_def_cfa_offset 32
	sd                  ra,24(sp)
	sd                  s0,16(sp)
	.cfi_offset         1, -8
	.cfi_offset         8, -16
	addi                s0,sp,32
	.cfi_def_cfa        8, 0
	addi                a5,s0,-24
	li                  a3,1
	lla                 a2,thread_function
	li                  a1,0
	mv                  a0,a5
	call                pthread_create@plt
	addi                a5,s0,-32
	li                  a3,2
	lla                 a2,thread_function
	li                  a1,0
	mv                  a0,a5
	call                pthread_create@plt
	ld                  a5,-24(s0)
	li                  a1,0
	mv                  a0,a5
	call                pthread_join@plt
	ld                  a5,-32(s0)
	li                  a1,0
	mv                  a0,a5
	call                pthread_join@plt
	li                  a5,0
	mv                  a0,a5
	ld                  ra,24(sp)
	.cfi_restore        1
	ld                  s0,16(sp)
	.cfi_restore        8
	.cfi_def_cfa        2, 32
	addi                sp,sp,32
	.cfi_def_cfa_offset 0
	jr                  ra
	.cfi_endproc
.LFE7:
	.size               main, .-main

.ident "GCC:
	(Debian             13.2.0-1) 13.2.0"
	.section            .note.GNU-stack,"",@progbits
