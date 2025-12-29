
/* scanRecord.h */
#ifndef INCscanRecordh
#define INCscanRecordh
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
struct scanRecord	{
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
	/* start of scan specific fields */ 
	double		vers;		/* Code Version */
	double		val;		/* Value Field */
	char		smsg[40];		/* Record State Msg */
	unsigned short	cmnd;		/* Command Field */
	unsigned char	alrt;		/* Operator Alert */
	char		p111[1];		/* Created Pad  */
	void *           rpvt;		/* Ptr to Pvt Struct */
	short		mpts;		/* Max # of Points */
	short		exsc;		/* Execute Scan */
	unsigned char	pxsc;		/* Previous XScan */
	char		p112[1];		/* Created Pad  */
	short		npts;		/* Number of Points */
	unsigned short	fpts;		/* Freeze Num of Points */
	unsigned short	ffo;		/* Freeze Flag Override */
	short		cpt;		/* Current Point */
	short		dpt;		/* Desired Point */
	short		pcpt;		/* Point ofLast Posting */
	unsigned short	pasm;		/* After Scan Mode */
	unsigned long	tolp;		/* Time of Last Posting */
	char		p1pv[40];		/* Positioner 1 PV Name */
	char		p2pv[40];		/* Positioner 2 PV Name */
	char		p3pv[40];		/* Positioner 3 PV Name */
	char		p4pv[40];		/* Positioner 4 PV Name */
	char		r1pv[40];		/* P1 Readback  PV Name */
	char		r2pv[40];		/* P2 Readback  PV Name */
	char		r3pv[40];		/* P3 Readback  PV Name */
	char		r4pv[40];		/* P4 Readback  PV Name */
	char		d1pv[40];		/* Detector 1   PV Name */
	char		d2pv[40];		/* Detector 2   PV Name */
	char		d3pv[40];		/* Detector 3   PV Name */
	char		d4pv[40];		/* Detector 4   PV Name */
	char		d5pv[40];		/* Detector 5   PV Name */
	char		d6pv[40];		/* Detector 6   PV Name */
	char		d7pv[40];		/* Detector 7   PV Name */
	char		d8pv[40];		/* Detector 8   PV Name */
	char		d9pv[40];		/* Detector 9   PV Name */
	char		dapv[40];		/* Detector 10  PV Name */
	char		dbpv[40];		/* Detector 11  PV Name */
	char		dcpv[40];		/* Detector 12  PV Name */
	char		ddpv[40];		/* Detector 13  PV Name */
	char		depv[40];		/* Detector 14  PV Name */
	char		dfpv[40];		/* Detector 15  PV Name */
	char		t1pv[40];		/* Trigger 1    PV Name */
	char		t2pv[40];		/* Trigger 2    PV Name */
	char		bspv[40];		/* Before Scan  PV Name */
	char		aspv[40];		/* After Scan   PV Name */
	unsigned short	p1nv;		/* P1  PV Status */
	unsigned short	p2nv;		/* P2  PV Status */
	unsigned short	p3nv;		/* P3  PV Status */
	unsigned short	p4nv;		/* P4  PV Status */
	unsigned short	r1nv;		/* R1  PV Status */
	unsigned short	r2nv;		/* R2  PV Status */
	unsigned short	r3nv;		/* R3  PV Status */
	unsigned short	r4nv;		/* R4  PV Status */
	unsigned short	d1nv;		/* D1  PV Status */
	unsigned short	d2nv;		/* D2  PV Status */
	unsigned short	d3nv;		/* D3  PV Status */
	unsigned short	d4nv;		/* D4  PV Status */
	unsigned short	d5nv;		/* D5  PV Status */
	unsigned short	d6nv;		/* D6  PV Status */
	unsigned short	d7nv;		/* D7  PV Status */
	unsigned short	d8nv;		/* D8  PV Status */
	unsigned short	d9nv;		/* D9  PV Status */
	unsigned short	danv;		/* D10 PV Status */
	unsigned short	dbnv;		/* D11 PV Status */
	unsigned short	dcnv;		/* D12 PV Status */
	unsigned short	ddnv;		/* D13 PV Status */
	unsigned short	denv;		/* D14 PV Status */
	unsigned short	dfnv;		/* D15 PV Status */
	unsigned short	t1nv;		/* T1  PV Status */
	unsigned short	t2nv;		/* T2  PV Status */
	unsigned short	bsnv;		/* BeforeScan PV Status */
	unsigned short	asnv;		/* After Scan PV Status */
	char		p113[2];		/* Created Pad  */
	double		p1pp;		/* P1 Previous Position */
	double		p1cv;		/* P1 Current Value */
	double		p1dv;		/* P1 Desired Value */
	double		p1lv;		/* P1 Last Value Posted */
	double		p1sp;		/* P1 Start Position */
	double		p1si;		/* P1 Step Increment */
	double		p1ep;		/* P1 End Position */
	double		p1cp;		/* P1 Center Position */
	double		p1wd;		/* P1 Scan Width */
	double		r1cv;		/* P1 Readback Value */
	double		r1lv;		/* P1 Rdbk Last Val Pst */
	double		r1dl;		/* P1 Readback Delta */
	double		p1hr;		/* P1 High Oper Range */
	double		p1lr;		/* P1 Low  Oper Range */
	double *         p1pa;		/* P1 Step Array */
	double *         p1ra;		/* P1 Readback Array */
	unsigned short	p1fs;		/* P1 Freeze Start Pos */
	unsigned short	p1fi;		/* P1 Freeze Step Inc */
	unsigned short	p1fe;		/* P1 Freeze End Pos */
	unsigned short	p1fc;		/* P1 Freeze Center Pos */
	unsigned short	p1fw;		/* P1 Freeze Width */
	unsigned short	p1sm;		/* P1 Step Mode */
	unsigned short	p1ar;		/* P1 Absolute/Relative */
	char		p1eu[16];		/* P1 Engineering Units */
	short		p1pr;		/* P1 Display Precision */
	double		p2pp;		/* P2 Previous Position */
	double		p2cv;		/* P2 Current Value */
	double		p2dv;		/* P2 Desired Value */
	double		p2lv;		/* P2 Last Value Posted */
	double		p2sp;		/* P2 Start Position */
	double		p2si;		/* P2 Step Increment */
	double		p2ep;		/* P2 End Position */
	double		p2cp;		/* P2 Center Position */
	double		p2wd;		/* P2 Scan Width */
	double		r2cv;		/* P2 Readback Value */
	double		r2lv;		/* P2 Rdbk Last Val Pst */
	double		r2dl;		/* P2 Readback Delta */
	double		p2hr;		/* P2 High Oper Range */
	double		p2lr;		/* P2 Low  Oper Range */
	double *         p2pa;		/* P2 Step Array */
	double *         p2ra;		/* P2 Readback Array */
	unsigned short	p2fs;		/* P2 Freeze Start Pos */
	unsigned short	p2fi;		/* P2 Freeze Step Inc */
	unsigned short	p2fe;		/* P2 Freeze End Pos */
	unsigned short	p2fc;		/* P2 Freeze Center Pos */
	unsigned short	p2fw;		/* P2 Freeze Width */
	unsigned short	p2sm;		/* P2 Step Mode */
	unsigned short	p2ar;		/* P2 Absolute/Relative */
	char		p2eu[16];		/* P2 Engineering Units */
	short		p2pr;		/* P2 Display Precision */
	double		p3pp;		/* P3 Previous Position */
	double		p3cv;		/* P3 Current Value */
	double		p3dv;		/* P3 Desired Value */
	double		p3lv;		/* P3 Last Value Posted */
	double		p3sp;		/* P3 Start Position */
	double		p3si;		/* P3 Step Increment */
	double		p3ep;		/* P3 End Position */
	double		p3cp;		/* P3 Center Position */
	double		p3wd;		/* P3 Scan Width */
	double		r3cv;		/* P3 Readback Value */
	double		r3lv;		/* P3 Rdbk Last Val Pst */
	double		r3dl;		/* P3 Readback Delta */
	double		p3hr;		/* P3 High Oper Range */
	double		p3lr;		/* P3 Low  Oper Range */
	double *         p3pa;		/* P3 Step Array */
	double *         p3ra;		/* P3 Readback Array */
	unsigned short	p3fs;		/* P3 Freeze Start Pos */
	unsigned short	p3fi;		/* P3 Freeze Step Inc */
	unsigned short	p3fe;		/* P3 Freeze End Pos */
	unsigned short	p3fc;		/* P3 Freeze Center Pos */
	unsigned short	p3fw;		/* P3 Freeze Width */
	unsigned short	p3sm;		/* P3 Step Mode */
	unsigned short	p3ar;		/* P3 Absolute/Relative */
	char		p3eu[16];		/* P3 Engineering Units */
	short		p3pr;		/* P3 Display Precision */
	double		p4pp;		/* P4 Previous Position */
	double		p4cv;		/* P4 Current Value */
	double		p4dv;		/* P4 Desired Value */
	double		p4lv;		/* P4 Last Value Posted */
	double		p4sp;		/* P4 Start Position */
	double		p4si;		/* P4 Step Increment */
	double		p4ep;		/* P4 End Position */
	double		p4cp;		/* P4 Center Position */
	double		p4wd;		/* P4 Scan Width */
	double		r4cv;		/* P4 Readback Value */
	double		r4lv;		/* P4 Rdbk Last Val Pst */
	double		r4dl;		/* P4 Readback Delta */
	double		p4hr;		/* P4 High Oper Range */
	double		p4lr;		/* P4 Low  Oper Range */
	double *         p4pa;		/* P4 Step Array */
	double *         p4ra;		/* P4 Readback Array */
	unsigned short	p4fs;		/* P4 Freeze Start Pos */
	unsigned short	p4fi;		/* P4 Freeze Step Inc */
	unsigned short	p4fe;		/* P4 Freeze End Pos */
	unsigned short	p4fc;		/* P4 Freeze Center Pos */
	unsigned short	p4fw;		/* P4 Freeze Width */
	unsigned short	p4sm;		/* P4 Step Mode */
	unsigned short	p4ar;		/* P4 Absolute/Relative */
	char		p4eu[16];		/* P4 Engineering Units */
	short		p4pr;		/* P4 Display Precision */
	double		d1hr;		/* D1 High Oper Range */
	double		d1lr;		/* D1 Low  Oper Range */
	float *          d1da;		/* D1 Data Array */
	float		d1cv;		/* D1 Current Value */
	float		d1lv;		/* D1 Last Value Posted */
	unsigned long	d1ne;		/* D1 # of Elements/Pt */
	char		d1eu[16];		/* D1 Engineering Units */
	short		d1pr;		/* D1 Display Precision */
	char		p114[6];		/* Created Pad  */
	double		d2hr;		/* D2 High Oper Range */
	double		d2lr;		/* D2 Low  Oper Range */
	float *          d2da;		/* D2 Data Array */
	float		d2cv;		/* D2 Current Value */
	float		d2lv;		/* D2 Last Value Posted */
	unsigned long	d2ne;		/* D2 # of Elements/Pt */
	char		d2eu[16];		/* D2 Engineering Units */
	short		d2pr;		/* D2 Display Precision */
	char		p115[6];		/* Created Pad  */
	double		d3hr;		/* D3 High Oper Range */
	double		d3lr;		/* D3 Low  Oper Range */
	float *          d3da;		/* D3 Data Array */
	float		d3cv;		/* D3 Current Value */
	float		d3lv;		/* D3 Last Value Posted */
	unsigned long	d3ne;		/* D3 # of Elements/Pt */
	char		d3eu[16];		/* D3 Engineering Units */
	short		d3pr;		/* D3 Display Precision */
	char		p116[6];		/* Created Pad  */
	double		d4hr;		/* D4 High Oper Range */
	double		d4lr;		/* D4 Low  Oper Range */
	float *          d4da;		/* D4 Data Array */
	float		d4cv;		/* D4 Current Value */
	float		d4lv;		/* D4 Last Value Posted */
	unsigned long	d4ne;		/* D4 # of Elements/Pt */
	char		d4eu[16];		/* D4 Engineering Units */
	short		d4pr;		/* D4 Display Precision */
	char		p117[6];		/* Created Pad  */
	double		d5hr;		/* D5 High Oper Range */
	double		d5lr;		/* D5 Low  Oper Range */
	float *          d5da;		/* D5 Data Array */
	float		d5cv;		/* D5 Current Value */
	float		d5lv;		/* D5 Last Value Posted */
	unsigned long	d5ne;		/* D5 # of Elements/Pt */
	char		d5eu[16];		/* D5 Engineering Units */
	short		d5pr;		/* D5 Display Precision */
	char		p118[6];		/* Created Pad  */
	double		d6hr;		/* D6 High Oper Range */
	double		d6lr;		/* D6 Low  Oper Range */
	float *          d6da;		/* D6 Data Array */
	float		d6cv;		/* D6 Current Value */
	float		d6lv;		/* D6 Last Value Posted */
	unsigned long	d6ne;		/* D6 # of Elements/Pt */
	char		d6eu[16];		/* D6 Engineering Units */
	short		d6pr;		/* D6 Display Precision */
	char		p119[6];		/* Created Pad  */
	double		d7hr;		/* D7 High Oper Range */
	double		d7lr;		/* D7 Low  Oper Range */
	float *          d7da;		/* D7 Data Array */
	float		d7cv;		/* D7 Current Value */
	float		d7lv;		/* D7 Last Value Posted */
	unsigned long	d7ne;		/* D7 # of Elements/Pt */
	char		d7eu[16];		/* D7 Engineering Units */
	short		d7pr;		/* D7 Display Precision */
	char		p120[6];		/* Created Pad  */
	double		d8hr;		/* D8 High Oper Range */
	double		d8lr;		/* D8 Low  Oper Range */
	float *          d8da;		/* D8 Data Array */
	float		d8cv;		/* D8 Current Value */
	float		d8lv;		/* D8 Last Value Posted */
	unsigned long	d8ne;		/* D8 # of Elements/Pt */
	char		d8eu[16];		/* D8 Engineering Units */
	short		d8pr;		/* D8 Display Precision */
	char		p121[6];		/* Created Pad  */
	double		d9hr;		/* D9 High Oper Range */
	double		d9lr;		/* D9 Low  Oper Range */
	float *          d9da;		/* D9 Data Array */
	float		d9cv;		/* D9 Current Value */
	float		d9lv;		/* D9 Last Value Posted */
	unsigned long	d9ne;		/* D9 # of Elements/Pt */
	char		d9eu[16];		/* D9 Engineering Units */
	short		d9pr;		/* D9 Display Precision */
	char		p122[6];		/* Created Pad  */
	double		dahr;		/* D10 High Oper Range */
	double		dalr;		/* D10 Low  Oper Range */
	float *          dada;		/* D10 Data Array */
	float		dacv;		/* D10 Current Value */
	float		dalv;		/* D10 LastValue Posted */
	unsigned long	dane;		/* D10 # of Elements/Pt */
	char		daeu[16];		/* D10 EngineeringUnits */
	short		dapr;		/* D10 DisplayPrecision */
	char		p123[6];		/* Created Pad  */
	double		dbhr;		/* D11 High Oper Range */
	double		dblr;		/* D11 Low  Oper Range */
	float *          dbda;		/* D11 Data Array */
	float		dbcv;		/* D11 Current Value */
	float		dblv;		/* D11 LastValue Posted */
	unsigned long	dbne;		/* D11 # of Elements/Pt */
	char		dbeu[16];		/* D11 EngineeringUnits */
	short		dbpr;		/* D11 DisplayPrecision */
	char		p124[6];		/* Created Pad  */
	double		dchr;		/* D12 High Oper Range */
	double		dclr;		/* D12 Low  Oper Range */
	float *          dcda;		/* D12 Data Array */
	float		dccv;		/* D12 Current Value */
	float		dclv;		/* D12 LastValue Posted */
	unsigned long	dcne;		/* D12 # of Elements/Pt */
	char		dceu[16];		/* D12 EngineeringUnits */
	short		dcpr;		/* D12 DisplayPrecision */
	char		p125[6];		/* Created Pad  */
	double		ddhr;		/* D13 High Oper Range */
	double		ddlr;		/* D13 Low  Oper Range */
	float *          ddda;		/* D13 Data Array */
	float		ddcv;		/* D13 Current Value */
	float		ddlv;		/* D13 LastValue Posted */
	unsigned long	ddne;		/* D13 # of Elements/Pt */
	char		ddeu[16];		/* D13 EngineeringUnits */
	short		ddpr;		/* D13 DisplayPrecision */
	char		p126[6];		/* Created Pad  */
	double		dehr;		/* D14 High Oper Range */
	double		delr;		/* D14 Low  Oper Range */
	float *          deda;		/* D14 Data Array */
	float		decv;		/* D14 Current Value */
	float		delv;		/* D14 LastValue Posted */
	unsigned long	dene;		/* D14 # of Elements/Pt */
	char		deeu[16];		/* D14 EngineeringUnits */
	short		depr;		/* D14 DisplayPrecision */
	char		p127[6];		/* Created Pad  */
	double		dfhr;		/* D15 High Oper Range */
	double		dflr;		/* D15 Low  Oper Range */
	float *          dfda;		/* D15 Data Array */
	float		dfcv;		/* D15 Current Value */
	float		dflv;		/* D15 LastValue Posted */
	unsigned long	dfne;		/* D15 # of Elements/Pt */
	char		dfeu[16];		/* D15 EngineeringUnits */
	short		dfpr;		/* D15 DisplayPrecision */
	char		p128[2];		/* Created Pad  */
	float		t1cd;		/* T1 Cmnd */
	float		t2cd;		/* T2 Cmnd */
	float		bscd;		/* Before Scan Cmnd */
	float		ascd;		/* After Scan Cmnd */
};
typedef struct scanRecord scanRecord;
#endif

