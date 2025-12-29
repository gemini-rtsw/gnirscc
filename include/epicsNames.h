/*EPICS  record names*/
#define APPLY			"apply"

#define ACQ_DATUM		"acqDatumCad"
#define ACQ_PARK		"acqParkCad"
#define ACQ_STEPS		"acqStepsCad"
#define ACQ_POS			"acqPosCad"
#define ACQ_CAR			"acqC"
#define AST_GENSUB		"astCtx"

/* #define BIN_DONE		"binDone" */

#define CAMERA_DATUM		"cameraDatumCad"
#define CAMERA_PARK		"cameraParkCad"
#define CAMERA_POS			"cameraPosCad"
#define CAMERA_STEPS		"cameraStepsCad"
#define CAMERA_CAR			"cameraC"
#define COVER_DATUM		"coverDatumCad"
#define COVER_PARK		"coverParkCad"
#define COVER_POS		"coverPosCad"
#define COVER_STEPS		"coverStepsCad"
#define COVER_CAR		"coverC"
#define CRYO_CAR		"cryoC"
#define CRYO_CAD		"cryo"


#define DATUM_CAD		"datum"
#define DATUM_CAR		"datumC"
#define DECKER_DATUM		"deckerDatumCad"
#define DECKER_PARK		"deckerParkCad"
#define DECKER_POS		"deckerPosCad"
#define DECKER_STEPS		"deckerStepsCad"
#define DECKER_CAR		"deckerC"
#define DEBUG_CAD		"debug"
#define DEBUG_CAR		"debugC"
#define DIAGNOSE_CAD		"diagnose"
#define DIAGNOSE_CAR		"diagnoseC"

/* #define EXPOSEDRQ   		"exposedRQ" */

#define FW1_DATUM		"fw1DatumCad"
#define FW1_PARK		"fw1ParkCad"
#define FW1_STEPS		"fw1StepsCad"
#define FW1_POS  		"fw1PosCad"
#define FW1_CAR	  	        "fw1C"
#define FW2_DATUM		"fw2DatumCad"
#define FW2_PARK		"fw2ParkCad"
#define FW2_STEPS		"fw2StepsCad"
#define FW2_POS  		"fw2PosCad"
#define FW2_CAR	 	        "fw2C"
#define FOCUS_DATUM	       	"focusDatumCad"
#define FOCUS_PARK		"focusParkCad"
#define FOCUS_STEPS		"focusStepsCad"
#define FOCUS_POS  		"focusPosCad"
#define FOCUS_CAR		"focusC"

#define SYS_CAR_INTERFACE    "sysToggle"
#define GRATING_DATUM		"gratingDatumCad"
#define GRATING_PARK		"gratingParkCad"
#define GRATING_STEPS		"gratingStepsCad"
#define GRATING_POS  		"gratingPosCad"
#define GRATING_CAR		"gratingC"

/* health records*/
#define SYS_HEALTH     "sysHealth"
#define CCTOP_HEALTH    "ccTopHealth"
#define MOTOR_HEALTH    "motorHealth"
#define TEMPERATURE_HEALTH  "tempHealth"
#define PRESSURE_HEALTH    "pressureHealth"
#define SAD_HEALTH    "sadHealth"
#define ACQ_HEALTH "acqHealth"
#define CAMERA_HEALTH "cameraHealth"
#define COVER_HEALTH "coverHealth"
#define DECKER_HEALTH "deckerHealth"
#define SLIT_HEALTH "slitHealth"
#define FW1_HEALTH "fw1Health"
#define FW2_HEALTH "fw2Health"
#define FOCUS_HEALTH "focusHealth"
#define GRATING_HEALTH "gratingHealth"
#define XDISP_HEALTH "xdispHealth"

#define INIT_CAD		"init"
#define INIT_CAR		"initC"
#define INIT_DONE		"initialized"

/* #define INIT_WCS_CAD		"initWcs" */
/* #define INIT_WCS_CAR		"sysC" */
/* #define INIT_WCS_DONE		"initWcsDone" */

#define NOOP_CAR                "noOpC"

