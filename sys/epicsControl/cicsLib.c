static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: cicsLib.c,v 1.1 2009/06/10 15:05:09 gemvx Exp $"
};
/*
*   FILENAME
*   -------- 
*   cicsLib.c
*
*   PURPOSE
*   -------
*   This file contains the source for all the library functions used by the
*   Core Instrument Control System
*
*
*   FUNCTION NAME(S)
*   ----------------
*   cicsCheckBusy     - Check if character string contains CAR BUSY code
*   cicsCheckIdle     - Check if character string contains CAD IDLE code
*   cicsCheckMarked   - Check if character string contains CAD "marked" value
*   cicsCheckPaused   - Check if character string contains CAR PAUSED code
*   cicsDbGet         - Get a value from an EPICS field using database access
*   cicsDbPut         - Put a value to an EPICS field using database access
*   cicsInitLogging   - Create semaphore for CICS logging (optional)
*   cicsLogMessage    - Log a message.
*   cicsLogLong       - Log a message together with a long value.
*   cicsLogFloat      - Log a message together with a floating point value.
*   cicsLogDouble     - Log a message together with a double value.
*   cicsLogString     - Log a message together with a string value.
*   cicsSetDebug      - Set new debug level
*   cicsGetTop        - Get top level prefixes for EPICS database.
*   cicsSetTop        - Set top level prefixes for EPICS database.
*
*   DEPENDENCIES
*   ------------
*
*   LIMITATIONS
*   ------------
*   cicsLogLong, cicsLogFloat, cicsLogDouble and cicsLogString are not very
*   flexible, and they would be better provided as a single generic function
*   a la C++. They are useful for simple messages, but for complex messages
*   is it better to build a message using sprintf() and pass this to
*   cicsLogMessage().
*
*   The maximum EPICS string size (MAX_STRING_SIZE) is quite small, and
*   some messages might be truncated before being written to the "historyLog"
*   record.
*
*   The VxWorks logMsg() function returns before the message you have given
*   it has been displayed. On the one hand this means the function does not
*   delay real-time code. On the other hand this means that if logMsg() is
*   given a message buffer, that message buffer can be overwritten before
*   the message is displayed. The symptoms are some messages getting lost
*   while others are repeated. I have used a ring buffer in cicsLogMessage()
*   as a means of alleviating the problem, but it will still happen if
*   there are sufficient messages to fill the ring buffer
*
*   AUTHOR
*   ------
*   Steven Beard  (smb@roe.ac.uk)
*   Janet Tvedt   (tvedt@noao.edu)
*
*   HISTORY
*   -------
*   04-Dec-1996: Original version with just cicsLogMessage,
*                cicsLogLong and cicsLogString, based on Michelle
*                logging library (which did not conform to the
*                Gemini SPS).                                        (smb)
*   16-Dec-1996: cicsLogFloat added.                                 (smb)
*   10-Jan-1997: cicsLogDouble added.                                (smb)
*   15-Jan-1997: A means of setting and testing the debug level
*                added. Fixed some syntax and formatting problems
*                noticed by Janet Tvedt.                             (smb)
*   16-Jan-1997: Functions renamed to conform to new Gemini SPS.     (smb)
*   23-Jan-1997: Removed the "static" storage class specifier on
*                the various message string buffers. This was
*                causing messages generated simultaneously by
*                different parallel processes to be overwritten.
*                Use MAX_STRING_SIZE instead of hard-wired size for
*                log message.                                        (smb)
*   27-Jan-1997: Modified cicsLogMessage to use a ring buffer of
*                messages to get around a deficiency of logMsg().    (smb)
*   04-Jun-1997: Size of message buffer increased from 40 to 64.     (smb)
*   17-Jun-1997: Modified to use a semaphore to ensure that two
*                tasks do not attempt to use the same element of
*                the message buffer at the same time.                (smb)
*   17-Jun-1997: Set top level prefixes. Janet Tvedt's database
*                access functions added.                             (smb,tvedt)
*   26-Jun-1997: Channel Access get and put functions added, but
*                then transferred to "cicsLib2.c" because it was
*                not possible to include "dbAccess.h" and
*                "cadefs.h" in the same file..                       (smb)
*   12-Aug-1997: Added check functions used by CAD functions.        (smb)
*/
/* *INDENT-OFF* */
/*
 * $Log: cicsLib.c,v $
 * Revision 1.1  2009/06/10 15:05:09  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 */
/* *INDENT-ON* */


/* Global Constants */

#include  <vxWorks.h>
#include  <semLib.h>
#include  <types.h>
#include  <math.h>
#include  <time.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>

