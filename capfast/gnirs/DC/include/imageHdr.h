
#define NODBACCESS
#include "epCommon.h"
#define HDR_INTEGRITY   0     /* */
#define HDR_ARRAYTYP	1     /* ds*/
#define HDR_ARRAYID	    2         /* ds*/
#define HDR_HDRTIMING   3 
#define HDR_COADDS	     4       /* ds*/
#define HDR_LNRS	     5       /* ds*/
#define HDR_MODE	     6       /* ds*/
#define HDR_EXPTIME	     7       /* ds*/
#define HDR_UCODETYP     8	     /* ds*/
#define HDR_DAVGS	      9      /* ds*/
#define HDR_UCODENAM     10	     /* ds*/
#define HDR_DATE	     11      /* ds*/
#define HDR_DATEOBS	     12      /* ds*/
#define HDR_TIME_OBS	 13      /* ds*/
#define HDR_ENDUT	     14      /* ds*/
#define HDR_FRMSPCYCL    15     /* ds*/

#define NUM_HDR_DS 	16      /* */

#define HDR_CTYPE1	0/* frame*/
#define HDR_CRPIX1	1/* frame*/
#define HDR_CRVAL1	2/* frame*/
#define HDR_CTYPE2      3/* frame*/
#define HDR_CRPIX2	4/* frame*/
#define HDR_CRVAL2	5/* frame*/
#define HDR_CD1_1	6/* frame*/
#define HDR_CD1_2	7/* frame*/
#define HDR_CD2_1	8/* frame*/
#define HDR_CD2_2	9/* frame*/
#define HDR_RADECSYS	10/* frame*/
#define HDR_EQUINOX	11/* frame*/
#define HDR_MJDOBS	12/* frame*/
#define HDR_DROINUM	13/* frame*/
#define HDR_LOWROW	14/* frame*/
#define HDR_LOWCOL	15/* frame*/
#define HDR_HIROW	16/* frame*/
#define HDR_HICOL	17/* frame*/

#define NUM_HDR_FRAME        18/* */

#define HDR_TDETABS	0 /* ds*/
#define HDR_TMOUNT	1 /* ds*/
#define HDR_VSET	2 /* ds*/
#define HDR_VDDCL1	3 /* ds*/
#define HDR_VDDCL2	4 /* ds*/
#define HDR_VGGCL1	5 /* ds*/
#define HDR_VGGCL2	6 /* ds*/
#define HDR_VDDUC	7 /* ds*/
#define HDR_VDET	8 /* ds*/
#define NUM_HDR_DSVAR 9
#define NUM_HDR 	(NUM_HDR_DS + NUM_HDR_DSVAR*2 +1)       /*this number must be greater than the total 
                              number of headers for ds NUM_HDR_DS + */

#define HDR_END		64	
#define HDR_DS    0
#define HDR_FRAME 1
/* these 2  arrays have the header name and the epics variable it is taken from ( if it applies)
   This set is the standard headers*/

