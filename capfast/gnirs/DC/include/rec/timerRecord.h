
/* timerRecord.h */
#ifndef INCtimerRecordh
#define INCtimerRecordh
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
struct timerRecord	{
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
	/* start of timer specific fields */ 
	struct link	torg;		/* Trigger Origin */
	struct link	out;		/* Output Specification */
	short		val;		/* Status */
	unsigned short	tsrc;		/* Clock Source */
	unsigned short	ptst;		/* Pre-Trigger State */
	short		tevt;		/* Event on Trigger */
	short		prec;		/* Display Precision */
	unsigned short	timu;		/* Time Units */
	unsigned short	main;		/* Maintain on reboot */
	char		p139[2];		/* Created Pad  */
	float		rdt1;		/* Reboot Delay of 1 */
	float		rpw1;		/* Reboot Width of 1 */
	float		pdly;		/* Delay Source to Inp */
	float		dut1;		/* Delay Until Trigger 1 */
	float		opw1;		/* Output Pulse Width  1 */
	float		dut2;		/* Delay Until Trigger 2 */
	float		opw2;		/* Output Pulse Width  2 */
	float		dut3;		/* Delay Until Trigger 3 */
	float		opw3;		/* Output Pulse Width  3 */
	float		dut4;		/* Delay Until Trigger 4 */
	float		opw4;		/* Output Pulse Width  4 */
	float		dut5;		/* Delay Until Trigger 5 */
	float		opw5;		/* Output Pulse Width  5 */
	char		p140[4];		/* Created Pad  */
	double		t1dl;		/* Delay for trigger 1 */
	double		t1wd;		/* Width of Trigger 1 */
	double		t2dl;		/* Delay for trigger 2 */
	double		t2wd;		/* Width of Trigger 2 */
	double		t3dl;		/* Delay for trigger 3 */
	double		t3wd;		/* Width of Trigger 3 */
	double		t4dl;		/* Delay for trigger 4 */
	double		t4wd;		/* Width of Trigger 4 */
	double		t5dl;		/* Delay for trigger 5 */
	double		t5wd;		/* Width of Trigger 5 */
	float		t1td;		/* Trailing Delay of 1 */
	float		t1ld;		/* Leading Delay of 1 */
	float		t2td;		/* Trailing Delay of 2 */
	float		t2ld;		/* Leading Delay of 2 */
	float		t3td;		/* Trailing Delay of 3 */
	float		t3ld;		/* Leading Delay of 3 */
	float		t4td;		/* Trailing Delay of 4 */
	float		t4ld;		/* Leading Delay of 4 */
	float		t5td;		/* Trailing Delay of 5 */
	float		t5ld;		/* Leading Delay of 5 */
	float		trdl;		/* Trigger Origin Delay */
	short		tdis;		/* Timing Pulse Disable */
};
typedef struct timerRecord timerRecord;
#endif

