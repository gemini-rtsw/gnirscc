#ifndef EPDEFINE
#define EPDEFINE

extern char *dbTop;
extern char *dbSadTop;
extern char *dummy;
#define DIRECTIVE pCad->dir  
#define MESSAGE pCad->mess


/* debug levels*/
#define SDSU_ERROR  CICS_DB_NOLOG
#define SDSU_LG  CICS_DB_NONE
#define SDSU_DEBUG1  CICS_DB_MIN
#define SDSU_DEBUG2  CICS_DB_FULL

/*EPICS  record names*/
#define APPLY			"apply"
#define ABORT_CAD		"abort"
#define ABORT_CAR		"observeC"    

#define AST_GENSUB		"astCtx"

#define BIN_DONE		"binDone"

#define CONTINUE_CAR	        "observeC"   
#define CONTINUE_CAD	        "continue"

#define DEBUG_CAR		"genSysC"

#define EXPOSEDRQ   		"exposedRQ"

#define GENSYS_CAR_INTERFACE    "genSysToggle"

#define INIT_CAD		"init"
#define INIT_CAR		"initC"
#define INIT_DONE		"initDone"

#define INIT_WCS_CAD		"initWcs"
#define INIT_WCS_CAR		"genSysC"
#define INIT_WCS_DONE		"initWcsDone"


#define OBSERVE_CAD		"observe"
#define OBSERVE_CAR		"observeC"
#define OBSERVING		"observing"
#define DCSETUP_CAD	        "dcSetup"
#define DCSETUP_CAR	        "dcSetupC"
#define DCSETUP_CAR_INTERFACE  "dcSetupToggle"
#define DCSETUP_DONE	        "dcSetupDone"
#define OBS_PAUSED		"obsPaused"

#define PARK_CAD		"park"
#define PARK_CAR		"parkC"	   
#define PAUSE_CAD		"pause"
#define PAUSE_CAR		"observeC" 

#define REBOOT_CAD		"reboot"
/* #define REBOOT_CAR		"genSysC" */
#define ROI_CAD			"roi"
#define ROI_CAR			"dcSetupC"
#define REMAINING_TIME	        "remainingTime"
#define ROI_DONE           "roiSetupDone"
	

/*status and alarm records*/
#define SAD_ACQ			"acq"
#define SAD_AMP_IN_USE		"portCnt"
#define SAD_CON_ID		"conID"
#define SAD_DARKTIME		"darktime"
#define SAD_DATALABEL		"dataLabel"
#define SAD_DCINIT		"dcInit"
#define SAD_DET_ID		"detId"
#define SAD_DET_TYPE		"detType"
#define SAD_ELAPSED		"elapsed"
#define SAD_EXPOSED		"exposed"
#define SAD_EXPOSEDRQ		"exposedRQ"
#define SAD_GAIN		"gain"
#define SAD_HEADTEMP		"detTemp"
#define SAD_HEALTH		"healthmbbi"
#define SAD_DHS_HEALTH		"dhsHealth"
#define SAD_HEARTBEAT		"heartBeat"
#define SAD_NAME		"name"
#define SAD_OBSERVE_ID		"dataLabel"
#define SAD_PREP		"prep"
#define SAD_RDOUT		"rdout"
#define SAD_RDSPEED		"rdspeed"
#define SAD_ROI			"roi"	
#define SAD_ROICNT		"roiCnt"	
#define SAD_SHUTPOS		"shutPos"
#define SAD_SHUTTER_HEALTH	"shutHealth"
#define SAD_STATE		"state"
#define SAD_TIME_LEFT		"timeleft"
#define SAD_UTEND		"utend"
#define SAD_UTSTART		"utstart"
#define SAD_XPIXELS1		"det1MaxCol"
#define SAD_XPIXELS2		"det2MaxCol"
#define SAD_XPIXELS3		"det3MaxCol"
#define SAD_YPIXELS1		"det1MaxRow"
#define SAD_YPIXELS2		"det2MaxRow"
#define SAD_YPIXELS3		"det3MaxRow"
#define SAD_XBIN		"xBin"
#define SAD_YBIN		"yBin"


#define SETDHSINFO_CAD	        "setDhsInfo"
#define SETDHSINFO_CAR	        "genSysC"
#define STOP_CAD		"stop"
#define STOP_CAR		"observeC"  

#define TEST_CAD		"test"
#define TEST_CAR		"testC"	

#define UCODEDWNLD_DONE         "ucodeDwnLdDone"
/* #define WCS_CAR			"genSysC" */


/*noops*/
#define VERIFY_CT 	"verifyToggle"
#define	END_VERIFY_CT	"endVerifyToggle"
#define GUIDE_CT	"guideToggle"
#define END_GUIDE_CT	"endGuideToggle"
#define DATUM_CT	"datumToggle"
#define END_OBSERVE_CT	"endObserveToggle"

/* init defines*/
#define SIM a
/*dcSetupCad defines*/

/* #define INT_TIME a */
#define SHUTTER b
#define FILENAME f
#define GAIN c
#define SPEED d
#define AMPS e
#define INIT_STATUS t



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
