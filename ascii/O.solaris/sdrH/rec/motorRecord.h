
/* motorRecord.h */
#ifndef INCmotorRecordh
#define INCmotorRecordh
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
struct motorRecord	{
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
	/* start of motor specific fields */ 
	float		vers;		/* Code Version */
	char		p32 [4];		/* Created Pad  */
	double		off;		/* User Offset (EGU) */
	unsigned short	foff;		/* Offset-Freeze Switch */
	short		fof;		/* Freeze Offset */
	short		vof;		/* Variable Offset */
	unsigned short	dir;		/* User Direction */
	unsigned short	set;		/* Set/Use Switch */
	short		sset;		/* Set SET Mode */
	short		suse;		/* Set USE Mode */
	char		p33 [2];		/* Created Pad  */
	float		velo;		/* Velocity (EGU/s) */
	float		vbas;		/* Base Velocity (EGU/s) */
	float		s;		/* Speed (revolutions/sec) */
	float		sbas;		/* Base Speed (RPS) */
	float		accl;		/* Seconds to Velocity */
	float		bdst;		/* BL Distance (EGU) */
	float		bvel;		/* BL Velocity (EGU/s) */
	float		sbak;		/* BL Speed (RPS) */
	float		bacc;		/* BL Seconds to Velocity */
	float		frac;		/* Move Fraction */
	struct link	out;		/* Output Specification */
	short		card;		/* Card Number */
	char		p34 [6];		/* Created Pad  */
	struct link	rdbl;		/* Readback Location */
	struct link	dol;		/* Desired Output Loc */
	unsigned short	omsl;		/* Output Mode Select */
	char		p35 [6];		/* Created Pad  */
	struct link	rlnk;		/* Readback OutLink */
	unsigned short	mode;		/* Operating Mode */
	char		p36 [2];		/* Created Pad  */
	long		srev;		/* Steps per Revolution */
	float		urev;		/* EGU's per Revolution */
	float		mres;		/* Motor Step Size (EGU) */
	float		eres;		/* Encoder Step Size (EGU) */
	float		rres;		/* Readback Step Size (EGU */
	float		res;		/* Step Size (EGU) */
	unsigned short	ueip;		/* Use Encoder If Present */
	unsigned short	urip;		/* Use RDBL Link If Presen */
	unsigned short	trak;		/* Make Readback = Value */
	short		prec;		/* Display Precision */
	char		egu[16];		/* Engineering Units */
	float		hlm;		/* User High Limit */
	float		llm;		/* User Low Limit */
	float		dhlm;		/* Dial High Limit */
	float		dllm;		/* Dial Low Limit */
	float		hopr;		/* High Operating Range */
	float		lopr;		/* Low Operating Range */
	short		hls;		/* User High Limit Switch */
	short		lls;		/* User Low Limit Switch */
	short		rhls;		/* Raw High Limit Switch */
	short		rlls;		/* Raw Low Limit Switch */
	float		hihi;		/* Hihi Alarm Limit (EGU) */
	float		lolo;		/* Lolo Alarm Limit (EGU) */
	float		high;		/* High Alarm Limit (EGU) */
	float		low;		/* Low Alarm Limit (EGU) */
	unsigned short	hhsv;		/* Hihi Severity */
	unsigned short	llsv;		/* Lolo Severity */
	unsigned short	hsv;		/* High Severity */
	unsigned short	lsv;		/* Low Severity */
	unsigned short	hlsv;		/* HW Limit Violation Svr */
	char		p37 [2];		/* Created Pad  */
	float		mdel;		/* Monitor Deadband */
	float		adel;		/* Archive Deadband */
	float		rdbd;		/* Retry Deadband (EGU) */
	short		rcnt;		/* Retry count */
	short		rtry;		/* Max retry count */
	short		miss;		/* Ran out of retries */
	unsigned short	spmg;		/* Stop/Pause/Move/Go */
	unsigned short	lspg;		/* Last SPMG */
	short		stop;		/* Stop */
	short		homf;		/* Home Forward */
	short		homr;		/* Home Reverse */
	short		jogf;		/* Jog motor Forward */
	short		jogr;		/* Jog motor Reverse */
	short		twf;		/* Tweak motor Forward */
	short		twr;		/* Tweak motor Reverse */
	float		twv;		/* Tweak Step Size (EGU) */
	double		val;		/* User Desired Value (EGU */
	double		lval;		/* Last User Des Val (EGU) */
	double		dval;		/* Dial Desired Value (EGU */
	double		ldvl;		/* Last Dial Des Val (EGU) */
	long		rval;		/* Raw Desired Value (step */
	long		lrvl;		/* Last Raw Des Val (steps */
	double		rlv;		/* Relative Value (EGU) */
	double		lrlv;		/* Last Rel Value (EGU) */
	double		rbv;		/* User Readback Value */
	double		drbv;		/* Dial Readback Value */
	double		diff;		/* Difference dval-drbv */
	long		rdif;		/* Difference rval-rrbv */
	long		rrbv;		/* Raw Readback Value */
	long		rmp;		/* Raw Motor Position */
	long		rep;		/* Raw Encoder Position */
	long		rvel;		/* Raw Velocity */
	short		dmov;		/* Done moving to value */
	short		movn;		/* Motor is moving */
	unsigned long	msta;		/* Motor Status */
	short		lvio;		/* Limit violation */
	short		tdir;		/* Direction of Travel */
	short		athm;		/* At HOME */
	short		cvel;		/* Constant Velocity */
	short		posm;		/* Positive motion */
	char		p38 [2];		/* Created Pad  */
	double		alst;		/* Last Value Archived */
	double		mlst;		/* Last Value Monitored */
	short		pp;		/* Post process command */
	short		mip;		/* Motion In Progress */
	unsigned long	mmap;		/* Monitor Mask */
	unsigned long	nmap;		/* Monitor Mask (more) */
};
typedef struct motorRecord motorRecord;
#endif