#define OBSERVING		"observing"
/* #define DCSETUP_CAD	        "dcSetup" */
/* #define DCSETUP_CAR	        "dcSetupC" */
/* #define DCSETUP_CAR_INTERFACE  "dcSetupToggle" */
/* #define DCSETUP_DONE	        "dcSetupDone" */
/* #define OBS_PAUSED		"obsPaused" */

#define PARK_CAD		"park"
#define PARK_CAR		"parkC"	 
#define PRESSURE_TC1    "pressureTC1"
#define PRESSURE_TC2    "pressureTC2"
#define PRESSURE_IG    "pressureIG"


#define REBOOT_CAD		"reboot"
#define REBOOT_CAR		"sysC"
#define ROI_CAD			"roi"
#define ROI_CAR			"dcSetupC"
/* #define REMAINING_TIME	        "remainingTime" */
/* #define ROI_DONE           "roiSetupDone" */
	
#define SLIT_DATUM		"slitDatumCad"
#define SLIT_PARK		"slitParkCad"
#define SLIT_STEPS		"slitStepsCad"
#define SLIT_POS  		"slitPosCad"
#define SLIT_CAR		"slitC"
#define SPARE1_DATUM		"spare1DatumCad"
#define SPARE1_PARK		"spare1ParkCad"
#define SPARE1_STEPS 		"spare1StepsCad"
#define SPARE1_POS		"spare1PosCad"
#define SPARE1_CAR		"spare1C"
#define SPARE2_DATUM		"spare2DatumCad"
#define SPARE2_PARK		"spare2ParkCad"
#define SPARE2_STEPS		"spare2StepsCad"
#define SPARE2_POS  		"spare2PosCad"
#define SPARE2_CAR		"spare2C"

#define XDISP_DATUM	      	"xdispDatumCad"
#define XDISP_PARK		"xdispParkCad"
#define XDISP_STEPS		"xdispStepsCad"
#define XDISP_POS  		"xdispPosCad"
#define XDISP_CAR		"xdispC"

/*status and alarm records*/
#define SAD_ACQ			"acq"
#define SAD_AMP_IN_USE		"portCnt"
#define SAD_CON_ID		"conID"
/* #define SAD_DARKTIME		"darktime" */
/* #define SAD_DATALABEL       	"dataLabel" */
/* #define SAD_DCINIT		"dcInit" */
/* #define SAD_DET_ID		"detId" */
/* #define SAD_DET_TYPE		"detType" */
/* #define SAD_ELAPSED		"elapsed" */
/* #define SAD_EXPOSED		"exposed" */
/* #define SAD_EXPOSEDRQ       	"exposedRQ" */
/* #define SAD_GAIN		"gain" */
#define SAD_HEADTEMP		"detTemp"
#define HEALTH_INPUT	"healthmbbi"
/* #define SAD_DHS_HEALTH		"dhsHealth" */
#define SAD_HEARTBEAT		"heartBeat"
#define SAD_NAME		"name"
#define SAD_OBSERVE_ID		"dataLabel"
#define SAD_PREP		"prep"
#define SAD_RDOUT		"rdout"
#define SAD_RDSPEED		"rdspeed"
#define SAD_ROI			"roi"	
/* #define SAD_ROICNT		"roiCnt" */	
#define SAD_SHUTPOS		"shutPos"
#define SAD_SHUTTER_HEALTH	"shutHealth"
#define SAD_STATE		"state"
/* #define SAD_TIME_LEFT		"timeLeft" */
/* #define SAD_UTEND		"utEnd" */
/* #define SAD_UTSTART		"utStart" */
/* #define SAD_XPIXELS1		"det1MaxCol" */
/* #define SAD_XPIXELS2		"det2MaxCol" */
/* #define SAD_XPIXELS3		"det3MaxCol" */
/* #define SAD_YPIXELS1		"det1MaxRow" */
/* #define SAD_YPIXELS2		"det2MaxRow" */
/* #define SAD_YPIXELS3		"det3MaxRow" */
/* #define SAD_XBIN		"xBin" */
/* #define SAD_YBIN		"yBin" */


