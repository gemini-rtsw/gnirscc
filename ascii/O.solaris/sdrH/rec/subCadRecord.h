
/* subCadRecord.h */
#ifndef INCsubCadRecordh
#define INCsubCadRecordh
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
struct subCadRecord	{
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
	/* start of subCad specific fields */ 
	long		val;		/* Return Error Code */
	char		snam[40];		/* Subroutine Name */
	void *sadr;		/* Subroutine Address */
	short		styp;		/* Subr symbol type */
	char		inam[40];		/* Init Routine Name */
	unsigned short	dir;		/* CAD Directive */
	char		icid[40];		/* Client ID (In) */
	char		mess[40];		/* Message */
	short		ctyp;		/* Number of CAD Args */
	short		prec;		/* Display Precision */
	struct link	alnk;		/* Abort  Link */
	struct link	clnk;		/* Clear  Link */
	struct link	plnk;		/* Preset Link */
	struct link	stlk;		/* Start  Link */
	struct link	splk;		/* Stop   Link */
	char		ocid[40];		/* Client ID (Out) */
	unsigned short	osim;		/* Simulation Mode (Out) */
	short		narg;		/* No. Inputs used */
	unsigned short	mark;		/* Is Record Preset? */
	unsigned short	ersv;		/* Error Alarm Severity */
	struct link	siol;		/* Simulation Error Link */
	long		sval;		/* Simulation Error */
	char		p19 [4];		/* Created Pad  */
	struct link	siml;		/* Simulation Mode Link */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	sims;		/* Sim Mode Alarm Svrity */
	char		p20 [4];		/* Created Pad  */
	struct link	inpa;		/* Input Link A */
	struct link	inpb;		/* Input Link B */
	struct link	inpc;		/* Input Link C */
	struct link	inpd;		/* Input Link D */
	struct link	inpe;		/* Input Link E */
	struct link	inpf;		/* Input Link F */
	struct link	inpg;		/* Input Link G */
	struct link	inph;		/* Input Link H */
	struct link	inpi;		/* Input Link I */
	struct link	inpj;		/* Input Link J */
	struct link	inpk;		/* Input Link K */
	struct link	inpl;		/* Input Link L */
	struct link	inpm;		/* Input Link M */
	struct link	inpn;		/* Input Link N */
	struct link	inpo;		/* Input Link O */
	struct link	inpp;		/* Input Link P */
	struct link	inpq;		/* Input Link Q */
	struct link	inpr;		/* Input Link R */
	struct link	inps;		/* Input Link S */
	struct link	inpt;		/* Input Link T */
	struct link	outa;		/* Output Link A */
	struct link	outb;		/* Output Link B */
	struct link	outc;		/* Output Link C */
	struct link	outd;		/* Output Link D */
	struct link	oute;		/* Output Link E */
	struct link	outf;		/* Output Link F */
	struct link	outg;		/* Output Link G */
	struct link	outh;		/* Output Link H */
	struct link	outi;		/* Output Link I */
	struct link	outj;		/* Output Link J */
	struct link	outk;		/* Output Link K */
	struct link	outl;		/* Output Link L */
	struct link	outm;		/* Output Link M */
	struct link	outn;		/* Output Link N */
	struct link	outo;		/* Output Link O */
	struct link	outp;		/* Output Link P */
	struct link	outq;		/* Output Link Q */
	struct link	outr;		/* Output Link R */
	struct link	outs;		/* Output Link S */
	struct link	outt;		/* Output Link T */
	char		a[40];		/* Value of Input A */
	char		b[40];		/* Value of Input B */
	char		c[40];		/* Value of Input C */
	char		d[40];		/* Value of Input D */
	char		e[40];		/* Value of Input E */
	char		f[40];		/* Value of Input F */
	char		g[40];		/* Value of Input G */
	char		h[40];		/* Value of Input H */
	char		i[40];		/* Value of Input I */
	char		j[40];		/* Value of Input J */
	char		k[40];		/* Value of Input K */
	char		l[40];		/* Value of Input L */
	char		m[40];		/* Value of Input M */
	char		n[40];		/* Value of Input N */
	char		o[40];		/* Value of Input O */
	char		p[40];		/* Value of Input P */
	char		q[40];		/* Value of Input Q */
	char		r[40];		/* Value of Input R */
	char		s[40];		/* Value of Input S */
	char		t[40];		/* Value of Input T */
	void *vala;		/* Value of Output A */
	void *valb;		/* Value of Output B */
	void *valc;		/* Value of Output C */
	void *vald;		/* Value of Output D */
	void *vale;		/* Value of Output E */
	void *valf;		/* Value of Output F */
	void *valg;		/* Value of Output G */
	void *valh;		/* Value of Output H */
	void *vali;		/* Value of Output I */
	void *valj;		/* Value of Output J */
	void *valk;		/* Value of Output K */
	void *vall;		/* Value of Output L */
	void *valm;		/* Value of Output M */
	void *valn;		/* Value of Output N */
	void *valo;		/* Value of Output O */
	void *valp;		/* Value of Output P */
	void *valq;		/* Value of Output Q */
	void *valr;		/* Value of Output R */
	void *vals;		/* Value of Output S */
	void *valt;		/* Value of Output T */
	unsigned short	ftva;		/* Type of VALA */
	unsigned short	ftvb;		/* Type of VALB */
	unsigned short	ftvc;		/* Type of VALC */
	unsigned short	ftvd;		/* Type of VALD */
	unsigned short	ftve;		/* Type of VALE */
	unsigned short	ftvf;		/* Type of VALF */
	unsigned short	ftvg;		/* Type of VALG */
	unsigned short	ftvh;		/* Type of VALH */
	unsigned short	ftvi;		/* Type of VALI */
	unsigned short	ftvj;		/* Type of VALJ */
	unsigned short	ftvk;		/* Type of VALK */
	unsigned short	ftvl;		/* Type of VALL */
	unsigned short	ftvm;		/* Type of VALM */
	unsigned short	ftvn;		/* Type of VALN */
	unsigned short	ftvo;		/* Type of VALO */
	unsigned short	ftvp;		/* Type of VALP */
	unsigned short	ftvq;		/* Type of VALQ */
	unsigned short	ftvr;		/* Type of VALR */
	unsigned short	ftvs;		/* Type of VALS */
	unsigned short	ftvt;		/* Type of VALT */
};
typedef struct subCadRecord subCadRecord;
#endif

