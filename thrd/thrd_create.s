	/*    ---------- ---------- thread_create ---------- ---------- */

	/*
	*     主线程的 id=0 ，但是实际上是存放在 tids[1] 这个位置 ， 要不我搞一个映射：返回值是 cnt-1
	*     父线程不用管，只用返回 0 就行了，主要是要改一下子线程
	*/
thrd_create: # @thrd_create
	addi  sp, sp, -80
	sd    ra, 72(sp)                                                     # 8-byte Folded Spill
	sd    s0, 64(sp)                                                     # 8-byte Folded Spill
	addi  s0, sp, 80

.Lpcrel_hi0:
	auipc a0, %pcrel_hi(tids)
	addi  a0, a0, %pcrel_lo(.Lpcrel_hi0)
	lw    a0, 0(a0)
	bnez  a0, .LBB1_2
	j     .LBB1_1 // if tids[0] == 0
.LBB1_1:
	li    a0, 178
	call  syscall // gettid
.Lpcrel_hi1:
	auipc a1, %pcrel_hi(tids)
	addi  a1, a1, %pcrel_lo(.Lpcrel_hi1)
	lw    a2, 0(a1)
	addiw a2, a2, 1
	sw    a2, 0(a1)
	slli  a2, a2, 2
	add   a1, a1, a2
	sw    a0, 0(a1) // tids[++tids[0]] = gettid
	j     .LBB1_2
.LBB1_2:
.Lpcrel_hi2:
	auipc a0, %pcrel_hi(tids)
	addi  a0, a0, %pcrel_lo(.Lpcrel_hi2)
	lw    a0, 4(a0)
	sd    a0, -72(s0)                                                    # 8-byte Folded Spill
	li    a0, 178
	call  syscall
	mv    a1, a0
	ld    a0, -72(s0)                                                    # 8-byte Folded Reload
	beq   a0, a1, .LBB1_4
	j     .LBB1_3 // if tids[1] != gettid
.LBB1_3:
	li    a0, -1
	sw    a0, -20(s0)
	j     .main_fail
.LBB1_4:
	addi  a0, s0, -32
	li    a1, 16
	lui   a2, 16
	call  posix_memalign
	ld    a0, -32(s0)
	bnez  a0, .LBB1_6
	j     .LBB1_5 // if stack == NULL
.LBB1_5:
	li    a0, -1
	sw    a0, -20(s0)
	j     .main_fail
.LBB1_6:
	ld    a0, -32(s0)
	lui   a1, 16
	add   a0, a0, a1
	sd    a0, -40(s0) // stack_top = stack + 65536
	li    a0, 104 // 用于保存 ra、 s0~s11 等
	call  malloc
	sd    a0, -48(s0)
	ld    a0, -48(s0)
	bnez  a0, .LBB1_8
	j     .LBB1_7 // if regs == NULL
.LBB1_7:
	li    a0, -1
	sw    a0, -20(s0)
	j     .main_fail
.LBB1_8:

	//    FIXME 先保存寄存器
	//    可以用的寄存器：a1、a2、a3
	ld    a0, -48(s0)

	ld    a1, -8(s0) // ra
	sd    a1, 0(a0)

	ld    a1, -16(s0) // s0'
	sd    a1, 8(a0)

	sd    s1, 16(a0)
	sd    s2, 24(a0)
	sd    s3, 32(a0)
	sd    s4, 40(a0)
	sd    s5, 48(a0)
	sd    s6, 56(a0)
	sd    s7, 64(a0)
	sd    s8, 72(a0)
	sd    s9, 80(a0)
	sd    s10, 88(a0)
	sd    s11, 96(a0)

	ld    a1, -32(s0) // stack
	sd    a1, 104(a0)

	ld    a1, -40(s0) // stack_top
	sd    a1, 112(a0)

	ld    a1, -56(s0) // cnt
	sd    a1, 116(a0)

	add   a1, sp, s0
	sd    a1, 120(a0)

	//    FIXME 寄存器保存完毕

	lui   a0, 4945
	addiw a0, a0, -256
	sw    a0, -52(s0)
