/* config.h - Motorola MVME167,166 configuration header */

/* Copyright 1984-1994 Wind River Systems, Inc. */

/*
modification history
--------------------
02b,11apr94,vin  excluded enp and excelan drivers as onboard ethernet present
02a,24mar94,vin  added RAM_LOW_ADRS in the comments.
01z,10mar94,vin  allocated low-end memory for shared memory.
		 added RAM_LOW_ADRS and defined it to 0x00020000
		 to accomodate shared  memory. 
		 removed special cache configuration for shared memory.
01y,06dec93,dzb  added MVME166 support.  added FLASH memory macros.
		 fixed mailbox interrupt comment (SPR #2860).
                 removed NV_CPU_SPEED macro (SPR #3023).
           +dkd  VSB configuration macros.
01x,22feb93,ccc  added NV_CPU_SPEED constant for board speed,
		 removed SYS_CPU_FREQ (not used).
01w,13oct92,jcf  configured cache for shared memory facilities.
01v,31oct92,caf  changed LOCAL_MEM_SIZE back to 0x400000 for ROMs (SPR 1729).
		 increased ROM_SIZE to 0x00080000, RAM_HIGH_ADRS to 0x00100000.
01u,29oct92,caf  increased ROM_SIZE to 0x00040000.
01t,22oct92,caf  changed LOCAL_MEM_SIZE and the associated comment.
01s,22oct92,ccc  added INCLUDE_DOSFS for SCSI.
01r,16oct92,ccc  changed note about make dependancy.
01q,01sep92,jcf  cache configuration, changed to INCLUDE_MMU_BASIC.
01p,02sep92,caf  changed NV_RAM constants.
01o,19aug92,rdc  changed INCLUDE_MMU to INCLUDE_BASIC_MMU_SUPPORT.
01n,31jul92,ccc  removed redefine of IO_ADRS_ENP and IO_ADRS_EX.
01m,30jul92,pme  Added shared memory objects #define's.
01l,25jul92,elh  changed BP macros to SM.
01k,08jul92,rdc  temporary addition of INCLUDE_MMU.
01j,28jun92,caf  changed NV_BOOT_LINE.
01i,19jun92,ccc  added clock min/max values, board GCSR group number.
01h,26may92,rrr  the tree shuffle
01g,14oct91,ccc  remove LOCAL_MEM_LOCAL_ADRS, now configured
		 dynamically in sysProcNumSet().
01f,15sep91,jpb  redefined IO_AM_EX_MASTER for 32-bit Excelan access.
01e,27aug91,shl  changed RAM_TEXT_HIGH_ADRS to RAM_HIGH_ADRS.
01d,15aug91,ccc  made BP_OFF_BOARD = FALSE - use local ram for bp.
01c,12aug91,ccc  changed memory size to 4MB, bootroms will work on all boards.
01b,31jul91,ccc  fixed BAUD_CLK_FREQ constant.
01a,10jun91,ccc  derived from mv147/config.h.
*/

/*
This file contains the configuration parameters for the
Motorola MVME167,166.
*/

#ifndef	INCconfigh
#define	INCconfigh

#include "configAll.h"
#include "mv167.h"

#ifdef	MVME166

#define BSP_REV		"/0"		/* 0 for the first mv166 version */
#define DEFAULT_BOOT_LINE \
"ei(0,0)host:/usr/vw/config/mv166/vxWorks h=90.0.0.3 e=90.0.0.50 u=target"

#else	/* MVME166 */

#define BSP_REV		"/1"		/* 1 for the second mv167 version */
#define DEFAULT_BOOT_LINE \
"ei(0,0)host:/usr/vw/config/mv167/vxWorks h=90.0.0.3 e=90.0.0.50 u=target"

#endif	/* MVME166 */

#define	INCLUDE_MMU_BASIC	/* bundled mmu support */

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       (CACHE_COPYBACK | CACHE_SNOOP_ENABLE) 

/*
 * Device controller I/O addresses:
 */

#define	INCLUDE_EI	/* include 82596 driver */
#undef  INCLUDE_ENP	/* exclude enp driver */
#undef  INCLUDE_EX	/* exclude excelan driver */

