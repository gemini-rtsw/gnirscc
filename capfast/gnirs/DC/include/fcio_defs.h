/*******************************************************************************
 * Program:     firedrvr
 * File:	fcio_defs.h
 * Purpose:	Defines structures and routine prototypes for SUN fire C i/o
 *		communications server 
 * Author:	Nick C. Buchholz
 * History:
 *	14-Dec-1990 - created file - ncb
 *
 *$Id: fcio_defs.h,v 1.2 2009/05/27 19:32:27 fkraemer Exp $
 *
 *
 *$Log: fcio_defs.h,v $
 *Revision 1.2  2009/05/27 19:32:27  fkraemer
 *fkraemer - copied my complete working dir over trunk
 *
 *Revision 1.1.1.1  1998/12/15 16:18:20  buchholz
 *Imported gnaacSrc into CVS
 *
 * Revision 1.0  1994/05/17  18:01:09  quenten
 * Initial revision
 *
 *****************************************************************************/
#define DEBUG	1

#ifndef FIRECIO
#define FIRECIO

#define COPYRIGHT	"Copyright Jan 1991 by Aura Inc."
#define MAXFILES	20		/* Maximum number of open files */

#define PROGNUM		0x42303131      /* program number "B011" ascii as int */
#define VERSNUM		0x46495245      /* version number "FIRE" ascii as text */

#define	MAX_MSG_DATA	25000		/* Max length of msg data contents */
#define	MAX_MSG_OVER	50		/* Max length of msg overhead */

/* Seek origin "cardinal" values for cio/tcio.  From Logical Systems */
#define	_SEEK_SET	0
#define	_SEEK_CUR	1
#define	_SEEK_END	2

/* network portable versions of magic values.  */
#define	RPC_EOF		(-1)		/* Portable version of EOF */
#define	RPC_NULL	0		/* Portable version of NULL */
#define	RPC_NOTEOF	1		/* Portable version of !EOF */
#define	RPC_NOTNULL	2		/* Portable version of !NULL */

/* Structure template for "localtime/gmtime" functions.  This structure
 * must be completely "packed" (no internal alignment gaps), by the host
 * "C" compiler to be compatible with the protocol and the Transputer.
 */
struct	rpc_time
{
    uchar	tm_sec;		/* Seconds after the minute [0-59] */
    uchar	tm_min;		/* Minutes after the hour [0-59] */
    uchar	tm_hour;	/* Hours since midnight [0-23] */
    uchar	tm_mday;	/* Day of month [1-31] */
    uchar	tm_mon;		/* Months since January [0-11] */
    uchar	tm_year;	/* Years since 1900 */
    uchar	tm_wday;	/* Days since Sunday [0-6] */
    uchar	low_tm_yday;	/* Day of year [0-365] (LOW BYTE) */
    uchar	high_tm_yday;	/* Day of year [0-365] (HIGH BYTE) */
    uchar	tm_isdst;	/* Daylight Savings Time flag */
};

#define	RPC_TIME_DATA	10		/*  # of bytes of time data in above */

/* I/O Function numbers as defined by the cio protocol from Logical Systems */
#define	FN_ARG_GET	0
#define	FN_ARG_SIZE	1
#define	FN_CIOEXT	2
#define	FN_CLEARERR	3
#define	FN_CLOSE	4
#define	FN_CREAT	5
#define	FN_DUP		6
#define	FN_DUP2		7
#define	FN_ERRNO	8
#define	FN_EXIT		9
#define	FN_FCLOSE	10
#define	FN_FCLOSEALL	11
#define	FN_FDOPEN	12
#define	FN_FEOF		13
#define	FN_FERROR	14
#define	FN_FFLUSH	15
#define	FN_FGETC	16
#define	FN_FGETS	17
#define	FN_FILENO	18
#define	FN_FOPEN	19
#define	FN_FPUTC	20
#define	FN_FREAD	21
#define	FN_FREOPEN	22
#define	FN_FSEEK	23
#define	FN_FTELL	24
#define	FN_FWRITE	25
#define	FN_GETCH	26
#define	FN_GETCHE	27
#define	FN_GETENV	28
#define	FN_GETS		29
#define	FN_GETW		30
#define	FN_KBHIT	31
#define	FN_LSEEK	32
#define	FN_OPEN		33
#define	FN_PUTW		34
#define	FN_READ		35
#define	FN_REMOVE	36
#define	FN_RENAME	37
#define	FN_SYSTEM	38
#define	FN_TIME		39
#define	FN_TIMEGMT	40
#define	FN_TIMELOC	41
#define	FN_TMPFILE	42
#define	FN_TMPNAM	43
#define	FN_UNGETC	44
#define	FN_WRITE	45

#endif				/* FIRECIO */

