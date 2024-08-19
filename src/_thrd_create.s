	.file               "my_thrd.c"
	.option             pic
	.attribute          arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute          unaligned_access, 0
	.attribute          stack_align, 16

# ########## ########## 下面是 .text 段 ########## ########## #

# ---------- ---------- son_leave ---------- ----------

	.text
	.align              1
	.globl              son_leave
	.type               son_leave, @function
son_leave:
	.cfi_startproc
	mv                  s8, a0                                                         # 保存一下 cnt 也就是 tid
	mv                  s11, sp                                                        # s11 = son sp

	lla                 a0, tmp_mem
	slliw               a2, s8, 8                                                      # cnt << 8
	add                 a1, a0, a2                                                     # a1 = tmp_mem[cnt]
	ld                  s10, 120(a1)                                                   # caller 栈大小 = tmp_mem[cnt][120]
	sub                 sp, sp, s10                                                    # 准备复制
	mv                  s9, a1                                                         # 暂存 s9 = tmp_mem[cnt]
	ld                  s7, 8(a1)                                                      # s7 = s0' 也就是 caller 的 s0
	sub                 s7, s7, s10                                                    # s7 = s7 - s10 得到 caller 的 sp

.LB_copy_frame:
	mv                  a0, sp                                                         # dest
	mv                  a1, s7                                                         # src
	mv                  a2, s10                                                        # 复制这么多的字节
	call                memcpy@plt                                                     # 浅拷贝 caller 的栈

	mv                  a7, s11                                                        # a7 看起来比较少用，son sp

.LB_restore:
	mv                  a1,s9                                                          # a1 = tmp_mem[cnt]
	mv                  a0,s8                                                          # FIXME s0 = cnt 返回值，后面不要改了

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

	ld                  ra, 0(a1)
	.cfi_restore        1
	mv                  s0, a7                                                         # 恢复 s0
	.cfi_restore        8
	ld                  s1, 16(a1)
	.cfi_restore        9
# li a0, 0 # 生成返回值
# a0 这个时候没有动过，注意
	addiw               a0, a0, -1
	jr                  ra
	.cfi_endproc
	.size               son_leave, .-son_leave

# ---------- ---------- _thrd_create ---------- ----------

	.align              1
	.globl              _thrd_create
	.type               _thrd_create, @function
_thrd_create:
.LFB7:
	.cfi_startproc
	addi                sp,sp,-64
	.cfi_def_cfa_offset 64
	sd                  ra,56(sp)
	sd                  s0,48(sp)
	sd                  s1,40(sp)
	sd                  a0, 32(sp)                                                     # a0 是传入的参数 = caller(s0-sp)
	.cfi_offset         1, -8
	.cfi_offset         8, -16
	.cfi_offset         9, -24
	addi                s0,sp,64
	.cfi_def_cfa        8, 0

.L5:
	lla                 a5,STACK_SIZE
	ld                  a4,0(a5)
	addi                a5,s0,-64
	mv                  a2,a4                                                          # s2 = stack_size
	li                  a1,16                                                          # a1 = 16 (align)
	mv                  a0,a5                                                          # a0 = a5 = &stack
	call                posix_memalign@plt
	ld                  a5,-64(s0)
	bnez                a5, .L7                                                        # if stack != NULL
	li                  a0, -1                                                         # fail
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

.L7:
	ld                  a4,-64(s0)
	lla                 a5,STACK_SIZE
	ld                  a5,0(a5)
	add                 a5,a4,a5
	sd                  a5,-40(s0)                                                     # top
	li                  a5,20254720
	addi                a5,a5,-256
	sw                  a5,-44(s0)                                                     # flag

	lla                 t1, tids
	lw                  a5, 0(t1)
	addiw               a5, a5, 1
	sw                  a5, 0(t1)
	sw                  a5, -48(s0)                                                    # cnt

.LBB_backup:
	lla                 a1, tmp_mem
	lw                  a0, -48(s0)                                                    # cnt
	slliw               a0, a0, 8
	add                 a1,a1,a0                                                       # tmp_mem[cnt]
	ld                  ra, -8(s0)                                                     # ra
	sd                  ra,0(a1)
	ld                  a0, -16(s0)                                                    # s0'
	sd                  a0, 8(a1)
	ld                  a0, -24(s0)                                                    # s1
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

	ld                  a0, -32(s0)                                                    # caller 的 s0 - sp 栈大小
	sd                  a0, 120(a1)

# addi a0, sp, 64 # 因为 s0 是栈底
	mv                  a0, s0                                                         # 因为 s0 是栈底
	sd                  a0, 104(a1)                                                    # sp + 64 当前栈的 栈底

	ld                  a0, -64(s0)
	sd                  a0, 112(a1)                                                    # stack

.LB_clone:
	lw                  a5,-48(s0)
	slli                a4,a5,2
	lla                 a5,tids
	add                 a3,a4,a5
	lw                  a2,-44(s0)                                                     # a2 = flag
	li                  a6, 0                                                          # a6 = NULL
	li                  a5,0                                                           # a5 = NULL
	mv                  a4,a3                                                          # a4 = &tids[cnt] # 存放的是线程 tid
	lw                  a3,-48(s0)                                                     # a3 = 传入子线程的 id
	ld                  a1,-40(s0)                                                     # a1 = stack_top
	lla                 a0,son_leave                                                   # a0 = son_leave
	call                clone@plt                                                      # TODO 重点
	j                   .main_leave

.main_leave:
	li                  a0, 0                                                          # main thread 总是 == 0
	ld                  ra,56(sp)
	.cfi_restore        1
	ld                  s0,48(sp)
	.cfi_restore        8
	ld                  s1,40(sp)
	.cfi_restore        9
	addi                sp,sp,64
	.cfi_def_cfa_offset 0
	jr                  ra

	.cfi_endproc
.LFE7:
	.size               _thrd_create, .-_thrd_create

.ident "GCC:
	(Debian             13.2.0-1) 13.2.0"
	.section            .note.GNU-stack,"",@progbits
