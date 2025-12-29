
/* mosubRecord.h */
#ifndef INCmosubRecordh
#define INCmosubRecordh
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
struct mosubRecord	{
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
	/* start of mosub specific fields */ 
	double		val;		/* Result */
	char		inam[16];		/* Init Routine Name */
	char		snam[16];		/* Subroutine Name */
	void *		sadr;		/* Subroutine Address */
	short		styp;		/* Subr symbol type */
	char		p21 [2];		/* Created Pad  */
	struct link	inpa;		/* Input A */
	struct link	inpb;		/* Input B */
	struct link	inpc;		/* Input C */
	struct link	inpd;		/* Input D */
	struct link	inpe;		/* Input E */
	struct link	inpf;		/* Input F */
	struct link	inpg;		/* Input G */
	struct link	inph;		/* Input H */
	struct link	inpi;		/* Input I */
	struct link	inpj;		/* Input J */
	struct link	inpk;		/* Input K */
	struct link	inpl;		/* Input L */
	struct link	inpm;		/* Input M */
	struct link	outa;		/* Output A */
	struct link	outb;		/* Output B */
	struct link	outc;		/* Output C */
	struct link	outd;		/* Output D */
	struct link	oute;		/* Output E */
	struct link	outf;		/* Output F */
	struct link	out1;		/* Output 1 */
	struct link	out2;		/* Output 2 */
	struct link	out3;		/* Output 3 */
	struct link	out4;		/* Output 4 */
	struct link	out5;		/* Output 5 */
	struct link	out6;		/* Output 6 */
	char		egu[16];		/* Units Name */
	float		hopr;		/* High Operating Rng */
	float		lopr;		/* Low Operating Range */
	float		hihi;		/* Hihi Alarm Limit */
	float		lolo;		/* Lolo Alarm Limit */
	float		high;		/* High Alarm Limit */
	float		low;		/* Low Alarm Limit */
	short		prec;		/* Display Precision */
	unsigned short	brsv;		/* Bad Return Severity */
	unsigned short	hhsv;		/* Hihi Severity */
	unsigned short	llsv;		/* Lolo Severity */
	unsigned short	hsv;		/* High Severity */
	unsigned short	lsv;		/* Low Severity */
	char		p22 [4];		/* Created Pad  */
	double		hyst;		/* Alarm Deadband */
	double		adel;		/* Archive Deadband */
	double		mdel;		/* Monitor Deadband */
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
	char		m[40];		/* Value of Input M */
	double		vala;		/* Value of Output A */
	double		valb;		/* Value of Output B */
	double		valc;		/* Value of Output C */
	double		vald;		/* Value of Output D */
	double		vale;		/* Value of Output E */
	double		valf;		/* Value of Output F */
	char		str1[40];		/* String 1 */
	char		str2[40];		/* String 2 */
	char		str3[40];		/* String 3 */
	char		str4[40];		/* String 4 */
	char		str5[40];		/* String 5 */
	char		str6[40];		/* String 6 */
	double		la;		/* Prev Value of A */
	double		lb;		/* Prev Value of B */
	double		lc;		/* Prev Value of C */
	double		ld;		/* Prev Value of D */
	double		le;		/* Prev Value of E */
	double		lf;		/* Prev Value of F */
	double		lg;		/* Prev Value of G */
	double		lh;		/* Prev Value of H */
	double		li;		/* Prev Value of I */
	double		lj;		/* Prev Value of J */
	double		lk;		/* Prev Value of K */
	double		ll;		/* Prev Value of L */
	char		lm[40];		/* Prev Value of M */
	double		lva;		/* Prev Output A */
	double		lvb;		/* Prev Output B */
	double		lvc;		/* Prev Output C */
	double		lvd;		/* Prev Output D */
	double		lve;		/* Prev Output E */
	double		lvf;		/* Prev Output F */
	char		ls1[40];		/* Prev String 1 */
	char		ls2[40];		/* Prev String 2 */
	char		ls3[40];		/* Prev String 3 */
	char		ls4[40];		/* Prev String 4 */
	char		ls5[40];		/* Prev String 5 */
	char		ls6[40];		/* Prev String 6 */
	double		lalm;		/* Last Value Alarmed */
	double		alst;		/* Last Value Archived */
	double		mlst;		/* Last Value Monitored */
};
typedef struct mosubRecord mosubRecord;
#endif

