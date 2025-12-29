#define NODBACCESS
#include "epCommon.h"

 

/*****************************************************************************
 * basic typedef
 *****************************************************************************/
typedef union HeaderTypedVals
{
	int	 ival;
	long lval;
	float fval;
	double dval;
	char sval[MAX_STRING_SIZE];
} HeaderTypedVals;

typedef struct HeaderVals
{
	HeaderTypedVals value;
	int             status;
}HeaderVals;

/*
 * header definition (will be static)
 */
typedef struct dhsHeadersDef {
	char 				*keyword;
	int 				type;
	int 				timing;
	int                 detail;
	char 				*pvname;
	VOIDFUNCPTR 		proc;
	int                 id;
	} dhsHeadersDef;


/*****************************************************************************
 * user defined processing prototypes
 *****************************************************************************/
void sethdrINTEGRITY(HeaderVals *hdrval);
void sethdrDROINUM(HeaderVals *hdrval);
void sethdrLOWROW(HeaderVals *hdrval);
void sethdrLOWCOL(HeaderVals *hdrval);
void sethdrHIROW(HeaderVals *hdrval);
void sethdrHICOL(HeaderVals *hdrval);
void sethdrPrecision3Digits(HeaderVals *hdrval);
void sethdrPrecision4Digits(HeaderVals *hdrval);

STATUS setHeaderDEBUG(int captBuf,unsigned int xstart1,unsigned int xstart2 ,unsigned int vme);


/*=============================================================================
 * Top definition
 *============================================================================*/

#define DB_TOP "nirs:"
#define DB_GATEWAY_TOP "nirsg:" /* All DC side channels go thru gateway*/
#define DC_TOP DB_GATEWAY_TOP "dc:"
#define CC_TOP DB_TOP "cc:"
#define TCS_SAD_TOP "tcs:sad:"
#define TCS_TOP "tcs:"



/*=============================================================================
 * Dataset headers definition
 *============================================================================*/

typedef enum {
	HDR_CTYPE1,
	HDR_CRPIX1,
	HDR_CRVAL1,
	HDR_CTYPE2,
	HDR_CRPIX2,
	HDR_CRVAL2,
	HDR_CD1_1,
	HDR_CD1_2,
	HDR_CD2_1,
	HDR_CD2_2,
	HDR_MJD_OBS,
	HDR_INTEGRITY,
	HDR_FRAME
} dhsDsHdrIndex ;


