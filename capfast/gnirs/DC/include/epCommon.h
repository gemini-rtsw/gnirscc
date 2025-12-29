 /*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 *    FILENAME:
 *    epCommon.h
 *
 *    PURPOSE:
 *     This include file contains definitions and prototypes used in the
 *     EPICS support routines in the GNAAC software system.
 * 
 *INDENT-OFF*
 * $Log: epCommon.h,v $
 * Revision 1.4  2011/08/18 20:01:27  gemvx
 * back out of using the local copies of the current frame, wavelength and equinox
 * This was causing a seg fault when calculating the wcs.  Ended up fixing getDbCaInfo
 * to release connections instead ... I'd like to get back to this at some point.
 *
 * Revision 1.3  2011/08/04 21:44:35  gemvx
 * use the local copies of the current frame, wavelength and equinox
 *
 * Revision 1.2  2009/05/27 19:32:26  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:19  buchholz
 * Imported gnaacSrc into CVS
 *
 *INDENT-ON* 
 */

#ifndef EPCOMMON
#define EPCOMMON

/* Language include files */
#include  <vxWorks.h>
#include  <types.h>
#include  <math.h>
#include  <time.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>

#if !defined(NODBACCESS)
/* EPICS include files */
#include  <dbDefs.h>
#include  <cadRecord.h>
#include  <genSubRecord.h>
#include  <dbCommon.h>
#include  <recSup.h>
#include  <cad.h>
#include  <dbAccess.h>

/* CICS include files */
#include  <cicsConst.h>
#include  <cicsLib.h>
#endif
#include <debug.h>

#define DIRECTIVE pCad->dir
#define SDSU_DEBUG1  CICS_DB_MIN
#define MSG pCad->mess
#define MAX_ARRAYS 1

/* bit defines for control register for sequence programs */
#define IDLE_FLAG	1		/* put sequencer into idle mode */
#define DIE		2		/* kill sequencer */
#define Read_Flag	4		/* read array flag */
#define SDT_MODE	256		/* continuous sdt mode */
#define ABORT_INT	512		/* abort integration */

/* defines for stupid RPC return values */
#define RPC_OK 1
#define RPC_ERROR 0

/* Numeric definitions */
#define DETECTOR_MIN 0
#define DETECTOR_MAX 1023
#define MAX_STRING_SIZE 40
#define MAX_NAME_SIZE 29
#define MAX_STRING 80

/* #define I_SKIP 0 */
/* #define J_SKIP 1 */

#define GNAAC_DONE 0
#define GNAAC_NOT_DONE 1

#define VOLTS_TOLERANCE		.10	
/* EPICS record name definitions for string construction in 'C' code after
 * TOP and SADTOP Put new names in alphabetical order in the list
 */
#define TOP "naac:dc:"
#define SADTOP "naac:sad:dc:"

#define ABORT_CAD	"abort"
#define ADCGFCI		"c208_223:a15"
#define ACHVD_BIAS 	"achvdBias"
#define ACTIVATE 	"activate"
#define ACTIVE_CHK 	"activChk"
#define ACQ		"acq"
#define ADCHLTH     	"adcHlth"      /*  A-to-D Conv Health  */
#define APPLY_CAR   	"applyC"
#define ARRAY_ID   	"arrayIDStr"
#define ARRAY_SIZE 	"arSize" /* pr 2-27*/
#define ARRAY_TYPE 	"arrayTypeStr"
#define ARSETUP_CAD 	"arSetup"
#define ARSETUP_CAR 	"arSetupC"
#define ARSETUP_DONE 	"arSetupDone"

#define BIAS_FAILED "biasFailed"
#define BOK "c64_79:a3"

/* #define CAR_VALS_PAR    "data/carInitVals.par" */
/* #define CAD_VALS_PAR    "data/cadInitVals.par" */
#define CAR_VALS    "data/carInitVals.pv"
#define CAD_VALS    "data/cadInitVals.pv"
#define CLEAR_CARS      "data/clearCars.pv"
#define CLEAR_CADS      "data/clearCads.pv"
#define COL_HI 		"colHigh"
#define COMMENT 	"comment"
#define CUR_COL 	"curCol" /* pr 2-27 */
#define CUR_MAX_COL 	"curMaxCol"
#define CUR_MAX_ROW 	"curMaxRow"
#define CUR_ROW 	"curRow" /* pr 2-27 */

/* #define CURRENT_FRAME            "nirsg:dc:astCtx.VALB" */
/* #define CURRENT_WAVELENGTH       "nirsg:dc:astCtx.VALD" */
/* #define CURRENT_EQUINOX          "nirsg:dc:astCtx.VALC" */
#define CURRENT_FRAME            "tcs:sad:sourceATrackFrame"
#define CURRENT_WAVELENGTH       "tcs:sad:sourceAWavelength"
#define CURRENT_EQUINOX          "tcs:sad:sourceATrackEq"


