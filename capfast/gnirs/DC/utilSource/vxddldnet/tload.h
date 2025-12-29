/************************************************************************/
/*									*/
/*	File:	tload.h							*/
/*									*/
/*	System definitions for the Tranputer utilities which actually	*/
/*	interface to hardware.  This includes the various downloading	*/
/*	programs (such as LD-ONE), and the various I/O drivers which	*/
/*	are used (TIO, CIO, etc.)					*/
/*									*/
/*		Written by Kirk Bailey of Logical Systems.		*/
/*									*/
/*		Copyright (c) 1986-1989 by Logical Systems.		*/
/*									*/
/*				12/21/89				*/
/*									*/
/************************************************************************/

/*
 *	Define the external definitions for the INMOS "link.c" standard link
 *	I/O interface.  All the routines which need to communicate with a
 *	Transputer link or array will do so through these calls.  The
 *	hardware specific implementation of these calls constitutes most of
 *	the effort needed when porting the Transputer Toolset.
 */
#ifndef	ASM_CODE

extern	int	AnalyzeLink();		/* Assert ANALYZE associated w/link */
extern	int	CloseLink();		/* Close specified link */
extern	int	OpenLink();		/* Open specified link for I/O */
extern	int	ReadLink();		/* Read # of bytes from link */
extern	int	ResetLink();		/* Assert RESET associated w/link */
extern	int	TestError();		/* Status of ERROR associated w/link */
extern	int	TestRead();		/* Determine if byte ready to read */
extern	int	TestWrite();		/* Determine if byte ready to write */
extern	int	WriteLink();		/* Write # of bytes to link */

#endif	/* ASM_CODE */

#define	ANALYZE_DELAY	500		/* MS to pause during analyze action */
#define	INFINITE_TIMEOUT	0	/* No timeout for ReadLink/WriteLink */
#define	MICRO_LOW_TICK	64		/* # of microseconds/low prior. tick */
#define	RESET_DELAY	10		/* MS to pause during reset action */

/*
 *	Record ID types for bootstrap download records.
 */
#define	D_DONE		0		/* Finished, contains SP and PC */
#define	D_DATA		1		/* Data record */
#define	D_LOAD		2		/* Specify load address */
#define	D_STORAGE	3		/* Zeroed out space */

#define	D_MASK		0x3		/* Mask to contain above commands */

/*
 *	CPU type information used by the bootstraps.  The first three macros
 *	are used to define the expected processor class.  This works as
 *	follows:
 *
 *	The bootstrap first determines what the actual CPU type is and mangles
 *	the information such that only 32 bit CPU's without FPU's (ie. T4's),
 *	have the BID_T4 bit set.  If this bit isn't set then the processor is
 *	either a 32 bit CPU with a FPU (ie. a T8), or a 16 bit processor.  This
 *	is determined by testing the BID_T2 bit which if set indicates that the
 *	processor is a 16 bitter.  Otherwise the CPU is assumed to be a T8
 *	class CPU.  The aforementioned "mangling" consists of dividing the
 *	actual value by the maximum number of mask revs allowed for each CPU
 *	(MAX_MASK_MREVS), in the numbering scheme and then adding a constant
 *	value (BID_TRANS), to achieve a easier to decode bitwise decision tree.
 *
 *	Note that the expected CPU class is provided as part of the
 *	"netboot.tal" bootstrap used with LD-NET.  If the CPU is of the wrong
 *	type, the bootstrap passes back an error message which indicates
 *	exactly which CPU was found.  Thus the bootstrap only tests for one of
 *	the three classes of processors unless a mismatch is detected whereupon
 *	the "exact" type is determined and reported.
 *
 *	It is STRONGLY recommended that you do not modify the following
 *	information without a careful study of the loaders and (particularly),
 *	the actual bootstrap code contained in "boot.tal" and "netboot.tal".
 */
#define	BID_T2		0x2		/* T212/T222/T225 */
#define	BID_T4		0x4		/* T400/T414/T425 */
#define	BID_T8		0x8		/* T800/T801/T805 */

#define	BID_MAX_MREVS	10		/* Max. mask revs per CPU type */
#define	BID_TRANS	0x7		/* See above block comment */

/*
 *	Actual CPU types as used by the loaders and bootstraps.
 */
#define	BID_T2XX	200		/* T212/T222 */
#define	BID_T225	40		/* T225 */
#define	BID_T400	50		/* T400 */
#define	BID_T414	400		/* T414 */
#define	BID_T425	0		/* T425 */
#define	BID_T800	170		/* T800 */
#define	BID_T801	20		/* T801 */
#define	BID_T805	10		/* T805 */

/*
 *	The following definitions are used by the network loader and bootstrap
 *	to distinguish what sort of error is being reported.  Each definition
 *	must be a single bit and all must be unused in any other possible
 *	data stored in the error (a CPU type or channel address).  All bits
 *	should reside in the bottom 15 bits to avoid problems with MINT
 *	collisions on T2's (also T2's only reliably propagate the bottom 16
 *	bits of errors anyway).  If a non-zero value comes in which doesn't
 *	have one of these bits set it is understood to be a link address of
 *	an intermediate CPU representing one stage of the error traceback
 *	process.
 */
#define	ERR_BOOT	0x200		/* Or'ed with actual input link */
#define	ERR_CPU		0x400		/* Or'ed in with CPU type ... */
#define	ERR_TIMEOUT	0x800		/* Or'ed with child input link */

/*
 *	Local overlay template used by the "netboot.tal" bootstrap and the
 *	"ld-net.c" network loader.
 *
 *	First a template for the bootstrap child I/O process.
 */
#define	C_M3		0		/* WS(-3) */
#define	C_M2		1		/* WS(-2) */
#define	C_M1		2		/* WS(-1) */
#define	C_ICHAN		3		/* WS(0) Input channel address */
#define	C_STATUS	4		/* WS(1) Completion status */
#define	C_MPTR		5		/* WS(2) Output message pointer */

/*
 *	Now the overall local storage template used in the bootstrap.
 */
#define	CALL_OFFSET	4		/* # of words a call moves you in WS */
#define	IOWSD		3		/* # of words required by I/O desch. */
#define	LOCAL_OFFSET	1		/* Locals start in second word */
#define	LOCALS_STACK	(CALL_OFFSET+IOWSD)	/* Locally required stack */

#define	OUR_ADDRESS	0		/* Current node address */
#define	SCRATCH1	1		/* Scratch storage */
#define	UP_IN		2		/* Parent input channel */
#define	SCRATCH2	3		/* Scratch storage */
#define	ICPU_TYPE	4		/* Expected CPU type (overlay) */
#define	LENGTH		4		/* Message length */
#define	IUP_IN		5		/* Expected boot in link (overlay) */
#define	MESS_PTR	5		/* Message pointer */
#define	TIMEOUT		6		/* Current timeout time */
#define	STACK		7		/* Initial stack pointer */
#define	RIGHT		8		/* Right child process template */
#define	LEFT		14		/* Left child process template */
#define	USER_NODE	20		/* User supplied node # (0 is root) */
#define	ENTRY		21		/* Entrypoint */
#define	INIT_TIMEOUT	22		/* Timeout interval for this node */
#define	PROG_NUM	23		/* Desired program number */

#define	PARAM_LENGTH	(PROG_NUM + 1)	/* Locals size (in words) */
