#ifndef _DEVICETREE_H
#define _DEVICETREE_H

#include <arch/types.h>

/* The following is a tree-like representation of the Devicetree structure as
   described by the Devicetree v0.4 specification. Every data field is in
   big-endian format.

   + [8-bytes alignment padding].
   |
   + 1. Header.
   | + struct devicetreeheader
   |
   + (Free)
   |
   + 2. Memory reservation block.
   + + struct devicetreereserveentry
   + + ...
   |
   + 3. Structure block.
   | |
   | + Node.
   | | |
   | | + [DEVICETREE_NOP ...]
   | | + DEVICETREE_BEGIN_NODE
   | | | + Node's name string.
   | | |
   | | + Node's property.
   | | | |
   | | | + [4-bytes alignment padding].
   | | | + [DEVICETREE_NOP ...].
   | | | + DEVICETREE_PROP
   | | | + struct devicetreepropertyinfo
   | | |
   | | + Subnodes (nested instances of nodes, of this same structure).
   | | + ...
   | | |
   | | + [DEVICETREE_NOP ...]
   | | + DEVICETREE_END_NODE
   | |
   | + ...
   | |
   | + DEVICETREE_END
   |
   + 4. Strings block.
*/

/* 1. Header. */

#define DEVICETREE_MAGIC_0 0xd0
#define DEVICETREE_MAGIC_1 0x0d
#define DEVICETREE_MAGIC_2 0xfe
#define DEVICETREE_MAGIC_3 0xed
#define DEVICETREE_LAST_COMP_VERSION 16

struct devicetreeheader {
	u8 magic[4];
	u8 totalsize[4];
	u8 offdtstruct[4];
	u8 offdtstrings[4];
	u8 offmemrsvmap[4];
	u8 version[4];
	u8 lastcompversion[4];
	u8 bootcpuid_phys[4];
	u8 sizedtstrings[4];
	u8 sizedtstruct[4];
};

/* 2. Memory reservation block. */

struct devicetreereserveentry {
	u8 address[8];
	u8 size[8];
};

/* 3. Structure block. */

/* Tokens. */
#define DEVICETREE_BEGIN_NODE (u32)1
#define DEVICETREE_END_NODE   (u32)2
#define DEVICETREE_PROP       (u32)3
#define DEVICETREE_NOP        (u32)4
#define DEVICETREE_END        (u32)9

struct devicetreepropertyinfo {
	u8 len[4];
	u8 nameoff[4];
};

/* It returns the address to the property of the Devicetree at the dt address,
   given the property's name n. If no property is found or the given Devicetree
   is not valid, NULL is returned instead. */
struct devicetreepropertyinfo *devicetreeproperty(void *dt, char *n);

#endif
