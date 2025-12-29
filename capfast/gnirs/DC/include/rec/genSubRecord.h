
/* genSubRecord.h */
#ifndef INCgenSubRecordh
#define INCgenSubRecordh
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
struct genSubRecord	{
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
	/* start of genSub specific fields */ 
	long		val;		/* Subr. return value */
	long		oval;		/* Old return value */
	long		sadr;		/* Subroutine Address */
	long		osad;		/* Old Subr. Address */
	unsigned short	lflg;		/* Link Flag */
	unsigned short	eflg;		/* Event Flag */
	char		p23 [4];		/* Created Pad  */
	struct link	subl;		/* Subroutine Input Link */
	char		inam[40];		/* Init Routine Name */
	char		snam[40];		/* Process Subr. Name */
	char		onam[40];		/* Old Subroutine Name */
	short		styp;		/* Subr symbol type */
	unsigned short	brsv;		/* Bad Return Severity */
	short		prec;		/* Display Precision */
	char		p24 [2];		/* Created Pad  */
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
	struct link	inpu;		/* Input Link U */
	char		ufa[40];		/* Input Structure A */
	char		ufb[40];		/* Input Structure B */
	char		ufc[40];		/* Input Structure C */
	char		ufd[40];		/* Input Structure D */
	char		ufe[40];		/* Input Structure E */
	char		uff[40];		/* Input Structure F */
	char		ufg[40];		/* Input Structure G */
	char		ufh[40];		/* Input Structure H */
	char		ufi[40];		/* Input Structure I */
	char		ufj[40];		/* Input Structure J */
	char		ufk[40];		/* Input Structure K */
	char		ufl[40];		/* Input Structure L */
	char		ufm[40];		/* Input Structure M */
	char		ufn[40];		/* Input Structure N */
	char		ufo[40];		/* Input Structure O */
	char		ufp[40];		/* Input Structure P */
	char		ufq[40];		/* Input Structure Q */
	char		ufr[40];		/* Input Structure R */
	char		ufs[40];		/* Input Structure S */
	char		uft[40];		/* Input Structure T */
	char		ufu[40];		/* Input Structure U */
	void *a;		/* Value of Input A */
	void *b;		/* Value of Input B */
	void *c;		/* Value of Input C */
	void *d;		/* Value of Input D */
	void *e;		/* Value of Input E */
	void *f;		/* Value of Input F */
	void *g;		/* Value of Input G */
	void *h;		/* Value of Input H */
	void *i;		/* Value of Input I */
	void *j;		/* Value of Input J */
	void *k;		/* Value of Input K */
	void *l;		/* Value of Input L */
	void *m;		/* Value of Input M */
	void *n;		/* Value of Input N */
	void *o;		/* Value of Input O */
	void *p;		/* Value of Input P */
	void *q;		/* Value of Input Q */
	void *r;		/* Value of Input R */
	void *s;		/* Value of Input S */
	void *t;		/* Value of Input T */
	void *u;		/* Value of Input U */
	unsigned short	fta;		/* Type of A */
	unsigned short	ftb;		/* Type of B */
	unsigned short	ftc;		/* Type of C */
	unsigned short	ftd;		/* Type of D */
	unsigned short	fte;		/* Type of E */
	unsigned short	ftf;		/* Type of F */
	unsigned short	ftg;		/* Type of G */
	unsigned short	fth;		/* Type of H */
	unsigned short	fti;		/* Type of I */
	unsigned short	ftj;		/* Type of J */
	unsigned short	ftk;		/* Type of K */
	unsigned short	ftl;		/* Type of L */
	unsigned short	ftm;		/* Type of M */
	unsigned short	ftn;		/* Type of N */
	unsigned short	fto;		/* Type of O */
	unsigned short	ftp;		/* Type of P */
	unsigned short	ftq;		/* Type of Q */
	unsigned short	ftr;		/* Type of R */
	unsigned short	fts;		/* Type of S */
	unsigned short	ftt;		/* Type of T */
	unsigned short	ftu;		/* Type of U */
	char		p25 [2];		/* Created Pad  */
	unsigned long	noa;		/* No. in A */
	unsigned long	nob;		/* No. in B */
	unsigned long	noc;		/* No. in C */
	unsigned long	nod;		/* No. in D */
	unsigned long	noe;		/* No. in E */
	unsigned long	nof;		/* No. in F */
	unsigned long	nog;		/* No. in G */
	unsigned long	noh;		/* No. in H */
	unsigned long	noi;		/* No. in I */
	unsigned long	noj;		/* No. in J */
	unsigned long	nok;		/* No. in K */
	unsigned long	nol;		/* No. in L */
	unsigned long	nom;		/* No. in M */
	unsigned long	non;		/* No. in N */
	unsigned long	noo;		/* No. in O */
	unsigned long	nop;		/* No. in P */
	unsigned long	noq;		/* No. in Q */
	unsigned long	nor;		/* No. in R */
	unsigned long	nos;		/* No. in S */
	unsigned long	not;		/* No. in T */
	unsigned long	nou;		/* No. in U */
	char		p26 [4];		/* Created Pad  */
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
	struct link	outu;		/* Output Link U */
	char		ufva[40];		/* Output Structure A */
	char		ufvb[40];		/* Output Structure B */
	char		ufvc[40];		/* Output Structure C */
	char		ufvd[40];		/* Output Structure D */
	char		ufve[40];		/* Output Structure E */
	char		ufvf[40];		/* Output Structure F */
	char		ufvg[40];		/* Output Structure G */
	char		ufvh[40];		/* Output Structure H */
	char		ufvi[40];		/* Output Structure I */
	char		ufvj[40];		/* Output Structure J */
	char		ufvk[40];		/* Output Structure K */
	char		ufvl[40];		/* Output Structure L */
	char		ufvm[40];		/* Output Structure M */
	char		ufvn[40];		/* Output Structure N */
	char		ufvo[40];		/* Output Structure O */
	char		ufvp[40];		/* Output Structure P */
	char		ufvq[40];		/* Output Structure Q */
	char		ufvr[40];		/* Output Structure R */
	char		ufvs[40];		/* Output Structure S */
	char		ufvt[40];		/* Output Structure T */
	char		ufvu[40];		/* Output Structure U */
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
	void *valu;		/* Value of Output U */
	void *ovla;		/* Old Output A */
	void *ovlb;		/* Old Output B */
	void *ovlc;		/* Old Output C */
	void *ovld;		/* Old Output D */
	void *ovle;		/* Old Output E */
	void *ovlf;		/* Old Output F */
	void *ovlg;		/* Old Output G */
	void *ovlh;		/* Old Output H */
	void *ovli;		/* Old Output I */
	void *ovlj;		/* Old Output J */
	void *ovlk;		/* Old Output K */
	void *ovll;		/* Old Output L */
	void *ovlm;		/* Old Output M */
	void *ovln;		/* Old Output N */
	void *ovlo;		/* Old Output O */
	void *ovlp;		/* Old Output P */
	void *ovlq;		/* Old Output Q */
	void *ovlr;		/* Old Output R */
	void *ovls;		/* Old Output S */
	void *ovlt;		/* Old Output T */
	void *ovlu;		/* Old Output U */
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
	unsigned short	ftvu;		/* Type of VALU */
	char		p27 [2];		/* Created Pad  */
	unsigned long	nova;		/* No. in VALA */
	unsigned long	novb;		/* No. in VALB */
	unsigned long	novc;		/* No. in VALC */
	unsigned long	novd;		/* No. in VALD */
	unsigned long	nove;		/* No. in VALE */
	unsigned long	novf;		/* No. in VALF */
	unsigned long	novg;		/* No. in VALG */
	unsigned long	novh;		/* No. in VAlH */
	unsigned long	novi;		/* No. in VALI */
	unsigned long	novj;		/* No. in VALJ */
	unsigned long	novk;		/* No. in VALK */
	unsigned long	novl;		/* No. in VALL */
	unsigned long	novm;		/* No. in VALM */
	unsigned long	novn;		/* No. in VALN */
	unsigned long	novo;		/* No. in VALO */
	unsigned long	novp;		/* No. in VALP */
	unsigned long	novq;		/* No. in VALQ */
	unsigned long	novr;		/* No. in VALR */
	unsigned long	novs;		/* No. in VALS */
	unsigned long	novt;		/* No. in VALT */
	unsigned long	novu;		/* No. in VALU */
	unsigned long	tova;		/* Total bytes for VALA */
	unsigned long	tovb;		/* Total bytes for VALB */
	unsigned long	tovc;		/* Total bytes for VALC */
	unsigned long	tovd;		/* Total bytes for VALD */
	unsigned long	tove;		/* Total bytes for VALE */
	unsigned long	tovf;		/* Total bytes for VALF */
	unsigned long	tovg;		/* Total bytes for VALG */
	unsigned long	tovh;		/* Total bytes for VAlH */
	unsigned long	tovi;		/* Total bytes for VALI */
	unsigned long	tovj;		/* Total bytes for VALJ */
	unsigned long	tovk;		/* Total bytes for VALK */
	unsigned long	tovl;		/* Total bytes for VALL */
	unsigned long	tovm;		/* Total bytes for VALM */
	unsigned long	tovn;		/* Total bytes for VALN */
	unsigned long	tovo;		/* Total bytes for VALO */
	unsigned long	tovp;		/* Total bytes for VALP */
	unsigned long	tovq;		/* Total bytes for VALQ */
	unsigned long	tovr;		/* Total bytes for VALR */
	unsigned long	tovs;		/* Total bytes for VALS */
	unsigned long	tovt;		/* Total bytes for VALT */
	unsigned long	tovu;		/* Total bytes for VALU */
};
typedef struct genSubRecord genSubRecord;
#endif

