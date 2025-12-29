static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: spare1Cad.c,v 1.2 2013/06/06 01:54:28 gemvx Exp $"
};
typedef struct testStruct
{
    long l;
    double d;
    char s[80];
}testStruct;
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	spare1Cad.c
 *
 * DESCRIPTION 
 * 	spare1Cad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	spare1DatmCad
 *	spare1ParkCad
 *	spare1StepsCad
 *	spare1PosCad
 *	spare1Ctrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: spare1Cad.c,v $
 * Revision 1.2  2013/06/06 01:54:28  gemvx
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
 * Revision 1.1  2009/06/10 15:05:14  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>

/* EPICS specific include files */
/* #include "gnirsCcDefs.h" */
#include "gnirsTasks.h"
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsCC.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>

/*Global variables*/
static MSG_Q_ID spare1MotorQ;
int spare1Id;

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* Forward declarations of the functions in this file */
long spare1DatmCad( struct cadRecord* pCad);

long spare1ParkCad( struct cadRecord* pCad);

long spare1StepsCad( struct cadRecord* pCad);

long spare1PosCad( struct cadRecord* pCad);

int spare1Ctrl( int n1, int n2, int n3, int n4, int n5, int n6, 
	     int n7, int n8, int n9, int n10 );
int val[10];

long spare1DatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
	/*create msg q*/
	spare1MotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
	if(spare1MotorQ == NULL)
	{
	    status = CAD_REJECT;
	    strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
	}
	/*spawn task*/
	if(status == CAD_ACCEPT)
	    spare1Id = taskSpawn("tSpare1Ctrl",50,VX_FP_TASK,4000,spare1Ctrl, 0,0,0,0,0,0,0,0,0,0);
	if(spare1Id == ERROR)
	{
	    status = CAD_REJECT;
	    strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
	}
	if(status = CAD_ACCEPT)
	    done = 1;
    }
    
    /*valh is connected to car record*/
    *(long *)pCad->valh = carVal;
    return status;
}
long spare1ParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
    
    /*valh is connected to car record*/
    *(long *)pCad->valh = carVal;
    return status;
}
long spare1PosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
#if 0
    char *env;
    char line[80];
    char fileName[80];
    FILE *fp;
    /**/
    env = getenv ("INITDIR");
    sprintf(fileName,"%sspare1.cad",env);
    printf("\n%s\n",fileName);
    if((fp = fopen(fileName, "r")) != NULL)
    {
	/*open file and put values in pCad*/
	while(fgets(line,79,fp) != NULL)
	{
	    setCad(pCad,line); 

	}
	fclose(fp);
    }
    else
    {
	printf("spare1PosCadInit: failed to open initialization file\n");
	status = CAD_REJECT;
    }
#endif
    /*valh is connected to car record*/
    *(long *)pCad->valh = carVal;
    return status;
}
long spare1StepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
    
    /*valh is connected to car record*/
    *(long *)pCad->valh = carVal;
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	spare1Ctrl
 *
 * INVOCATION:
 *	Started as seperate task in initTasks.
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	Set of 10 integer variables, interpreted as variety of 
 *	parameters.
 *
 * FUNCTION VALUE:
 *	never returns
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *	Cad record releases semaphore which allows spare1DatmCtrl to run.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	None
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *
 *-
 */

