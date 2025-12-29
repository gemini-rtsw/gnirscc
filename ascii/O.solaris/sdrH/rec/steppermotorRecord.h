
/* steppermotorRecord.h */
#ifndef INCsteppermotorRecordh
#define INCsteppermotorRecordh
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
struct steppermotorRecord	{
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
	/* start of steppermotor specific fields */ 
	float		val;		/* Desired Value */
	char		p132[4];		/* Created Pad  */
	struct link	out;		/* Output Specification */
	struct link	rdbl;		/* Readback Location */
	struct link	dol;		/* Desired Output Loc */
	unsigned short	omsl;		/* Output Mode Select */
	char		p133[2];		/* Created Pad  */
	float		accl;		/* Seconds to Velocity */
	float		velo;		/* Velocity Rotation/Sec */
	float		dist;		/* Dist of One Pulse */
	float		ival;		/* Value at init */
	unsigned short	mode;		/* Operating Mode */
	unsigned short	cmod;		/* Current Operating Mode */
	unsigned short	ialg;		/* Initialization Alg */
	unsigned short	mres;		/* Motor Pulses/Revolution */
	unsigned short	eres;		/* Encoder Pulses/Rev */
	short		prec;		/* Display Precision */
	char		egu[16];		/* Engineering Units */
	float		drvh;		/* Drive High Limit */
	float		drvl;		/* Drive Low Limit */
	float		hopr;		/* High Operating Range */
	float		lopr;		/* Low Operating Range */
	float		hihi;		/* Hihi Alarm Limit */
	float		lolo;		/* Lolo Alarm Limit */
	float		high;		/* High Alarm Limit */
	float		low;		/* Low Alarm Limit */
	unsigned short	hhsv;		/* Hihi Severity */
	unsigned short	llsv;		/* Lolo Severity */
	unsigned short	hsv;		/* High Severity */
	unsigned short	lsv;		/* Low Severity */
	unsigned short	hlsv;		/* HW Limit Violation Svr */
	char		p134[2];		/* Created Pad  */
	float		mdel;		/* Monitor Deadband */
	float		adel;		/* Archive Deadband */
	float		rdbd;		/* Retry Deadband */
	short		rtry;		/* Number of retries */
	short		sthm;		/* Set Home */
	short		stop;		/* Stop motor */
	short		dmov;		/* Done moving to value */
	long		rval;		/* Current Raw Value */
	float		rbv;		/* Readback Value */
	long		rrbv;		/* Raw Readback Value */
	float		alst;		/* Last Value Archived */
	float		mlst;		/* Last Value Monitored */
	short		init;		/* Initialize */
	short		mcw;		/* Mtr Clckws Lim */
	short		mccw;		/* Mtr Cntr Clockwise Lmt */
	short		cw;		/* Clockwise Limit */
	short		ccw;		/* Counter Clockwise Lmt */
	short		dir;		/* Direction of Travel */
	short		movn;		/* Moving Status */
	short		cvel;		/* Constant Velocity */
	short		rcnt;		/* Retry count */
	short		posm;		/* Positive motion */
	float		lval;		/* Last Value */
	float		epos;		/* Encoder position rdbck */
	float		mpos;		/* Motor position rdbck */
	float		miss;		/* First attemp error */
	float		lvel;		/* Last Velocity set */
	float		lacc;		/* Last acc set */
};
typedef struct steppermotorRecord steppermotorRecord;
#endif

