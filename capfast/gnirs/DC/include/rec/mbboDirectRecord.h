
/* mbboDirectRecord.h */
#ifndef INCmbboDirectRecordh
#define INCmbboDirectRecordh
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
struct mbboDirectRecord	{
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
	/* start of mbboDirect specific fields */ 
	unsigned short	val;		/* Word */
	unsigned short	omsl;		/* Output Mode Select */
	short		nobt;		/* Number of Bits */
	char		p97 [2];		/* Created Pad  */
	struct link	dol;		/* Desired Output Loc */
	struct link	out;		/* Output Specification */
	unsigned char	b0;		/* Bit 0 */
	unsigned char	b1;		/* Bit 1 */
	unsigned char	b2;		/* Bit 2 */
	unsigned char	b3;		/* Bit 3 */
	unsigned char	b4;		/* Bit 4 */
	unsigned char	b5;		/* Bit 5 */
	unsigned char	b6;		/* Bit 6 */
	unsigned char	b7;		/* Bit 7 */
	unsigned char	b8;		/* Bit 8 */
	unsigned char	b9;		/* Bit 9 */
	unsigned char	ba;		/* Bit 10 */
	unsigned char	bb;		/* Bit 11 */
	unsigned char	bc;		/* Bit 12 */
	unsigned char	bd;		/* Bit 13 */
	unsigned char	be;		/* Bit 14 */
	unsigned char	bf;		/* Bit 15 */
	unsigned long	rval;		/* Raw Value */
	unsigned long	oraw;		/* Prev Raw Value */
	unsigned long	rbv;		/* Readback Value */
	unsigned long	orbv;		/* Prev Readback Value */
	unsigned long	mask;		/* Hardware Mask */
	unsigned long	mlst;		/* Last Value Monitored */
	unsigned long	lalm;		/* Last Value Alarmed */
	unsigned long	shft;		/* Shift */
	struct link	siol;		/* Sim Output Specifctn */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
	unsigned short	ivoa;		/* INVALID outpt action */
	unsigned short	ivov;		/* INVALID output value */
};
typedef struct mbboDirectRecord mbboDirectRecord;
#endif

