

#include <vxWorks.h>
#include <stdarg.h>
#include <semLib.h>
#include <string.h>
#include <logLib.h>
#include "cicsLib.h"
#include "saver.h"
#include "gnDCADefs.h"
long debugLevel;
extern saverParams svrP;
/* ===================================================================== */


/*+
 *   FUNCTION NAME:
 *   logMessage
 *
 *   PURPOSE:
 *   Log an informational message
 *   This routine is called whenever the SDSU software needs to output
 *   an informational message. By directing all messages through this
 *   routine it should be possible to redirect the messages easily,
 *   for example through the "historyLog" record monitored by the Data
 *   Handling System. For the moment the messages are passed to the
 *   EPICS "LogMsg" function.
 *
 *   INVOCATION:
 *   long debug;
 *   logMessage( debug, "Message" );
 *
 *   PARAMETERS (">" input, "!" modified, "<" output)
 *      < debug        long       Debug level associated with message.
 *                                0 means message is always logged.
 *                                1 is used for important log messages.
 *                                2 is used for supplemental messages.
 *                                3 is used for unimportant messages.
 *                                In general, messages are only logged
 *                                if debug <= DebugLevel.
 *      < message     char*       Message to be logged.
 *
 *   FUNCTION VALUE:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *      sdsuLogMsg        VxWorks function for logging a message.
 *
 *      External variables:
 *      > DebugLevel  long        Current debugging level.
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
 *   PRIOR REQUIREMENTS:
 *   It is assumed the current debug level has already been defined.
 *
 *   DEFICIENCIES:
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
 *   while others are repeated. I have used a ring buffer in logMessage()
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
void logMessage(const long debug, const char *format, ...)
{
    va_list arglist;
    static char buffer[MESSAGE_BUFFERS][MESSAGE_LENGTH];
    static char composeBuffer[MESSAGE_LENGTH];
   
    /* Ring buffer */

    static long next = 0;       /* Index into ring buffer */

    /* char fieldName[MAX_STRING_SIZE]; <--- COMMENTED OUT FOR NOW */
    /* Field name of history log record */

    /* Message to be written to history log */
    char histMsg[MAX_STRING_SIZE];

/* If a semaphore is being used, wait until the semaphore is available,
 * then continue with the logging.  Cannot use this routine in an
 * interrupt context if using semaphore!!!!
 */

 /*    if (useSem == 1) */
/*         semTake(semMutex, WAIT_FOREVER); */

   
    /* if a serious error, the health can't be good */
    if (debug == CICS_DB_ERROR)
    {
        svrP.health = BAD;
    }
 /*    if (debug > CICS_DB_MARK) */
/*         debug = CICS_DB_ERROR; */
/*
 * Only display the message if the debug value associated with it is less
 * than or equal to the current debug level.
 */

    if (debug <= debugLevel) {

/* At this point the message can be written to the "historyLog" record.
 *  Using PvPut. This is still to be added.
 */

/* build message from input */
        va_start(arglist, format);
        vsprintf(composeBuffer, format, arglist);
        va_end(arglist);

/* Copy message to ErrorMessage */
      /*   strncpy(ErrorMessage, composeBuffer, MESSAGE_LENGTH-1); */

/* Copy that part of the message which can fit in the history log. */

        strncpy(histMsg, composeBuffer, MAX_STRING_SIZE-1);

/* Copy the message to the history log using Channel Access. */

/*
 * NOTE: THE FOLLOWING LINES ARE COMMENTED OUT, AS THEY HAVEN'T BEEN TESTED YET.
 *
 *
 *      sprintf( fieldName, "%shistoryLog.VAL", sdsuSadtop );
 *      status = sdsuCaPut( fieldName, errMess, DBF_STRING, histMsg );
 */

/* Copy the message to the next available location in the ring buffer. */

        strncpy(buffer[next], composeBuffer, MESSAGE_LENGTH-1);

/* Give this location in the ring buffer to logMsg() */

        logMsg("%s\n", (int) buffer[next], 0, 0, 0, 0, 0);

/*
 * Increment the ring buffer pointer, resetting it to the start if greater
 * than the buffer size.
 */

        if (++next >= MESSAGE_BUFFERS)
            next = 0;

    }
/* If a semaphore is being used, make it available to the next task */

 /*    if (useSem == 1) */
/*         semGive(semMutex); */

}
