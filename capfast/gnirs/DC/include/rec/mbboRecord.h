
/* mbboRecord.h */
#ifndef INCmbboRecordh
#define INCmbboRecordh
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
struct mbboRecord	{
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
	/* start of mbbo specific fields */ 
	unsigned short	val;		/* Desired Value */
	char		p95 [6];		/* Created Pad  */
	struct link	dol;		/* Desired Output Loc */
	unsigned short	omsl;		/* Output Mode Select */
	short		nobt;		/* Number of Bits */
	char		p96 [4];		/* Created Pad  */
	struct link	out;		/* Output Specification */
	unsigned long	zrvl;		/* Zero Value */
	unsigned long	onvl;		/* One Value */
	unsigned long	twvl;		/* Two Value */
	unsigned long	thvl;		/* Three Value */
	unsigned long	frvl;		/* Four Value */
	unsigned long	fvvl;		/* Five Value */
	unsigned long	sxvl;		/* Six Value */
	unsigned long	svvl;		/* Seven Value */
	unsigned long	eivl;		/* Eight Value */
	unsigned long	nivl;		/* Nine Value */
	unsigned long	tevl;		/* Ten Value */
	unsigned long	elvl;		/* Eleven Value */
	unsigned long	tvvl;		/* Twelve Value */
	unsigned long	ttvl;		/* Thirteen Value */
	unsigned long	ftvl;		/* Fourteen Value */
	unsigned long	ffvl;		/* Fifteen Value */
	char		zrst[16];		/* Zero String */
	char		onst[16];		/* One String */
	char		twst[16];		/* Two String */
	char		thst[16];		/* Three String */
	char		frst[16];		/* Four String */
	char		fvst[16];		/* Five String */
	char		sxst[16];		/* Six String */
	char		svst[16];		/* Seven String */
	char		eist[16];		/* Eight String */
	char		nist[16];		/* Nine String */
	char		test[16];		/* Ten String */
	char		elst[16];		/* Eleven String */
	char		tvst[16];		/* Twelve String */
	char		ttst[16];		/* Thirteen String */
	char		ftst[16];		/* Fourteen String */
	char		ffst[16];		/* Fifteen String */
	unsigned short	zrsv;		/* State Zero Severity */
	unsigned short	onsv;		/* State One Severity */
	unsigned short	twsv;		/* State Two Severity */
	unsigned short	thsv;		/* State Three Severity */
	unsigned short	frsv;		/* State Four Severity */
	unsigned short	fvsv;		/* State Five Severity */
	unsigned short	sxsv;		/* State Six Severity */
	unsigned short	svsv;		/* State Seven Severity */
	unsigned short	eisv;		/* State Eight Severity */
	unsigned short	nisv;		/* State Nine Severity */
	unsigned short	tesv;		/* State Ten Severity */
	unsigned short	elsv;		/* State Eleven Severity */
	unsigned short	tvsv;		/* State Twelve Severity */
	unsigned short	ttsv;		/* State Thirteen Sevr */
	unsigned short	ftsv;		/* State Fourteen Sevr */
	unsigned short	ffsv;		/* State Fifteen Sevr */
	unsigned short	unsv;		/* Unknown State Sevr */
	unsigned short	cosv;		/* Change of State Sevr */
	unsigned long	rval;		/* Raw Value */
	unsigned long	oraw;		/* Prev Raw Value */
	unsigned long	rbv;		/* Readback Value */
	unsigned long	orbv;		/* Prev Readback Value */
	unsigned long	mask;		/* Hardware Mask */
	unsigned short	mlst;		/* Last Value Monitored */
	unsigned short	lalm;		/* Last Value Alarmed */
	short		sdef;		/* States Defined */
	unsigned short	shft;		/* Shift */
	struct link	siol;		/* Sim Output Specifctn */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
	unsigned short	ivoa;		/* INVALID outpt action */
	unsigned short	ivov;		/* INVALID output value */
};
typedef struct mbboRecord mbboRecord;
#endif

