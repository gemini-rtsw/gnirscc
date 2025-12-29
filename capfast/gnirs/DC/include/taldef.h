/************************************************************************/
/*									*/
/*	File:	taldef.h						*/
/*									*/
/*	System definitions for the Transputer assembler (TASM), linker	*/
/*	(TLNK), librarian (TLIB), and the various program loading	*/
/*	utilities.							*/
/*									*/
/*		Written by Kirk Bailey of Logical Systems.		*/
/*									*/
/*		Copyright (c) 1986-1989 by Logical Systems.		*/
/*									*/
/*				09/01/86				*/
/*									*/
/************************************************************************/

#define	KIT_VERSION	FALSE		/* TRUE for T400-only version	*/

#ifndef	ONLY_REL_RECS		/* If we just need record #'s */
/*
 *	H_OPSYS	- the host operating system that TCX will run under.
 *
 *	The host environments are:
 *
 *		H_OPSYS
 *		-------
 *		MS_DOS	- TCX will run under MSDOS
 *		VMS	- TCX will run under VAX/VMS
 *		UNIX	- TCX will run under Unix
 *		BSD	- TCX will run under "modern" BSD Unix
 *		MPW	- TCX will run under MPW on the Mac
 *		XENIX   - TCX will run under Microsoft XENIX on PC's
 */
#if !defined(vxWorks)
#define	BSD		1
#endif
#define	MPW		2
#define	MS_DOS		3
#define	UNIX		4
#define	VMS		5
#define	XENIX		6
#define VXWORKS		7 /* ADDED 2-APR-97 to make ldnet for vxworks */
/*
 *	Define host environment.
 */

#if defined(vxWorks)
#define	H_OPSYS		VXWORKS
#else
#define	H_OPSYS		BSD
#endif

#if	((H_OPSYS == BSD) || (H_OPSYS == UNIX) || (H_OPSYS == XENIX))
#define	STDERR	stderr
#else	/* ! (BSD || UNIX || XENIX) */
#define	STDERR	stdout
#endif	/* BSD || UNIX || XENIX */

#if	H_OPSYS == VMS
#define	unlink(name)		delete(name)
#endif	/* H_OPSYS == VMS */

#if	H_OPSYS == BSD
#define	strchr			index
#define	strrchr			rindex
#define	memcpy(dst, src, len)	bcopy(src, dst, len)
#define	setvbuf(f,b,n,s)	(setbuffer(f,b,s),0)
#endif	/* H_OPSYS == BSD */

#if	H_OPSYS != MPW
#define	VERBOSE		1
#else
#define	VERBOSE		0
#endif	/* H_OPSYS != MPW */

/*
 *	Type shortforms.
 */

#define	MAXINT		32767		/* (2**15)-1, max positive int */
typedef	FILE		*STREAM;
typedef	char		*STRING;

#if !defined(vxWorks)
typedef	int		BOOL;
typedef	unsigned char	UCHAR;
typedef	unsigned int	UINT;
typedef	unsigned short	USHORT;
typedef	unsigned long	ULONG;
#endif

typedef	long		SLONG;
#define	MAXULONG	((unsigned long) 0xFFFFFFFFL)	/* (2**32)-1 */

/*
 *	General definitions.
 */

#define	FALSE		0

#if !defined(vxWorks)
#define	FOREVER		for(;;)
#endif

#define	TRUE		1
#define	UNUSED		0
#define	VOID		void		/* Unused return value */

#define	ASM_NAME	"TASM Assembler"
#define	COPYRIGHT	"Copyright (c) 1986-1991 by Logical Systems"
#define	LIB_NAME	"TLIB Librarian"
#define	LIB_PATH	"TLIB"		/* Env. var. for library search path */
#define	LINK_NAME	"LINKNAME"	/* Transputer link name for driver */
#define	LNK_NAME	"TLNK Linker"
#define	PATH_PUNC	';'		/* Punc. for TLIB string */
#define TCC_NAME	"TCC Transputer Compiler Interface"
#define	TEMP_PATH	"TMP"		/* Env. var. for temp directory */
#define	VERSION		"Version 91.1"

