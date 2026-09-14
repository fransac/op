.section .text
.global astart

astart:
	# Switch to physical memory.
	csrwi satp, 0

	call initsstatus

	la   t0,    kernelinterrupt
	csrw stvec, t0

	# (sp = kernelstacks[(tp + 1) * 4096])
	la   sp, kernelstacks
	mv   t0, tp
	addi a0, t0, 1
	li   a1, 4096
	call multiply
	add  sp, sp, a0

	j kernel

initsstatus:
	csrr t0, sstatus

	# Set SUM to 1.
	li t1, 1 << 18
	or t0, t0, t1

	# Set MXR to 1.
	li t1, 1 << 19
	or t0, t0, t1

	csrw sstatus, t0

	ret

# (a0 = a0 * a1)
# Only works with unsigned values.
multiply:
	li t0, 0

1:
	# If (a1 <= 0) then return.
	blez a1, 2f

	# (t0 += a0)
	add t0, t0, a0

	# (a1--)
	addi a1, a1, -1

	# Loop back.
	j 1b

2:
	mv a0, t0
	ret