#if	FALSE			/* change FALSE to TRUE for SCSI interface */
#define	INCLUDE_SCSI		/* include ncr710 driver */
#define	INCLUDE_SCSI_BOOT	/* include ability to boot from SCSI */
#define	INCLUDE_DOSFS		/* file system to be used */
#endif	/* FALSE/TRUE */

/* Interrupt vectors */

#define INT_VEC_ACFAIL          (UTIL_INT_VEC_BASE0 + LBIV_VME_ACFAIL)
#define INT_VEC_ABORT           (UTIL_INT_VEC_BASE0 + LBIV_ABORT)

#define INT_VEC_SCSI            (PCC2_INT_VEC_BASE + PCC2_INT_SCSI)
#define INT_VEC_CLOCK           (PCC2_INT_VEC_BASE + PCC2_INT_TT1)
#define INT_VEC_AUX_CLOCK       (PCC2_INT_VEC_BASE + PCC2_INT_TT2)
#define INT_VEC_LN              (PCC2_INT_VEC_BASE + PCC2_INT_LANC)
#define	INT_VEC_LN_ERR		(PCC2_INT_VEC_BASE + PCC2_INT_LANC_ERR)

/* Miscellaneous definitions */

#define NV_RAM_SIZE    	BBRAM_USER_SIZE		/* 4K user bytes */
#define NV_RAM_ADRS     ((char *) BBRAM_ADRS)

#define BAUD_CLK_FREQ	20000000	/* 20 MHz baud rate "P Clock" (fixed) */

#undef	NUM_TTY
#define	NUM_TTY		N_SIO_CHANNELS

#define SYS_CLK_RATE_MIN  3             /* minimum system clock rate */
#define SYS_CLK_RATE_MAX  5000          /* maximum system clock rate */
#define AUX_CLK_RATE_MIN  3             /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX  5000          /* maximum auxiliary clock rate */

/* Backplane network parameters */

#define	GCSR_GROUP_ADDR	(0xcc)		/* recommended value in manual */
#define SM_INT_TYPE	SM_INT_MAILBOX_1        /* 1-byte write mailbox int */
#define SM_INT_ARG1	VME_AM_SUP_SHORT_IO     /* bus address space */
#define SM_INT_ARG2	((GCSR_GROUP_ADDR << 8) + (sysProcNumGet() << 4) + 2)
						/* bus address */
#define SM_INT_ARG3	0x08			/* value (SIG3 bit) */

/*
 * The backplane master (usually cpu 0) also needs to know the following
 * shared memory pool parameters.
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

/* NONE = allocate from memory */
#define SM_MEM_ADRS	(RAM_LOW_ADRS - (SM_MEM_SIZE + SM_OBJ_MEM_SIZE))

/* sh. mem Objects pool adrs  define NONE to allocate from memory */
#define SM_OBJ_MEM_ADRS (SM_MEM_ADRS+SM_MEM_SIZE)

#define SM_MEM_SIZE	0x0000e000	/* 64K - 8k */
#define SM_OBJ_MEM_SIZE 0x10000         /* sh. mem Objects pool size 64K */
#endif	/* SM_OFF_BOARD */

/* Memory addresses */

/*
 * Local-to-Bus memory address constants:
 * The MVME167 local memory always appears at address 0 locally;
 * its address on the bus is set by the Slave Base Address Register in the
 * VMECHIP2.  The MVME167 can handle mezzanine boards with memory sizes
 * of 4, 8, 16, or 32Mbytes.  If the board has 4Mbytes of memory then the
 * memory will be mapped to the standard (24-bit) address range starting
 * at LOCAL_MEM_BUS_A24.
 *
 * If the board has 8Mbytes or more of DRAM then the memory will be mapped to
 * the extended (32-bit) address range as follows:
 *
 *	 8MB memory at 0x01000000
 *	16MB memory at 0x02000000
 *	32MB memory at 0x04000000
 *
 * MVME167 boards with base addresses set to 0x1000000 (16 Mb) or greater
 * may only be accessed in extended (32-bit) addressing mode (i.e. the remote
 * board must use an extended address modifier code).  This corresponds to
 * a MVME167 with 8 Mbytes or more of RAM.  Be sure that any other boards
 * which will access the MVME167's memory use the appropriate addressing mode.
 *
 * To determine the actual memory size use sysMemTop().  The constant
 * LOCAL_MEM_SIZE is used to configure the MMU in sysLib.c.  For optimal use
 * of cache, build VxWorks with LOCAL_MEM_SIZE set to the size of the
 * MVME167's local RAM.
 */

