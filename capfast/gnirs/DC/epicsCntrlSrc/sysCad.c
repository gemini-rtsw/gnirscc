static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: sysCad.c,v 1.3 2010/08/19 04:00:19 mrippa Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * sysCad.c
 *
 * DESCRIPTION
 * This file contains the source for all the functions used by the
 * NAAC system CAD records. These functions are used to validate the 
 * arguments given to the CAD record.  Each function checks that the 
 * command is acceptable and returns a status. A message is supplied 
 * for rejected commands or error conditions.  The status and message 
 * is subsequently written to the VAL and MESS fields of the CAD record 
 * by the record support routines. 
 * 
 * FUNCTION NAME(S)
 * debugProc       - select debug mode 
 * initProc        - Initialise
 * rebootProc      - Reboot IOC's
 * setWcsProc      - Set world coordinate system matrix to be associated with
 *		     the current observation
 * testProc        - Self test
 * setDhsInfoProc  - Sets the value of the DHS parameters to be used
 *		     for Data display, quick-look and archiving
 *   
 * DEPENDENCIES
 * The names of the subroutines in this file should be identical to those
 * declared in the SNAM field of each CAD record. If a change is made to
 * the name of a subroutine, that change should be reflected in the SNAM
 * field, and vice versa.
 *
 * The ordering of the arguments within each CAD record (A, B, C...) is
 * defined in the description of the interface between the CAD database
 * and its clients. Changes to that interface should be reflected in this
 * file.
 *
 * The input arguments for each CAD record (A, B, C...) are all strings
 * and must be converted to their appropriate data types before validation.
 * However, the data type of each output argument (VALA, VALB, VALC...)
 * is determined by the (FTVA, FTVB, FTVC....) fields in the CAD record.
 * The data types assumed here must match those declared in the CAD
 * record properties.
 *
 *INDENT-OFF*
 * $Log: sysCad.c,v $
 * Revision 1.3  2010/08/19 04:00:19  mrippa
 * Removes unused setDHSInfoProc() routine.
 *
 * Revision 1.2  2009/05/27 19:32:22  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:50  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:13:56  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:28  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */


/* int dqRebootRPC( ); */
#include <taskLib.h>
#include <epCommon.h>
#include <naacTasks.h>
#include <car.h>
extern char *dbTop;
int dqRebootRPC( );
static char dummy[80];
/*
 *+
 * FUNCTION NAME:
 * initProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = initProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "init" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the init CAD record is processed.
 * init is the command for reinitializing the subsystem.  The command has 
 * has one argument, the simulation mode, which is restricted to one of the
 * following:  NONE, VSM, FAST or FULL.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */

long initProc( struct cadRecord *pCad ) 
{
    long retVal;
   long status ;         /* return status */
  static  int sim;

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "init - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "init - PRESET directive.");
	 cicsLogString( 3, "Requested simulation mode =", pCad->a);
	 if(strcmp(pCad->a,"NONE") == 0)
	 {
	     sim = SIM_NONE;
	 }
	 else if(strcmp(pCad->a,"VSM") == 0)
	 {
	     sim = SIM_VSM;
	 }
	 else if(strcmp(pCad->a,"FAST") == 0)
	 {
	     sim = SIM_FAST;
	 }
	 else if(strcmp(pCad->a,"FULL") == 0)
	 {
	     sim = SIM_FULL;
	 }
	 else
	 {
	    strcpy( pCad->mess, "Unrecognized simulation mode" );
	    status = CAD_REJECT;
	 }

         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "init - CLEAR directive.");
         break;

      /* CAD START directive detected. Check to see if an observation
	 is in progress.  If so, reject the command.  Otherwise, set
         the CAR to BUSY and startup the init control task.
      */
      case CAD_START:
         cicsLogMessage( 2, "init - START directive.");	
	 if(status == CAD_ACCEPT)
	     retVal = assignVal(pCad->ftva,&sim,pCad->vala,pCad->mess);
	 cicsLogLong( 3, "CAD output =", *(long *)pCad->vala);
	 if(carStatus(pCad->b) == CAR_IDLE)
	 {	 
		 cicsLogMessage( 3, "set car to busy");
	     retVal = setCar(INIT_CAR,CAR_BUSY,OK,"",dummy);
		 cicsLogMessage( 3, "done");
	    
	     if(retVal == OK)
		 semGive(semInit);
	     else
		 status = CAD_REJECT;
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"init - cannot be done during observe");
	     cicsLogMessage(1, pCad->mess);
	 }
         break;

      /* CAD STOP directive detected. It is not possible to stop initialization
         once it has started, so reject the directive. 
      */
      case CAD_STOP:
         cicsLogMessage( 1, "init - STOP directive. Cannot be stopped.");
         strcpy( pCad->mess, "Cannot be stopped" );
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}




