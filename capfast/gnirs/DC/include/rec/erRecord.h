
/* erRecord.h */
#ifndef INCerRecordh
#define INCerRecordh
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
struct erRecord	{
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
	/* start of er specific fields */ 
	char		val;		/* Worthless Value */
	char		p74 [7];		/* Created Pad  */
	struct link	out;		/* Output Specification */
	unsigned short	enab;		/* Master Enable */
	char		p75 [2];		/* Created Pad  */
	long		taxi;		/* Taxi Violation */
	long		ltax;		/* Last Taxi Violation */
	unsigned short	trg0;		/* Trigger 0 Enable */
	unsigned short	trg1;		/* Trigger 1 Enable */
	unsigned short	trg2;		/* Trigger 2 Enable */
	unsigned short	trg3;		/* Trigger 3 Enable */
	unsigned short	trg4;		/* Trigger 4 Enable */
	unsigned short	trg5;		/* Trigger 5 Enable */
	unsigned short	trg6;		/* Trigger 6 Enable */
	unsigned short	otp0;		/* OTP 0 Enable */
	unsigned short	otp1;		/* OTP 1 Enable */
	unsigned short	otp2;		/* OTP 2 Enable */
	unsigned short	otp3;		/* OTP 3 Enable */
	unsigned short	otp4;		/* OTP 4 Enable */
	unsigned short	otp5;		/* OTP 5 Enable */
	unsigned short	otp6;		/* OTP 6 Enable */
	unsigned short	otp7;		/* OTP 7 Enable */
	unsigned short	otp8;		/* OTP 8 Enable */
	unsigned short	otp9;		/* OTP 9 Enable */
	unsigned short	otpa;		/* OTP 10 Enable */
	unsigned short	otpb;		/* OTP 11 Enable */
	unsigned short	otpc;		/* OTP 12 Enable */
	unsigned short	otpd;		/* OTP 13 Enable */
	unsigned short	otl0;		/* OTL 0 Enable */
	unsigned short	otl1;		/* OTL 1 Enable */
	unsigned short	otl2;		/* OTL 2 Enable */
	unsigned short	otl3;		/* OTL 3 Enable */
	unsigned short	otl4;		/* OTL 4 Enable */
	unsigned short	otl5;		/* OTL 5 Enable */
	unsigned short	otl6;		/* OTL 6 Enable */
	long		dgcm;		/* DG Change Mask */
	unsigned short	dg0e;		/* DG 0 Enable */
	unsigned short	dg0d;		/* DG 0 Delay */
	unsigned short	dg0w;		/* DG 0 Width */
	unsigned short	dg1e;		/* DG 1 Enable */
	unsigned short	dg1d;		/* DG 1 Delay */
	unsigned short	dg1w;		/* DG 1 Width */
	unsigned short	dg2e;		/* DG 2 Enable */
	unsigned short	dg2d;		/* DG 2 Delay */
	unsigned short	dg2w;		/* DG 2 Width */
	unsigned short	dg3e;		/* DG 3 Enable */
	unsigned short	dg3d;		/* DG 3 Delay */
	unsigned short	dg3w;		/* DG 3 Width */
};
typedef struct erRecord erRecord;
#endif