#define DACS_FAILED	"DACsFailed"
#define DATUM_CAR       "datumC"
#define DBIAS		"dBias"
#define DET_ID           "detID"
#define DET_TYPE        "detType"
#define DL_COUNT                "ucDlCount"
#define DQHLTH 		"dcaHlth"     /*  board Health ??? */
#define DCAREADY	"DCAReady"
#define DRROISET_CAD 	"drRoiSet"
#define DRROISET_CAR 	"drRoiSetC"
#define DRROISET_DONE 	"drRoiSetDone"
#define DWNLD_FAILED 	"dwnLdFailed"
#define DWNLD_STATE 	"dwnLdState"

#define END_GUIDE_CAR   "endGuideC"
#define END_OBSERVE_CAR "endObserveC"
#define END_VERIFY_CAR  "endVerifyC"
#define ERR_INT_TIME 	"errIntTime"

#define FDELAY 		"fDly"
#define FORCE_DWN_LD 	"forceDownLd"
#define FRAMEREADY	"frameReady"

#define GFCIHLTH    	"gfciHlth"     /* GFCI Health */
#define GSYS_CAR 	"gSysC"
#define GUIDE_CAR       "guideC"

#define HALT_PROC 	"stopProcess"
#define HDR_DETAIL 	"hdrDetail"
#define HDRTIMING 	"hdrTiming"
#define HEARTBEAT   	"heartBeat"
#define HICOL		"hiCol"
#define HIROW		"hiRow"
#define HKFREEZE 	"instHKFreeze"
#define HK0          "c0_15"
#define HK1          "c16_31"
#define HK2          "c32_47"
#define HK3          "c48_63"
#define HK4          "c64_79"
#define HK5          "c80_95"
#define HK6          "c96_111"
#define HK7          "c112_127"
#define HK8          "c128_143"
#define HK9          "c144_159"
#define HK10          "c160_175"
#define HK11          "c176_191"
#define HK12          "c192_207"
#define HK13          "c208_223"
#define HK14          "c224_239"
#define HK15          "c240_255"
#define HK_FREEZE 	"inst_HKFreeze"
#define HK_STATE 	"HKState"
#define HK_VDDUC	"hkVddUc"
#define HK_VDDCL1	"hkVddCl1"
#define HK_VDDCL2	"hkVddCl2"
#define HK_VGGCL1	"hkVggCl1"
#define HK_VGGCL2	"hkVggCl2"
#define HK_VDET		"hkVDet"
#define HK_VSET		"hkVSet"

#define IM_NAME          "imName"
#define IM_NUM           "imNum"
#define IM_PATH          "imPath"
/* #define INIT_VALS_PAR    "data/initVals.par" */
#define INIT_VALS    "data/initVals.pv"
#define ICON_TRACEFLAG  "icon.trace_flag"
#define INIT_CAD        "init"
#define INIT_CAR        "initC"
#define INST_ECHO_AO    "inst_EchoMe"
#define INST_ECHO_AI    "instEchoMe"
#define INT_TIME        "intTime"
#define IS1024          "is1024"
#define IS128           "is128"
#define IS256           "is256"
#define IS384           "is384"
#define IS512           "is512"
#define IS768           "is768"

#define KILLUC          "KillUC"
#define LOWCOL 		"lowCol"
#define LOWROW 		"lowRow"

#define MAX_PHOTON_TIME  "maxPhotonTime"
#define MAX_SPEED       "maxSpeed"
#define MAXCOL		"maxCol"	
#define MAXROW		"maxRow"
#define MININT		"minInt"
#define MINREAD		"minRead"	

#define NOOP_C          "noopC"
#define NUM_COADDS      "numCoAdds"
#define NUM_DAVGS       "numDAvgs"
#define NUM_LNRS        "numLNRs"
#define NUM_PICS        "numPics"

#define OBSERVE_CAD     "observe"
#define OBSERVE_CAR     "observeC"
#define OBSERVEDETAILSSEQ     "observeDetailsSeq"
#define OBSERVECMDSEQ     "observeCmdSeq"
#define OBSSETUP_CAD    "obsSetup"
#define OBSSETUP_CAR    "obsSetupC"
#define OBSSETUP_DONE   "obsSetupDone"
#define OVERALLHLTH 	"dcHealth"       /* Overall Health */

