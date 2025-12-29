static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: healthScan.c,v 1.1 2009/06/10 15:05:12 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * healthScan.c
 *
 * DESCRIPTION 
 *  
 * 
 * FUNCTION NAME(S)
 * healthScan -   Monitors various health alarm and car values and sets health 
 *                 status records in EPICS.
 *   
 * DEPENDENCIES
 * 
 *
 *INDENT-OFF*
 * $Log: healthScan.c,v $
 * Revision 1.1  2009/06/10 15:05:12  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>

/* Controller specific include files */
#include "car.h"
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsCC.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include "status.h"

extern char * alarmSeverityString[];

/*function prototypes*/
long getHealth (char *name, long *health,char *errMess);
long setHealth (char *name, long health,char *errMess);
long getAlarm (char *name, long *alarm, char *errMess);

/*
 *+
 * FUNCTION NAME:
 *    healthScan
 * INVOCATION:
 *             spawned from initTasks routine.
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None used
 *
 * FUNCTION VALUE:
 * long status
 *
 * PURPOSE:
 *    Monitors various health alarm and car values and sets health status records
 *     in EPICS.
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 *
 *-
 */
long healthScan ( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
				int n8, int n9, int n10)
{
	char name[80];
	int i;
	long status = OK;
	long ret = OK;
	char space[200];
	char errMess[80];
	long initC,cryoC,testC;
	long health = HEALTH_GOOD;
	long motorHealth = HEALTH_GOOD;
	long tempAlarm = NO_ALARM;
	long pressureAlarm1 = NO_ALARM;
	long pressureAlarm2 = NO_ALARM;
	long pressureAlarm = NO_ALARM;
	long tempHealth = HEALTH_GOOD;
	long pressureHealth = HEALTH_GOOD;
	long sadHealth = HEALTH_GOOD;
	long sysHealth = HEALTH_GOOD;
	long ccTopHealth = HEALTH_GOOD;

	while (ret == OK)
	{
		/* motor overall health
		 * acqHealth, cameraHealth, coverHealth, deckerHealth, gratingHealth,
		 * xdispHealth, focusHealth, slitHealth, fw1Health, fw2Health
		 */
		motorHealth = HEALTH_GOOD;
	
		sprintf(name,"%s%s",sadTop,ACQ_HEALTH);
		getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,CAMERA_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,COVER_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,DECKER_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,GRATING_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,XDISP_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,FOCUS_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,SLIT_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,FW1_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
		sprintf(name,"%s%s",sadTop,FW2_HEALTH);
		if(health != HEALTH_BAD) getHealth (name, &motorHealth,errMess);
   

		/*temperature health
		 * about 60 temperatures
		 */

		i = 0;
		tempAlarm = NO_ALARM;
		while ((status == OK) &&
			   (tempAlarm != MAJOR_ALARM) &&
			   (strcmp (tempNames[i] ,"END") != 0))
		{
			if(strlen(tempNames[i]))
			{
				sprintf (space,"healthScan: %s,%d\n",tempNames[i],i);
/* 				LOG_MSG(DEBUG2_MSG,space); */
				status = getAlarm(tempNames[i], &tempAlarm, errMess);
			}
		    i++;
			
		}

		if(tempAlarm == MAJOR_ALARM)
			tempHealth = HEALTH_BAD;
		else if (tempAlarm == MINOR_ALARM)
			tempHealth = HEALTH_WARNING;
		else if (tempAlarm == NO_ALARM)
			tempHealth = HEALTH_GOOD;

	   /* pressure Health
		* 
		*/
		pressureAlarm = NO_ALARM;
		status = getAlarm(PRESSURE_IG, &pressureAlarm, errMess);
		if(pressureAlarm == MAJOR_ALARM)
		{
			pressureHealth = HEALTH_BAD;
		}
		else if (pressureAlarm == MINOR_ALARM)
		{
			pressureHealth = HEALTH_WARNING;
		}
		else if (pressureAlarm == NO_ALARM)
		{
			status = getAlarm(PRESSURE_TC1, &pressureAlarm1, errMess);
			status = getAlarm(PRESSURE_TC2, &pressureAlarm2, errMess);
			if((pressureAlarm1 == NO_ALARM)||(pressureAlarm2 == NO_ALARM))
				pressureHealth = HEALTH_GOOD;
			else if((pressureAlarm1 == MINOR_ALARM)&&(pressureAlarm1 == MINOR_ALARM))
				pressureHealth = HEALTH_WARNING;
			else if(((pressureAlarm1 == MINOR_ALARM)&&(pressureAlarm1 == MAJOR_ALARM))||((pressureAlarm1 == MAJOR_ALARM)&&(pressureAlarm1 == MINOR_ALARM)))
				pressureHealth = HEALTH_BAD;
		}
		
		/*sad Health
		 *pressureHealth, temperature Health
		 */
		if((pressureHealth == HEALTH_BAD) || (tempHealth == HEALTH_BAD))
			sadHealth = HEALTH_BAD;
		else if((pressureHealth == HEALTH_WARNING) || (tempHealth == HEALTH_WARNING))
			sadHealth = HEALTH_WARNING;
		else 
			sadHealth = HEALTH_GOOD;

		/* sys health
		 * initC, cryoC, testC
		 */
		status = getDbInfoT(dbTop,INIT_CAR,errMess,DBF_LONG,&initC);
		status = getDbInfoT(dbTop,CRYO_CAR,errMess,DBF_LONG,&cryoC);
		status = getDbInfoT(dbTop,TEST_CAR,errMess,DBF_LONG,&testC);
		if ((initC == CAR_ERROR) 
			|| (cryoC == CAR_ERROR)
			|| (testC == CAR_ERROR))
			sysHealth = HEALTH_BAD;
		else if ((initC == CAR_BUSY ) 
				 || (cryoC == CAR_BUSY) 
				 || (testC == CAR_BUSY))
			sysHealth = HEALTH_WARNING;
		else 
			sysHealth = HEALTH_GOOD;
	
		/*ccTopHealth
		 * sysHealth, pressureHealth, temperature Health, motorHealth, applyC
		 */
		if (( sysHealth == HEALTH_BAD)
			|| (pressureHealth == HEALTH_BAD)
			|| (tempHealth == HEALTH_BAD)
			|| (motorHealth == HEALTH_BAD))
			ccTopHealth = HEALTH_BAD;
		else if (( sysHealth == HEALTH_WARNING) 
				 || (pressureHealth == HEALTH_WARNING)
				 || (tempHealth == HEALTH_WARNING)
				 || (motorHealth == HEALTH_WARNING))
			ccTopHealth = HEALTH_WARNING;
		else 
			ccTopHealth = HEALTH_GOOD;
		
		
		/* set health values*/
		setHealth(MOTOR_HEALTH,motorHealth,errMess);
		setHealth(TEMPERATURE_HEALTH,tempHealth,errMess);
		setHealth(PRESSURE_HEALTH,pressureHealth,errMess);	
		setHealth(SAD_HEALTH,sadHealth,errMess);
		setHealth(SYS_HEALTH,sysHealth,errMess);
		setHealth(CCTOP_HEALTH,ccTopHealth,errMess);

		/* convert from EPICS health to CC health*/
		if(pressureHealth == HEALTH_BAD)
		  ccPressureHealth = BAD;
		if(pressureHealth == HEALTH_WARNING)
		  ccPressureHealth = WARNING;
		if(pressureHealth == HEALTH_GOOD)
		  ccPressureHealth = GOOD;
		
		if(tempHealth == HEALTH_BAD)
		  ccTempHealth = BAD;
		if(tempHealth == HEALTH_WARNING)
		  ccTempHealth = WARNING;
		if(tempHealth == HEALTH_GOOD)
		  ccTempHealth = GOOD;

		sleep(5,0);

	}
	
	return ret;
}

