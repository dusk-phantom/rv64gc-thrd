# FIXME 注意一下这一段

	.text
	.align         1
	.globl         __get_tid
	.type          __get_tid, @function
__get_tid:
.LFB6__get_tid:
	.cfi_startproc
	mv             a7, a0                        # a7 = gettid()
.while_init__get_tid:
	lla            a1, tids                      # a1 = tids 基址
	lw             a0, 0(a1)                     # a0 = cnt
	j              .while_cond__get_tid
.while_block__get_tid:
	addiw          a0, a0, -1
.while_cond__get_tid:
	blez           a0, .return__get_tid
	mv             a2, a0
	slliw          a2, a2, 2
	add            a2, a1, a2
	lw             a2, 0(a2)                     # a2 = tids[a0]
	bne            a2, a7, .while_block__get_tid
.return__get_tid:
# 返回 a0 ，这里 a0 就是循环变量
	jr             ra
	.cfi_endproc
.LFE6__get_tid:
	.size          __get_tid, .-__get_tid

# FIXME 注意一下这一段