#define DHSDSHEADER (61)
#ifndef DUM_GBLSOURCE
extern dhsHeadersDef dhsDsHeader[];
extern HeaderVals dsHdr		[MAXCAPTBUFS][DHSDSHEADER];
#else
HeaderVals dsHdr			[MAXCAPTBUFS][DHSDSHEADER];
dhsHeadersDef dhsDsHeader[] =
  {/*   keyword     type      timing   detail    pvname                       proc     id #*/
/*    {"DEBUG", 	 DCASTRING,  NEVER,   HDR_DC,   NULL,  	                       NULL,  0}, */
    {"CTYPE1",   DCASTRING,  NEVER,   HDR_DC,   DC_TOP WCS_CTYPE1,             NULL,  0},/*  the handling of the WCS	    */
    {"CRPIX1",	 DCADOUBLE,  NEVER,   HDR_DC,    DC_TOP WCS_CRPIX1,             NULL,  1},/*  is somewhat different       */
    {"CRVAL1",	 DCADOUBLE,  BEFORE,  HDR_DC,    DC_TOP WCS_CRVAL1,             NULL,  2},/* and will need to be sorted out*/
    {"CTYPE2",   DCASTRING,  NEVER,   HDR_DC,    DC_TOP WCS_CTYPE2,             NULL,  3},/*   for now it's got it's	    */
    {"CRPIX2",   DCADOUBLE,  NEVER,   HDR_DC,    DC_TOP WCS_CRPIX2,             NULL,  4},/*      own         		 */
    {"CRVAL2",   DCADOUBLE,  NEVER,   HDR_DC,    DC_TOP WCS_CRVAL2,             NULL,  5},/*      	getWCS		*/
    {"CD1_1",    DCADOUBLE,  NEVER,   HDR_DC,    DC_TOP WCS_CD1_1,              NULL,  6},/*      and			 */
    {"CD1_2",    DCADOUBLE,  NEVER,   HDR_DC,    DC_TOP WCS_CD1_2,              NULL,  7},/*      	getWCSinfo	*/
    {"CD2_1",	 DCADOUBLE,  NEVER,   HDR_DC,    DC_TOP WCS_CD2_1,              NULL,  8},/*      routine		*/
    {"CD2_2",	 DCADOUBLE,  NEVER,   HDR_DC,    DC_TOP WCS_CD2_2,              NULL,  9},/*  I flagged them as never */
   
    {"MJD_OBS",	 DCADOUBLE,  NEVER,   HDR_DC,    DC_TOP WCS_MJDOBS,             NULL,  10},/*  for this reason    	*/
    {"INTEGRITY",DCASTRING,  AFTER,   HDR_DC,    NULL,  	     sethdrINTEGRITY,  11},
    {"FRAME",    DCASTRING,  AFTER,   HDR_DC,    TCS_SAD_TOP "sourceAInputFrame",NULL, 12},/* here it's NOW or NEVER - so NOW */	 
    {"FRAME",  	 DCASTRING,  NOW,     HDR_DC,    TCS_SAD_TOP "sourceAInputFrame", NULL,13},/* here it's NOW or NEVER - so NOW */
    {"UCODEPTH", DCASTRING,  BEFORE,  HDR_DC,   "nirsg:dc:arSetup.A",	        NULL, 14},
    {"UCODENAM", DCASTRING,  BEFORE,  HDR_DC,   "nirsg:dc:arSetup.B",	        NULL, 15},
    {"COADDS",   DCALONG,    BEFORE,  HDR_DC,   "nirsg:dc:obsSetup.VALE",	NULL, 16},
    {"LNRS",     DCALONG,    BEFORE,  HDR_DC,   "nirsg:dc:obsSetup.VALD",	NULL, 17},
    {"NDAVGS",   DCALONG,    BEFORE,  HDR_DC,   "nirsg:dc:obsSetup.VALC",        NULL, 18},
    {"ROWS",     DCALONG,    BEFORE,  HDR_DC,   "nirsg:dc:curMaxRow.VAL",	NULL, 19},
    {"P_MODE",   DCASTRING,  BEFORE,  HDR_DC,   "nirsg:sad:dc:procMode.VAL",	NULL, 20},
    {"COLS",     DCALONG,    BEFORE,  HDR_DC,   "nirsg:dc:curMaxCol.VAL",	NULL, 21},
    {"EXPTIME",	 DCADOUBLE,  BEFORE,  HDR_DC,   "nirsg:dc:integTime.VAL",	NULL, 22},
    {"MIN_INT",	 DCADOUBLE,  BEFORE,  HDR_DC,   "nirsg:dc:minInt.VAL",	        NULL, 23},
    {"FRMS_EXP", DCALONG,    BEFORE,  HDR_DC,   "nirsg:dc:ucFrmsPCycle.VAL",	NULL, 24},
    {"DSRDTEMP", DCADOUBLE,  BEFORE,  HDR_DC,   "nirsg:dc:C1.SETP",	        NULL, 27},
    {"DETTEMP",  DCADOUBLE,  BEFORE,  HDR_DC,   "nirsg:dc:C1Tmp",	        NULL, 28},
    {"TMP_ERROR",DCADOUBLE,  NEVER,   HDR_DC,   "nirsg:dc:rdFootHGain",	        NULL, 29},
    {"FOOT_PWR", DCADOUBLE,  BEFORE,  HDR_DC,   "nirsg:dc:C1Heat",	        NULL, 30},
    {"MNT_PWR",  DCADOUBLE,  NEVER,   HDR_DC,   "nirsg:dc:mntHtrFB",	        NULL, 31},
    {"IMG_NAME", DCASTRING,  NEVER,  HDR_DC,   "nirsg:dc:imName.VAL",	        NULL, 32},
    {"IMG_NUM",  DCALONG,    NEVER,  HDR_DC,   "nirsg:dc:imNum.VAL",	        NULL, 33}, 
    {NULL,  	 DCASTRING, BEFORE,  HDR_DC,   NULL,	NULL, 34}
  };

  

#endif

/*=============================================================================
 * Frame definition
 *============================================================================*/

/* WCS is replicated in the dataset and the frame header */

typedef enum {
	FRHDR_CTYPE1,
	FRHDR_CRPIX1,
	FRHDR_CRVAL1,
	FRHDR_CTYPE2,
	FRHDR_CRPIX2,
	FRHDR_CRVAL2,
	FRHDR_CD1_1,
	FRHDR_CD1_2,
	FRHDR_CD2_1,
	FRHDR_CD2_2,
	FRHDR_MJD_OBS,
	FRHDR_FRAME
} dhsFrameHdrIndex ;

/*
 * there is no frame header for now, keep a 'NONE' definition
 * as a skeleton and stay ANSI compliant
 */



#define DHSFRAMEHEADER (DHSDSHEADER+12)

#ifndef DUM_GBLSOURCE
	extern dhsHeadersDef dhsFrameHeader[];
	extern HeaderVals frameHdr		[MAXCAPTBUFS][DHSFRAMEHEADER-DHSDSHEADER];
