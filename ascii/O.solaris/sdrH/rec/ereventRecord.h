
/* ereventRecord.h */
#ifndef INCereventRecordh
#define INCereventRecordh
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
struct ereventRecord	{
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
	/* start of erevent specific fields */ 
	char		val;		/* Worthless Value */
	char		p76 [7];		/* Created Pad  */
	struct link	out;		/* Output Specification */
	unsigned short	enab;		/* Enable */
	char		p77 [2];		/* Created Pad  */
	long		enm;		/* Event Number */
	long		lenm;		/* Last Event Number */
	long		lout;		/* Last Out Enable Mask */
	unsigned short	out0;		/* Out 0 Enable */
	unsigned short	out1;		/* Out 1 Enable */
	unsigned short	out2;		/* Out 2 Enable */
	unsigned short	out3;		/* Out 3 Enable */
	unsigned short	out4;		/* Out 4 Enable */
	unsigned short	out5;		/* Out 5 Enable */
	unsigned short	out6;		/* Out 6 Enable */
	unsigned short	out7;		/* Out 7 Enable */
	unsigned short	out8;		/* Out 8 Enable */
	unsigned short	out9;		/* Out 9 Enable */
	unsigned short	outa;		/* Out 10 Enable */
	unsigned short	outb;		/* Out 11 Enable */
	unsigned short	outc;		/* Out 12 Enable */
	unsigned short	outd;		/* Out 13 Enable */
	unsigned short	vme;		/* VME IRQ Enable */
};
typedef struct ereventRecord ereventRecord;
#endif

