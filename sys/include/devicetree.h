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
   | | + [4-bytes alignment padding].
   | | + [DEVICETREE_NOP ...]
   | | |
   | | + [4-bytes alignment padding].
   | | + DEVICETREE_BEGIN_NODE
   | | | + Node's name string.
   | | |
   | | + Node's property.
   | | | |
   | | | + [4-bytes alignment padding].
   | | | + [DEVICETREE_NOP ...].
   | | | |
   | | | + [4-bytes alignment padding].
   | | | + DEVICETREE_PROP
   | | | + struct devicetreepropertyinfo
   | | |
   | | + Subnodes (nested instances of nodes, of this same structure).
   | | + ...
   | | |
   | | + [4-bytes alignment padding].
   | | + [DEVICETREE_NOP ...]
   | | |
   | | + [4-bytes alignment padding].
   | | + DEVICETREE_END_NODE
   | |
   | + ...
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

#define DEVICETREE_BEGIN_NODE (u8)1
#define DEVICETREE_END_NODE   (u8)2
#define DEVICETREE_PROP       (u8)3
#define DEVICETREE_NOP        (u8)4
#define DEVICETREE_END        (u8)9

struct devicetreepropertyinfo {
	u8 len[4];
	u8 nameoff[4];
};

#endif