/*
 *+
 * FUNCTION NAME:
 * debugProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = debugProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "debug" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the debug CAD record is processed.
 * debug is the command for setting the debug level.
 * The command has  one argument: 
 *   	debug level - which is restricted to one of the following:  
 *			NONE, MIN, or FULL.
 *
 * EXTERNAL VARIABLES:
 * debugLevel - in the CICS logging routines
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * The CAR reocrd associated with is being set correctly upon a START directive.
 * The changes in CAR state can be observed using camonitor.  However, this
 * is not reflected on the DM screen.  I do not have an explanation for this.
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */

long debugProc( struct cadRecord *pCad ) 
{
   long status;         /* return status */
   static int dbg;

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "debug - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "debug - PRESET directive.");
	 cicsLogString( 3, "Requested Debug Level =", pCad->a);
	 if(strcmp(pCad->a,"NOLOG") == 0)
	 {
	    dbg = DBG_NOLOG;
	    printf("debugproc nolog\n");
	 }
	 else if(strcmp(pCad->a,"NONE") == 0)
	 {
	     dbg = DBG_NONE;
	    printf("debugproc none\n");
	 }
	 else if(strcmp(pCad->a,"MIN") == 0)
	 {
	     dbg = DBG_MIN;
	    printf("debugproc min\n");
	 }
	 else if(strcmp(pCad->a,"FULL") == 0)
	 {
	     dbg = DBG_FULL;
	    printf("debugproc full\n");
	 }
	 else
	 {
	    strcpy( pCad->mess, "Unrecognized debug level" );
	    status = CAD_REJECT;
	 }
	
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "debug - CLEAR directive.");
         break;

      /* CAD START directive detected. Set CAR to BUSY, call function to set the debug
	 level and set CAR back to IDLE. 
      */
      case CAD_START:
         cicsLogMessage( 2, "debug - START directive."); 
		 if(status == CAD_ACCEPT)
			 status = assignVal(pCad->ftva,&dbg,pCad->vala,pCad->mess);
		 /*  setCar(TEST_CAR,CAR_BUSY,OK,"",dummy); */
		 processRec(dbTop,"testCSeq");
		 cicsSetDebug(*(long *) pCad->vala);
		 /*  setCar(TEST_CAR,CAR_IDLE,OK,"",dummy); */
		 
         break;

      /* CAD STOP directive detected. 
      */
      case CAD_STOP:
         cicsLogMessage( 1, "debug - STOP directive. Cannot be stopped.");
         status = CAD_REJECT;
         strcpy( pCad->mess, "Cannot be stopped" );
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}

/*
 *+
 * FUNCTION NAME:
 *	wfsBusReset
 *
 * INVOCATION:
 *	wfsBusReset();
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	none
 *
 * FUNCTION VALUE:
 *	void
 *
 * PURPOSE:
 *	Reboot the computer
 *
 * DESCRIPTION:
 *	Reboot vme by writing to a register
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *	none
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 *
 *-
 */


#define BIT_SET(p, d)      { __typeof__ (* (p)) __temp = (* (p));        \
                             * (p) = __temp | (d); }

#define BUS_RESET_BIT_MV167   0x01800000
#define BUS_RESET_REG_MV167   0xfff40060 
#define HW_REG32 volatile unsigned long



