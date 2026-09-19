#ifndef _ENDIANNESS_H
#define _ENDIANNESS_H

#include <arch/types.h>

#define BIG_ENDIAN (*(u16 *)"\0\1" == 1)

#endif
