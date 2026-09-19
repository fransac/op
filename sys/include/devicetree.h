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

/* Max length of Devicetree node paths. */
#define DEVICETREE_PATH_MAX 64

/* 1. Header. */

#define DEVICETREE_MAGIC 0xd00dfeed
#define DEVICETREE_LAST_COMP_VERSION 16

struct devicetreeheader {
	u32 magic;
	u32 totalsize;
	u32 offdtstruct;
	u32 offdtstrings;
	u32 offmemrsvmap;
	u32 version;
	u32 lastcompversion;
	u32 bootcpuid_phys;
	u32 sizedtstrings;
	u32 sizedtstruct;
};

/* 2. Memory reservation block. */

struct devicetreereserveentry {
	u64 address;
	u64 size;
};

/* 3. Structure block. */

/* Tokens. */
#define DEVICETREE_BEGIN_NODE (u32)1
#define DEVICETREE_END_NODE   (u32)2
#define DEVICETREE_PROP       (u32)3
#define DEVICETREE_NOP        (u32)4
#define DEVICETREE_END        (u32)9

struct devicetreepropertyinfo {
	u32 len;
	u32 nameoff;
};

/* It returns the address to the property of the Devicetree at the dt address,
   given the property's name n. If no property is found or the given Devicetree
   is not valid, NULL is returned instead. */
struct devicetreepropertyinfo *devicetreeproperty(void *dt, char *n);

#endif
