.section .text
.global start

start:
	# OpenSBI provides the core ID in the a0 register.
	mv tp, a0

	j astart
