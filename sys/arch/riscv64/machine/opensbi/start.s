.section .text
.global start

start:
	# OpenSBI provides the core ID in the a0 register.
	mv tp, a0

	la  sp, machinestack
	li  t0, 4096
	add sp, sp, t0

	# OpenSBI provides the Devicetree address in the a1 register.
	mv a0, a1
	call machinefree

	j astart