.Lpcrel_hi3:
	auipc a0, %pcrel_hi(tids)
	addi  a0, a0, %pcrel_lo(.Lpcrel_hi3)
	lw    a1, 0(a0) // cnt = ++tids[0]
	addiw a1, a1, 1
	sw    a1, 0(a0)
	sw    a1, -56(s0)
	ld    a1, -40(s0) // a1 = stack_top
	lw    a2, -52(s0) // a2 = flags
	ld    a3, -48(s0) // a3 = regs buffer 给 dummy 传入的参数
	lw    a4, -56(s0) // cnt
	slli  a5, a4, 2
	add   a4, a0, a5 // a4 = &tids[cnt]
.Lpcrel_hi4:
	auipc a0, %pcrel_hi(futexs)
	addi  a0, a0, %pcrel_lo(.Lpcrel_hi4)
	add   a6, a0, a5 // a6 = &futexs[cnt]
.Lpcrel_hi5:
	auipc a0, %pcrel_hi(.son_leave)
	addi  a0, a0, %pcrel_lo(.Lpcrel_hi5)
	li    a5, 0 // a5 = NULL （ 应该是设置 tls 啥的 ）
	call  clone // TODO 这里很重要
	j     .main_leave // 主线程离开

.main_fail: # error 情况退出
	lw    a0, -20(s0)
	ld    ra, 72(sp)                                                     # 8-byte Folded Reload
	ld    s0, 64(sp)                                                     # 8-byte Folded Reload
	addi  sp, sp, 80
	ret

.main_leave: # 能到达这里的，只有主线程
	sw    a0, -60(s0) // ret = clone 的返回值
	lw    a0, -60(s0)
	blez  a0, .main_fail
	j     .LBB1_9 // if ret > 0
.main_success:
	auipc a1, %pcrel_hi(tids)
	addi  a1, a1, %pcrel_lo(.main_success)
	lw    a0, 0(a1)
	ld    ra, 72(sp)                                                     # 8-byte Folded Reload
	ld    s0, 64(sp)                                                     # 8-byte Folded Reload
	addi  sp, sp, 80
	jr    ra


.son_leave: # 能达到这里的，只有子线程，这里得到 a0 = regs 缓冲区 // FIXME
	//    也就是有一个问题：子线程什么情况下会结束 ---> 碰到 ret 会结束，因此函数的返回就不能用 ret ，而应当使用 jr
	ld    ra, 0(a0)
	ld    s0, 8(a0)
	//    ld s1, 16(a0) ，暂时不复制，因为我下面会稍微用到
	ld    s2, 24(a0)
	ld    s3, 32(a0)
	ld    s4, 40(a0)
	ld    s5, 48(a0)
	ld    s6, 56(a0)
	ld    s7, 64(a0)
	ld    s8, 72(a0)
	ld    s9, 80(a0)
	ld    s10, 88(a0)
	ld    s11, 96(a0)

	//    s1 暂存 a0，也就是 regs 表
	mv    s1, a0

	//    复制 main 的 栈 1024B
	ld    a1, 120(s1)
	addi  sp, sp, -1024
	mv    a0, sp
	li    a2, 1024
	call  memcpy

	//    恢复 s1，并 free regs
	addi  sp, sp, -16
	ld    a0, 16(s1)
	sw    a0, 0(sp)
	mv    a0, s1
	call  free
	ld    s1, 0(sp)
	addi  sp, sp, 16

.Lpcrel_hi6:
	auipc a0, %pcrel_hi(.tids)
	addi  a0, a0, %pcrel_lo(.Lpcrel_hi6)
	lw    a0, 0(a0) // cnt = tids[0]

	//    但是我如果外面传入的话，我要先 ++
	addi  a0, a0, -1
	jr    ra

tids:
	.zero 80

futexs:
	.zero 80