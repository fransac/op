#include <devicetree.h>

#include <endianness.h>
#include <math.h>
#include <pmem.h>
#include <string.h>

struct devicetreepropertyinfo *
devicetreeproperty(void *dt, char *n)
{
	struct devicetreeheader *dth = (struct devicetreeheader *)dt;
	void *structureblock, *stringsblock;
	u32 magic, *token;

	if (LITTLE_ENDIAN)
		magic = BEU32_TO_LEU32(dth->magic);
	else
		magic = dth->magic;

	/* TODO: Check version ensuring big-endian compatibility. */

	if (magic != DEVICETREE_MAGIC)
		return NULL;

	structureblock = (void *)((uptr)dt + (uptr)dth->offdtstruct);
	stringsblock = (void *)((uptr)dt + (uptr)dth->offdtstrings);

	/* Iterate through the tokens of the structure block in the order they
	   are stored, until the DEVICETREE_END token. */
	token = structureblock;
	while (*token != DEVICETREE_END) {
		struct devicetreepropertyinfo *propinfo;
		u32 propnamelen = 0;
		char *propname = NULL;

		switch (*token) {
		case DEVICETREE_NOP:
		case DEVICETREE_END_NODE:
			/* Just skip the token to get to the next. */
			token = (u32 *)((uptr)token + sizeof(*token));
			break;
		case DEVICETREE_PROP:
			/* The property information struct comes right after
			   this token. */
			propinfo = (struct devicetreepropertyinfo *)
			           ((uptr)token + sizeof(*token));

			propnamelen = propinfo->len;
			propname = (char *)((uptr)stringsblock
			                  + (uptr)propinfo->nameoff);

			token = (u32 *)((uptr)propinfo + sizeof(*propinfo));
			break;
		case DEVICETREE_BEGIN_NODE:
			/* Skip the token and align to 4-bytes. */
			token = (u32 *)CEIL((uptr)token + sizeof(*token), 4);
			break;
		default:
			/* All possible tokens have been checked; the Devicetree
			   must be invalid. */
			return NULL;
		}

		if (!propname)
			continue;

		/* If propname is not NULL, the loop have reached a property. */

		/* If the name matches, return propinfo, which must have been
		   set if propname has also been set. Otherwise, reset propname
		   to NULL and continue the loop. */
		if (!strncmp(propname, n, propnamelen))
			return propinfo;
		else
			propname = NULL;
	}

	/* No property of the Devicetree matched the name. */
	return NULL;
}