#define PARK_CAD        "park"
#define PARK_CAR        "parkC"
#define PAGFCI		"c80_95:a15"
#define PREAMPHLTH      "preAmpHlth"   /* analog preamp Health */
#define PREP 	        "prep"
#define PROC_MODE       "procMode"
#define PUC_CODETYPE    "pucuCodeType"
#define PUC_DAVGDLY     "pucDAvgsDly"
#define PUC_DONE        "pucDone"
#define PUC_FRMSPCYCLE  "pucFrmsPCycle"
#define PUC_MINDLY      "pucMinDly"
#define PUC_MININT      "pucMinInt"
#define PUC_MINREAD     "pucMinRead"
#define PWRSUPHLTH      "pwrSupHlth"   /* Power Supply Health */
#if 0
#define RD_FOOT_REF     "rdFootRef"
#define RD_MNT_REF       "rdMntRef"
#endif
#define REBOOT_CAD        "reboot"
#define REBOOT_CAR        "gSysC"
#define READHK          "ReadHK"
#define RDOUT	        "rdout"
#define ROW_HI          "rowHigh"

#define SCBACC          "instSCBRegVal"
#define SCBSET          "inst_SCBRegVal"
#define SEQCOLCNTSET    "seq_ColCnt"
#define SEQREG          "seqCntrlReg"
#define SEQREGSET       "seq_CntrlReg"
#define SEQ_NUM         "seqNum"
#define SEQTRACEFLAG  	"seqTraceFlag"
#define SETDHSINFO_CAD  "setDhsInfo"
#define DHSCONNECT_CAD  "dhsConnect"
#define SETWCS_CAD      "setWcs"
#define START_PROC      "startProcess"
#define STARTUC         "StartUC"
#define STATE           "state"
#define STOP_CAD        "stop"
/* #define SUP_VALS_PAR	"data/supportInitVals.par" */
#define SUP_VALS	"data/supportInitVals.pv"
#if 0
#define TEMP_DETABS	"footLGain"
#define TEMP_DET_ERROR  "rdFootHGain"
#define TEMP_MNTABS	"mntLGain"
#define TEMPHLTH        "tempHlth"     /* Array and ADC temperature Health */
#endif
#define TEST_CAD        "test"
#define TEST_CAR        "testC"
/* #define TEST_VALS_PAR   "data/testVals.par" */
#define TEST_VALS   "data/testVals.pv"
#define TITLE           "title"

#define UC_CODETYPE     "uCodeType"
#define UC_DAVGDLY      "ucDAvgsDly"
#define UC_FIRSTDLY      "ucFirstDly"
#define UC_FRMSPCYCLE   "ucFrmsPCycle"
#define UC_MINDLY       "ucMinDly"
#define UC_MININT       "ucMinInt"
#define UC_MINREAD      "ucMinRead"

#define VDDCL1          "VddCl1"
#define VDDCL1_CHAN     "c48_63:a13"
#define VDDCL2          "VddCl2"
#define VDDCL2_CHAN     "c64_79:a14"
#define VDDUC_CHAN      "c80_95:a10"
#define VDET            "vDet"
#define VDET_CHAN       "c80_95:a1"
#define VERIFY_CAR      "verifyC"
#define VGGCL1          "VggCl1"
#define VGGCL1_CHAN     "c64_79:a0"
#define VGGCL2          "VggCl2"
#define VGGCL2_CHAN     "c64_79:a11"
#define VOLTHLTH        "voltHlth"     /* controller array voltage health */
#define VSET            "VSet"
#define VSET_CHAN       "c64_79:a12"


/* wcs information*/
#define WCS_CAR                  "gSysC"
#define WCS_CTYPE1 "wcs_ctype1"
#define WCS_CRPIX1 "wcs_crpix1"
#define WCS_CRVAL1 "wcs_crval1"
#define WCS_CTYPE2 "wcs_ctype2"
#define WCS_CRPIX2 "wcs_crpix2"
#define WCS_CRVAL2 "wcs_crval2"
#define WCS_CD1_1 "wcs_cd11"
#define WCS_CD1_2 "wcs_cd12"
#define WCS_CD2_1 "wcs_cd21"
#define WCS_CD2_2 "wcs_cd22"
#define WCS_RADECSYS "wcs_radecsys"
#define WCS_EQUINOX "wcs_equinox"
#define WCS_MJDOBS "wcs_mjdobs"

#define WFIREHLTH       "wFireHlth"    /* wFire ICON and SEQ board health */

/* Error code definitions */
#define ERROR_MEMORY -99
#define ERROR_WCS -4
#define ERROR_EPICS -3
#define ERROR_RANGE -2
#define ERROR_TYPE -1

/* Health values for EPICS health variables */
#define HLTHGOOD "GOOD"
#define HLTHWARN "WARNING"
#define HLTHBAD  "BAD"

#if !defined(NOCODE)
#define	NOCODE	0	/* no code loaded */
#define RRD	1       /* (row) reset read fast microcode */
#define RDD     2       /* reset read read slow microcode */
#define RD      3       /* global reset one read ucode */
#endif

/* Numbers of Housekeeping channels & inputs */
#define MAX_HK_CHANNELS 16
#define NUM_HK_INPUTS   16

