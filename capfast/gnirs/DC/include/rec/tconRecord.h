
/* tconRecord.h */
#ifndef INCtconRecordh
#define INCtconRecordh
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
struct tconRecord	{
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
	/* start of tcon specific fields */ 
	long		val;		/* Current value */
	char		p146[4];		/* Created Pad  */
	struct link	out;		/* Output Specification */
	unsigned short	egu;		/* Units name */
	char		p147[2];		/* Created Pad  */
	long		hopr;		/* High Operating Range */
	long		lopr;		/* Low Operating Range */
	long		hihi;		/* Hihi Alarm Limit */
	long		lolo;		/* Lolo Alarm Limit */
	long		high;		/* High Alarm Limit */
	long		low;		/* Low Alarm Limit */
	unsigned short	hhsv;		/* Hihi Severity */
	unsigned short	llsv;		/* Lolo Severity */
	unsigned short	hsv;		/* High Severity */
	unsigned short	lsv;		/* Low Severity */
	long		hyst;		/* Alarm Deadband */
	long		adel;		/* Archive Deadband */
	long		mdel;		/* Monitor Deadband */
	long		lalm;		/* Last Value Alarmed */
	long		alst;		/* Last Value Archived */
	long		mlst;		/* Last Val Monitored */
	char		p148[4];		/* Created Pad  */
	struct link	siol;		/* Sim Input Specifctn */
	long		sval;		/* Simulation Value */
	char		p149[4];		/* Created Pad  */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
	long		prec;		/* Display Precision */
	long		mon;		/* Post monitor */
	char		p150[4];		/* Created Pad  */
	double		setp;		/* Set Point */
	double		lstp;		/* Old Set Point */
	double		gain;		/* Gain */
	double		lgan;		/* Old Gain */
	double		rate;		/* Rate */
	double		lrte;		/* Old Rate */
	double		rst;		/* Reset */
	double		lrst;		/* Old Reset */
	unsigned short	rang;		/* Heater range */
	unsigned short	lrng;		/* Old Heater range */
	unsigned short	tune;		/* Autotuning */
	unsigned short	ltun;		/* Old Autotuning */
};
typedef struct tconRecord tconRecord;
#endif