void wfsBusReset(void)
{

    printf("wfsBusReset: BUS RESET - SYSTEM WILL REBOOT.\n");

    /* Brief pause to allow message to flush..   */
  sleep(1,0);

    /* ..then waggle the hardware bits            */
    BIT_SET((HW_REG32 *) BUS_RESET_REG_MV167, BUS_RESET_BIT_MV167);
}
/*
 *+
 * FUNCTION NAME:
 *	rebootNow
 *
 * INVOCATION:
 *	status = rebootNow();
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	none
 * FUNCTION VALUE:
 *	status 
 *
 * PURPOSE:
 *	reboot the epics and coadder backplanes
 *
 * DESCRIPTION:
 *	Uses rpc to tell the coadder to reboot and then spawns a task that reboots
 *		itself
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *	Each task must call dqRpcInit once before it calls an rpc procedure.  Currently
 *		dqRpcInit is called from the doReboot task.  If any other task wishes to
 *		call this or any other procedure(dqRpcInit), They myst call this function 
 *		before running the rpc routine.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 *
 *-
 */

int rebootNow()
{

     /* tell coadder to reboot*/ 

    dqRebootRPC();
    
    taskSpawn("suicide", 20, VX_NO_STACK_FILL, 2000, (FUNCPTR) wfsBusReset, 
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 

    return OK;
}
/*
 *+
 * FUNCTION NAME:
 * rebootProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = rebootProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "reboot" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the reboot CAD record is processed.
 * reboot is the co0mmand to reboot the IOC's.
 * The command has  has no arguments.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */
long rebootProc( struct cadRecord *pCad ) 
{
   long status;         /* return status */
   char mess[MAX_STRING_SIZE];

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "reboot - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "reboot - PRESET directive.");
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "reboot - CLEAR directive.");
         break;

      /* CAD START directive detected. Check to see if an observation
	 is in progresss.  If so, reject the command.  Otherwise,
	 allow a reboot. (Eventually the reboot command
         should be uncommented.  For now,I put in a logMessage and
         a sleep.)
      */
      case CAD_START:
         cicsLogMessage( 2, "reboot - START directive.");
	 if(carStatus(pCad->a) == CAR_IDLE)
	 {
	     sprintf(mess,"%s will reboot ",dbTop);
	     cicsLogMessage(1,mess);
	     semGive(semReboot);
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"reboot - cannot be done during observe");
	     cicsLogMessage(1, pCad->mess);
	 }
         break;

      /* CAD STOP directive detected.       */
      case CAD_STOP:
         cicsLogMessage( 1, "reboot - STOP directive. Cannot be stopped.");
         strcpy( pCad->mess, "Cannot be stopped" );
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}



/*
 *+
 * FUNCTION NAME:
 * testProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = testProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "test" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the test CAD record is processed.
 * test it she commmand to perform a self test.
 *  The command has  has no arguments.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */

long testProc( struct cadRecord *pCad ) 
{
   long status;         /* return status */

   /* initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "test - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "test - PRESET directive.");
         break;

      /* CAD CLEAR directive detected. Do nothing. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "test - CLEAR directive.");
         break;

      /* CAD START directive detected. Reject the command if an observation is
	 in progress.  Otherwise, set the CAR reocrd to BUSY and invoke the
	 control task.
      */
      case CAD_START:
         cicsLogMessage( 2, "test - START directive.");
	 if(carStatus(pCad->a) == CAR_IDLE)
	 {
	     status = setCar(TEST_CAR,CAR_BUSY,OK,"",dummy);
	   
	     if(status == OK)
		 semGive(semTest);
	     else
		 status = CAD_REJECT;
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"test - cannot be done during observe");
	     cicsLogMessage(1, pCad->mess);
	 }

         break;

      /* CAD STOP directive detected. 
      */
      case CAD_STOP:
         cicsLogMessage( 1, "test - STOP directive. Cannot be stopped.");
	 strcpy( pCad->mess, "Cannot be stopped");
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}


/*
 *+
 * FUNCTION NAME:
 * setDhsInfoProc 
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = setDhsInfoProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "setDhsInfo" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the setDhsInfo CAD record is processed.
 * setDhsInfo is the command for setting the value of the DHS parameters 
 * to be used for Data display, quick-look and archiving .  
 * The command has one argument:
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */

