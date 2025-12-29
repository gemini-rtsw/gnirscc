
/* pulseDelayRecord.h */
#ifndef INCpulseDelayRecordh
#define INCpulseDelayRecordh
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
struct pulseDelayRecord	{
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
	/* start of pulseDelay specific fields */ 
	struct link	out;		/* Output Specification */
	unsigned short	unit;		/* Delay Time Units */
	char		p102[6];		/* Created Pad  */
	double		dly;		/* Pulse Delay */
	double		wide;		/* Pulse Width */
	double		odly;		/* Old Pulse Delay */
	double		owid;		/* Old Pulse Width */
	unsigned short	ctyp;		/* Clock Type */
	unsigned short	cedg;		/* Clock Signal Edge */
	short		ecs;		/* Ext Clock Source */
	char		p103[2];		/* Created Pad  */
	double		ecr;		/* Ext Clock Rate (HZ) */
	unsigned short	llow;		/* Low Logic Level */
	unsigned short	val;		/* Trigger Detect */
	unsigned short	ttyp;		/* Trigger Type */
	unsigned short	hts;		/* Hardware Trigger Src */
	struct link	stl;		/* Soft Trigger Location */
	unsigned short	stv;		/* Soft Trigger Value */
	char		p104[6];		/* Created Pad  */
	struct link	glnk;		/* Soft Gate Location */
	unsigned short	gate;		/* Soft Gate Value */
	char		p105[2];		/* Created Pad  */
	float		hopr;		/* High Operating Range */
	float		lopr;		/* Low Operating Range */
	short		prec;		/* Display Precision */
	unsigned short	pfld;		/* Field Processing */
};
typedef struct pulseDelayRecord pulseDelayRecord;
#endif

