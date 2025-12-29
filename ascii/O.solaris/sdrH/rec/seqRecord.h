
/* seqRecord.h */
#ifndef INCseqRecordh
#define INCseqRecordh
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
struct seqRecord	{
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
	/* start of seq specific fields */ 
	long		val;		/* Used to trigger */
	unsigned short	selm;		/* Select Mechanism */
	unsigned short	seln;		/* Link Selection */
	struct link	sell;		/* Link Selection Loc */
	short		prec;		/* Display Precision */
	char		p110[6];		/* Created Pad  */
	double		dly1;		/* Delay 1 */
	struct link	dol1;		/* Input link1 */
	double		do1;		/* Constant input 1 */
	struct link	lnk1;		/* Output Link 1 */
	double		dly2;		/* Delay 2 */
	struct link	dol2;		/* Input link 2 */
	double		do2;		/* Constant input 2 */
	struct link	lnk2;		/* Output Link 2 */
	double		dly3;		/* Delay 3 */
	struct link	dol3;		/* Input link 3 */
	double		do3;		/* Constant input 3 */
	struct link	lnk3;		/* Output Link 3 */
	double		dly4;		/* Delay 4 */
	struct link	dol4;		/* Input link 4 */
	double		do4;		/* Constant input 4 */
	struct link	lnk4;		/* Output Link 4 */
	double		dly5;		/* Delay 5 */
	struct link	dol5;		/* Input link 5 */
	double		do5;		/* Constant input 5 */
	struct link	lnk5;		/* Output Link 5 */
	double		dly6;		/* Delay 6 */
	struct link	dol6;		/* Input link 6 */
	double		do6;		/* Constant input 6 */
	struct link	lnk6;		/* Output Link 6 */
	double		dly7;		/* Delay 7 */
	struct link	dol7;		/* Input link 7 */
	double		do7;		/* Constant input 7 */
	struct link	lnk7;		/* Output Link 7 */
	double		dly8;		/* Delay 8 */
	struct link	dol8;		/* Input link 8 */
	double		do8;		/* Constant input 8 */
	struct link	lnk8;		/* Output Link 8 */
	double		dly9;		/* Delay 9 */
	struct link	dol9;		/* Input link 9 */
	double		do9;		/* Constant input 9 */
	struct link	lnk9;		/* Output Link 9 */
	double		dlya;		/* Delay 10 */
	struct link	dola;		/* Input link 10 */
	double		doa;		/* Constant input 10 */
	struct link	lnka;		/* Output Link 10 */
};
typedef struct seqRecord seqRecord;
#endif

