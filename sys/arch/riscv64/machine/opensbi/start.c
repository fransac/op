#include <arch/types.h>
#include <external/smoldtb.h>
#include <pmem.h>
#include <string.h>

#define RESERVED_RANGES 128

/* It frees the free physical memory regions given the Devicetree at address
   dt. */
void machinefree(void *dt);

/* It returns 1 if the device_type property of n node exists and is "memory"; it
   returns 0 otherwise. */
static u8 ismemorynode(dtb_node *n);

/* It updates the r ranges array of length n to have the addresses sorted
   numerically. */
static void sortranges(dtb_pair *r, uptr n);

/* Stack used for machine start code. */
u8 machinestack[4096];

/* The buffer containing the reserved-memory ranges. */
dtb_pair resmemranges[RESERVED_RANGES];

void
machinefree(void *dt)
{
	/* The SMOLDTB_STATIC_BUFFER_SIZE macro has been set directly in the
	   source file of smoldtb, thus malloc and free are never used. The
	   on_error callback cannot be used as well, since there is no way of
	   logging to the serial console without first knowing the UART
	   address. */
	dtb_ops ops = { 0 };
	dtb_node *resmem, *c;
	dtb_prop *acells, *scells;
	dtb_pair reglayout;
	uptr pairslen = 0;

	dtb_init((uptr)dt, ops);

	if (!(resmem = dtb_find("/reserved-memory")))
		return;

	/* Set the pair layour of each reg property to
	   (#addess-cells, #size-cells). */

	if (!(acells = dtb_find_prop(resmem, "#address-cells")))
		return;

	if (!(scells = dtb_find_prop(resmem, "#size-cells")))
		return;

	if (dtb_read_prop_1(acells, 1, &reglayout.a) != 1)
		return;

	if (dtb_read_prop_1(scells, 1, &reglayout.b) != 1)
		return;

	/* Iterate through the children of /reserved-memory. */
	for (c = dtb_get_child(resmem); c; c = dtb_get_sibling(c)) {
		dtb_prop *reg;
		uptr regpairslen;

		if (!(reg = dtb_find_prop(c, "reg")))
			continue;

		if (!(regpairslen = dtb_read_prop_2(reg, reglayout, NULL)))
			continue;

		/* If pairslen would exceed the fixed-size array, then abort. */
		if (pairslen > RESERVED_RANGES - regpairslen)
			break;

		pairslen += regpairslen;

		/* Write the pairs to the resmemranges array, starting from
		   where left off. */
		(void)dtb_read_prop_2(reg, reglayout,
		                      &resmemranges[pairslen - regpairslen]);
	}

	/* TODO: For every mapped range, allocate the memory in between.
	   Although, how is it possible to know the size of the entire physical
	   memory, yet? */
}

u8
ismemorynode(dtb_node *n)
{
	dtb_prop *devtype;
	char exp[7] = "memory";

	if (!n)
		return 0;

	if (!(devtype = dtb_find_prop(n, "device_type")))
		return 0;

	return !strncmp(dtb_read_prop_string(devtype, 0), exp, sizeof(exp));
}

void
sortranges(dtb_pair *r, uptr n)
{
	uptr i;

	/* Using the insertion sort algorithm. */
	for (i = 1; i < n; i++) {
		uptr j;
		dtb_pair k = r[i];

		for (j = i; j > 0 && r[j - 1].a > k.a; j--)
			r[j] = r[j - 1];
	}
}
