
/* aoRecord.h */
#ifndef INCaoRecordh
#define INCaoRecordh
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
struct aoRecord	{
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
	/* start of ao specific fields */ 
	double		val;		/* Desired Output */
	double		oval;		/* Output Value */
	struct link	out;		/* Output Specification */
	float		oroc;		/* Output Rate of Chang */
	char		p42 [4];		/* Created Pad  */
	struct link	dol;		/* Desired Output Loc */
	unsigned short	omsl;		/* Output Mode Select */
	unsigned short	oif;		/* Out Full/Incremental */
	short		prec;		/* Display Precision */
	unsigned short	linr;		/* Linearization */
	float		eguf;		/* Eng Units Full */
	float		egul;		/* Eng Units Low */
	char		egu[16];		/* Engineering Units */
	long		roff;		/* Raw Offset */
	char		p43 [4];		/* Created Pad  */
	double		eslo;		/* EGU to Raw Slope */
	float		drvh;		/* Drive High Limit */
	float		drvl;		/* Drive Low Limit */
	float		hopr;		/* High Operating Range */
	float		lopr;		/* Low Operating Range */
	float		aoff;		/* Adjustment Offset */
	float		aslo;		/* Adjustment Slope */
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
	long		rval;		/* Current Raw Value */
	long		oraw;		/* Previous Raw Value */
	long		rbv;		/* Readback Value */
	long		orbv;		/* Prev Readback Value */
	double		pval;		/* Previous value */
	double		lalm;		/* Last Value Alarmed */
	double		alst;		/* Last Value Archived */
	double		mlst;		/* Last Val Monitored */
	void *	pbrk;		/* Ptrto brkTable */
	short		init;		/* Initialized? */
	short		lbrk;		/* LastBreak Point */
	struct link	siol;		/* Sim Output Specifctn */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
	unsigned short	ivoa;		/* INVALID output action */
	char		p44 [2];		/* Created Pad  */
	double		ivov;		/* INVALID output value */
	unsigned char	omod;		/* Was OVAL modified? */
};
typedef struct aoRecord aoRecord;
#endif

