/* mv162/config.h - Motorola MVME162,MVME162LX configuration header */

/* Copyright 1984-1994 Wind River Systems, Inc. */

/*
modification history
--------------------
01f,04apr95,tmk  revved up the BSP revision number.
01e,24mar94,vin  added RAM_LOW_ADRS in the comments.
01d,17mar94,vin  allocated low-end memory for shared memory.
                 added RAM_LOW_ADRS and defined it to 0x00020000
                 to accomodate shared  memory.
01c,11mar94,dzb  added MVME162LX support.  Fixed VMEbus comments.
		 fixed mailbox interrupt comment (SPR #2860).
01b,25jan93,ccc  changed ROM_BASE_ADRS for on-board EPROMs.
01a,04jan93,ccc  derived from mv167/config.h.
*/

/*
This file contains the configuration parameters for the
Motorola MVME162,MVME162LX.
*/

#ifndef	INCconfigh
#define	INCconfigh

#include "configAll.h"
#include "mv162.h"

#if	FALSE		/* change to true if using a MVME162LX target */
#define	MVME162LX
#define BSP_REV         "/0"            /* 0 for the first mv162lx version */
#else	/* FALSE/TRUE */
#define BSP_REV         "/2"            /* 1 for the second mv162 version */
#endif	/* FALSE/TRUE */

#define DEFAULT_BOOT_LINE \
"ei(0,0)host:/usr/vw/config/mv162/vxWorks h=90.0.0.3 e=90.0.0.50 u=target"

#define	INCLUDE_MMU_BASIC	/* bundled mmu support */

#if	FALSE		/* change to true if 68EC040 is installed */
#define	MV162_68EC040	/* define if a 68EC040 without MMU is installed */
#endif	/* FALSE/TRUE */

#ifdef	MV162_68EC040
#undef	USER_D_CACHE_ENABLE
#undef	INCLUDE_MMU_BASIC
#endif	/* MV162_68EC040 */

#undef	USER_D_CACHE_MODE
#define	USER_D_CACHE_MODE	(CACHE_COPYBACK | CACHE_SNOOP_ENABLE)

/*
 * Device controller I/O addresses:
 */

#define	INCLUDE_EI	/* include 82596 driver */

#if	FALSE			/* change FALSE to TRUE for SCSI interface */
#define	INCLUDE_SCSI		/* include ncr710 driver */
#define	INCLUDE_SCSI_BOOT	/* include ability to boot from SCSI */
#define	INCLUDE_DOSFS		/* file system to be used */
#endif	/* FALSE/TRUE */

/* Interrupt vectors */

#define INT_VEC_ABORT           (MCC_INT_VEC_BASE + MCC_INT_ABORT)
#define INT_VEC_SCSI            (MCC_INT_VEC_BASE + MCC_INT_SCSI)
#define INT_VEC_CLOCK           (MCC_INT_VEC_BASE + MCC_INT_TT1)
#define INT_VEC_AUX_CLOCK       (MCC_INT_VEC_BASE + MCC_INT_TT2)
#define INT_VEC_LN              (MCC_INT_VEC_BASE + MCC_INT_LANC)
#define	INT_VEC_LN_ERR		(MCC_INT_VEC_BASE + MCC_INT_LANC_ERR)

/* Miscellaneous definitions */

#define NV_RAM_SIZE     BBRAM_SIZE      /* 8184 bytes */
#define NV_RAM_ADRS     ((char *) BBRAM_ADRS)
#define NV_BOOT_LINE    (NV_RAM_ADRS + NV_BOOT_OFFSET)

#define SYS_CPU_FREQ	25000000	/* 25 MHz system clock */
#define BAUD_CLK_FREQ   10000000        /* 10 MHz baud rate "P Clock" (fixed) */

#undef	NUM_TTY

#ifdef	MVME162LX
#define	NUM_TTY		N_SIO_CHANNELS_LX
#else	/* MVME162LX */
#define	NUM_TTY		N_SIO_CHANNELS
#endif	/* MVME162LX */

#define SYS_CLK_RATE_MIN  3             /* minimum system clock rate */
#define SYS_CLK_RATE_MAX  5000          /* maximum system clock rate */
#define AUX_CLK_RATE_MIN  3             /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX  5000          /* maximum auxiliary clock rate */

/* Backplane network parameters */

#define	GCSR_GROUP_ADDR	(0xc2)		/* A15-A8 for board mailbox addr */
#define SM_INT_TYPE	SM_INT_MAILBOX_1        /* 1-bytes write mailbox int */
#define SM_INT_ARG1	VME_AM_SUP_SHORT_IO     /* bus address space */
#define SM_INT_ARG2	((GCSR_GROUP_ADDR << 8) + (sysProcNumGet() << 4) + 2)
						/* bus address */
#define SM_INT_ARG3	0x08			/* value (SIG3 bit) */

/* the backplane master (usually cpu 0) also needs to know the following
 * shared memory pool parameters. */

/*
 * If the user is using the VxWorks backplane driver, make sure you change
 * the definition of SM_OFF_BOARD if you will be using the mv162's memory
 * for the backplane anchor.
 */

