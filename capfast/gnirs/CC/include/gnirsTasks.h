/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 *    FILENAME:
 *    gnirsTasks.h
 *
 *    PURPOSE:  This file contains the definitions needed for the VxWorks
 *    control tasks of the GNIRS software system.
 * 
 *INDENT-OFF*
 * $Log: gnirsTasks.h,v $
 * Revision 1.3  2013/06/06 01:54:26  gemvx
 * Checking in for fixes related to REL-1149.
 *
 * REL-1149 requires moving FW1 into a blocking position when the aquisition
 * mirror is moving out. This will inhibit, "squiggles" on the detector caused
 * by a bright star reflecting off of the aquisition mirror onto the detector.
 *
 * See: http://swgserv01.cl.gemini.edu:8080/browse/REL-1149
 *
 * tom.c
 *
 * Revision 1.2  2009/05/27 19:31:50  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 *INDENT-ON* 
 */

#ifndef GNIRSTASKS
#define GNIRSTASKS


/* Include VxWorks semaphore definitions */
#include <semLib.h>
#include <msgQLib.h>


typedef struct motorMsg
{
    int op;
    struct cadRecord* pCad;
} motorMsg;

typedef struct mechNameMsg
{
    int op;
    struct genSubRecord* pGsub; 
} mechNameMsg;

/* Declarations of semaphores used to startup each of the
   VxWorks control tasks.
*/

extern SEM_ID semAbort;
extern SEM_ID semAcqDatm;
extern SEM_ID semAcqPark;
extern SEM_ID semAcqSteps;
extern SEM_ID semAcqPos;


extern SEM_ID semCamDatm;
extern SEM_ID semCamPark;
extern SEM_ID semCamSteps;
extern SEM_ID semCamPos;
extern SEM_ID semContinue;
extern SEM_ID semCoverDatm;
extern SEM_ID semCoverPark;
extern SEM_ID semCoverSteps;
extern SEM_ID semCoverPos;
extern SEM_ID semCryo;

extern SEM_ID semDatm;
extern SEM_ID semDeckDatm;
extern SEM_ID semDeckPark;
extern SEM_ID semDeckSteps;
extern SEM_ID semDeckPos;
extern SEM_ID semDiagnose;
extern SEM_ID semDebug;

extern SEM_ID semFw1Datm;
extern SEM_ID semFw1Park;
extern SEM_ID semFw1Steps;
extern SEM_ID semFw1Pos;
extern SEM_ID semFw2Datm;
extern SEM_ID semFw2Park;
extern SEM_ID semFw2Steps;
extern SEM_ID semFw2Pos;
extern SEM_ID semFocusDatm;
extern SEM_ID semFocusPark;
extern SEM_ID semFocusSteps;
extern SEM_ID semFocusPos;

extern SEM_ID semGratDatm;
extern SEM_ID semGratPark;
extern SEM_ID semGratSteps;
extern SEM_ID semGratPos;

extern SEM_ID semInit;
extern SEM_ID semInitWcs;

extern SEM_ID semObsCtl;
extern SEM_ID semObserve;
extern SEM_ID semObsSetup;

extern SEM_ID semPark;
extern SEM_ID semPause;
extern SEM_ID semContinue;

extern SEM_ID semReboot;
extern SEM_ID semRoi;

extern SEM_ID semSetDhsInfo;
extern SEM_ID semSetWcs;
extern SEM_ID semSlitDatm;
extern SEM_ID semSlitPark;
extern SEM_ID semSlitSteps;
extern SEM_ID semSlitPos;
extern SEM_ID semStop;
extern SEM_ID semSpare1Datm;
extern SEM_ID semSpare1Park;
extern SEM_ID semSpare1Steps;
extern SEM_ID semSpare1Pos;
extern SEM_ID semSpare2Datm;
extern SEM_ID semSpare2Park;
extern SEM_ID semSpare2Steps;
extern SEM_ID semSpare2Pos;

extern SEM_ID semTest;

extern SEM_ID semXdispDatm;
extern SEM_ID semXdispPark;
extern SEM_ID semXdispSteps;
extern SEM_ID semXdispPos;



/* Function prototypes for all the VxWorks control tasks.  Ten integer
   parameter are defined because of the VxWorks function prototype for
   taskSpawn.  Only the first parameter (n1) is used.  It contains the
   address of the CAD record associated with the task.
*/


extern int testCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int rebootCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int parkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int initCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int stopCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int datumCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int healthScan( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int diagnoseCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int debugCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int fw1DatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int fw1ParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int fw1PosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int fw1StepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int fw2DatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int fw2ParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int fw2PosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int fw2StepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int acqDatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int acqParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int acqPosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int acqStepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int gratDatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int gratParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int gratPosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int gratStepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int focusDatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int focusParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int focusPosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int focusStepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int xdispDatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int xdispParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int xdispPosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int xdispStepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int camDatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int camParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int camPosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int camStepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int coverDatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int coverParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int coverPosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int coverStepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);

extern int slitDatmCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int slitParkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int slitPosCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern int slitStepsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10);
extern void cryoHead(int);
#endif
