static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: epicsGlobals.c,v 1.2 2009/05/27 19:32:24 fkraemer Exp $"
};
#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wdLib.h>
#include "gnDCADefs.h"
#undef MAIN
#include "gnDCAVars.h"
#include "saver.h"


/* tcs alive boolean*/
long TCS;

/* epicsCntrlSrc*/
 int epdebug = 0;  

/* utilSource/epicstp:wfire */
int wfiredebug = 0;
int hkDelay = 6;

/* utilSource/socket*/
int socketdebug = 0;

/* utilSource/ucdl:vxldnet*/
int LinkId;

/* semaphores*/

SEM_ID semArSetup = NULL;
SEM_ID semObsSetup = NULL;
SEM_ID semDcaReady = NULL;
SEM_ID semFrameReady = NULL;
SEM_ID semDrRoiSet = NULL;
SEM_ID semTest = NULL;
SEM_ID semInit = NULL;
SEM_ID semSetWcs = NULL;
SEM_ID semSetDhsInfo = NULL;
SEM_ID semSetDhsConnect = NULL;
SEM_ID semObserve = NULL;
SEM_ID semPark = NULL;
SEM_ID semReboot = NULL;
SEM_ID semAbort = NULL;
SEM_ID semStop = NULL;
SEM_ID semDhsConnect = NULL;

SEM_ID semInitWcs = NULL;


char tldFile[256];
char cmdFile[256];

