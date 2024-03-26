	.file               "my_thrd.c"
	.option             pic
	.attribute          arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute          unaligned_access, 0
	.attribute          stack_align, 16

	.text
	.globl              tids
	.bss
	.align              3
	.type               tids, @object
	.size               tids, 80
tids:
	.zero               80

	.globl              futexs
	.align              3
	.type               futexs, @object
	.size               futexs, 80
futexs:
	.zero               80

	.globl              tmp_mem
	.align              3
	.type               tmp_mem, @object
	.size               tmp_mem, 1600
tmp_mem:
	.zero               1600

	.text
	.align              1
	.globl              dummy
	.type               dummy, @function
dummy:
.LFB6:
	.cfi_startproc
	addi                sp,sp,-16
	.cfi_def_cfa_offset 16
	sd                  s0,8(sp)
	.cfi_offset         8, -8
	addi                s0,sp,16
	.cfi_def_cfa        8, 0
	lla                 a5,tids
	li                  a4,23
	sw                  a4,20(a5)
	lla                 a5,tmp_mem
	li                  a4,97
	sb                  a4,10(a5)
	li                  a5,0
	mv                  a0,a5
	ld                  s0,8(sp)
	.cfi_restore        8
	.cfi_def_cfa        2, 16
	addi                sp,sp,16
	.cfi_def_cfa_offset 0
	jr                  ra
	.cfi_endproc
.LFE6:
	.size               dummy, .-dummy

	.align              1
	.globl              thrd_create
	.type               thrd_create, @function
thrd_create:
.LFB7:
	.cfi_startproc
	addi                sp,sp,-64
	.cfi_def_cfa_offset 64
	sd                  ra,56(sp)
	sd                  s0,48(sp)
	sd                  s1,40(sp)
	.cfi_offset         1, -8
	.cfi_offset         8, -16
	.cfi_offset         9, -24
	addi                s0,sp,64
	.cfi_def_cfa        8, 0
	lla                 a5,tids
	lw                  a5,0(a5)
	bne                 a5,zero,.L4
	li                  a0,178
	call                syscall@plt
	mv                  a3,a0
	lla                 a5,tids
	lw                  a5,0(a5)
	addiw               a5,a5,1
	sext.w              a4,a5
	lla                 a5,tids
	sw                  a4,0(a5)
	lla                 a5,tids
	lw                  a5,0(a5)
	sext.w              a4,a3
	lla                 a3,tids
	slli                a5,a5,2
	add                 a5,a3,a5
	sw                  a4,0(a5)
.L4:
	lla                 a5,tids
	lw                  a5,4(a5)
	mv                  s1,a5
	li                  a0,178
	call                syscall@plt
	mv                  a5,a0
	beq                 s1,a5,.L5
	li                  a5,-1
	j                   .L8
.L5:
	lla                 a5,STACK_SIZE.0
	ld                  a4,0(a5)
	addi                a5,s0,-64
	mv                  a2,a4
	li                  a1,16
	mv                  a0,a5
	call                posix_memalign@plt
	ld                  a5,-64(s0)
	bne                 a5,zero,.L7
	li                  a5,-1
	j                   .L8
.L7:
	ld                  a4,-64(s0)
	lla                 a5,STACK_SIZE.0
	ld                  a5,0(a5)
	add                 a5,a4,a5
	sd                  a5,-40(s0)
	li                  a5,20254720
	addi                a5,a5,-256
	sw                  a5,-44(s0)
	lla                 a5,tids
	lw                  a5,0(a5)
	addiw               a5,a5,1
	sext.w              a4,a5
	lla                 a5,tids
	sw                  a4,0(a5)
	lla                 a5,tids
	lw                  a5,0(a5)
	sw                  a5,-48(s0)
	lw                  a5,-48(s0)
	slli                a4,a5,2
	lla                 a5,tids
	add                 a3,a4,a5
	lw                  a5,-48(s0)
	slli                a4,a5,2
	lla                 a5,futexs
	add                 a5,a4,a5
	lw                  a2,-44(s0)
	mv                  a6,a5
	li                  a5,0
	mv                  a4,a3
	li                  a3,0
	ld                  a1,-40(s0)
	lla                 a0,dummy
	call                clone@plt
	mv                  a5,a0
	sw                  a5,-52(s0)
	lw                  a5,-48(s0)
.L8:
	mv                  a0,a5
	ld                  ra,56(sp)
	.cfi_restore        1
	ld                  s0,48(sp)
	.cfi_restore        8
	.cfi_def_cfa        2, 64
	ld                  s1,40(sp)
	.cfi_restore        9
	addi                sp,sp,64
	.cfi_def_cfa_offset 0
	jr                  ra
	.cfi_endproc
.LFE7:
	.size               thrd_create, .-thrd_create

	.section            .rodata
	.align              3
	.type               STACK_SIZE.0, @object
	.size               STACK_SIZE.0, 8
STACK_SIZE.0:
	.dword              65536

.ident "GCC:
	(Debian             13.2.0-1) 13.2.0"
	.section            .note.GNU-stack,"",@progbits
