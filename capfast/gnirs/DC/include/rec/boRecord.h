
/* boRecord.h */
#ifndef INCboRecordh
#define INCboRecordh
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
struct boRecord	{
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
	/* start of bo specific fields */ 
	unsigned short	val;		/* Current Value */
	unsigned short	omsl;		/* Output Mode Select */
	char		p50 [4];		/* Created Pad  */
	struct link	dol;		/* Desired Output Loc */
	struct link	out;		/* Output Specification */
	float		high;		/* Seconds to Hold High */
	char		znam[20];		/* Zero Name */
	char		onam[20];		/* One Name */
	unsigned long	rval;		/* Raw Value */
	unsigned long	oraw;		/* prev Raw Value */
	unsigned long	mask;		/* Hardware Mask */
	void *  rpvt;		/* Record Private */
	void *	wdpt;		/* Watch Dog Timer ID */
	unsigned short	zsv;		/* Zero Error Severity */
	unsigned short	osv;		/* One Error Severity */
	unsigned short	cosv;		/* Change of State Sevr */
	char		p51 [2];		/* Created Pad  */
	unsigned long	rbv;		/* Readback Value */
	unsigned long	orbv;		/* Prev Readback Value */
	unsigned short	mlst;		/* Last Value Monitored */
	unsigned short	lalm;		/* Last Value Alarmed */
	char		p52 [4];		/* Created Pad  */
	struct link	siol;		/* Sim Output Specifctn */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
	unsigned short	ivoa;		/* INVALID outpt action */
	unsigned short	ivov;		/* INVALID output value */
};
typedef struct boRecord boRecord;
#endif

