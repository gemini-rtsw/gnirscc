/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 *    FILENAME:
 *    naacTasks.h
 *
 *    PURPOSE:  This file contains the definitions needed for the VxWorks
 *    control tasks of the GNAAC software system.
 * 
 *INDENT-OFF*
 * $Log: naacTasks.h,v $
 * Revision 1.2  2009/05/27 19:32:28  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:19  buchholz
 * Imported gnaacSrc into CVS
 *
 *INDENT-ON* 
 */

#ifndef NAACTASKS
#define NAACTASKS


/* Include VxWorks semaphore definitions */
#include <semLib.h>


/* NAAC status definitions which are the values taken on by the
   mbbi reocrds used for command sequencing.
*/
#define NAAC_DONE 0
#define NAAC_UNKNOWN 1
#define NAAC_BUSY 2
#define NAAC_ERROR 3

/* Declarations of semaphores used to startup each of the
   VxWorks control tasks.
*/
extern SEM_ID semArSetup;
extern SEM_ID semObsSetup;
extern SEM_ID semDrRoiSet;
extern SEM_ID semTest;
extern SEM_ID semInit;
extern SEM_ID semSetWcs;
extern SEM_ID semSetDhsInfo;
extern SEM_ID semDhsConnect;
extern SEM_ID semObserve;
extern SEM_ID semPark;
extern SEM_ID semReboot;
extern SEM_ID semAbort;
extern SEM_ID semStop;
extern SEM_ID semDcaReady;
extern SEM_ID semFrameReady;

/* Function prototypes for all the VxWorks control tasks.  Tem integer
   parameter are defined because of the VxWorks function prototype for
   taskSpawn.  Only the first parameter (n1) is used.  It contains the
   address of the CAD record associated with the task.
*/
extern int doArSetup( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doObsSetup( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doDrRoiSet( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doTest( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doInit( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doSetWcs( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doSetDhsInfo( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doDhsConnect( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
                  int n8, int n9, int n10);
extern int doObserve( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doPark( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doReboot( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doAbort( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
extern int doStop( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);


#endif
