
/* compressRecord.h */
#ifndef INCcompressRecordh
#define INCcompressRecordh
#include <ellLib.h>	/* list structure definition for the monitor list */
#ifndef	INCLsemLibh
#include <semLib.h>	/* semaphore structure definition for the monitor lock*/
#endif
#ifndef INCLfast_lockh
#include <fast_lock.h>	/* fast lock data structures and macros */
#endif
#ifndef INCLlinkh
#include <link.h>	/* fast lock data structures and macros */
#endif
#ifndef INC_tsDefs_h
#include <tsDefs.h>	/* time-stamp related definitions */
#endif
struct compressRecord	{
	char		name[29];		/* Record Name */
	char		desc[29];		/* Descriptor */
	char		asg[29];		/* Access Security Group */
	char		p1  [1];		/* Created Pad  */
	unsigned short	scan;		/* Scan Mechanism */
	unsigned short	pini;		/* Process at iocInit */
	short		phas;		/* Scan Phase */
	short		evnt;		/* Event Number */
	short		tse;		/* Time Stamp Event */
	char		p2  [6];		/* Created Pad  */
	struct link	tsel;		/* Time Stamp Link */
	unsigned short	dtyp;		/* Device Type */
	short		disv;		/* Disable Value */
	short		disa;		/* Disable */
	char		p3  [2];		/* Created Pad  */
	struct link	sdis;		/* Scanning Disable */
	FAST_LOCK	mlok;		/* Monitor fastlock */
	ELLLIST		mlis;		/* Monitor List */
	unsigned char	disp;		/* Disable putField */
	unsigned char	proc;		/* Force Processing */
	unsigned short	stat;		/* Alarm Status */
	unsigned short	sevr;		/* Alarm Severity */
	unsigned short	nsta;		/* New Alarm Status */
	unsigned short	nsev;		/* New Alarm Severity */
	unsigned short	acks;		/* Alarm Ack Severity */
	unsigned short	ackt;		/* Alarm Ack Transient */
	unsigned short	diss;		/* Disable Alarm Sevrty */
	short		lset;		/* Lock Set */
	unsigned char	lcnt;		/* Lock Count */
	unsigned char	pact;		/* Record active */
	unsigned char	putf;		/* dbPutField process */
	unsigned char	rpro;		/* Reprocess  */
	char		p4  [2];		/* Created Pad  */
	void		*asp;		/* Access Security Pvt */
	void		*ppn;		/* addr of PUTNOTIFY */
	void		*ppnn;		/* next record PUTNOTIFY */
	void		*spvt;		/* Scan Private */
	void		*rset;		/* Address of RSET */
	struct dset	*dset;		/* DSET address */
	void		*dpvt;		/* Device Private */
	unsigned short	prio;		/* Scheduling Priority */
	unsigned char	tpro;		/* Trace Processing */
	char bkpt;		/* Break Point */
	unsigned char	udf;		/* Undefined */
	char		p5  [3];		/* Created Pad  */
	TS_STAMP	time;		/* Time */
	struct link	flnk;		/* Forward Process Link */
	/* start of compress specific fields */ 
	void *		val;		/* Value */
	char		p56 [4];		/* Created Pad  */
	struct link	inp;		/* Input Specification */
	short		res;		/* Reset */
	unsigned short	alg;		/* Compression Algorithm */
	unsigned long	nsam;		/* Number of Values */
	unsigned long	n;		/* N to 1 Compression */
	float		ihil;		/* Init High Interest Lim */
	float		ilil;		/* Init Low Interest Lim */
	float		hopr;		/* High Operating Range */
	float		lopr;		/* Low Operating Range */
	short		prec;		/* Display Precision */
	char		egu[16];		/* EngineeringUnits */
	char		p57 [2];		/* Created Pad  */
	unsigned long	off;		/* Offset */
	unsigned long	nuse;		/* Number Used */
	double		*bptr;		/* Buffer Pointer */
	double		*sptr;		/* Summing Buffer Ptr */
	double		*wptr;		/* Working Buffer Ptr */
	char		p58 [4];		/* Created Pad  */
	double		cvb;		/* Compress Value Buffer */
	unsigned long	inx;		/* Compressed Array Inx */
};
typedef struct compressRecord compressRecord;
#endif