/*
 *	Symbolic definitions of the various processor word sizes in bytes.
 */

#define	WORD_SIZE_16B	2		/* For 16 bit processors */
#define	WORD_SIZE_32B	4		/* For 32 bit processors */

/*
 *	Maximum instruction sizes in bytes for range checking when binding PC
 *	relative references.
 */

#define	MAX_I_SIZE_16B	4		/* For 16 bit processors */
#define	MAX_I_SIZE_32B	8		/* For 32 bit processors */

/*
 *	I/O definitions.  Note that those which apply to TCX and PP are
 *	duplicated using different names in the TCX and PP directories (ugh!)
 */

#define	LD_INFO_EXT	".nif"		/* LD-NET net info file default ext. */
#define	PP_AINPUT_EXT	".pal"		/* PP asm input file default ext. */
#define	PP_CINPUT_EXT	".c"		/* PP "C" input file default ext. */
#define	PP_OUTPUT_EXT	".pp"		/* PP output file default extension */
#define	TASM_INPUT_EXT	".tal"		/* TASM input file default extension */
#define	TASM_OUTPUT_EXT	".trl"		/* TASM output file default ext. */
#define	TASM_LIST_EXT	".lst"		/* TASM listing file extension */
#define	TCX_INPUT_EXT	PP_OUTPUT_EXT	/* TCX input file default extension */
#define	TCX_OUTPUT_EXT	TASM_INPUT_EXT	/* TCX output file default extension */
#define	TLNK_INPUT_EXT	TASM_OUTPUT_EXT	/* TLNK input file default ext. */
#define	TLNK_LIB_EXT	".tll"		/* TLNK library file default ext. */
#define	TLNK_OUTPUT_EXT	".tld"		/* TLNK output file default ext. */
#define	TLNK_CMD_EXT	".lnk"		/* TLNK command file default ext. */

/*
 *	Configuration limits.
 */

#if	H_OPSYS == BSD
#include <sys/param.h>
#define	FNSIZE		MAXPATHLEN
#else	/* H_OPSYS != BSD */
#define	FNSIZE		80		/* # of characters in file pathname */
#endif	/* H_OPSYS == BSD */

#define	LOCAL_BUF_SIZE	16384		/* Local buffering size */
#define	MAXLNSIZE	300		/* Longest legal input line */
#define	NUMMODULES	256		/* # of modules (1 <= # <= 255) */
#define	SLBMODULE	250		/* Module reserved for "static link" */
#define	SYMNMESIZE	255		/* # chars in symbol (0 <= # <= 255) */

/*
 *	Allowed values for program "exit"s.
 */

#define	ERRORS		1		/* Abnormal termination */
#define	NOERRORS	0		/* Normal termination */

/************************************************************************/
/*									*/
/*			Portability Configuration Options		*/
/*									*/
/************************************************************************/

/*
 *	CRLF defines whether the system needs to convert CR,LF pairs into
 *	linefeeds for text files (needed on many micro's).  The effect of this
 *	switch (if TRUE), is to make "fopen" calls contain a trailing "b" for
 *	binary files and "t" for text files.  For most UNIX installations this
 *	switch should be FALSE.
 */

#define	CRLF		TRUE

/*
 *	IOBUFSIZE defines the size of explicit buffer (in bytes), which is
 *	assigned to each temporary file in TASM and TLNK via a "setvbuf"
 *	call.  If this value is set to 0, then the system supplied default
 *	buffer size is used (no "setvbuf" call).  The maximum value of this
 *	macro is defined to be the largest positive integer the system
 *	"C" compiler supports for the "int" type (usually (2^15)-1 or
 *	(2^32)-1).  The value as shipped (32256) works well for most systems
 *	and probably won't need to be changed...
 */

#if	H_OPSYS == XENIX
#define	IOBUFSIZE	0
#else
#define	IOBUFSIZE	32256
#endif	/* H_OPSYS == XENIX */

