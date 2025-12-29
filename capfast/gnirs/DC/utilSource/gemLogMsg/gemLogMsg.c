/********************************************************************
 * Module Name
 *		gemLogMsg
 *
 * Description:
 *		provide logging facility through the EPICS ioclog
 *
 * 
 * Author: Matthieu Bec, Gemini Software
 *
 *
 ********************************************************************/


#include "epicsPrint.h"
#include "gemLogMsg.h"
#define OFF	0
#define ON	1

static int gemUseiocLog=OFF;		/* default to INFO level */
static int gemLogLevel=GEMLOG_INFO;		/* default to INFO level */
static char *GEMLOGSTR[]={
   "EMERG",
   "ALERT",
   "CRIT",
   "ERR",
   "WARNING",
   "NOTICE",
   "INFO",
   "DEBUG" };

/********************************************************************
 * gemLogMsg
 ********************************************************************/

int gemLogMsg(int level, const char *pFormat, ...) {
   va_list		pvar;

   if (level <= gemLogLevel) {
      va_start (pvar, pFormat);
      if (gemUseiocLog) {
      	iocLogPrintf("%s ",GEMLOGSTR[level]);
      	return iocLogVPrintf(pFormat, pvar);
      }
      else {

      	epicsPrintf("%s ",GEMLOGSTR[level]);
	return epicsVprintf(pFormat, pvar);
      }
   }
   else return 0;
}

/********************************************************************
 * gemSetLogLevel
 ********************************************************************/
void gemSetLogLevel(int level) {
   gemLogLevel = level;
}

/********************************************************************
 * gemSetUseiocLog
 ********************************************************************/
void gemSetUseiocLog(int state) {

   if (state >= ON) {	
      gemUseiocLog = ON;
      return;
   }
   gemUseiocLog = OFF;
}

