
/* aiRecord.h */
#ifndef INCaiRecordh
#define INCaiRecordh
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
struct aiRecord	{
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
	/* start of ai specific fields */ 
	double		val;		/* Current EGU Value */
	struct link	inp;		/* Input Specification */
	short		prec;		/* Display Precision */
	unsigned short	linr;		/* Linearization */
	float		eguf;		/* Engineer Units Full */
	float		egul;		/* Engineer Units Low */
	char		egu[16];		/* Engineering Units */
	float		hopr;		/* High Operating Range */
	float		lopr;		/* Low Operating Range */
	float		aoff;		/* Adjustment Offset */
	float		aslo;		/* Adjustment Slope */
	float		smoo;		/* Smoothing */
	float		hihi;		/* Hihi Alarm Limit */
	float		lolo;		/* Lolo Alarm Limit */
	float		high;		/* High Alarm Limit */
	float		low;		/* Low Alarm Limit */
	unsigned short	hhsv;		/* Hihi Severity */
	unsigned short	llsv;		/* Lolo Severity */
	unsigned short	hsv;		/* High Severity */
	unsigned short	lsv;		/* Low Severity */
	double		hyst;		/* Alarm Deadband */
	double		adel;		/* Archive Deadband */
	double		mdel;		/* Monitor Deadband */
	double		lalm;		/* Last Value Alarmed */
	double		alst;		/* Last Value Archived */
	double		mlst;		/* Last Val Monitored */
	double		eslo;		/* Rawto EGU Slope */
	long		roff;		/* Raw Offset */
	void *	pbrk;		/* Ptrto brkTable */
	short		init;		/* Initialized? */
	short		lbrk;		/* LastBreak Point */
	long		rval;		/* Current Raw Value */
	long		oraw;		/* Previous Raw Value */
	char		p39 [4];		/* Created Pad  */
	struct link	siol;		/* Sim Input Specifctn */
	double		sval;		/* Simulation Value */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
};
typedef struct aiRecord aiRecord;
#endif

