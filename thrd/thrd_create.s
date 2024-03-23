dummy: # @dummy
	addi   sp, sp, -32
	sd     ra, 24(sp)                           # 8-byte Folded Spill
	sd     s0, 16(sp)                           # 8-byte Folded Spill
	addi   s0, sp, 32

	sd     a0, -24(s0)
	ld     a0, -24(s0)
	call   free

	li     a0, 0
	ld     ra, 24(sp)                           # 8-byte Folded Reload
	ld     s0, 16(sp)                           # 8-byte Folded Reload
	addi   sp, sp, 32
	ret

thrd_create: # @thrd_create
	addi   sp, sp, -80
	sd     ra, 72(sp)                           # 8-byte Folded Spill
	sd     s0, 64(sp)                           # 8-byte Folded Spill
	addi   s0, sp, 80
.Lpcrel_hi0:
	auipc  a0, %pcrel_hi(thrd_create._cnt)
	addi   a0, a0, %pcrel_lo(.Lpcrel_hi0)
	lw     a0, 0(a0) // _cnt = 0
	bnez   a0, .LBB1_2
	j      .LBB1_1 // if _cnt == 0
.LBB1_1:
	li     a0, 178 // gettid
	call   syscall
.Lpcrel_hi1:
	auipc  a1, %pcrel_hi(thrd_create.main_thrd)
	addi   a1, a1, %pcrel_lo(.Lpcrel_hi1)
	sw     a0, 0(a1) // main_thrd = gettid
	j      .LBB1_2
.LBB1_2:
.Lpcrel_hi2:
	auipc  a0, %pcrel_hi(thrd_create.main_thrd)
	addi   a0, a0, %pcrel_lo(.Lpcrel_hi2)
	lw     a0, 0(a0)
	sd     a0, -64(s0) // 临时变量，暂存 main_thrd     # 8-byte Folded Spill
	li     a0, 178
	call   syscall
	mv     a1, a0
	ld     a0, -64(s0)                          # 8-byte Folded Reload
	beq    a0, a1, .LBB1_4
	j      .LBB1_3 // if main_thrd == gettid
.LBB1_3:
.Lpcrel_hi3:
	auipc  a0, %pcrel_hi(.L.str)
	addi   a0, a0, %pcrel_lo(.Lpcrel_hi3)
	call   perror
	li     a0, -1
	sw     a0, -20(s0)
	j      .LBB1_13
.LBB1_4:
	addi   a0, s0, -32
	li     a1, 16
	lui    a2, 16 // 16 << 20 == 65536
	call   posix_memalign
	ld     a0, -32(s0)
	bnez   a0, .LBB1_6
	j      .LBB1_5 // if stack == NULL
.LBB1_5:
.Lpcrel_hi4:
	auipc  a0, %pcrel_hi(.L.str.1)
	addi   a0, a0, %pcrel_lo(.Lpcrel_hi4)
	call   perror
	li     a0, -1
	sw     a0, -20(s0)
	j      .LBB1_13
.LBB1_6:
	ld     a0, -32(s0)
	lui    a1, 16
	add    a0, a0, a1
	sd     a0, -40(s0)
	li     a0, 96
	call   malloc
	sd     a0, -48(s0)
	ld     a0, -48(s0)
	bnez   a0, .LBB1_8
	j      .LBB1_7 // if regs == NULL
.LBB1_7:
.Lpcrel_hi5:
	auipc  a0, %pcrel_hi(.L.str.2)
	addi   a0, a0, %pcrel_lo(.Lpcrel_hi5)
	call   perror
	li     a0, -1
	sw     a0, -20(s0)
	j      .LBB1_13
.LBB1_8:
	lui    a0, 593
	addiw  a0, a0, -256
	sw     a0, -52(s0) // flags
	ld     a1, -40(s0)
	lw     a2, -52(s0)
	ld     a3, -48(s0)
.Lpcrel_hi6:
	auipc  a0, %pcrel_hi(dummy)
	addi   a0, a0, %pcrel_lo(.Lpcrel_hi6)
	call   clone // clone()
	sw     a0, -56(s0)
	lw     a0, -56(s0)
	bgez   a0, .LBB1_10
	j      .LBB1_9
.LBB1_9:
	li     a0, -1
	sw     a0, -20(s0)
	j      .LBB1_13
.LBB1_10:
	lw     a0, -56(s0)
	bnez   a0, .LBB1_12
	j      .LBB1_11
.LBB1_11:
	li     a0, 0
	sw     a0, -20(s0)
	j      .LBB1_13
.LBB1_12:
	li     a0, 1
	call   sleep
.Lpcrel_hi7:
	auipc  a0, %pcrel_hi(_root)
	addi   a0, a0, %pcrel_lo(.Lpcrel_hi7)
	ld     a0, 0(a0)
.Lpcrel_hi8:
	auipc  a1, %pcrel_hi(thrd_create._cnt)
	addi   a1, a1, %pcrel_lo(.Lpcrel_hi8)
	sd     a1, -72(s0)                          # 8-byte Folded Spill
	lw     a1, 0(a1)
	lw     a2, -56(s0)
	call   insert
	ld     a2, -72(s0)                          # 8-byte Folded Reload
	lw     a0, 0(a2)
	addiw  a1, a0, 1
	sw     a1, 0(a2)
	sw     a0, -20(s0)
	j      .LBB1_13
.LBB1_13:
	lw     a0, -20(s0)
	ld     ra, 72(sp)                           # 8-byte Folded Reload
	ld     s0, 64(sp)                           # 8-byte Folded Reload
	addi   sp, sp, 80
	ret
thrd_create._cnt:
	.word  0                                    # 0x0

thrd_create.main_thrd:
	.word  0                                    # 0x0

.L.str:
	.asciz "not main thread"

.L.str.1:
	.asciz "posix_memalign stack error"

.L.str.2:
	.asciz "malloc regs error"

_root:
	.quad  0