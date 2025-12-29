/* romInit.s - Motorola MVME162,MVME162LX ROM initialization module */

/* Copyright 1984-1995 Wind River Systems, Inc. */
	.data
	.globl	_copyright_wind_river
	.long	_copyright_wind_river

/*
modification history
--------------------
01d,18may95,kvk  Updated the copyright to include 1995.
01c,10mar94,dzb  Added for SRAM sizing.
                 fix for CPU speed determination code (SPR 2625).
01b,12feb93,caf  made I/O region noncachable, serialized (SPR 2005).
01a,04jan93,ccc  written by modifying v01i of mv167/romInit.s
*/

/*
DESCRIPTION
This module contains the entry code for the VxWorks bootrom.
The entry point romInit, is the first code executed on power-up.
It sets the BOOT_COLD parameter to be passed to the generic
romStart() routine.

The routine sysToMonitor() jumps to the location 8 bytes
past the beginning of romInit, to perform a "warm boot".
This entry point allows a parameter on the stack to be passed
to romStart().

This code is intended to be generic across 680X0 boards.
Hardware that requires special register setting or memory
mapping to be done immediately, may do so here.
*/

#define	_ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "config.h"

	/* internals */

	.globl	_romInit	/* start of system code */
	.globl	_sdata		/* start of data */

	/* externals */

	.globl	_romStart	/* system initialization routine */

_sdata:

	.asciz	"start of data"

	.text
	.even

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*

* romInit
*     (
*     int startType	/@ only used by 2nd entry point @/
*     )

*/

_romInit:
	movew   #0x3700,sr      /* disable interrupts, turn on M bit */
	bra     cold
	movew   #0x3700,sr	/* subsequent ROM starts (2nd entry point) */
	bra     warm

	/* copyright notice appears at beginning of ROM (in TEXT segment) */

	.ascii   "Copyright 1984-1995 Wind River Systems, Inc."
	.even

cold:
	/* set DRAM/SRAM size registers */

	movel	#LOCAL_MEM_LOCAL_ADRS,d7	/* get local address */
	rorl	#8,d7				/* shift bits */
	rorl	#8,d7
	moveb	d7,MCC_DRAM_BASE_AR_LOW		/* save address bits */
	rorl	#8,d7				/* get upper byte */
	moveb	d7,MCC_DRAM_BASE_AR_HIGH	/* save upper bits */

	/* set PROM speed and disable PROM at address 0 */

	moveb	MCC_VERSION_REG,d7		/* read board configuration */
	andb	#VERSION_REG_SPEED,d7		/* check speed */
	beqs	cpu25mhz			/* if 25MHz board */

	moveb	#PROM_ACCESS_33M_160NS,MCC_PROM_ACCESS_TIME
	moveb	#FLASH_ACCESS_33M_130NS,MCC_FLASH_ACCESS_TIME
	moveb	#33,MCC_BUS_CLK_REG		/* set bus clock to 33 MHz */
	bras	cpu33mhz

cpu25mhz:
	moveb	#PROM_ACCESS_25M_180NS,MCC_PROM_ACCESS_TIME
	moveb	#FLASH_ACCESS_25M_140NS,MCC_FLASH_ACCESS_TIME
	moveb	#25,MCC_BUS_CLK_REG		/* set bus clock to 25 MHz */

cpu33mhz:
	moveb	MCC_DRAM_SRAM_OPTIONS,d7	/* read size */
	andb	#DRAM_SRAM_OPTIONS_SMASK,d7	/* save SRAM bits */
	rorl	#3,d7				/* shift bits */
	bne	sramHigh			/* if (SRAM < 512k) then */
	orb	#SRAM_SPACE_512K,d7		/*     SRAM = 512K */
sramHigh:					/* .endif */
	moveb	d7,MCC_SRAM_SPACE_SIZE		/* write to size register */

	moveb	MCC_DRAM_SRAM_OPTIONS,d7	/* read size */
	andb	#DRAM_SRAM_OPTIONS_DMASK,d7	/* save DRAM bits */
	moveb	d7,MCC_DRAM_SPACE_SIZE		/* write to size register */
	cmpib	#DRAM_SPACE_SIZE_NONE,d7	/* if ECC DRAM is not present */
	beq	eccDRAM				/* .then */
	moveb	#DRAM_CONTROL_REG_RAM_EN,MCC_DRAM_CONTROL_REG /* enable RAM */
	bra	noECC
eccDRAM:					/* .else */
        moveb	#MCECC_ID_RESET,MCECC_ID	/*     reset the MCECC chip */
	moveb	MCC_DRAM_BASE_AR_HIGH,MCECC_BAR	/*     set upper address bits */
	moveb	MCC_DRAM_BASE_AR_LOW,d7		/*     save lower address */
        andb	#MCECC_RCR_BAD,d7		/*     mask BAD22 and BAD23 */
	orb	d7,MCECC_RCR			/*     set BAD22 and BAD23 */
	moveb	MCC_BUS_CLK_REG,MCECC_BCR	/*     set bus clock freq */
	moveb	#MCECC_SCRUB_DIS,MCECC_SCR	/*     disable scrubber */
	orb	#MCECC_RCR_RAMEN,MCECC_RCR	/*     enable RAM */
        movel   #LOCAL_MEM_LOCAL_ADRS,a0	/*     load address of DRAM */
        movel   #0x0a,d0			/*     set counter=10 */
readRam:					/*     do */
        movel   (a0)+,d1			/*         access DRAM */
        dbf     d0,readRam			/*     .while --counter >= 0 */
noECC:						/* .endif */

	/* clear Power-Up reset */

	moveb	MCC_RESET_CR,d7			/* read register */
	andb	#3,d7				/* save lower to bits */
	orb	#RESET_CR_CPURS,d7		/* add clear power-up bit */
	moveb	d7,MCC_RESET_CR

	movel	#BOOT_COLD,d7	/* force startType */
	bra	start		/* skip over next instruction */
warm:
        movel   a7@(0x4),d7     /* put startType in d7 */
start:

        /* NOTE: Caches reconfigured in usrInit() as per configAll.h */

        movel   #0,d0           /* disable data and instruction caches */
        .word   0x4e7b,0x0002   /* movec d0,cacr                       */
        movel   #0x00ffe000,d0  /* enable instruction passthrough      */
                                /* with writethrough caching from      */
                                /* 0x00000000..0xffffffffff            */
        .word   0x4e7b,0x0004   /* movec d0,ITT0                       */

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

        movel   #0x0000e000,d0  /* enable data passthrough                */
                                /* with writethrough caching from         */
                                /* 0x00000000..0x00ffffff                 */
        .word   0x4e7b,0x0006   /* movec d0,DTT0                          */
        movel   #0x00ffe040,d0  /* enable data passthrough                */
                                /* noncachable & serialized from          */
                                /* 0x00000000..0xffffffff                 */
        .word   0x4e7b,0x0007   /* movec d0,DTT1                          */

        .word   0xf498          /* cinva ic, invalidate instruction cache */
        .word   0xf458          /* cinva dc ,invalidate data cache        */
        movel   #0x00008000,d0  /* enable instruction cache only          */
        .word   0x4e7b,0x0002   /* movec d0,cacr                          */

        /* calculate C entry point: routine - entry point + ROM base */

        movel   #_romStart,a0
        subl    #_romInit,a0
        addl    #ROM_TEXT_ADRS,a0

        movel   #STACK_ADRS,a7  /* stack grows down from STACK_ADRS */
        movel   d7,a7@-         /* push start type */
        jsr     a0@
