#ifndef _ENDIANNESS_H
#define _ENDIANNESS_H

#include <arch/types.h>

#if (*(u8 *)&(uint16_t){1})
/* Defined if the host is little-endian. */
# define LITTLE_ENDIAN
#endif

/* It returns the big-endian integer n as little-endian. */
#define BEU32_TO_LEU32(n) ((((n) & 0x000000ff) << 24) \
                         | (((n) & 0x0000ff00) << 8) \
                         | (((n) & 0x00ff0000) >> 8) \
                         | (((n) & 0xff000000) >> 24))

/* It returns the big-endian integer n as little-endian. */
#define BEU64_TO_LEU64(n) ((((n) & 0x00000000000000ff) << 56) \
                         | (((n) & 0x000000000000ff00) << 40) \
                         | (((n) & 0x0000000000ff0000) << 24) \
                         | (((n) & 0x00000000ff000000) << 8) \
                         | (((n) & 0x000000ff00000000) >> 8) \
                         | (((n) & 0x0000ff0000000000) >> 24) \
                         | (((n) & 0x00ff000000000000) >> 40) \
                         | (((n) & 0xff00000000000000) >> 56))

#endif
