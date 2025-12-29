
/* waitRecord.h */
#ifndef INCwaitRecordh
#define INCwaitRecordh
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
struct waitRecord	{
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
	/* start of wait specific fields */ 
	float		vers;		/* Code Version */
	float		hopr;		/* High Operating Range */
	float		lopr;		/* Low Operating Range */
	short		init;		/* Initialized? */
	char		p141[2];		/* Created Pad  */
	void *         cbst;		/* Pointer to cbStruct */
	char		inan[40];		/* INPA PV Name */
	char		inbn[40];		/* INPB PV Name */
	char		incn[40];		/* INPC PV Name */
	char		indn[40];		/* INPD PV Name */
	char		inen[40];		/* INPE PV Name */
	char		infn[40];		/* INPF PV Name */
	char		ingn[40];		/* INPG PV Name */
	char		inhn[40];		/* INPH PV Name */
	char		inin[40];		/* INPI PV Name */
	char		injn[40];		/* INPJ PV Name */
	char		inkn[40];		/* INPK PV Name */
	char		inln[40];		/* INPL PV Name */
	char		doln[40];		/* DOL  PV Name */
	char		outn[40];		/* OUT  PV Name */
	unsigned short	inav;		/* INPA PV Status */
	unsigned short	inbv;		/* INPB PV Status */
	unsigned short	incv;		/* INPC PV Status */
	unsigned short	indv;		/* INPD PV Status */
	unsigned short	inev;		/* INPE PV Status */
	unsigned short	infv;		/* INPF PV Status */
	unsigned short	ingv;		/* INPG PV Status */
	unsigned short	inhv;		/* INPH PV Status */
	unsigned short	iniv;		/* INPI PV Status */
	unsigned short	injv;		/* INPJ PV Status */
	unsigned short	inkv;		/* INPK PV Status */
	unsigned short	inlv;		/* INPL PV Status */
	unsigned short	dolv;		/* DOL  PV Status */
	unsigned short	outv;		/* OUT  PV Status */
	double		a;		/* Value of Input A */
	double		b;		/* Value of Input B */
	double		c;		/* Value of Input C */
	double		d;		/* Value of Input D */
	double		e;		/* Value of Input E */
	double		f;		/* Value of Input F */
	double		g;		/* Value of Input G */
	double		h;		/* Value of Input H */
	double		i;		/* Value of Input I */
	double		j;		/* Value of Input J */
	double		k;		/* Value of Input K */
	double		l;		/* Value of Input L */
	double		la;		/* Last Val of Input A */
	double		lb;		/* Last Val of Input B */
	double		lc;		/* Last Val of Input C */
	double		ld;		/* Last Val of Input D */
	double		le;		/* Last Val of Input E */
	double		lf;		/* Last Val of Input F */
	double		lg;		/* Last Val of Input G */
	double		lh;		/* Last Val of Input H */
	double		li;		/* Last Val of Input I */
	double		lj;		/* Last Val of Input J */
	double		lk;		/* Last Val of Input K */
	double		ll;		/* Last Val of Input L */
	unsigned short	inap;		/* INPA causes I/O INTR */
	unsigned short	inbp;		/* INPB causes I/O INTR */
	unsigned short	incp;		/* INPC causes I/O INTR */
	unsigned short	indp;		/* INPD causes I/O INTR */
	unsigned short	inep;		/* INPE causes I/O INTR */
	unsigned short	infp;		/* INPF causes I/O INTR */
	unsigned short	ingp;		/* INPG causes I/O INTR */
	unsigned short	inhp;		/* INPH causes I/O INTR */
	unsigned short	inip;		/* INPI causes I/O INTR */
	unsigned short	injp;		/* INPJ causes I/O INTR */
	unsigned short	inkp;		/* INPK causes I/O INTR */
	unsigned short	inlp;		/* INPL causes I/O INTR */
	char		calc[36];		/* Calculation */
	char		p142[4];		/* Created Pad  */
	char    rpcl[184];		/* Reverse Polish Calc */
	long		clcv;		/* CALC Valid */
	char		p143[4];		/* Created Pad  */
	double		val;		/* Value Field */
	double		oval;		/* Old Value */
	short		prec;		/* Display Precision */
	unsigned short	oopt;		/* Output Execute Opt */
	float		odly;		/* Output Execute Delay */
	unsigned short	dopt;		/* Output Data Option */
	char		p144[6];		/* Created Pad  */
	double		dold;		/* Desired Output Data */
	unsigned short	oevt;		/* Event To Issue */
	char		p145[6];		/* Created Pad  */
	double		adel;		/* Archive Deadband */
	double		mdel;		/* Monitor Deadband */
	double		alst;		/* Last Value Archived */
	double		mlst;		/* Last Val Monitored */
	struct link	siol;		/* Sim Input Specifctn */
	double		sval;		/* Simulation Value */
	struct link	siml;		/* Sim Mode Location */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim mode Alarm Svrty */
};
typedef struct waitRecord waitRecord;
#endif

