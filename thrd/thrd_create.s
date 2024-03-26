	.file               "my_thrd.c"
	.option             pic
	.attribute          arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute          unaligned_access, 0
	.attribute          stack_align, 16

# ########## ########## 下面是 .bss 段 ########## ########## #

	.text
	.globl              tids
	.bss
	.align              3
	.set                .LANCHOR_tids,. + 0                                            # 设置锚点
	.type               tids, @object
	.size               tids, 80
tids:
	.zero               80

	.globl              futexs
	.align              3
	.set                .LANCHOR_futexs,. + 0                                          # 设置锚点
	.type               futexs, @object
	.size               futexs, 80
futexs:
	.zero               80

	.globl              tmp_mem
	.align              3
	.set                .LANCHOR_tmp_mem,. + 0                                         # 设置锚点
	.type               tmp_mem, @object
	.size               tmp_mem, 1600
tmp_mem:
	.zero               1600

# ########## ########## 下面是 .text 段 ########## ########## #

# ---------- ---------- son_leave ---------- ----------

	.text
	.align              1
	.globl              son_leave
	.type               son_leave, @function
son_leave:
	.cfi_startproc
# 这个时候，a0 传进来的是 cnt
	lla                 a1, .LANCHOR_tmp_mem
	lla                 a0, .LANCHOR_tids
	lw                  a0, 0(a0)                                                      # a0 = cnt

	slliw               a2, a0, 8
	add                 a1, a1,a2                                                      # a1 = tids[cnt]

.LB_restore:
	ld                  ra,0(a1)
	ld                  s0,8(a1)
	ld                  s1,16(a1)
	ld                  s2,24(a1)
	ld                  s3,32(a1)
	ld                  s4,40(a1)
	ld                  s5,48(a1)
	ld                  s6,56(a1)
	ld                  s7,64(a1)
	ld                  s8,72(a1)
	ld                  s9,80(a1)
	ld                  s10,88(a1)
	ld                  s11,96(a1)

	mv                  s1,a1                                                          # s1 = tids[cnt]
	mv                  s0,a0                                                          # s0 = cnt

	ld                  a1,8(a1)                                                       # a1 = src
	addi                sp,sp,-800                                                     # 栈是 800B 的大小
	mv                  a0,sp                                                          # a0 = dest
	li                  a2,800                                                         # a2 = 800
	call                memcpy@plt

	mv                  a1,s1                                                          # a1 = tids[cnt]
	mv                  a0,s0                                                          # s0 = cnt

	ld                  ra,0(a1)
	ld                  s0,8(a1)                                                       # 这个 s0 和 s1 按道理来说应该是没有改变的
	ld                  s1,16(a1)
	ld                  ra,0(a1)
	li                  a0, 0                                                          # 生成返回值
	jr                  ra

	.cfi_endproc
	.size               son_leave, .-son_leave

# ---------- ---------- thrd_create ---------- ----------

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
	bne                 a5,zero,.L4                                                    # if tids[0] == 0
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
	j                   .fail                                                          # if tids[1] != gettid
.L5:
	lla                 a5,STACK_SIZE.0
	ld                  a4,0(a5)
	addi                a5,s0,-64
	mv                  a2,a4
	li                  a1,16
	mv                  a0,a5
	call                posix_memalign@plt
	ld                  a5,-64(s0)
	bne                 a5,zero,.L7                                                    # if stack != NULL
	j                   .fail
.L7:
	ld                  a4,-64(s0)
	lla                 a5,STACK_SIZE.0
	ld                  a5,0(a5)
	add                 a5,a4,a5
	sd                  a5,-40(s0)                                                     # top
	li                  a5,20254720
	addi                a5,a5,-256
	sw                  a5,-44(s0)                                                     # flag
	lla                 a5,tids
	lw                  a5,0(a5)
	addiw               a5,a5,1
	sext.w              a4,a5
	lla                 a5,tids
	sw                  a4,0(a5)                                                       # ++tids[0]
	lla                 a5,tids
	lw                  a5,0(a5)
	sw                  a5,-48(s0)                                                     # cnt

.LBB_backup:
	lla                 a1, tmp_mem
	lw                  a0, -48(s0)                                                    # cnt
	slliw               a0, a0, 8
	add                 a1,a1,a0
	ld                  ra, -8(s0)                                                     # r0
	sd                  ra,0(a1)
	ld                  a0, -16(s0)
	sd                  a0,8(a1)
	ld                  a0, -24(a1)                                                    # s1
	sd                  a0,16(a1)
	sd                  s2,24(a1)
	sd                  s3,32(a1)
	sd                  s4,40(a1)
	sd                  s5,48(a1)
	sd                  s6,56(a1)
	sd                  s7,64(a1)
	sd                  s8,72(a1)
	sd                  s9,80(a1)
	sd                  s10,88(a1)
	sd                  s11,96(a1)

.LB_clone:
	lw                  a5,-48(s0)
	slli                a4,a5,2
	lla                 a5,tids
	add                 a3,a4,a5
	lw                  a5,-48(s0)
	slli                a4,a5,2
	lla                 a5,futexs
	add                 a5,a4,a5
	lw                  a2,-44(s0)                                                     # a2 = flag
	mv                  a6,a5                                                          # a6 = futexs[cnt]
	li                  a5,0                                                           # a5 = NULL
	mv                  a4,a3                                                          # a4 = &tids[cnt]
# lla a3, tids # 传入参数是 tids
# lw a3, -48(s0)
	li                  a3, 0                                                          # a3 = 0
# li a3,0 # a3 = NULL
	ld                  a1,-40(s0)                                                     # a1 = stack_top
	lla                 a0,son_leave                                                   # a0 = son_leave
	call                clone@plt                                                      # TODO 重点
	j                   .main_leave

.main_leave:
	lw                  a5,-48(s0)
	mv                  a0, a5
	addi                a0, a0, -1
	ld                  ra,56(sp)
	ld                  s0,48(sp)
	ld                  s1,40(sp)
	addi                sp,sp,64
	jr                  ra

.fail:
	li                  a0, -1
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