/*
 *	NOFILECASE is TRUE if file/path names are case insensitive for the
 *	purposes of comparison.
 */

#define	NOFILECASE	TRUE

#endif	/* ONLY_REL_RECS */
/*
 *	The definitions for the 16 primary Transputer functions (opcodes).
 */

#define	J		0x00		/* Jump */
#define	LDLP		0x10		/* Load local pointer */
#define	PFIX		0x20		/* Prefix */
#define	LDNL		0x30		/* Load non-local */
#define	LDC		0x40		/* Load constant */
#define	LDNLP		0x50		/* Load non-local pointer */
#define	NFIX		0x60		/* Negative prefix */
#define	LDL		0x70		/* Load local */
#define	ADC		0x80		/* Add constant */
#define	CALL		0x90		/* Call */
#define	CJ		0xA0		/* Conditional jump */
#define	AJW		0xB0		/* Adjust workspace */
#define	EQC		0xC0		/* Equals constant */
#define	STL		0xD0		/* Store local */
#define	STNL		0xE0		/* Store non-local */
#define	OPR		0xF0		/* Operate */

#define	DOTLDC		(LDC | 0x1)	/* ".LDC", optimizing "LDC" */
#define	DOTLDCR		(LDC | 0x2)	/* ".LDC" forced PC relative */
#define	LDPI		0x1B		/* "LDPI" encoding */
#define	LDPI_SIZE	2		/* Length of "LDPI" instruction */
#define	MINT		0x42		/* "MINT" encoding */
#define	MINT_SIZE	2		/* Length of "MINT" instruction */
#define	NOP		AJW		/* Used to pad instructions out */
#define	RET		0x20		/* Not one of basic 16, but useful */

/*
 *	Record definitions used in the various files created during Transputer
 *	program development.
 */

#define	T_RESERVED	0		/* For TASM/TLNK internal use only */
#define	T_REL_FILE	1		/* File is normal relocatable file */
#define	T_LIB_FILE	2		/* File is a library file */
#define	T_LD_FILE	3		/* File is linked and down-loadable */
#define	T_SIZE		4		/* File/module size in bytes */
#define	T_EOF		5		/* End-of-file record */
#define	T_SYMBOL	6		/* External/public symbol and info */
#define	T_FILENAME	7		/* Specifies the original filename */
#define	T_MODULE	8		/* Specifies the active module # */
#define	T_ALIGN		9		/* Next record should be aligned */
#define	T_DATA		10		/* Initialized data record */
#define	T_REL_DATA	11		/* PC relative data record */
#define	T_RELSYM_DATA	12		/* PC relative symbolic data record */
#define	T_RELREL_DATA	13		/* "rel-rel" data record */
#define	T_ADDR_DATA	14		/* Address-of-symbol data record */
#define	T_STORAGE	15		/* Uninitialized data record */
#define	T_DEF		16		/* A label definition */
#define	T_SET		17		/* Constant equate record */
#define	T_REL_OP	18		/* PC relative instruction */
#define	T_RELSYM_OP	19		/* PC relative symbolic instruction */
#define	T_RELREL_OP	20		/* "rel-rel" instruction */
#define	T_ADDR_OP	21		/* Address-of-symbol instruction */
#define	T_LOAD		22		/* Load address for next records */
#define	T_STACK		23		/* Starting stack addr for program */
#define	T_ENTRY		24		/* Entrypoint address for program */
#define	T_DEBUG_DATA	25		/* Non-symbolic debugging info */
#define	T_DEBUGSYM_DATA	26		/* Symbolic debugging info */
#define	T_WRELREL_OP	27		/* Word size "rel-rel" instruction */

/*
 *	Target processor description field values for use with the T_REL_FILE
 *	and T_LIB_FILE records.
 */

#define	TYPE_UNKNOWN	0		/* Unknown (32 bit), processor type */
#define	TYPE_414	1		/* T414/T425 target processor type */
#define	TYPE_800	2		/* T800/T801/T805 processor type */
#define	TYPE_212	3		/* T212/T222/T225 processor type */
