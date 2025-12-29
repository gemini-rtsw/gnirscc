/* telescope.h
 */

#define	TELERRORMESS(cond, mess)	\
	if ((strlen (tp->telerror) == 0) && (cond)) \
	    strcpy (tp->telerror, mess);

/* telescope structure definition
 */

#define	LEN_WINFO	64	/* default maximum string parameter length */

typedef	struct	{
	PP	telproto;		/* pointer to protocol struct	*/
	long	teltype;		/* entry in cap file		*/
	long	time;			/* UT in IRAF time units	*/
	char	telname[LEN_WINFO];	/* telescope name		*/
	char	telerror[LEN_WINFO];	/* telescope error string	*/
	char	dateobs[LEN_WINFO];	/* date of obs.			*/
	char	ut[LEN_WINFO];		/* universal time		*/
	char	st[LEN_WINFO];		/* sidereal time		*/
	char	ra[LEN_WINFO];		/* right ascension		*/
	char	dec[LEN_WINFO];		/* declination			*/
	char	epoch[LEN_WINFO];	/* epoch of ra & dec		*/
	char	ha[LEN_WINFO];		/* hour angle			*/
	char	zd[LEN_WINFO];		/* zenith distance		*/
	char	airmass[LEN_WINFO];	/* airmass			*/
	char	telfocus[LEN_WINFO];	/* telescope focus		*/
	char	telfilters[LEN_WINFO];	/* telescope filters		*/
	char	teltemp[LEN_WINFO];	/* tel. temperature		*/
	char	windspeed[LEN_WINFO];	/* wind speed			*/
	char	winddirection[LEN_WINFO];	/* wind direction	*/
	char	humidity[LEN_WINFO];	/* humidity			*/
	char	seeing[LEN_WINFO];	/* seeing			*/
	char	rotangle[LEN_WINFO];	/* rotation angle		*/
	char	pressure[LEN_WINFO];	/* barometer			*/
} TELESCOPE;

typedef	TELESCOPE	*TP;

#define	LEN_TELESCOPE	( sizeof(TELESCOPE)/sizeof(long) )

/* telescope action numbers
 */

#define	AT_INIT		1

/* protocol parameter numbers
 */

#define	P_TELTYPE	1		/* entry in cap file		*/
#define	P_TIME		2		/* UT in IRAF time units	*/
#define	P_TELNAME	1002		/* entry in cap file		*/
#define	P_TELERROR	1003		/* error string			*/
#define	P_DATEOBS	1004		/* date of obs.			*/
#define	P_UT		1005		/* universal time		*/
#define	P_ST		1006		/* sidereal time		*/
#define	P_RA		1007		/* right ascension		*/
#define	P_DEC		1008		/* declination			*/
#define	P_EPOCH		1009		/* epoch of ra & dec		*/
#define	P_HA		1010		/* hour angle			*/
#define	P_ZD		1011		/* zenith distance		*/
#define	P_AIRMASS	1012		/* airmass			*/
#define	P_TELFOCUS	1013		/* telescope focus		*/
#define	P_TELFILTERS	1014		/* telescope focus		*/
#define	P_TELTEMP	1015		/* tel. temperature		*/
#define	P_WINDSPEED	1016		/* wind speed			*/
#define	P_WINDDIRECTION	1017		/* wind direction		*/
#define	P_HUMIDITY	1018		/* humidity			*/
#define	P_SEEING	1019		/* seeing			*/
#define	P_ROTANGLE	1020		/* rotation angle		*/
#define	P_PRESSURE	1021		/* barometer			*/
