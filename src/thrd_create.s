	.file          "thrd_create.c"
	.option        pic
	.attribute     arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute     unaligned_access, 0
	.attribute     stack_align, 16

	.text
	.align         1
	.globl         __get_tid
	.type          __get_tid, @function
__get_tid:
.LFB6__get_tid:
	.cfi_startproc

# 判断是不是还没有创建过线程
	lla            t1, tids
	lw             t1, 0(t1)                                                      # cnt
	bnez           t1, .LFB6__get_tid_start
	li             a0, 0
	jr             ra

.LFB6__get_tid_start:
	addi           sp, sp, -8
	sd             ra, 0(sp)
	call           gettid@plt
	mv             a7, a0
.while_init__get_tid:
	lla            a1, tids                                                       # a1 = tids 基址
	lw             a0, 0(a1)                                                      # a0 = cnt
	j              .while_cond__get_tid
.while_block__get_tid:
	addiw          a0, a0, -1
.while_cond__get_tid:
	blez           a0, .return__get_tid
	mv             a2, a0
	slliw          a2, a2, 2
	add            a2, a1, a2
	lw             a2, 0(a2)                                                      # a2 = tids[a0]
	bne            a2, a7, .while_block__get_tid

.return__get_tid:
# 返回 a0 ，这里 a0 就是循环变量
	ld             ra, 0(sp)
	addi           sp, sp, 8
	addiw          a0, a0, -1                                                     # 下标 与 tid 正好差一位
	jr             ra
	.cfi_endproc
.LFE6__get_tid:
	.size          __get_tid, .-__get_tid




	.text
	.align         1
	.globl         thrd_create
	.type          thrd_create, @function
thrd_create:
.__thrd_create_prepare: # 准备阶段
	.cfi_startproc

.__thrd_create_if_not_main_then_return:
	addi           sp, sp, -16
	sd             ra, 0(sp)
	sd             a0, 8(sp)                                                      # num
	call           __get_tid@plt
	ld             ra, 0(sp)
	ld             a1, 8(sp)                                                      # a1 = num
	addi           sp, sp, 16
	beqz           a0, .__thrd_create_fork_prepare                                # a0 = __get_tid()
# 是子线程调用了 thrd_create
	jr             ra                                                             # 注意一下，我上面没有修改过 a0

.__thrd_create_fork_prepare:
# 也只有主线程能进来这里，那么这一部分是 顺序执行的
	sub            a5, s0, sp                                                     # a5 = 主线程的 caller 栈 s0 - sp
	lla            t1, tmp_mem
	sd             a5, 0(t1)                                                      # s0 - sp
	sd             a1, 8(t1)                                                      # num
	sd             ra, 16(t1)                                                     # ra

.__thrd_create_if_cnt_0:
	lla            t1, tids
	lw             t2, 0(t1)
	bnez           t2, .__thrd_create_fork
# 如果是 0 ，那么就要初始化一下，总线程数 ++ ，tids[1] = gettid
	li             t2, 1
	sw             t2, 0(t1)
	li             a0,178
	call           syscall@plt
	lla            t1, tids
	sw             a0, 4(t1)
	j              .__thrd_create_fork

.__thrd_create_fork:
	lla            t1, tmp_mem
	ld             a0, 0(t1)
	call           _thrd_create@plt                                               # fork
	bnez           a0, .__thrd_create_son_while
	j              .__thrd_create_main

.__thrd_create_son_while:
	nop
	lla            t1, tmp_mem
	ld             a1, 8(t1)                                                      # num
	bgtz           a1, .__thrd_create_son_while
	lla            t1, tmp_mem
	ld             ra, 16(t1)                                                     # ra
	jr             ra                                                             # 这个时候 a0=id 是返回值
# 注意，这个时候不要破坏 a0 , 因为 a0 是返回值，也就是 tid


.__thrd_create_main: # 主线程
	lla            t1, tmp_mem
	ld             a1, 8(t1)
	addi           a1, a1, -1
	sd             a1, 8(t1)
	bgtz           a1, .__thrd_create_fork
	lla            t1, tmp_mem
	ld             ra, 16(t1)                                                     # ra

	lla            t1, tmp_mem
	li             t2, 0

	li             a0, 0                                                          # 主线程返回 0
	jr             ra
	.cfi_endproc
.LFE6:
	.size          thrd_create, .-thrd_create

.ident "GCC:
	(Debian        13.2.0-23) 13.2.0"
	.section       .note.GNU-stack,"",@progbits