#include  <dbDefs.h>
#include  <dbCommon.h>
#include  <recSup.h>
#include  <dbAccess.h>
#include  <car.h>

#include  <cicsConst.h>
#include  <cicsLib.h>

/*
 * The cicsLogMessage() function uses a ring buffer to help prevent messages
 * displayed on the console by logMsg() being overwritten. This constant
 * defines the number of messages in the ring buffer.
 */

#define MESSAGE_BUFFERS   64

/*
 * This constant is used by cicsLogLong, cicsLogFloat and cicsLogDouble
 * to determine the maximum length of the introductory message. This allows
 * (CICS_MESSAGE_LENGTH - MAX_INTRO_SIZE) characters left for the actual value.
 * MAX_INTRO_STRING is used by cicsLogString.
 */

#define MAX_INTRO_SIZE    60
#define MAX_INTRO_STRING  50


/* Global Variables */

static char cicsTop[MAX_STRING_SIZE] = "cics:";      /* Top level prefix */
static char cicsSadtop[MAX_STRING_SIZE] = "cics::";  /* Top level SAD prefix */

SEM_ID semMutex;    /* Mutual exclusion semaphore to prevent more than
		       a single task from accessing the static message
		       array in cicsLogMessage */

static short useSem = 0;        /* Flag set to 1 if semaphore is to be used */

static long debugLevel = 1;     /* Current debugging level */
 /*                                0 = NOLOG
 *                                     No debugging or logging.
 *                                     Errors and warnings only.
 *                                 1 = NONE (default)
 *                                     Logging but no debugging.
 *                                     Errors and warnings, plus important
 *                                     log messages.
 *                                 2 = MIN
 *                                     Minimum debugging.
 *                                     Errors, warnings, log messages, plus
 *                                     supplemental information.
 *                                 3 = FULL
 *                                     Full debugging. All messages.
 */


/* ===================================================================== */


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsCheckBusy
 *
 *   Purpose:
 *   Check whether the given string contains the CAR BUSY code.
 *
 *   Purpose:
 *   This is a utility function, used mainly by CAD function, which returns
 *   a "1" if the input argument contains the CAR code for "BUSY" and a "0"
 *   if the code contains anything else.
 *
 *   Invocation:
 *   status = cicsCheckBusy( char *string );
 *
 *   Parameters in:
 *      string        *string     string containing integer CAR value.
 *
 *   Parameters out:
 *
 *   Return value:
 *      < busyFlag    long        = 1 if the given code represents "BUSY".
 *                                = 0 if the given code contains anything else.
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   16-Apr-1997: Original version.                        (smb)
 *   11-Aug-1997: Modified to take string argument.        (smb)
 *-
 */