#define SETDHSINFO_CAD	        "setDhsInfo"
#define SETDHSINFO_CAR	        "aysC"

/*tcs records*/
#define CURRENT_FRAME		"tcs:sad:sourceATrackFrame"
#define CURRENT_WAVELENGTH	"tcs:sad:sourceAWavelength"
#define CURRENT_EQUINOX		"tcs:sad:sourceATrackEq"

#define TEST_CAD		"test"
#define TEST_CAR		"testC"	

#define UCODEDWNLD_DONE         "ucodeDwnLdDone"
#define WCS_CAR			"sysC"


/*noops*/
#define ABORT_CAD		"abortCad"
#define ABORT_CAR		"observeC"   
#define CONTINUE_CAR	        "observeC"   
#define CONTINUE_CAD	        "continueCad"
#define DATUM_CT	"datumToggle"
#define END_GUIDE_CT	"endGuideToggle"
#define END_OBSERVE_CT	"endObserveToggle"
#define	END_VERIFY_CT	"endVerifyToggle"
#define GUIDE_CT	"guideToggle"
#define OBSERVE_CAD		"observeCad"
#define OBSERVE_CAR		"observeC"  
#define PAUSE_CAD		"pauseCad"
#define PAUSE_CAR		"observeC"  
#define STOP_CAD		"stopCad"
#define STOP_CAR		"observeC"  
#define VERIFY_CT 	"verifyToggle"


/*Temperatures*/
#define SHELL_TEMP 32
#define SHIELD_ACTIVE_TEMP   33
#define  SLIT_MM2_TEMP   34
#define  FW1_MM2_TEMP   35
#define  FW2_MM2_TEMP   36
#define  FW1_HOUSING_TEMP 37  
#define  FW2_HOUSING_TEMP   38
#define  DECKER_MM2_TEMP   39
#define  PRISM_MM2_TEMP   40
#define  GRATING_MM2_TEMP   41
#define  BENCH_WFS_GIMBALS_TEMP   42
#define  SHIELD1_FLOATING_TEMP   43
#define  GROUND_A2_TEMP  45
#define  SHIELD2_FLOATING_TEMP   46
#define  THERMAL_BUS_BAR_TEMP   48
#define  G10_TRUSS1_TEMP   49
#define  G10_TRUSS2_TEMP   50
#define  G10_TRUSS3_TEMP  51
#define  ACQ_MM2_TEMP  54
#define  CAMERA_MM2_TEMP  55
#define  CAMERA_TURRET_TEMP  56
#define  FOCUS_MM2_TEMP   57
#define  DIODE2_TEMP   60
#define  RESISTOR_100K_TEMP   61
#define  V1_2_TEMP   62
#define  GROUND_B2_TEMP  63
 
#define  OFFNER_TEMP 0   
#define  COLLIMATOR_TEMP  1
#define  SLIT_MM_TEMP   2
#define  FW1_MM_TEMP  3
#define  FW2_MM_TEMP 4
#define  DECKER_MM_TEMP   5
#define  SHIELD1_TEMP   6
#define  BENCH_PRE_SLIT_TEMP   7
#define  BENCH_WFS_TEMP  9   
#define  BENCH_TEMP_POINT_TEMP   10
#define  SHIELD2_TEMP   13
#define  PRISM_MM_TEMP   14
#define  GROUND_A1_TEMP   15
#define  GRATING_MM_TEMP   16
#define  CAMERA_MM_TEMP   17
#define  ACQ_MM_TEMP   18
#define  FOCUS_MM_TEMP   19
#define  CRYO1_1S_TEMP   20
#define  CRYO1_2S_TEMP   21
#define  CRYO2_1S_TEMP   22
#define  CRYO2_2S_TEMP   23
#define  CRYO3_1S_TEMP   24
#define  CRYO3_2S_TEMP  25
#define  CRYO4_1S_TEMP  26
#define  CRYO4_2S_TEMP  27
#define  DIODE1_TEMP  28
#define  RESISTOR_1OOK_TEMP  29
#define  V1_1_TEMP  30
#define  GROUND_B1_TEMP  31
 
extern char *tempNames[NUM_TEMPS];