#define LOCAL_MEM_LOCAL_ADRS	0x00000000	/* fixed at zero */
#define LOCAL_MEM_SIZE		0x00400000	/* Min memory: 4 Mbytes */

#define	LOCAL_MEM_BUS_A24	0x00800000	/* Bus address in A24 range */
                                         /* for boards with 4 Mbytes of RAM */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, RAM_LOW_ADRS and RAM_HIGH_ADRS 
 * are defined in config.h, MakeSkel, Makefile, and Makefile.
 * All definitions for these constants must be identical.
 */

#define	ROM_BASE_ADRS		0xff800000	/* base address of ROM */
#define ROM_TEXT_ADRS		(ROM_BASE_ADRS+8)       /* with PC & SP */
#define	ROM_SIZE		0x00080000	/* 512K ROM space */

#define RAM_HIGH_ADRS		0x00100000	/* RAM address for ROM boot */
#define RAM_LOW_ADRS		0x00020000      /* system image load adrs */


/* MVME166 definitions */

#ifdef	MVME166			/* if using a MVME166 target */

#define	INCLUDE_VSB		/* define if VSB bus is being used */
#define	MVME166_ROM		/* undef if booting from FLASH memory */

#ifdef	MVME166_ROM
#define INCLUDE_FLASH
#define FLASH_ADRS              FLASH_BASE_ADRS
#define FLASH_WIDTH             4
#define FLASH_SIZE_WRITEABLE    0x00020000	/* reasonable write size */
#define FLASH_SIZE              0x00100000	/* 1 Mbyte total flash size */

#undef	ROM_BASE_ADRS
#undef	ROM_SIZE
#define ROM_BASE_ADRS		0xfff80000	/* base address of ROM */
#undef	ROM_TEXT_ADRS
#define ROM_TEXT_ADRS		(ROM_BASE_ADRS+8)       /* with PC & SP */
#define ROM_SIZE		0x00020000	/* 128K ROM space */
#else	/* MVME166_ROM */
#undef	ROM_SIZE
#define ROM_SIZE		0x00100000	/* 1Meg ROM space (FLASH) */
#endif	/* MVME166_ROM */

/*
 * The following parameters are used in the mapping of the local RAM to and
 * from the VSB bus on the MVME166.  The possible addressing space for the VSB
 * is determined by the VSB_BASE_ADRS (indicating the starting address) and the
 * VSB_END_ADRS (indicating the ending address).  The VSB_STEP_ADRS parameter
 * is the address offset from one board mapping to the next.  Where the local
 * RAM of a board is mapped to the VSB address space is determined in part by
 * the processor number assigned to the board.  The location that the local RAM
 * of a board is mapped into the VSB address space is determined by the
 * following equation:
 *
 *     VSB_BASE_ADRS + VSB_STEP_ADRS * (sysProcNumGet() + 1)
 *
 * Hence, processor 0 will be mapped at VSB_BASE_ADRS + VSB_STEP_ADRS, and
 * processor 1 will be mapped at VSB_BASE_ADRS + VSB_STEP_ADRS * 2, etc.
 */

#define VSB_BASE_ADRS   ((unsigned long) 0xe0000000)	/* start of VSB bus */
#define VSB_END_ADRS    ((unsigned long) 0xefffffff)	/* end of VSB bus */
                                          /* must be less than 0xf0000000 */
#define VSB_STEP_ADRS   ((unsigned long) 0x01000000)	/* per target offset */
#define VSB_OFFSET_ADRS ((unsigned long) 0x00000000)	/* VSB offset */
 
#endif	/* MVME166 */

#endif	/* INCconfigh */