#define	SM_OFF_BOARD	FALSE

#if	SM_OFF_BOARD
#undef	SM_ANCHOR_ADRS
#define SM_ANCHOR_ADRS	((char *)0xf0800000)	/* off-board anchor address */
#define SM_MEM_ADRS	SM_ANCHOR_ADRS	/* off-board shared memory address */
#define SM_MEM_SIZE	0x00080000	/* 512K */
#define SM_OBJ_MEM_ADRS (SM_MEM_ADRS+SM_MEM_SIZE)/* sh. mem Objects pool adrs */
#define SM_OBJ_MEM_SIZE 0x80000         /* sh. mem Objects pool size 512K */
#else

/* shared memory pool address define NONE = allocate from memory */
#define SM_MEM_ADRS	(RAM_LOW_ADRS - (SM_MEM_SIZE + SM_OBJ_MEM_SIZE))

/* sh. mem Objects pool address  define NONE to allocate from memory */
#define SM_OBJ_MEM_ADRS (SM_MEM_ADRS+SM_MEM_SIZE)

#define SM_MEM_SIZE	0x000e000	/* 64K - 8k */
#define SM_OBJ_MEM_SIZE 0x10000         /* sh. mem Objects pool size 64K */
#endif	/* SM_OFF_BOARD */

/* undef it  if the board does not have the VMECHIP2 */
#define	INCLUDE_VMECHIP2

#ifndef INCLUDE_VMECHIP2	/* if vmechip2 is not included */
#undef	INCLUDE_ENP
#undef	INCLUDE_EX
#undef	INCLUDE_SM_NET
#endif	/* INCLUDE_VMECHIP2 */

/* Memory addresses */

/*
 * Local-to-Bus memory address constants:
 * The MVME162 local memory always appears at address 0 locally;
 * its address on the bus is set by the Slave Base Address Register in the
 * VMECHIP2.
 *
 * If the board has 4Mbytes of memory then the memory will be
 * mapped to the standard (24-bit) address range starting at LOCAL_MEM_BUS_A24.
 * Regardless of the local memory size, the local DRAM will also be mapped to
 * the extended (32-bit) address range starting at LOCAL_MEM_BUS_A32.
 *
 * The size of local memory is determined from the LOCAL_MEM_SIZE macro.
 * Be sure to set LOCAL_MEM_SIZE to the size of local RAM so that VMEbus
 * mapping is properly performed.
 *
 * It should be noted that the default configuration has LOCAL_MEM_SIZE set
 * to the minimum size of 4Mbytes, which will invoke a mapping of local memory
 * to the VMEbus address given by LOCAL_MEM_BUS_A24.  This is in standard
 * (A24) address space.  Local DRAM will also be mapped to the VMEbus address
 * given by LOCAL_MEM_BUS_A32. This is in extended (A32) address space.
 *
 * Also note that only processor 0 (the backplane master) has its RAM dual-
 * ported onto the VMEbus.
 */

#define LOCAL_MEM_LOCAL_ADRS	0x00000000	/* fixed at zero */
#define	LOCAL_SRAM_LOCAL_ADRS	0xffe00000	/* SRAM address */
#define LOCAL_MEM_SIZE		0x00800000	/* 8MB (Min. memory: 4 MB) */

#define	LOCAL_MEM_BUS_A24	0x00c00000	/* Bus address in A24 range */
#define LOCAL_MEM_BUS_A32       0x02000000      /* Bus address in A32 range */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, RAM_LOW_ADRS and RAM_HIGH_ADRS 
 * are defined in config.h, MakeSkel, Makefile, and Makefile.
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS		0xff800000	/* base address of ROM */
#define ROM_TEXT_ADRS		(ROM_BASE_ADRS+8)       /* with PC & SP */
#define ROM_SIZE		0x00040000	/* 256K ROM space */

#define RAM_HIGH_ADRS		0x00100000	/* RAM address for ROM boot */
#define RAM_LOW_ADRS            0x00020000      /* system image load adrs */

/*
 * Industry Pack module support is determined by INDUSTRY_PACK_C_FILE and
 * INDUSTRY_PACK_H_FILE for the location of the driver and header file
 * for a user supplied module driver.
 *
 * If more than one driver or header file is required then they should
 * be added below and added to sysLib.c.
 */

#if	FALSE		/* change to TRUE to include the driver below */
#ifndef	INDUSTRY_PACK_C_FILE
#define	INDUSTRY_PACK_C_FILE	"path/driver.c"	/* change to actual values */
#endif	/* INDUSTRY_PACK_C_FILE */

#ifndef	INDUSTRY_PACK_H_FILE
#define	INDUSTRY_PACK_H_FILE	"drv/path/header.h" /* change to actual */
#endif	/* INDUSTRY_PACK_H_FILE */
#endif	/* FALSE */

#ifdef	 INDUSTRY_PACK_H_FILE
#include INDUSTRY_PACK_H_FILE
#endif	 /* INDUSTRY_PACK_H_FILE */

#define IP_OCTAL
#define DATACUBE

#endif	/* INCconfigh */