#else
	HeaderVals frameHdr		[MAXCAPTBUFS][DHSFRAMEHEADER-DHSDSHEADER];
	dhsHeadersDef dhsFrameHeader[] =
	  {/*   keyword     type      timing   detail     ----pvname------               proc      id #*/
	  {"CTYPE1",	DCASTRING,  NEVER,   HDR_DC,	DC_TOP WCS_CTYPE1,   NULL, DHSDSHEADER},/*  the handling of the WCS   */
	  {"CRPIX1",	DCADOUBLE,  NEVER,   HDR_DC,	DC_TOP WCS_CRPIX1,   NULL, DHSDSHEADER+1},/*  is somewhat different   */
	  {"CRVAL1",	DCADOUBLE,  NEVER,   HDR_DC, 	DC_TOP WCS_CRVAL1,   NULL, DHSDSHEADER+2},/* and will need to be sorted out*/
	  {"CTYPE2",	DCASTRING,  NEVER,   HDR_DC,	DC_TOP WCS_CTYPE2,   NULL, DHSDSHEADER+3},/*   for now it's got it's  */
	  {"CRPIX2",	DCADOUBLE,  NEVER,   HDR_DC,	DC_TOP WCS_CRPIX2,   NULL, DHSDSHEADER+4},/*      own              */
	  {"CRVAL2",	DCADOUBLE,  NEVER,   HDR_DC,	DC_TOP WCS_CRVAL2,   NULL, DHSDSHEADER+5},/*      	getWCS	*/
	  {"CD1_1",	DCADOUBLE,  NEVER,   HDR_DC,	DC_TOP WCS_CD1_1,    NULL, DHSDSHEADER+6},   /*      and	   */
	  {"CD1_2",	DCADOUBLE,  NEVER,   HDR_DC,	DC_TOP WCS_CD1_2,    NULL, DHSDSHEADER+7},   /*      	getWCSinfo*/
	  {"CD2_1",	DCADOUBLE,  NEVER,   HDR_DC,	DC_TOP WCS_CD2_1,    NULL, DHSDSHEADER+8},   /*      routine	*/
	  {"CD2_2",	DCADOUBLE,  NEVER,   HDR_DC,	DC_TOP WCS_CD2_2,    NULL, DHSDSHEADER+9},   /*  I flagged them as never*/
	  {"MJD_OBS",	DCADOUBLE,  NEVER,   HDR_DC,	DC_TOP WCS_MJDOBS,   NULL, DHSDSHEADER+10},   /*  for this reason    */
	  {NULL,  	DCASTRING,  BEFORE,  HDR_DC,   NULL,	             NULL, DHSDSHEADER+11}
	};
#endif

/*=============================================================================
 * FrameRoi definition
 *============================================================================*/

typedef enum {
	HDR_DROINUM,
	HDR_LOWROW,
	HDR_LOWCOL,
	HDR_HIROW,
	HDR_HICOL
} dhsFrameRoiHdrIndex ;

#define DHSFRAMEROIHEADER (HDR_HICOL+1)
#define NUM_HEADER (DHSFRAMEHEADER + DHSFRAMEROIHEADER)

#ifndef DUM_GBLSOURCE
	extern dhsHeadersDef dhsFrameRoiHeader[];
	extern HeaderVals frameRoiHdr	[MAXCAPTBUFS][DHSFRAMEROIHEADER];
#else
	HeaderVals frameRoiHdr	[MAXCAPTBUFS][DHSFRAMEROIHEADER];
	dhsHeadersDef dhsFrameRoiHeader[] =
	{/*   keyword     type    timing   detail    pvname   proc      id #*/
    	{"DROINUM",	DCALONG,   NOW,    NULL, 	 NULL,   NULL,  DHSFRAMEHEADER+0	},  /* only support NOW timing */
    	{"LOWROW",	DCALONG,   NOW,    NULL, 	 NULL,   NULL,  DHSFRAMEHEADER+1 },  /* only support NOW timing */
    	{"LOWCOL",	DCALONG,   NOW,    NULL, 	 NULL,   NULL,  DHSFRAMEHEADER+2 },  /* only support NOW timing */
    	{"HIROW",	DCALONG,   NOW,    NULL, 	 NULL,   NULL,  DHSFRAMEHEADER+3 },	/* only support NOW timing */
    	{"HICOL",	DCALONG,   NOW,    NULL, 	 NULL,   NULL,  DHSFRAMEHEADER+4 }, 	/* only support NOW timing */
		{NULL,  	         DCASTRING,     BEFORE,  HDR_DC,   NULL,	NULL, DHSDSHEADER+54}
	};
#endif