int spare1Ctrl( int n1, int n2, int n3, int n4, int n5, int n6, 
	     int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = OK;
    long simMode;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;

  
    strncpy (rMsg,"Error in spare1Ctrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
	LOG_MSG(DEBUG2_MSG, "Task tspare1Ctrl sleeping...\n");
	
	if( msgQReceive( spare1MotorQ, (char *)&msg, sizeof(motorMsg ), 
			 WAIT_FOREVER ) == ERROR ) 
	{
	    printf("error in spare1Ctrl msgReceive\n");
	    continue;
	}

	pCad = msg.pCad;

	    
	  
	/* Find out what execution mode we are in	*/
	printf("dbTop = %s, name = %s\n",dbTop,INIT_CAD);
	getDbInfoT(dbTop, INIT_CAD ".VALA",dummy,DBF_LONG,&simMode);
	printf("simmode = %d\n",simMode);

	switch (simMode)
	{
	  case SIM_NONE:
	  case SIM_VSM:
	  case SIM_FULL:
	  case SIM_FAST:
	    switch (msg.op)
	    {
	      case DATUM:
		printf("spare1 datum;");
		if((status = moveMech(SPARE1,rMsg)) == VME_OK)
		{
		    /*set datumed sad record*/
		}
		break;
	      case PARK:
		printf("spare1 park");
		if((status = moveMech(SPARE1,rMsg)) == VME_OK)
		{
		    /*set park sad record*/
		}
		break;
	      case POS:
		printf("spare1 pos");
		if((status = moveMech(SPARE1,rMsg)) == VME_OK)
		{
		    
		}
		break;
	      case STEPS:
		printf("spare1 steps");
		if((status = moveMech(SPARE1,rMsg)) == VME_OK)
		{
		    
		}
		break;
	      default:
		printf("unknown operation in spare1Ctrl\n");
	
		break;
	    }
	 

	  
	    break;
	  default:		/* Don't see any action	*/
	    LOG_MSG(DEBUG2_MSG, "Simulation is irrelevant!");
	    break;
	}
	if(status == OK)
	{
	    carVal = CAR_IDLE;
	    if(setCar(SPARE1_CAR,carVal,OK,"",dummy) != OK)
		DPRINT(DPdebug,ERROR_MSG,dummy);
	}
	else
	{
	    carVal = CAR_ERROR;
	    if(setCar(SPARE1_CAR,carVal,status,rMsg,dummy) != OK)
		DPRINT(DPdebug,ERROR_MSG,dummy);
	}

	printf("spare1Ctrl\n");

    }
    
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	spare1DatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = spare1DatmCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "spare1Datm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when spare1DatmCad record is processed by EPICS.  It 
 *	implements the "park" command, which places the system in a 
 *	safe shutdown status.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized, with no parameters.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */

long spare1DatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1DatmCad - MARK directive");
	    strcpy(MESSAGE,"");
	break;

      case CAD_PRESET:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1DatmCad - PRESET directive");
#if 0
	if(datumMech(SPARE1)!= VME_OK)
	{
	    sprintf(MESSAGE,"spare1Datmcad: Error in preset");
	    status = CAD_REJECT;
	}
#endif
      case CAD_CLEAR:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1DatmCad - CLEAR directive");
	    strcpy(MESSAGE,"");
	break;

      case CAD_START:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1DatmCad - START directive");
	/*copy inputs to outputs*/


	/*set car to busy (ctrl task will set back to idle)*/
	carVal = CAR_BUSY;
	msg.op = DATUM;
	msg.pCad = pCad;
	
	if(msgQSend(spare1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
		    MSG_PRI_NORMAL) != OK)
	{
	    printf("msgQSend error in spare1DatmCad\n");
	}

	/*valh is connected to car record*/
	*(long *)pCad->valh = carVal;
	
	break;

      case CAD_STOP:	/* Can't stop	*/
	LOG_MSG(DEBUG2_MSG,
		 "spare1DatmCad - STOP directive");
	/*check if car is busy and then call  abortMech*/
	
	strncpy(MESSAGE,"spare1DatmCad: Can't stop",MAX_STRING_SIZE - 1);
	status = CAD_ACCEPT;
	break;

      default:	/* Unknown directive	*/
	strncpy(MESSAGE, "spare1DatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;
    } 
   
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	spare1ParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = spare1ParkCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "spare1Park" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when spare1ParkCad record is processed by EPICS.  It 
 *	implements the "park" command, which places the system in a 
 *	safe shutdown status.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized, with no parameters.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */
long spare1ParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1ParkCad - MARK directive");
	    strcpy(MESSAGE,"");
	break;

      case CAD_PRESET:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1ParkCad - PRESET directive");
	if(parkMech(SPARE1)!= VME_OK)
	{
	    sprintf(MESSAGE,"spare1ParkCad: Error in preset");
	    status = CAD_REJECT;
	}
      case CAD_CLEAR:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1ParkCad - CLEAR directive");
	    strcpy(MESSAGE,"");
	break;

      case CAD_START:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1ParkCad - START directive");


	/*set car to busy (ctrl task will set back to idle)*/
	carVal = CAR_BUSY;
	
	
	DPRINT(DPdebug,DEBUG2_MSG,"Waking park\n");

	msg.op = PARK;
	msg.pCad = pCad;
	
	if(msgQSend(spare1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
		    MSG_PRI_NORMAL) != OK)
	{
	    printf("msgQSend error in spare1DatmCad\n");
	}

	
	    *(long *)pCad->valh = carVal;
	
	break;

      case CAD_STOP:	/* Can't stop	*/
	LOG_MSG(DEBUG2_MSG,
		 "spare1ParkCad - STOP directive");
		
	strncpy(MESSAGE,"spare1ParkCad: Can't stop",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;

      default:	/* Unknown directive	*/
	strncpy(MESSAGE, "spare1ParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;
    } 
   
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	spare1StepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = spare1StepsCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "spare1Steps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when spare1StepsCad record is processed by EPICS.  It 
 *	implements the "park" command, which places the system in a 
 *	safe shutdown status.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized, with no parameters.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */
long spare1StepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1StepsCad - MARK directive");
	    strcpy(MESSAGE,"");
	break;

      case CAD_PRESET:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1StepsCad - PRESET directive");

	/* check datumed before*/
	if(!atoi(pCad->b))
	    LOG_MSG(ERROR_MSG, 
		     "spare1StepsCad - Spare1 not Datumed");
	if(setPositionReq(SPARE1,atoi(pCad->a))!= VME_OK)
	{
	    sprintf(MESSAGE,"spare1StepsCad: Error in preset");
	    status = CAD_REJECT;
	}
      case CAD_CLEAR:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1StepsCad - CLEAR directive");
	    strcpy(MESSAGE,"");
	break;

      case CAD_START:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1StepsCad - START directive");
	/*copy inputs to outputs*/

	/*set car to busy (ctrl task will set back to idle)*/
	carVal = CAR_BUSY;
	
	
	DPRINT(DPdebug,DEBUG2_MSG,"Waking park\n");
	msg.op = STEPS;
	msg.pCad = pCad;
	
	if(msgQSend(spare1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
		    MSG_PRI_NORMAL) != OK)
	{
	    printf("msgQSend error in spare1DatmCad\n");
	}
	
	
	    *(long *)pCad->valh = carVal;
	break;

      case CAD_STOP:	/* Can't stop	*/
	LOG_MSG(DEBUG2_MSG,
		 "spare1StepsCad - STOP directive");
		
	strncpy(MESSAGE,"spare1StepsCad: Can't stop",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;

      default:	/* Unknown directive	*/
	strncpy(MESSAGE, "spare1StepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;
    } 
   
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	spare1PosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = spare1PosCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *        pCad->a  = position
 *        pCad->b  = Datumed sad record value
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "spare1Pos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when spare1PosCad record is processed by EPICS.  It 
 *	implements the "park" command, which places the system in a 
 *	safe shutdown status.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized, with no parameters.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */
long spare1PosCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1PosCad - MARK directive");
	    strcpy(MESSAGE,"");
	break;

      case CAD_PRESET:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1PosCad - PRESET directive");
	if(!atoi(pCad->b))
	{
	    sprintf(MESSAGE,"spare1PosCad - Spare1 not Datumed");
	    LOG_MSG(ERROR_MSG, MESSAGE);
	}
	
	if(valid(SPARE1,pCad->a)!= VME_OK)
	{
	    sprintf(MESSAGE,"spare1PosCad: Error in preset");
	    status = CAD_REJECT;
	}
      case CAD_CLEAR:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1PosCad - CLEAR directive");
	    strcpy(MESSAGE,"");
	break;

      case CAD_START:	/* No action required	*/
	LOG_MSG(DEBUG2_MSG, 
		 "spare1PosCad - START directive");
	/*copy inputs to outputs*/

	/*set car to busy (ctrl task will set back to idle)*/
	  
	carVal = CAR_BUSY; 
	
	
	DPRINT(DPdebug,DEBUG2_MSG,"Waking park\n");
	msg.op = POS;
	msg.pCad = pCad;
	
	if(msgQSend(spare1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
		    MSG_PRI_NORMAL) != OK)
	{
	    sprintf(MESSAGE,"spare1PosCad: Error in msgQSend ");
	    status = CAD_REJECT;
	}
	
	    *(long *)pCad->valh = carVal;
	
	break;

      case CAD_STOP:	/* Can't stop	*/
	LOG_MSG(DEBUG2_MSG,
		 "spare1PosCad - STOP directive");
		
	strncpy(MESSAGE,"spare1PosCad: Can't stop",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;

      default:	/* Unknown directive	*/
	strncpy(MESSAGE, "spare1PosCad: Unrecognized directive",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;
    } 
   
    return status;
}
