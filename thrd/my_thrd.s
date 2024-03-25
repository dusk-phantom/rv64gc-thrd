	.file      "my_thrd.c"
	.option    pic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16

# ########## ########## 下面是 .bss 段 ########## ########## #

	.text
	.globl     tids
	.bss
	.align     3
	.type      tids, @object
	.size      tids, 80
tids:
	.zero      80
	.globl     futexs
	.align     3
	.type      futexs, @object
	.size      futexs, 80
futexs:
	.zero      80

# ########## ########## 下面是 .text 段 ########## ########## #

	.text
	.align     1
	.globl     dummy
	.type      dummy, @function
dummy:
.LFB6:
	mv         s1, a0

	ld         a2, 104(s1)
	addi       sp, sp, -800
	mv         a0, sp
	li         a2, 800
	call       memcpy@plt

	addi       sp,sp, -16
	ld         a0, 0(s1)                                                      # 暂存 ra
	sd         ra, 8(sp)
	ld         a0, 16(s1)                                                     # 暂存 s1
	sd         a0, 0(sp)

	mv         a1, s1
# ld ra,0(a1)
	ld         s0,8(a1)
# ld s1,16(a1)
	ld         s2,24(a1)
	ld         s3,32(a1)
	ld         s4,40(a1)
	ld         s5,48(a1)
	ld         s6,56(a1)
	ld         s7,64(a1)
	ld         s8,72(a1)
	ld         s9,80(a1)
	ld         s10,88(a1)
	ld         s11,96(a1)

	mv         a0, a1
	call       free@plt

	ld         ra, 0(sp)
	ld         s1, 16(sp)
	addi       sp, sp, 16
	jr         ra
.LFE6:
	.size      dummy, .-dummy

# ---------- ---------- thrd_create ---------- ----------

	.align     1
	.globl     thrd_create
	.type      thrd_create, @function
thrd_create:
.LFB7:
	addi       sp,sp,-80
	sd         ra,72(sp)
	sd         s0,64(sp)
	sd         s1,56(sp)
	addi       s0,sp,80
	lla        a5,tids
	lw         a5,0(a5)
	bne        a5,zero,.L4                                                    # if tids[0] == 0
	li         a0,178
	call       syscall@plt
	mv         a3,a0
	lla        a5,tids
	lw         a5,0(a5)
	addiw      a5,a5,1
	sext.w     a4,a5
	lla        a5,tids
	sw         a4,0(a5)
	lla        a5,tids
	lw         a5,0(a5)
	sext.w     a4,a3
	lla        a3,tids
	slli       a5,a5,2
	add        a5,a3,a5
	sw         a4,0(a5)                                                       # tids[++tids[0]] = gittid
.L4:
	lla        a5,tids
	lw         a5,4(a5)
	mv         s1,a5
	li         a0,178
	call       syscall@plt
	mv         a5,a0
	beq        s1,a5,.L5                                                      # if tids[1] == gettid
	j          .fail
.L5:
	lla        a5,STACK_SIZE.0
	ld         a4,0(a5)
	addi       a5,s0,-72
	mv         a2,a4
	li         a1,16
	mv         a0,a5
	call       posix_memalign@plt
	ld         a5,-72(s0)
	bne        a5,zero,.L7                                                    # if stack == NULL
	j          .fail
.L7:
	ld         a4,-72(s0)
	lla        a5,STACK_SIZE.0
	ld         a5,0(a5)
	add        a5,a4,a5
	sd         a5,-40(s0)                                                     # top
	li         a5,20254720
	addi       a5,a5,-256
	sw         a5,-44(s0)                                                     # flag
	li         a0,128
	call       malloc@plt
	mv         a5,a0
	sd         a5,-56(s0)
	ld         a5,-56(s0)
	bne        a5,zero,.L8                                                    # if regs == NULL
	j          .fail
.L8:
	lla        a5,tids
	lw         a5,0(a5)
	addiw      a5,a5,1
	sext.w     a4,a5
	lla        a5,tids
	sw         a4,0(a5)
	lla        a5,tids
	lw         a5,0(a5)
	sw         a5,-60(s0)                                                     # cnt = ++tids[0]

.LB_backup: # TODO
	ld         a1, -56(s0)                                                    # regs
	ld         a2, -8(s0)
	sd         a2, 0(a1)                                                      # ra
	ld         a2, -16(s0)
	sd         a2, 8(a1)                                                      # s0'
	ld         a2, -24(s0)
	sd         a2, 16(a1)                                                     # s1
	sd         s2,24(a1)
	sd         s3,32(a1)
	sd         s4,40(a1)
	sd         s5,48(a1)
	sd         s6,56(a1)
	sd         s7,64(a1)
	sd         s8,72(a1)
	sd         s9,80(a1)
	sd         s10,88(a1)
	sd         s11,96(a1)
	sd         s0, 104(a1)

.LB_clone:
	lw         a5,-60(s0)
	slli       a4,a5,2
	lla        a5,tids
	add        a3,a4,a5
	lw         a5,-60(s0)
	slli       a4,a5,2
	lla        a5,futexs
	add        a5,a4,a5
	lw         a2,-44(s0)
	mv         a6,a5                                                          # futexs[cnt]
	li         a5,0                                                           # a5 = NULL
	mv         a4,a3                                                          # a4 = &tids[cnt]
	ld         a3,-56(s0)                                                     # a3 = regs
	ld         a1,-40(s0)                                                     # a1 = top
	lla        a0,dummy                                                       # a0 = dummy
	call       clone@plt                                                      # TODO 这里比较重要
	j          main_leave
# sw a5,-64(s0) # ret = clone 这个没用到

.fail:
	li         a0 , -1
	ld         ra,72(sp)
	ld         s0,64(sp)
	ld         s1,56(sp)
	addi       sp,sp,80
	jr         ra


main_leave:
	ld         a0, -60(s0)                                                    # 父线程返回子线程的 tid
	ld         ra,72(sp)
	ld         s0,64(sp)
	ld         s1,56(sp)
	addi       sp,sp,80
	jr         ra
.LFE7:
	.size      thrd_create, .-thrd_create


# ########## ########## 下面是 .rodate 段 ########## ########## #

	.section   .rodata
	.align     3
	.type      STACK_SIZE.0, @object
	.size      STACK_SIZE.0, 8
STACK_SIZE.0:
	.dword     65536
.ident "GCC:
	(Debian    13.2.0-1) 13.2.0"
	.section   .note.GNU-stack,"",@progbits
