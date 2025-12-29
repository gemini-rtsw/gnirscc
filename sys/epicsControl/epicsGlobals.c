static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: epicsGlobals.c,v 1.1 2009/06/10 15:05:11 gemvx Exp $"
};

/* global variable definitions for GMOS system. */

#include <semLib.h>
#include <stdlib.h>
#include "gnirsCC.h"
char dummy[80];
int DPdebug=0;  /* debuggig level*/
long TCS = 0; /* if true TCS is present.  This is set in code.*/

/* database prefixes*/
char *dbTop="nirs:cc:"; 
char *sadTop="nirs:sad:cc:"; 

/* epics semaphores */
SEM_ID semAbort = NULL;

SEM_ID semContinue = NULL;
SEM_ID semCryo = NULL;
SEM_ID semDebug = NULL;

SEM_ID semInit = NULL;
SEM_ID semInitWcs = NULL;

SEM_ID semObsCtl = NULL;
SEM_ID semObserve = NULL;
SEM_ID semObsSetup = NULL;

SEM_ID semPark = NULL;
SEM_ID semPause = NULL;

SEM_ID semReboot = NULL;

SEM_ID semSadWatch = NULL;
SEM_ID semSetDhsInfo = NULL;
SEM_ID semSetWcs = NULL;
SEM_ID semStop = NULL;

SEM_ID semTest = NULL;

#if 0
void setdatum()
{
    int i;
	parkedCC = 0;
	healthCC = 0;
	initCCStatus = 2;
    for (i = 0;i< NUM_MECH;i++)
    {
		mechDatumed[i] = 1;
		mechParked[i] = 1;
		mechEng[i] = i;
		strcpy(mechHealth[i],"GOOD");
		strcpy(mechState[i],"RUNNING");
    }
    
}
void printhealth()
{
 int i;
 
    for (i = 0;i< NUM_MECH;i++)
    {
	
		printf("i = %d, health = %s  state = %s , parkpos = %d\n",i,mechHealth[i],mechState[i],mechParkPos[i]);
    }
}
void unsetdatum()
{
    int i;
	healthCC = 1;
	parkedCC=1;
	initCCStatus = 0;
    for (i = 0;i< NUM_MECH;i++)
    {
		mechDatumed[i] = 0;
		mechParked[i] = 0;
		strcpy(mechHealth[i],"BAD");
/* 		strcpy(mechState[i],"INITIALIZING"); */
    }

}
#endif
