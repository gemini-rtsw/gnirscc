
/* carRecord.h */
#ifndef INCcarRecordh
#define INCcarRecordh
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
struct carRecord	{
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
	/* start of car specific fields */ 
	unsigned short	val;		/* State */
	char		p12 [2];		/* Created Pad  */
	long		clid;		/* Client ID */
	char		omss[40];		/* Message (out) */
	long		oerr;		/* Error Code (out) */
	long		ival;		/* State (in) */
	struct link	icid;		/* Client ID (in) */
	char		imss[40];		/* Message (in) */
	long		ierr;		/* Error Code (in) */
	long		aval;		/* Last State Archived */
	long		mval;		/* Last State Monitored */
	long		acid;		/* Last Client ID Archived */
	long		mcid;		/* Last Client ID Monitore */
	char		amss[40];		/* Last Message Archived */
	char		mmss[40];		/* Last Message Monitored */
	long		aerr;		/* Last Error Code Archive */
	long		merr;		/* Last Error Code Monitor */
	unsigned short	ersv;		/* Error Alarm Severity */
	char		p13 [2];		/* Created Pad  */
	struct link	siol;		/* Simulation Error Link */
	long		sval;		/* Simulation Error */
	char		p14 [4];		/* Created Pad  */
	struct link	siml;		/* Simulation Mode Link */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim Mode Alarm Severity */
};
typedef struct carRecord carRecord;
#endif

