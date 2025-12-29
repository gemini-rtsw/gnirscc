#ifndef EPDEFINE
#define EPDEFINE
#include "ccDefines.h"

extern char *dbTop;
extern char *sadTop;
extern char *dummy;
#define DIRECTIVE pCad->dir  
#define MESSAGE pCad->mess

/* #define NUM_MECH 	10 */
/* #define EPICS_LEN	40 */
/* #define NUM_TEMPS 64 */

/* health values*/
#define BAD_HEALTH     "BAD"
#define WARNING_HEALTH "WARNING"
#define GOOD_HEALTH    "GOOD"
#define HEALTH_BAD     -1
#define HEALTH_WARNING 0
#define HEALTH_GOOD    1
/* debug levels*/
#define ERROR_MSG   CICS_DB_NOLOG
#define DEBUG0_MSG  CICS_DB_NONE
#define DEBUG1_MSG  CICS_DB_MIN
#define DEBUG2_MSG  CICS_DB_FULL


/* init defines*/
#define SIM a
#define HSIM b

/*mechanism defines*/
#define STEPSCAD a
#define POSCAD b


#if 0
/*dcSetupCad defines*/

#define INT_TIME a
#define SHUTTER b
#define FILENAME f
#define GAIN c
#define SPEED d
#define AMPS e
#define INIT_STATUS t
#endif


/*wcs defines*/
#define I_SKIP 0
#define J_SKIP 0

/* roi defines */
#define NUM_ROIS a
#define ROI_1COL b
#define ROW_BIN s
#define COL_BIN t
#define ROI_XBIN ".VALT"
#define ROI_YBIN ".VALS"

#endif
