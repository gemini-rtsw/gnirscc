
/* statusRecord.h */
#ifndef INCstatusRecordh
#define INCstatusRecordh
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
struct statusRecord	{
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
	/* start of status specific fields */ 
	long		val;		/* Current value */
	char		p129[4];		/* Created Pad  */
	struct link	inp;		/* Input Specification */
	char		egu[16];		/* Units name */
	long		hopr;		/* High Operating Range */
	long		lopr;		/* Low Operating Range */
	long		lval;		/* Last value */
	unsigned short	bi00;		/* bit 0 Value */
	unsigned short	bi01;		/* bit 1 Value */
	unsigned short	bi02;		/* bit 2 Value */
	unsigned short	bi03;		/* bit 3 Value */
	unsigned short	bi04;		/* bit 4 Value */
	unsigned short	bi05;		/* bit 5 Value */
	unsigned short	bi06;		/* bit 6 Value */
	unsigned short	bi07;		/* bit 7 Value */
	unsigned short	bi08;		/* bit 8 Value */
	unsigned short	bi09;		/* bit 9 Value */
	unsigned short	bi10;		/* bit 10 Value */
	unsigned short	bi11;		/* bit 11 Value */
	unsigned short	bi12;		/* bit 12 Value */
	unsigned short	bi13;		/* bit 13 Value */
	unsigned short	bi14;		/* bit 14 Value */
	unsigned short	bi15;		/* bit 15 Value */
	unsigned short	bi16;		/* bit 16 Value */
	unsigned short	bi17;		/* bit 17 Value */
	unsigned short	bi18;		/* bit 18 Value */
	unsigned short	bi19;		/* bit 19 Value */
	unsigned short	bi20;		/* bit 20 Value */
	unsigned short	bi21;		/* bit 20 Value */
	unsigned short	bi22;		/* bit 22 Value */
	unsigned short	bi23;		/* bit 23 Value */
	unsigned short	bi24;		/* bit 24 Value */
	unsigned short	bi25;		/* bit 25 Value */
	unsigned short	bi26;		/* bit 26 Value */
	unsigned short	bi27;		/* bit 27 Value */
	unsigned short	bi28;		/* bit 28 Value */
	unsigned short	bi29;		/* bit 29 Value */
	unsigned short	bi30;		/* bit 30 Value */
	unsigned short	bi31;		/* bit 31 Value */
	char		p130[4];		/* Created Pad  */
	struct link	lk00;		/* Forward Link 0 */
	struct link	lk01;		/* Forward Link 1 */
	struct link	lk02;		/* Forward Link 2 */
	struct link	lk03;		/* Forward Link 3 */
	struct link	lk04;		/* Forward Link 4 */
	struct link	lk05;		/* Forward Link 5 */
	struct link	lk06;		/* Forward Link 6 */
	struct link	lk07;		/* Forward Link 7 */
	struct link	lk08;		/* Forward Link 8 */
	struct link	lk09;		/* Forward Link 9 */
	struct link	lk10;		/* Forward Link 10 */
	struct link	lk11;		/* Forward Link 11 */
	struct link	lk12;		/* Forward Link 12 */
	struct link	lk13;		/* Forward Link 13 */
	struct link	lk14;		/* Forward Link 14 */
	struct link	lk15;		/* Forward Link 15 */
	struct link	lk16;		/* Forward Link 16 */
	struct link	lk17;		/* Forward Link 17 */
	struct link	lk18;		/* Forward Link 18 */
	struct link	lk19;		/* Forward Link 19 */
	struct link	lk20;		/* Forward Link 20 */
	struct link	lk21;		/* Forward Link 21 */
	struct link	lk22;		/* Forward Link 22 */
	struct link	lk23;		/* Forward Link 23 */
	struct link	lk24;		/* Forward Link 24 */
	struct link	lk25;		/* Forward Link 25 */
	struct link	lk26;		/* Forward Link 26 */
	struct link	lk27;		/* Forward Link 27 */
	struct link	lk28;		/* Forward Link 28 */
	struct link	lk29;		/* Forward Link 29 */
	struct link	lk30;		/* Forward Link 30 */
	struct link	lk31;		/* Forward Link 31 */
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
	struct link	siol;		/* Sim Input Specifctn */
	long		sval;		/* Simulation Value */
	char		p131[4];		/* Created Pad  */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
};
typedef struct statusRecord statusRecord;
#endif

