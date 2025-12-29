
#include <vxWorks.h>
#include <semLib.h>
#include <alarmString.h>
#include "epicsDefines.h"
#include <msgQLib.h>

MSG_Q_ID statusQ = NULL;
/*semaphores*/
SEM_ID semCamDatm;
SEM_ID semCamPark;
SEM_ID semCamSteps;
SEM_ID semCamPos;
SEM_ID semCoverDatm;
SEM_ID semCoverPark;
SEM_ID semCoverSteps;
SEM_ID semCoverPos;
SEM_ID semFw1Datm;
SEM_ID semFw1Park;
SEM_ID semFw1Steps;
SEM_ID semFw1Pos;
SEM_ID semFw2Datm;
SEM_ID semFw2Park;
SEM_ID semFw2Steps;
SEM_ID semFw2Pos;
SEM_ID semFocusDatm;
SEM_ID semFocusPark;
SEM_ID semFocusSteps;
SEM_ID semFocusPos;
SEM_ID semGratDatm;
SEM_ID semGratPark;
SEM_ID semGratSteps;
SEM_ID semGratPos;
SEM_ID semSlitDatm;
SEM_ID semSlitPark;
SEM_ID semSlitSteps;
SEM_ID semSlitPos;
SEM_ID semXdispDatm;
SEM_ID semXdispPark;
SEM_ID semXdispSteps;
SEM_ID semXdispPos;
SEM_ID semDatm;
SEM_ID semDiagnose;
SEM_ID semDeckDatm;
SEM_ID semDeckPark;
SEM_ID semDeckSteps;
SEM_ID semDeckPos;
SEM_ID semAcqDatm;
SEM_ID semAcqPark;
SEM_ID semAcqSteps;
SEM_ID semAcqPos;
SEM_ID semPark;
SEM_ID semSpare1Datm;
SEM_ID semSpare1Park;
SEM_ID semSpare1Steps;
SEM_ID semSpare1Pos;
SEM_ID semSpare2Datm;
SEM_ID semSpare2Park;
SEM_ID semSpare2Steps;
SEM_ID semSpare2Pos;
SEM_ID semScan;



/* Mechanism status these store the index values into the epicsStatus array above*/
/* long mechDatumed[NUM_MECH]; */
/* long mechParked[NUM_MECH]; */
/* long mechEng[NUM_MECH]; */
/* long mechHome[NUM_MECH]; */
/* long mechnLim[NUM_MECH]; */
/* long mechpLim[NUM_MECH]; */
/* long mechOT[NUM_MECH]; */
/* long mechFault[NUM_MECH]; */
/* long mechParkPos[NUM_MECH]; */
/* char mechPos[NUM_MECH][EPICS_LEN]; */
/* char mechName[NUM_MECH][EPICS_LEN]; */
/* char mechHealth[NUM_MECH][EPICS_LEN]; */
/* char mechState[NUM_MECH][EPICS_LEN]; */

/* overall mechanism info */
/* int healthCC; */
 int ccTempHealth ; 
 int ccPressureHealth ; 
/* int datumedCC; */
/* int parkedCC; */
/* int initCCStatus; */

/* Cryo Head Control and status */
/* long cryoSelectSw; */
/* long cryoOnOffSw; */
/* long cryoCpuControl; */

/* double temperatureCC[NUM_TEMPS]; */

char *tempNames[NUM_TEMPS] = 
{
	{"offner"},{"benchPreSlit"},{"activeShieldFwd"},{"slitMotor"},{"fw1Motor"},{"fw2Motor"},  /*0-5*/
	{"deckerMotor"},{"collimator"},{"benchUnderside"},{"activeShieldAft"},{"benchTempPoint"},{""}, /*6-11*/
	{""},{""},{"cryo1_1s"},{""},{"cryo2_1s"}, /*12-16*/
	{"cryo2_2s"},{"thermalBusBarS"},{"cryo3_1s"},{"cryo4_1s"},{"thermalBusBarP"}, /*17-21*/
	{"prismMotor"},{"gratingMotor"},{"cameraMotor"},{"acqMotor"}, /*22-25*/
	{"focusMotor"},{""},{""}, /*26-28*/
	{""},{""},{""},{""},{"activeShieldMid"}, /*29-33*/
	{"slitMM"},{""},{""},{"fw1Housing"},{"fw2Housing"}, /*34-38*/
	{"deckerMM"},{"prismMM"},{"gratingMM"},{"benchWFSGimbals"},{"benchWFS"}, /*39-43*/
	{"molSieve"},{""},{"shell"},{""},{"passiveShield"}, /*44-48*/
	{"g10Truss1"},{"g10Truss3"},{"g10Truss2"},{""},{""}, /*49-53*/
	{""},{"cameraMM"},{"cameraTurret"},{"focusMM"},{""},{"END"}/*54-57*/
};


/* Temperatures */
int testa = 999;
int testb = 999;
/* Pressure sensing */
double senTorrIg;
double senTorrCcg;
double senTorrTc1;
double senTorrTc2;

/* Grating info */
double gratingWavelength;
double gratingAngle;
int gratingOrder;
int gratingStep;
double gratingEnd[2];
double gratingResolution;


int focusMode;
int numStatus;