/*
 *+
 * FUNCTION NAME:
 *      getHealth
 * INVOCATION:
 *      status = getHealth(char *name, long *health, char *errMess);
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *      >   char *name
 *      !   long *health
 *      <   char *errMess
 *
 * FUNCTION VALUE:
 *     ERROR or OK
 * 
 *
 * PURPOSE:
 *    Read health value from EPICS variables
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 *
 *-
 */
long getHealth (char *name, long *health,char *errMess)
{
	long status = OK;
	static char healthStr[80];

	if(*health == HEALTH_BAD)
		return OK;
	status = getDbInfo (name,errMess,DBF_STRING,healthStr);
	if(strcmp(healthStr,BAD_HEALTH)==0)
	{
		*health = HEALTH_BAD;
	}
	else if(strcmp(healthStr,WARNING_HEALTH) == 0)
	{ 
		*health = HEALTH_WARNING;
	}

	return status;
}

/*
 *+
 * FUNCTION NAME:
 *      setHealth
 * INVOCATION:
 *      status = setHealth(char *name, long *health, char *errMess);
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *      >   char *name
 *      !   long *health
 *      <   char *errMess
 *
 * FUNCTION VALUE:
 *     ERROR or OK
 * 
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *   set a health value in the status database
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 *
 *-
 */
long setHealth (char *name, long health,char *errMess)
{
	long status = OK;
	if(health == HEALTH_GOOD)
		status = putDbInfoT (sadTop,name,errMess,DBF_STRING,GOOD_HEALTH);
	else if(health == HEALTH_WARNING)
		status = putDbInfoT (sadTop,name,errMess,DBF_STRING,WARNING_HEALTH);
	else
		status = putDbInfoT (sadTop,name,errMess,DBF_STRING,BAD_HEALTH);

	return status;
}

/*
 *+
 * FUNCTION NAME:
 *      getAlarm
 * INVOCATION:
 *      status = getAlarm(char *name, long *health, char *errMess);
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *      >   char *name
 *      !   long *alarm
 *      <   char *errMess
 *
 * FUNCTION VALUE:
 *     ERROR or OK
 * 
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 *
 *-
 */
long getAlarm (char *name, long *alarm, char *errMess)
{
	long status = OK;
	static char epicsName[80];
	static char alarmString[80];

	if(*alarm == MAJOR_ALARM)
		return OK;

	if((strlen(name) + strlen(sadTop))<35)
		sprintf(epicsName,"%s%s.SEVR",sadTop,name);

	status = getDbInfo (epicsName,errMess,DBF_STRING,alarmString);
	if(strcmp(alarmString,alarmSeverityString[MAJOR_ALARM])==0)
	{
		*alarm = MAJOR_ALARM;
	}
	else if(strcmp(alarmString,alarmSeverityString[MINOR_ALARM]) == 0)
	{ 
		*alarm = MINOR_ALARM;
	}
	return status;
}
