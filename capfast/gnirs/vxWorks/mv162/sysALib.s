/* sysALib.s - Motorola MVME162 system-dependent assembly routines */

/* Copyright 1984-1993 Wind River Systems, Inc. */
	.data
	.globl	_copyright_wind_river
	.long	_copyright_wind_river

/*
modification history
--------------------
01b,12feb93,caf  made I/O region noncachable, serialized (SPR 2005).
01a,04jan93,ccc  written by modifying 01i mv167/sysALib.s.
*/

/*
DESCRIPTION
This module contains system-dependent routines written
in assembly language.

The sysInit() routine is the system start-up code, the first code executed
after booting.

This module must be the first specified on the \f3ld\f1 command line used
to build the system.  The sysInit() routine is the entry point for VxWorks.

INTERNAL
Many routines in this module "link" and "unlk" the "c" frame pointer
a6@ although they don't use it in any way!  This is only for the benefit of
the stacktrace facility to allow it to properly trace tasks executing within
these routines.
*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "config.h"

	/* internals */

	.globl	_sysInit	/* start of system code */

	/* externals */

	.globl	_usrInit	/* system initialization routine */

	.text
	.even

/*******************************************************************************
*
* sysInit - start after boot
*
* This routine is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables the interrupts, sets up
* the MC68040 caches, sets up the stack, and jumps to the C routine usrInit()
* in usrConfig.c.
*
* Note:  This routine should not be called by the user.
*
* The initial stack is set to grow down from the address of sysInit().
* This stack is used only by usrInit() and is never used again.
* Memory for the stack must be accounted for when determining the load
* address of the system.

* VOID sysInit ()	/@ THIS IS NOT A CALLABLE ROUTINE @/

*/

_sysInit:
	movew	#0x3700,sr	/* disable interrupts, turn on M bit */

        /* NOTE: Caches reconfigured in usrInit() as per configAll.h */

        .word   0xf498              /* invalidate instruction cache        */
        .word   0xf458              /* invalidate data cache               */
        movel   #0,d0               /* disable data and instruction caches */
        .word   0x4e7b,0x0002	    /* movec d0,cacr                       */
        movel   #0x00ffe000,d0      /* enable instruction passthrough      */
                                    /* with writethrough caching from      */
                                    /* 0x00000000..0xffffffffff            */
        .word   0x4e7b,0x0004       /* movec d0,ITT0                       */

        /*
         * Set up one data window across the whole linear range to
         * specify non-cached operation.  This is essential for data
         * coherence from the VMEbus and local peripherals.
         *
         * A second window, in DTT0, is set up to cover the local DRAM.
         * Since DTT0 supersedes DTT1, we use this to allow caching to
         * the local DRAM.
         *
         * WARNING:
         * This works great if we have 16 Meg on board, but if
         * we do not, then accesses through the 4..15 meg range
         * to VMEbus A24 space WILL BE CACHED.  The only alternative
         * is to disable the data cache completely.
         */

        movel   #0x0000e000,d0          /* enable data passthrough          */
                                        /* with writethrough caching from   */
                                        /* 0x00000000..0x00ffffff           */
        .word   0x4e7b,0x0006           /* movec d0,DTT0                    */
        movel   #0x00ffe040,d0          /* enable data passthrough          */
                                        /* noncachable & serialized from    */
                                        /* 0x00000000..0xffffffff           */
        .word   0x4e7b,0x0007           /* movec d0,DTT1                    */

        movel   #_sysInit,a7            /* set stack to grow down from code */
        movel   #BOOT_WARM_AUTOBOOT,a7@- /* push start type arg = WARM_BOOT */
        jsr     _usrInit                /* never returns - starts up kernel */
