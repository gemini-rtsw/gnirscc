
/* pidRecord.h */
#ifndef INCpidRecordh
#define INCpidRecordh
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
struct pidRecord	{
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
	/* start of pid specific fields */ 
	float		val;		/* Setpoint */
	char		p98 [4];		/* Created Pad  */
	struct link	cvl;		/* Controlled Value Loc */
	struct link	stpl;		/* Setpoint Location */
	unsigned short	smsl;		/* Setpoint Mode Select */
	short		prec;		/* Display Precision */
	float		mdt;		/* Min Delta T */
	float		kp;		/* Proportional Gain */
	float		ki;		/* Intergral Gain */
	float		kd;		/* Derivative Gain */
	char		egu[16];		/* Engineering Units */
	float		hopr;		/* High Operating Range */
	float		lopr;		/* Low Operating Range */
	float		hihi;		/* Hihi Deviation Limit */
	float		lolo;		/* Lolo Deviation Limit */
	float		high;		/* High Deviation Limit */
	float		low;		/* Low Deviation Limit */
	unsigned short	hhsv;		/* Hihi Severity */
	unsigned short	llsv;		/* Lolo Severity */
	unsigned short	hsv;		/* High Severity */
	unsigned short	lsv;		/* Low Severity */
	float		hyst;		/* Alarm Deadband */
	float		adel;		/* Archive Deadband */
	float		mdel;		/* Monitor Deadband */
	float		odel;		/* DM Deadband */
	float		cval;		/* Controlled Value */
	float		dm;		/* Change in Manip Var */
	float		odm;		/* Prev Change */
	float		p;		/* P component */
	float		i;		/* I component */
	float		d;		/* D component */
	unsigned long	ct;		/* Clock Ticks Prev */
	float		dt;		/* Delta T */
	float		err;		/* Error */
	float		derr;		/* Change in Error */
	float		lalm;		/* Last Value Alarmed */
	float		alst;		/* Last Value Archived */
	float		mlst;		/* Last Value Monitored */
};
typedef struct pidRecord pidRecord;
#endif

