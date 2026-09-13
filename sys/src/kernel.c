#include <arch/page.h>
#include <arch/types.h>
#include <config.h>
#include <console.h>
#include <core.h>
#include <interrupt.h>
#include <machine/mem.h>
#include <memswitch.h>
#include <panic.h>
#include <pmem.h>
#include <process.h>
#include <schedule.h>
#include <string.h>
#include <timer.h>
#include <vmem.h>

#include "mem.h"
#include "kvmem.h"
#include "logs.h"

/* Does the initialization of core 0. On success, 0 is returned. On failure, -1
   is returned and the panic message is set. */
static s8 core0(void);

/* Temporarily one. Gets set as the return value of core0 to sync cores. */
static s8 core0ret = 1;

static pageentry *ptree = NULL;

s8
core0(void)
{
	initconsole();

	(void)consolewrite(CPU_LOG_PRE);
	(void)consolewriteb10u(NCPU);
	(void)consolewrite(" core(s)\n");

	if (freeallmem())
		goto panic;

	if (!(ptree = kvmem()))
		goto panic;

	return 0;

panic:
	tracepanicmsg("core0");
	return -1;
}

void
kernel(void)
{
	u16 c = core();

	disableinterrupts();

	if (!c && (core0ret = core0()) == -1)
		goto panic;

	while (core0ret);

	disableinterrupts();
	memswitch(ptree);
	enableinterrupts();

	setupnexttimer();

	schedule();

panic:
	tracepanicmsg("kernel");
	panic();
}