long setDhsInfoProc( struct cadRecord *pCad ) 
{
   long status;         /* return status */

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "setDhsInfo - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "setDhsInfo - PRESET directive.");

	 /*QLSstream */
	 strcpy (pCad->vala, pCad->a);
	 /* lifetime */
	 if (!(strcmp(pCad->b,DHSCAD_TEMP)))
	 	*(long*)pCad->valb=1;
	 else if (!(strcmp(pCad->b,DHSCAD_PERM)))
	 	*(long*)pCad->valb=0;
	 else {
	 	status = CAD_REJECT;
	 	sprintf(pCad->mess,"setDhsInfo args %s,%s",DHSCAD_PERM,DHSCAD_TEMP);
	 	}
	 
	 /* bitsperpixel */
	 if (!(strcmp(pCad->c,DHSCAD_INT8)))
	 	*(long*)pCad->valc=8;
	 else if (!(strcmp(pCad->c,DHSCAD_INT16)))
	 	*(long*)pCad->valc=16;
	 else if (!(strcmp(pCad->c,DHSCAD_INT32)))
	 	*(long*)pCad->valc=32;
	  else {
	 	status = CAD_REJECT;
	 	sprintf(pCad->mess,"setDhsInfo - bitsperpixel args %s,%s,%s",DHSCAD_INT32,DHSCAD_INT16,DHSCAD_INT8);
	 	}
	 
	 
	 
	 cicsLogString( 3, "setDhsInfo: QLSstream  =", pCad->a);
	 cicsLogString( 3, "setDhsInfo: lifetime (0:perm/1:temp) =", pCad->b);
	 cicsLogString( 3, "setDhsInfo: bitsperpixel  =", pCad->c);
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "setDhsInfo - CLEAR directive.");
         break;

      /* CAD START directive detected.  Reject the command if an
observation
	 is in progress.  Otherwise, set the CAR to BUSY and invoke the
	 control task.
      */
      case CAD_START:
         cicsLogMessage( 2, "setDhsInfo - START directive.");
	 if(carStatus(pCad->d) == CAR_IDLE)
	 {
	     status = setCar(GSYS_CAR,CAR_BUSY,OK,"",dummy);
	    
	     if(status == OK)
		 semGive(semSetDhsInfo);
	     else
		 status = CAD_REJECT;
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"setDhsInfo - cannot be done during observe");
	     cicsLogMessage(1, pCad->mess);
	 }
         break;

      /* CAD STOP directive detected.
      */
      case CAD_STOP:
         cicsLogMessage( 1, "setDhsInfo - STOP directive. Cannot bestopped.");
         strcpy( pCad->mess, "Cannot be stopped");
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an
error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}

/*
 *+
 * FUNCTION NAME:
 * dhsConnectProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = dhsConnectProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "dhsConnectProc" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the dhsConnect CAD record is processed.
 * dhsConnect is the command for connecting/disconnecting to the DHS
 * The command has one argument:
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 03-April-2002  creation   Matthieu Bec
 *
 *-
 */

long dhsConnectProc( struct cadRecord *pCad )
{
   long status=CAD_ACCEPT;

   switch (pCad->dir) {
      case CAD_MARK:
         break;
      case CAD_PRESET:
         if (!(strcmp(pCad->a,"connect")))
                      *(long*)pCad->vala=1;
         else if (!(strcmp(pCad->a,"disconnect")))
            *(long*)pCad->vala=0;
         else {
            status = CAD_REJECT;
            sprintf(pCad->mess,"dhsConnect invalid args");
            }
         break;
      case CAD_CLEAR:
         break;
      case CAD_START:
         setCar(GSYS_CAR,CAR_BUSY,OK,"",dummy);
              printf("give semDhsConnect\n");
              semGive(semDhsConnect);
         break;
      case CAD_STOP:
         strcpy( pCad->mess, "dhsConnect cannot be stopped");
         status = CAD_REJECT;
         break;
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}



