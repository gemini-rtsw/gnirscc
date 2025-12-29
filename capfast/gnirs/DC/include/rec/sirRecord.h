
/* sirRecord.h */
#ifndef INCsirRecordh
#define INCsirRecordh
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
struct sirRecord	{
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
	/* start of sir specific fields */ 
	struct link	inp;		/* Input Link */
	char		imss[40];		/* Message IN */
	char		fdsc[40];		/* Full Description */
	unsigned short	ftvl;		/* Type of value */
	char		egu[40];		/* Engineering Units */
	char		p15 [2];		/* Created Pad  */
	void *val;		/* Value OUT */
	void *rval;		/* Raw value IN */
	unsigned long	nelm;		/* Number of Elements */
	unsigned long	nord;		/* Number elements read */
	char		omss[40];		/* Message OUT */
	char		snam[40];		/* Subroutine Name */
	void *sadr;		/* Subroutine Address */
	short		styp;		/* Subr symbol type */
	short		prec;		/* Display Precision */
	void *aval;		/* Last Value Archived */
	void *mval;		/* Last Value Monitored */
	char		amss[40];		/* Last Message Archived */
	char		mmss[40];		/* Last Message Monitored */
	char		p16 [4];		/* Created Pad  */
	double		lalm;		/* Last Value Alarmed */
	float		hihi;		/* Hihi Alarm Limit */
	float		lolo;		/* Lolo Alarm Limit */
	float		high;		/* High Alarm Limit */
	float		low;		/* Low Alarm Limit */
	unsigned short	brsv;		/* Bad Sub Return Severity */
	unsigned short	hhsv;		/* Hihi Severity */
	unsigned short	llsv;		/* Lolo Severity */
	unsigned short	hsv;		/* High Severity */
	unsigned short	lsv;		/* Low Severity */
	char		p17 [6];		/* Created Pad  */
	double		hyst;		/* Alarm Deadband */
	double		adel;		/* Archive Deadband */
	double		mdel;		/* Monitor Deadband */
	struct link	siol;		/* Simulation Value Link */
	void *sval;		/* Simulation Value */
	char		p18 [4];		/* Created Pad  */
	struct link	siml;		/* Simulation Mode Link */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim Mode Alarm Severity */
};
typedef struct sirRecord sirRecord;
#endif

