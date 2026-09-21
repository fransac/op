# Target.
ARCH    = riscv64
MACHINE = opensbi

# Toolchain.
CROSS_COMPILE = $(ARCH)-elf-
AR            = $(CROSS_COMPILE)ar
AS            = $(CROSS_COMPILE)as
CC            = $(CROSS_COMPILE)gcc
LD            = $(CROSS_COMPILE)ld
OBJCOPY       = $(CROSS_COMPILE)objcopy

# Toolchain flags.
ASFLAGS = $(EXTRAASFLAGS)
CFLAGS  = $(EXTRACFLAGS) -std=c99 -O0 -fPIC -ffreestanding -Wall -Wextra
LDFLAGS = $(EXTRALDFLAGS) -s