long cicsCheckBusy( char *string ) 
{
  long cvstat;         /* conversion status */
  long ivalue;         /* Argument converted to integer */
  long busyFlag;       /* busy flag = return value */

/* Initialise return value */

  busyFlag = 0;

/* Convert the input argument to an integer. */

  cvstat = sscanf( string, "%d", &ivalue );

/*
 * Check the input argument was successfully converted. (cvstat contains the
 * number of items that "sscanf" has successfully converted, and there
 * should be 1).
 * If the argument cannot be converted check for the string "BUSY".
 */

  if ( cvstat == 1 )
  {

/*
 * Check if the argument translates to "CAR_BUSY".
 */

    if ( ivalue == CAR_BUSY )
    {

      busyFlag = 1;
    }
  }
  else
  {

    if ( strncmp( string, "BUSY", MAX_STRING_SIZE ) == 0 )
    {

      busyFlag = 1;
    }
  }

  return busyFlag;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsCheckIdle
 *
 *   Purpose:
 *   Check whether the given string contains the CAR IDLE code.
 *
 *   Purpose:
 *   This is a utility function, used mainly by CAD functions, which
 *   returns a "1" if the input argument contains the CAR code for "IDLE"
 *   and a "0" if the code contains anything else.
 *
 *   Invocation:
 *   status = cicsCheckIdle( char *string );
 *
 *   Parameters in:
 *      string        *string     string containing integer CAR value.
 *
 *   Parameters out:
 *
 *   Return value:
 *      < idleFlag    long        = 1 if the given code represents "IDLE".
 *                                = 0 if the given code contains anything else.
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   11-Aug-1997: Original version.                        (smb)
 *-
 */

long cicsCheckIdle( char *string ) 
{
  long cvstat;         /* conversion status */
  long ivalue;         /* Argument converted to integer */
  long idleFlag;       /* idle flag = return value */

/* Initialise return value */

  idleFlag = 0;

/* Convert the input argument to an integer. */

  cvstat = sscanf( string, "%d", &ivalue );

/*
 * Check the input argument was successfully converted. (cvstat contains the
 * number of items that "sscanf" has successfully converted, and there
 * should be 1).
 * If the argument cannot be converted check for the string "IDLE".
 */

  if ( cvstat == 1 )
  {

/*
 * Check if the argument translates to "CAR_IDLE".
 */

    if ( ivalue == CAR_IDLE )
    {

      idleFlag = 1;
    }
  }
  else
  {

    if ( strncmp( string, "IDLE", MAX_STRING_SIZE ) == 0 )
    {

      idleFlag = 1;
    }
  }

  return idleFlag;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsCheckMarked
 *
 *   Purpose:
 *   Check whether the given string contains the CAD "marked" code.
 *
 *   Purpose:
 *   This is a utility function, used mainly by CAD functions, which returns
 *   a "1" if the input argument contains the CAD code for "marked" and a "0"
 *   if the code contains anything else.
 *
 *   Invocation:
 *   status = cicsCheckMarked( char *string );
 *
 *   Parameters in:
 *      string        *string     string containing integer CAR value.
 *
 *   Parameters out:
 *
 *   Return value:
 *      < markedFlag  long        = 1 if the given code represents "marked".
 *                                = 0 if the given code contains anything else.
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   16-Apr-1997: Original version.                        (smb)
 *   11-Aug-1997: Modified to take string argument.        (smb)
 *-
 */

long cicsCheckMarked( char *string ) 
{
  long cvstat;         /* conversion status */
  long ivalue;         /* Argument converted to integer */
  long markedFlag;     /* marked flag = return value */

/* Initialise return value */

  markedFlag = 0;

/* Convert the input argument to an integer. */

  cvstat = sscanf( string, "%d", &ivalue );

/*
 * Check the input argument was successfully converted. (cvstat contains the
 * number of items that "sscanf" has successfully converted, and there
 * should be 1).
 */

  if ( cvstat == 1 )
  {

/*
 * Check if the argument translates to "marked".
 */

    if ( ivalue == 1 )
    {

     markedFlag = 1;
    }
  }

  return markedFlag;
}

/* ===================================================================== */


/*+
 *   Function name:
 *   cicsCheckPaused
 *
 *   Purpose:
 *   Check whether the given string contains the CAR PAUSED code.
 *
 *   Purpose:
 *   This is a utility function, used mainly by CAD functions, which
 *   returns a "1" if the input argument contains the CAR code for "PAUSED"
 *   and a "0" if the code contains anything else.
 *
 *   Invocation:
 *   status = cicsCheckPaused( char *string );
 *
 *   Parameters in:
 *      string        *string     string containing integer CAR value.
 *
 *   Parameters out:
 *
 *   Return value:
 *      < busyFlag    long        = 1 if the given code represents "PAUSED".
 *                                = 0 if the given code contains anything else.
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   11-Aug-1997: Original version.                        (smb)
 *-
 */

long cicsCheckPaused( char *string ) 
{
  long cvstat;         /* conversion status */
  long ivalue;         /* Argument converted to integer */
  long pausedFlag;     /* paused flag = return value */

/* Initialise return value */

  pausedFlag = 0;

/* Convert the input argument to an integer. */

  cvstat = sscanf( string, "%d", &ivalue );

/*
 * Check the input argument was successfully converted. (cvstat contains the
 * number of items that "sscanf" has successfully converted, and there
 * should be 1).
 * If the argument cannot be converted check for the string "PAUSED".
 */

  if ( cvstat == 1 )
  {

/*
 * Check if the argument translates to "CAR_PAUSED".
 */

    if ( ivalue == CAR_PAUSED )
    {

      pausedFlag = 1;
    }
  }
  else
  {

    if ( strncmp( string, "PAUSED", MAX_STRING_SIZE ) == 0 )
    {

      pausedFlag = 1;
    }
  }

  return pausedFlag;
}


/* ===================================================================== */

/*
 *+
 * FUNCTION NAME:
 * cicsDbGet
 *
 * INVOCATION:
 * long status;
 * char *fieldName;  
 * char *errMess;
 * unsigned short type;
 * double *outVal;
 *
 * status = cicsDbGet(fieldName, errMess, type, &outVal);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > fieldName (char *)               pointer to reocord_name.field_name
 * ! errMess   (char *)               pointer to string
 * > type      (unsigned short)       data type to convert output to 
 *                                    (DBF_DOUBLE, DBF_LONG, etc.)
 * < outval    (void *)               pointer to output value
 *
 * FUNCTION VALUE:
 * long  - status value returned to calling routine, a non-zero value
 *         indicates an error
 *
 * PURPOSE:
 * Function to get a value from an EPICS field by database access.
 *
 * DESCRIPTION:
 * This routine is called to obtain the value of the specified field 
 * within an EPICS database record.  It handles potential errors by
 * sending messages to the CICS logging functions and returning an
 * error emssage and status value to the calling routine.
 *
 * EXTERNAL VARIABLES:
 * dbNameToAddr    - EPICS database access routine for finding record address
 * dbGetField      - EPICS database access routine for retrieving the data
 * cicsLogMessage  - CICS logging function for a message
 * cicsLogString   - CICS logging function for a message + a string
 *
 * PRIOR REQUIREMENTS:
 * None.
 * 
 * DEFICIENCIES:
 * I am informed that the routine "recGblGetLinkValue" is preferred
 * to "dbGetField" by the EPICS community (because it can choose whether
 * to use Channel Access or Database Access according to circumstances),
 * but "recGblGetLinkValue" is much more complicated, and I don't understand
 * its description in the "EPICS IOC Application Developers Guide".
 *
 * HISTORY (optional):
 * 13-Mar-1997  Original version as getDbInfo.			Janet Tvedt
 * 17-Jun-1997  Imported into cicsLib.				Steven Beard
 *-
 */

long cicsDbGet(char *fieldName, char *errMess, unsigned short type, void *outVal)
{
    struct dbAddr addr;
    long ret, options=0L, nRq = 1L;
    long status;
    
    status = PASS;

    /* Get the address of the data structure  and handle any errors */
    if( (ret = dbNameToAddr (fieldName,&addr)) != 0L)
    {
	status = FAIL;
	sprintf(errMess, "dbNameToAddr error = %ld", ret);
	cicsLogMessage( CICS_DB_ERROR, errMess);
	cicsLogString( CICS_DB_MIN, "dbName = ", fieldName);
    }

    /* If address found, get the data.  Handle any errors. */
    if( status == PASS )
    {
       if( (ret = dbGetField(&addr, type, outVal, &options, &nRq, NULL)) != 0L)
       {
	   status = FAIL;
           sprintf(errMess, "dbGet error = %ld", ret);
	   cicsLogMessage( CICS_DB_ERROR, errMess);
       }
    }

    /* Return error status */
    return status;
}


/* ===================================================================== */

/*
 *+
 * FUNCTION NAME:
 * cicsDbPut
 *
 * INVOCATION:
 * char *fieldName;
 * char *errMess;
 * unsigned short type;
 * double outVal;
 * long status;
 *
 * status = cicsDbPut(fieldName, errMess, DBF_DOUBLE, &outVal)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > fieldName (char *)           pointer to record+field name
 * ! errMess   (char *)           pointer to string
 * > type      (unsigned short)   data type of value to put
 * > outVal    (void *)           pointer of data to put
 *
 * FUNCTION VALUE:
 * long  Status value returned to calling routine, a non-zero value indicates
 *       an error
 *
 * PURPOSE:
 * Function to put a value into an EPICS field by database access
 *
 * DESCRIPTION:
 * This routine may be called by a user subroutine to put a value
 * into an EPICS databaser record field.  The complete field name must
 * be specified (for example: sytem:subsystem:recordx.FIELDY).  This function
 * will also handle errors by logging them through the CICS logging functions
 * and by returning an error message and status value to the calling routine.
 *
 * EXTERNAL VARIABLES:
 * dbNameToAddr    - EPICS database access routine for finding record address
 * dbPutField      - EPICS database access routine for putting the data
 * cicsLogMessage  - CICS logging function for a message
 * cicsLogString   - CICS logging function for a message + a string
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * I am informed that the routine "recGblGetLinkValue" is preferred
 * to "dbGetField" by the EPICS community (because it can choose whether
 * to use Channel Access or Database Access according to circumstances),
 * but "recGblGetLinkValue" is much more complicated, and I don't understand
 * its description in the "EPICS IOC Application Developers Guide".
 *
 * HISTORY (optional):
 * 19-Mar-1997  Original version as putDbInfo.			Janet Tvedt
 * 17-Jun-1997  Imported into cicsLib				Steven Beard
 *-
 */


long cicsDbPut(char *fieldName, char *errMess, unsigned short type, void *outVal)
{
    struct dbAddr addr;
    long ret, nRq = 1L;
    long status;
    
    
    status = PASS;

    /* Get the address of the data structure  and handle any errors */
    if( (ret = dbNameToAddr (fieldName,&addr)) != 0L)
    {
	status = FAIL;
	sprintf(errMess, "dbNameToAddr error = %ld", ret);
	cicsLogMessage( CICS_DB_ERROR, errMess);
	cicsLogString( CICS_DB_MIN, "dbName = ", fieldName);
    }

    /* If address found, write the data.  Handle any errors. */
    if( status == PASS )
    {
       if( (ret = dbPutField(&addr, type, outVal, nRq)) != 0L)
       {
	   status = FAIL;
           sprintf(errMess, "dbPutField error = %ld", ret);
	   cicsLogMessage( CICS_DB_ERROR, errMess);
       }
    }

    /* Return error status */
    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsInitLogging
 *
 *   Purpose:
 *   Initialise mutual exclusion semaphore used by cicsLogMessage.
 *
 *   Purpose:
 *   This routine should be invoked from the VxWorks startup script
 *   if there is a need to use a mutual exclusion semaphore in the
 *   CICS logging. If the routine is not executed, the semaphore will
 *   not be used.
 *
 *   Invocation:
 *   cicsInitLogging(  );
 *
 *   Parameters in:
 *      < debug        long       Debug level associated with message.
 *                                0 means message is always logged.
 *                                1 is used for important log messages.
 *                                2 is used for supplemental messages.
 *                                3 is used for unimportant messages.
 *                                In general, messages are only logged
 *                                if debug <= debugLevel.
 *      < message     char*       Message to be logged.
 *
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *
 *      External variables:
 *      > useSem  short     Flag indicating whether to use a semaphore.
 *      > semMutex SEM_ID   ID of semaphore created.
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Janet Tvedt  (tvedt@naoo.edu)
 *   Steven Beard (smb@roe.ac.uk)
 *
 *   History:
 *   XX-Apr-1997: Original version as initCicsLogging.              (tvedt)
 *   17-Jun-1997: Comment prologue added. Renamed to
 *                cicsInitLogging to obey Gemini programming
 *                standards (which recommends that all library
 *                functions begin with the same prefix). Set
 *                "useSem" variable.                                (smb)
 *-
 */

void cicsInitLogging(void)
{
    semMutex = semBCreate(SEM_Q_FIFO, SEM_FULL);
    useSem = 1;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsLogMessage
 *
 *   Purpose:
 *   Log an informational message.
 *
 *   Purpose:
 *   This routine is called whenever the CICS software needs to output
 *   an informational message. By directing all messages through this
 *   routine it should be possible to redirect the messages easily,
 *   for example through the "historyLog" record monitored by the Data
 *   Handling System. For the moment the messages are passed to the
 *   EPICS "LogMsg" function.
 *
 *   Invocation:
 *   long debug;
 *   cicsLogMessage( debug, "Message" );
 *
 *   Parameters in:
 *      < debug        long       Debug level associated with message.
 *                                0 means message is always logged.
 *                                1 is used for important log messages.
 *                                2 is used for supplemental messages.
 *                                3 is used for unimportant messages.
 *                                In general, messages are only logged
 *                                if debug <= debugLevel.
 *      < message     char*       Message to be logged.
 *
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *      cicsLogMsg        VxWorks function for logging a message.
 *
 *      External variables:
 *      > debugLevel  long        Current debugging level.
 *                                0 = No debugging or logging.
 *                                    Errors and warnings only.
 *                                1 = Logging but no debugging.
 *                                    Errors and warnings, plus important
 *                                    log messages.
 *                                2 = Minimum debugging.
 *                                    Errors, warnings, log messages, plus
 *                                    supplemental information.
 *                                3 = Full debugging. All messages.
 *
 *   Requirements:
 *   It is assumed the current debug level has already been defined.
 *
 *   Limitations:
 *   This routine should not be used in very time-critical pieces of code
 *   where the time spent calling the routine and checking the debug level
 *   would be significant.
 *
 *   The "historyLog" record is not long enough to contain more than
 *   MAX_STRING_SIZE characters of the message, so history log messages
 *   might be truncated.
 *
 *   The VxWorks logMsg() function returns before the message you have given
 *   it has been displayed. On the one hand this means the function does not
 *   delay real-time code. On the other hand this means that if logMsg() is
 *   given a message buffer, that message buffer can be overwritten before
 *   the message is displayed. The symptoms are some messages getting lost
 *   while others are repeated. I have used a ring buffer in cicsLogMessage()
 *   as a means of alleviating the problem, but it will still happen if
 *   there are sufficient messages to fill the ring buffer
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *   Janet Tvedt  (tvedt@naoo.edu)
 *
 *   History:
 *   03-Dec-1996: Original version.                                 (smb)
 *   15-Jan-1996: Added check on debugLevel.                        (smb)
 *   23-Jan-1997: static storage class removed from histMsg.
 *                Use MAX_STRING_SIZE to declare size of history
 *                log message.                                      (smb)
 *   27-Jan-1997: It was found that some messages delivered to
 *                logMsg() were getting lost, whereas others were.
 *                getting overwritten (see "LIMITATIONS" above. Ring
 *                buffer introduced into which pending messages can
 *                be queued.                                        (smb)
 *   17-Jun-1997: Modified to use a semaphore.                      (tvedt,smb)
 *   27-Jun-1997: Code to write history log added (but commented
 *                out, since it still needs testing).               (smb)
 *-
 */

void cicsLogMessage ( const long debug, const char * message )
{
    void logMsg();                   /* External logMsg function */

    static char buffer[MESSAGE_BUFFERS][CICS_MESSAGE_LENGTH];
                                     /* Ring buffer */

    static long next=0;              /* Index into ring buffer */

 /* char fieldName[MAX_STRING_SIZE]; <--- COMMENTED OUT FOR NOW */
                                     /* Field name of history log record */

    char histMsg[MAX_STRING_SIZE];   /* Message to be written to history log */

/* If a semaphore is being used, wait until the semaphore is available,
 * then continue with the logging.
 */

    if ( useSem == 1 ) semTake(semMutex, WAIT_FOREVER);

/*
 * Only display the message if the debug value associated with it is less
 * than or equal to the current debug level.
 */

    if ( debug <= debugLevel )
    {

/* At this point the message can be written to the "historyLog" record.
 *  Using PvPut. This is still to be added.
 */

/* Copy that part of the message which can fit in the history log. */

      strncpy( histMsg, message, MAX_STRING_SIZE );

/* Copy the message to the history log using Channel Access. */

/*
 * NOTE: THE FOLLOWING LINES ARE COMMENTED OUT, AS THEY HAVEN'T BEEN TESTED YET.
 *
 *
 *      sprintf( fieldName, "%shistoryLog.VAL", cicsSadtop );
 *      status = cicsCaPut( fieldName, errMess, DBF_STRING, histMsg );
 */

/* Copy the message to the next available location in the ring buffer. */

      strncpy(  buffer[next], message, CICS_MESSAGE_LENGTH );

/* Give this location in the ring buffer to logMsg() */

      logMsg( "%s\n", buffer[next] );

/*
 * Increment the ring buffer pointer, resetting it to the start if greater
 * than the buffer size.
 */

      if( ++next >= MESSAGE_BUFFERS ) next = 0;

    }

/* If a semaphore is being used, make it available to the next task */

    if ( useSem == 1 ) semGive(semMutex);

}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsLogLong
 *
 *   Purpose:
 *   Log an informational message together with a "long" value.
 *
 *   Purpose:
 *   This routine is called whenever the CICS software needs to output
 *   an informational message followed by one "long" value. All output
 *   is routed through the "cicsLogMessage" function - see the description
 *   of that function above.
 *
 *   Invocation:
 *   long debug;
 *   long value;
 *   cicsLogLong( debug, "Value =", value );
 *
 *   Parameters in:
 *      < debug       long        Debug level associated with message.
 *                                For details see "cicsLogMessage" above.
 *      < message     char*       Message to be logged.
 *      < value       long        Value to be logged
 *
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *      cicsLogMessage   CICS function for logging a message.
 *
 *      External variables:
 *      > debugLevel  long        Current debugging level.
 *                                For details see "cicsLogMessage" above.
 *
 *   Requirements:
 *   It is assumed the current debug level has already been defined.
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   03-Dec-1996: Original version.                               (smb)
 *   15-Jan-1997: Fixed some syntax and formatting problems
 *                noticed by Janet Tvedt.                         (smb)
 *   23-Jan-1997: static storage class removed from character
 *                strings.                                        (smb)
 *   27-Jan-1997: MAX_MESSAGE_SIZE and MAX_INTRO_SIZE
 *                parameterised.                                  (smb)
 *   11-Aug-1997: Use CICS_MESSAGE_LENGTH.                          (smb)
 *-
 */

void cicsLogLong ( const long debug, const char * string, const long value )
{
    char intro[MAX_INTRO_SIZE];
    char message[CICS_MESSAGE_LENGTH];

/* Ensure that the given introductory string is no more than MAX_INTRO_SIZE
 * characters long, thus allowing sufficient characters for the value before
 * the CICS_MESSAGE_LENGTH character buffer is filled up.
 */

    strncpy( intro, string, MAX_INTRO_SIZE );

    sprintf( message, "%s %ld", intro, value );
    cicsLogMessage( debug, message );

}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsLogFloat
 *
 *   Purpose:
 *   Log an informational message together with a "float" value.
 *
 *   Purpose:
 *   This routine is called whenever the CICS software needs to output
 *   an informational message followed by one "float" value. All output
 *   is routed through the "cicsLogMessage" function - see the description
 *   of that function above.
 *
 *   Invocation:
 *   long debug;
 *   float value;
 *   cicsLogFloat( debug, "Value =", value );
 *
 *   Parameters in:
 *      > debug        long       Debug level associated with message.
 *                                For details see "cicsLogMessage" above.
 *      > message     char*       Message to be logged.
 *      > value       float       Value to be logged
 * 
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *      cicsLogMessage   CICS function for logging a message.
 *
 *      External variables:
 *      > debugLevel  long       Current debugging level.
 *                               For details see "cicsLogMessage" above.
 *
 *   Requirements:
 *   It is assumed the current debug level has already been defined.
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   16-Dec-1996: Original version.                               (smb)
 *   15-Jan-1997: Fixed some syntax and formatting problems
 *                noticed by Janet Tvedt.                         (smb)
 *   23-Jan-1997: static storage class removed from character
 *                strings.                                        (smb)
 *   27-Jan-1997: MAX_MESSAGE_SIZE and MAX_INTRO_SIZE
 *                parameterised.                                  (smb)
 *   11-Aug-1997: Use CICS_MESSAGE_LENGTH.                          (smb)
 *-
 */

void cicsLogFloat ( const long debug, const char * string, const float value )
{
    char intro[MAX_INTRO_SIZE];
    char message[CICS_MESSAGE_LENGTH];

/* Ensure that the given introductory string is no more than MAX_INTRO_SIZE
 * characters long, thus allowing sufficient characters for the value before
 * the CICS_MESSAGE_LENGTH character buffer is filled up.
 */

    strncpy( intro, string, MAX_INTRO_SIZE );

    sprintf( message, "%s %f", intro, value );
    cicsLogMessage( debug, message );

}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsLogDouble
 *
 *   Purpose:
 *   Log an informational message together with a "double" value.
 *
 *   Purpose:
 *   This routine is called whenever the CICS software needs to output
 *   an informational message followed by one "double" value. All output
 *   is routed through the "cicsLogMessage" function - see the description
 *   of that function above.
 *
 *   Invocation:
 *   long debug;
 *   double value;
 *   cicsLogDouble( debug, "Value =", value );
 *
 *   Parameters in:
 *      > debug        long       Debug level associated with message.
 *                                For details see "cicsLogMessage" above.
 *      > message     char*       Message to be logged.
 *      > value       double      Value to be logged
 * 
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *      cicsLogMessage   CICS function for cicsLogging a message.
 *
 *      External variables:
 *      > debugLevel  long        Current debugging level.
 *                                For details see "cicsLogMessage" above.
 *
 *   Requirements:
 *   It is assumed the current debug level has already been defined.
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   16-Dec-1996: Original version.                               (smb)
 *   15-Jan-1997: Fixed some syntax and formatting problems
 *                noticed by Janet Tvedt.                         (smb)
 *   23-Jan-1997: static storage class removed from character
 *                strings.                                        (smb)
 *   27-Jan-1997: MAX_MESSAGE_SIZE and MAX_INTRO_SIZE
 *                parameterised.                                  (smb)
 *   11-Aug-1997: Use CICS_MESSAGE_LENGTH.                          (smb)
 *-
 */

void cicsLogDouble ( const long debug, const char * string, const double value )
{
    char intro[MAX_INTRO_SIZE];
    char message[CICS_MESSAGE_LENGTH];

/* Ensure that the given introductory string is no more than MAX_INTRO_SIZE
 * characters long, thus allowing sufficient characters for the value before
 * the CICS_MESSAGE_LENGTH character buffer is filled up.
 */

    strncpy( intro, string, MAX_INTRO_SIZE );

    sprintf( message, "%s %f", intro, value );
    cicsLogMessage( debug, message );

}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsLogString
 *
 *   Purpose:
 *   Log an informational message together with a "string" value.
 *
 *   Purpose:
 *   This routine is called whenever the CICS software needs to output
 *   an informational message followed by one "string" value. All output
 *   is routed through the "cicsLogMessage" function - see the description
 *   of that function above.
 *
 *   Invocation:
 *   long debug;
 *   char *value;
 *   cicsLogString( debug, "Value =", value );
 *
 *   Parameters in:
 *      > debug        long       Debug level associated with message.
 *                                For details see "cicsLogMessage" above.
 *      > message     char*       Message to be logged.
 *      > value       char*       String to be logged
 * 
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *      cicsLogMessage   CICS function for logging a message.
 *
 *      External variables:
 *      > debugLevel  long        Current debugging level.
 *                                For details see "cicsLogMessage" above.
 *
 *   Requirements:
 *   It is assumed the current debug level has already been defined.
 *
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   03-Dec-1996: Original version.                               (smb)
 *   15-Jan-1997: Fixed some syntax and formatting problems
 *                noticed by Janet Tvedt.                         (smb)
 *   23-Jan-1997: static storage class removed from character
 *                strings.                                        (smb)
 *   27-Jan-1997: MAX_MESSAGE_SIZE and MAX_INTRO_STRING
 *                parameterised.                                  (smb)
 *   11-Aug-1997: Use CICS_MESSAGE_LENGTH.                        (smb)
 *-
 */

void cicsLogString ( const long debug, const char * string1,
                     const char * string2 )
{
    char intro[MAX_INTRO_STRING];
    char message[CICS_MESSAGE_LENGTH];

/* Ensure that the given introductory string is no more than MAX_INTRO_STRING
 * characters long, thus allowing sufficient characters for the value before
 * the CICS_MESSAGE_LENGTH character buffer is filled up.
 */

    strncpy( intro, string1, MAX_INTRO_STRING );

    sprintf( message, "%s %s", intro, string2 );
    cicsLogMessage( debug, message );

}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsSetDebug
 *
 *   Purpose:
 *   Change the current debugging level.
 *
 *   Purpose:
 *   This routine is called whenever the CICS software needs to change the
 *   current debugging level.
 *
 *   Invocation:
 *   cicsSetDebug( long debug  );
 *
 *   Parameters in:
 *      < debug       long       New debugging level.
 *                               For details see "cicsLogMessage" above.
 * 
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *
 *      External variables:
 *      < debugLevel  long        Current debugging level.
 *                                For details see "cicsLogMessage" above.
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   14-Jan-1997: Original version.                  (smb)
 *-
 */

void cicsSetDebug ( const long debug )
{

    debugLevel = debug;
}



/* ===================================================================== */

/*+
 *   Function name:
 *   cicsGetTop
 *
 *   Purpose:
 *   Get the current top level record name prefixes.
 *
 *   Purpose:
 *   This routine is called to get the current top level record name prefixes.
 *
 *   Invocation:
 *   status = cicsGetTop( char *top, char *sadtop  );
 *   It may be invoked from the VxWorks startup script.
 *
 *   Parameters in:
 * 
 *   Parameters out:
 *      > top       *string      Top level record name prefix.
 *      > sadtop    *string      Top level SAD record name prefix.
 *
 *   Return value:
 *      ! status    long         status=PASS if the function worked
 *                               status=FAIL if the strings provided were
 *                               not of sufficient length.
 *
 *   Globals:
 *      External functions:
 *
 *      External variables:
 *
 *   Requirements:
 *
 *   Limitations:
 *   The supplied strings must be at least MAX_STRING_LENGTH in size.
 *   This function ought to check for this, but I am not sure how to
 *   do it (since for some reason sizeof( top ) always returns 4 and
 *   strlen( top ) returns 0).
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   21-Aug-1997: Original version.                  (smb)
 *-
 */

long cicsGetTop ( char* top, char* sadtop )
{
    long status;

    status = PASS;

/*
 * Copy the contents of the static variables cicsTop and cicsSadtop
 * into the supplied character strings.
 */

    strncpy( top, cicsTop, MAX_STRING_SIZE );
    strncpy( sadtop, cicsSadtop, MAX_STRING_SIZE );

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   cicsSetTop
 *
 *   Purpose:
 *   Set the current top level record name prefixes.
 *
 *   Purpose:
 *   This routine is called to define the top level record name prefixes.
 *
 *   Invocation:
 *   cicsSetTop( const char *top, const char *sadtop  );
 *   It may be invoked from the VxWorks startup script.
 *
 *   Parameters in:
 *      < top       *string      Top level record name prefix.
 *      < sadtop    *string      Top level SAD record name prefix.
 * 
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *
 *      External variables:
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   17-Jun-1997: Original version.                  (smb)
 *-
 */

void cicsSetTop ( const char* top, const char* sadtop )
{

/*
 * Copy at most MAX_STRING_SIZE characters to the static variables
 * "cicsTop" and "cicsSadtop". These variables are used within cicsLib
 * and can be obtained by calling the cicsGetTop function.
 */

    strncpy( cicsTop, top, MAX_STRING_SIZE );
    strncpy( cicsSadtop, sadtop, MAX_STRING_SIZE );
}
