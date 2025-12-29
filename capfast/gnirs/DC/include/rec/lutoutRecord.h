
/* lutoutRecord.h */
#ifndef INClutoutRecordh
#define INClutoutRecordh
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
struct lutoutRecord	{
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
	/* start of lutout specific fields */ 
	char		val[40];		/* Input String */
	char		oval[40];		/* Old Input String */
	long		nval;		/* Number of Values */
	long		onvl;		/* Old Number */
	long		selb;		/* Selection Bits */
	long		prec;		/* Precision */
	char		fdir[40];		/* Init File Directory */
	char		fnam[40];		/* Init File Name */
	void * ltbl;		/* Lookup Table */
	char		p30 [4];		/* Created Pad  */
	struct link	llnk;		/* Lookup Table Link */
	long		load;		/* Reload Table */
	char		p31 [4];		/* Created Pad  */
	struct link	outa;		/* Output A */
	struct link	outb;		/* Output B */
	struct link	outc;		/* Output C */
	struct link	outd;		/* Output D */
	void *vala;		/* Value of Output A */
	void *valb;		/* Value of Output B */
	void *valc;		/* Value of Output C */
	void *vald;		/* Value of Output D */
	void *olda;		/* Old Value of Output A */
	void *oldb;		/* Old Value of Output B */
	void *oldc;		/* Old Value of Output C */
	void *oldd;		/* Old Value of Output D */
	unsigned short	ftva;		/* Type of Value A */
	unsigned short	ftvb;		/* Type of Value B */
	unsigned short	ftvc;		/* Type of Value C */
	unsigned short	ftvd;		/* Type of Value D */
};
typedef struct lutoutRecord lutoutRecord;
#endif

