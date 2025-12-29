
/* egRecord.h */
#ifndef INCegRecordh
#define INCegRecordh
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
struct egRecord	{
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
	/* start of eg specific fields */ 
	struct link	out;		/* Output Specification */
	unsigned short	mod1;		/* RAM 1 Operating Mode */
	char		p62 [6];		/* Created Pad  */
	double		r1sp;		/* RAM 1 Clock (Hz) */
	unsigned short	mod2;		/* RAM 2 Operating Mode */
	char		p63 [6];		/* Created Pad  */
	double		r2sp;		/* RAM 2 Clock (Hz) */
	unsigned short	lmd1;		/* Last Operating Mode1 */
	unsigned short	lmd2;		/* Last Operating Mode2 */
	unsigned short	fifo;		/* Input FIFO Enable */
	unsigned short	lffo;		/* Last FIFO Enable */
	char		clr1;		/* Clear Sequence 1 */
	char		clr2;		/* Clear Sequence 2 */
	char		trg1;		/* Trigger Sequence 1 */
	char		trg2;		/* Trigger Sequence 2 */
	unsigned short	enab;		/* Master Enable */
	char		p64 [2];		/* Created Pad  */
	long		lena;		/* Last Master Enable */
	long		taxi;		/* Taxi Violation */
	long		ltax;		/* Last Taxi Violation */
	long		vme;		/* Generate VME Event */
	unsigned short	ete0;		/* Trigger 0 Enable */
	char		p65 [2];		/* Created Pad  */
	long		et0;		/* Trigger 0 Event */
	long		let0;		/* Last Trigger 0 Event */
	unsigned short	ete1;		/* Trigger 1 Enable */
	char		p66 [2];		/* Created Pad  */
	long		et1;		/* Trigger 1 Event */
	long		let1;		/* Last Trigger 1 Event */
	unsigned short	ete2;		/* Trigger 2 Enable */
	char		p67 [2];		/* Created Pad  */
	long		et2;		/* Trigger 2 Event */
	long		let2;		/* Last Trigger 2 Event */
	unsigned short	ete3;		/* Trigger 3 Enable */
	char		p68 [2];		/* Created Pad  */
	long		et3;		/* Trigger 3 Event */
	long		let3;		/* Last Trigger 3 Event */
	unsigned short	ete4;		/* Trigger 4 Enable */
	char		p69 [2];		/* Created Pad  */
	long		et4;		/* Trigger 4 Event */
	long		let4;		/* Last Trigger 4 Event */
	unsigned short	ete5;		/* Trigger 5 Enable */
	char		p70 [2];		/* Created Pad  */
	long		et5;		/* Trigger 5 Event */
	long		let5;		/* Last Trigger 5 Event */
	unsigned short	ete6;		/* Trigger 6 Enable */
	char		p71 [2];		/* Created Pad  */
	long		et6;		/* Trigger 6 Event */
	long		let6;		/* Last Trigger 6 Event */
	unsigned short	ete7;		/* Trigger 7 Enable */
	char		p72 [2];		/* Created Pad  */
	long		et7;		/* Trigger 7 Event */
	long		let7;		/* Last Trigger 7 Event */
	char		val;		/* Worthless Value */
};
typedef struct egRecord egRecord;
#endif

