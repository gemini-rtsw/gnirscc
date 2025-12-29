
/* loadRecord.h */
#ifndef INCloadRecordh
#define INCloadRecordh
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
struct loadRecord	{
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
	/* start of load specific fields */ 
	long		val;		/* Status Value */
	char		msg[40];		/* Message Value */
	char		p81 [4];		/* Created Pad  */
	struct link	out;		/* Hardware Specifictn */
	struct link	dnl;		/* Download Specificatn */
	char		dnv[40];		/* Download Value */
	struct link	upl;		/* Upload Specificatn */
	char		upv[40];		/* Upload Value */
	long		pval;		/* Prev Status Value */
	char		pmsg[40];		/* Prev Message Value */
	char		pdnv[40];		/* Prev Download Value */
	char		pupv[40];		/* Prev Upload Value */
	char		p82 [4];		/* Created Pad  */
	struct link	sdnl;		/* Sim Downld Specifctn */
	char		sdnv[40];		/* Sim Downld Value */
	struct link	supl;		/* Sim Upload Specifctn */
	char		supv[40];		/* Sim Upload Value */
	struct link	siol;		/* Sim Value Location */
	long		sval;		/* Simulation Value */
	char		p83 [4];		/* Created Pad  */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
};
typedef struct loadRecord loadRecord;
#endif