/* Status levels returned by routines */
#define STATUS_GOOD    0
#define STATUS_WARNING 1
#define STATUS_BAD     2
#define STATUS_ERROR   3

/* structure for holding a voltage's base value and ranges */
typedef struct 
{
    int group;
    int channel;
    int	method;
    double base;
    double warn_min;
    double warn_max;
    double bad_min;
    double bad_max;
} hkLimits_t;

#if 0
/*  Enumerated type definitions I decided we aren't going to use enums any more - ncb*/
enum HEADER_LVL  {STDHEAD, MINHEAD, MIDHEAD, MAXHEAD};
enum DET_STATE   {DEACTIVATED, ACTIVATED};
enum HKSTATE    {UNFREEZE, FREEZE};
enum PROCMODE   {STARE , SEP, CHOP, CHOP3, TEST};
enum HDR_TIMING  {BEFORE = 1, AFTER, BOTH};
enum DEBUG_MODE  {DBG_NOLOG, DBG_NONE, DBG_MIN, DBG_FULL};
enum SIM_MODE    {SIM_NONE, SIM_VSM, SIM_FAST, SIM_FULL};
#endif

#define STDHEAD		0
#define MINHEAD		1
#define MIDHEAD		2
#define MAXHEAD		3

#define DEACTIVATED	0
#define ACTIVATED	1

#define UNFREEZE	0
#define FREEZE		1

#define STARE		0
#define SEP		1
#define CHOP		2
#define CHOP3		3
#define TEST		4

#define STD             0
#define BEFORE		1
#define AFTER		2
#define BOTH		3

#define DBG_NOLOG	0
#define DBG_NONE	1
#define DBG_MIN		2
#define DBG_FULL	3

#define SIM_NONE	0
#define SIM_VSM		1
#define SIM_FAST	2
#define SIM_FULL	3

#define DHSCAD_PERM "perm"
#define DHSCAD_TEMP "temp"
#define DHSCAD_INT32 "INT32"
#define DHSCAD_INT16 "INT16"
#define DHSCAD_INT8 "INT8"

#define REBOOTCAD_CTRL "mickey"
#define REBOOTCAD_DATA "minnie"
#define REBOOTCAD_BOTH "both"
#define REBOOTCAD_CTRLVAL 0x1
#define REBOOTCAD_DATAVAL 0x2
#define REBOOTCAD_BOTHVAL 0x3


/* Other global variable declarations */
extern char tldFile[256];
extern char cmdFile[256];
extern long debugLevel;   /* declared in cicsLib.c */

#if !defined(DQ_IOC)
/* Function prototypes */
int pow2(int x,int *n);
long assignVal( unsigned short type, void *inVal, void *outVal, char *errMess);
int cvt(unsigned short type, char *in, void *out);
int check_input (unsigned short type, char *in, void *min, void *max, void *out);
long getDbInfo(char *fieldName, char *errMess, unsigned short type, void *outVal);
long getDbInfoT(char *top,char *fieldName, char *errMess, unsigned short type, void *outVal);
long setAlarm(int i,int j,double low,double high,double lolo,double hihi);
long setEpicsAlarm(char *name,double low,double high,double lolo,double hihi);
short naacStatus( char *top, char *statusRec);
long carStatus(char *input);
long processRec(char *prefix, char *record);
long putDbInfo(char *fieldName, char *errMess, unsigned short type, void *outVal);
long putDbInfoT(char *top,char *fieldName, char *errMess, unsigned short type, void *outVal);
long putDaqFlags( const long, const long, const long );

#if !defined(NODBACCESS)
void printCadVals( long debugLvl, struct cadRecord *pCad, short numVals );
#endif
void sleep(int a, int b);
long setEpicsAlarm(char *name, double low, double high, double lolo, double hihi)
;
long setEpicsAlarmT(char *top,char *name, double low, double high, double lolo, double hihi);
long setEpicsAlarmStatus(char *name);
long setCar (char *name, long ival,long ierr,char  *message,char *error);
/* Prototypes for housekeeping utility functions */

#if !defined(NODBACCESS)
long checkHK( struct genSubRecord *pGenSub );
#endif
long updateHlthVars( );
int initHkArray(char *parfile);
int procInput(int channel, int input);
void setVal(char *a, void *vala, unsigned short ftva);
int dbr_sizeof(unsigned short ftva);
int checkStatus(double val, hkLimits_t vs);
int pvload( char *fname , char *macros);

int calcVoltsHlth(char *str);
int calcTempHlth(char *str);
int calcAdcHlth(char *str);
int calcPaHlth(char *str);
int calcwFireHlth(char *str);
int calcDqHlth(char *str);
int calcPwrSupHlth(char *str);
int calcGfciHlth(char *str);
int calcOverallHlth(char *str);
#endif

#endif





