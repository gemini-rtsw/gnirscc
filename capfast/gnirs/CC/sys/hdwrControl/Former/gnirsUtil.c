static struct
  {
      void *v;
      char *c;
  }
rcsid =
{
    &rcsid,
        "$Id: gnirsUtil.c,v 1.2 2009/05/27 19:32:08 fkraemer Exp $"
};

#include <vxWorks.h>
#include <stdio.h>
#include <fcntl.h>
#include <ioLib.h>
#include <vme.h>
#include <memLib.h>
#include <usrLib.h>             /* Debugging */
#include <cacheLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <intLib.h>
#include <logLib.h>
#include <iv.h>
#include <vxLib.h>
#include <ctype.h>
#include "stdarg.h"
#include "gnirs.h"
#include "timeLib.h"
#include "epicsTypes.h"

int doPosition(char *, char *, char *);


void getClockSpeed(void)
{
    gnirsG.clockRate = sysClkRateGet();
}


double gnirsGetTimeStamp()
{
    int status;
    double timeStamp;

    /* get the raw time */
    status = timeNow(&timeStamp);
    if (status) {
        timeStamp = 0.0;
    }
    
    return timeStamp;
}

/* Time stamp recording */
static int ymdhmsf[7];
void gnirsStartTime()
{
    int status;

    gnirsG.TStamp = 0;
    gnirsMarkTime();

    status = timeThenC(gnirsTimeStamps[0], UT1, 1, &ymdhmsf[0]);
    gnirsLogMessage(CICS_DB_FULL, "ymdhmsf %d %d %d %d %d %d %d",
       ymdhmsf[0], ymdhmsf[1], ymdhmsf[2], ymdhmsf[3], ymdhmsf[4],
       ymdhmsf[5], ymdhmsf[6]);

}

void gnirsMarkTime()
{

    if (gnirsG.TStamp == TIMESTAMPS) {
        /* XXX invalid in interrupt handler */
        gnirsLogMessage(CICS_DB_ERROR_TS, "Too many time stamps requested.");
        gnirsG.health = WARNING;
        return;
    }
    gnirsTimeStamps[gnirsG.TStamp] = gnirsGetTimeStamp();
    gnirsG.TStamp++;
}

/* Read the configuration file and set values */

int readConfig(char * file) {
    int rc, type, i, motor;
    FILE *fd;
    char buffer[CONFIG_CHAR_LEN];
    char orig[CONFIG_CHAR_LEN];
    char *p, *q, *start, *base;
    BOOL sectionSeen;
    BOOL inString;
    itemConfig * pItem;

    /* Open the file
     * For each line
     *   Strip out comments
     *   Strip out trailing blanks
     *   If line is now null, get next line
     *   Find '=', and set to '\0', thus delimiting the keyword
     *   Except within quoted strings, replace all ',' with a space
     *   Set keyword to lowercase and go to proper pocessing "routine"
     * Close the file
     */

    fd = fopen(file, "r");
    if (fd == NULL) {
        gnirsLogMessage(CICS_DB_ERROR1, "Cannot open '%s'.", file);
        return VME_ERROR;
    }
    sectionSeen = FALSE;
    motor = -1;   /* flag for finishMotorConfig call */
    while (fgets(buffer, CONFIG_CHAR_LEN, fd) != NULL) {
        buffer[CONFIG_CHAR_LEN-1] = '\0';      /* just to be sure */
        strcpy(orig, buffer);
        p = strchr(buffer, '\n');
        if (p != NULL)
            *p = '\0';
        /* remove comments */
        p = strchr(buffer, '#');
        if (p != NULL)
            *p = '\0';
        /* Strip leading blanks */
        p = buffer;
        while ((*p == ' ') || (*p == '\t'))
            p++;
        if (!*p)   /* reached a null ... blank line */
            continue;
        /* Parse Section Header */
        if (*p == '[') {
            p++;
            sectionSeen = TRUE;
            if ((q = strchr(p, ']')) == NULL) {
                gnirsLogMessage(CICS_DB_FILE, "No ']' in section header: %s",
                        orig);
                return VME_ERROR;
            }
            *q = '\0';
            /* lower case */
            for (q = p; *q; q++)
                if (isupper(*q))
                    *q = tolower(*q);
            /* Skip any leading white space within '[]' */
            while ((*p == ' ') || (*p == '\t'))
                p++;
            q = strchr(p, ' ');
            /* ok for q to be NULL as it means we have [keyword] with no spaces
             * and the ']' has already been set to null, delimiting the
             * keyword.
             */
            if (q)
                *q++ = '\0';
            /* Now 'q' points to what follows the keyword, if anything */

            /* Find the header type:
             *      motor
             *      mechanism
             *      filters
             *      temperature
             *      cryo
             *      grating
             */
            if (strncmp (p, "motor", 5) == 0) {
                /* which motor */
                i = atoi(q);
                if ( (i < 0) || (i >= NUM_MOTORS)) {
                    gnirsLogMessage(CICS_DB_FILE,
                            "Illegal motor number in %s", orig);
                    return VME_ERROR;
                }
                /* Moving on to different motor, so finish previous one */
                if (motor != -1)
                    finishMotorConfig(motor);
                rc = setupMotor(i, "");
                if (rc != VME_OK)
                    return rc;
                type = CONF_MOTOR;
                motor = i;
                base = (char *) motors[motor];
                pItem = &motorConfig[0];
            } else if (strncmp (p, "mechanism", 9) == 0) {
                /* Change to lower case and remove spaces */
                for (p = start = q; *p; p++) {
                    if (isupper(*p))
                        *q++ = tolower(*p);
                    else if (!isspace(*p))
                        *q++ = *p;
                }
                *q = '\0';
                type = CONF_MECHANISM;
                /* which mechanism */
                base = (char  *)NULL;
                for (i = 0; i < NUM_MECH; i++) {
                    if (strncmp(mechanism[i].configName, start,
                                        strlen(mechanism[i].configName)) == 0) {
                        base = (char *) &mechanism[i];
                    break;
                    }
                }
                if (!base) {
                    gnirsLogMessage(CICS_DB_FILE,
                            "Cannot find mechanism in %s\n", orig);
                    return VME_ERROR;
                }
                pItem = &mechConfig[0];

            } else if (strncmp (p, "filters", 7) == 0) {
                type = CONF_FILTER;
            } else if (strncmp (p, "temperature", 10) == 0) {
                /* which sensor */
                i = atoi(q);
                if ( (i < 0) || (i > (NUM_TEMPS - 1))) {
                    gnirsLogMessage(CICS_DB_FILE,
                            "Illegal temperature sensor in %s", orig);
                    return VME_ERROR;
                }
                type = CONF_TEMP;
                base = (char *)&temperatures[i];
                pItem = &tempConfig[0];
                /* XXX If it is a reference input, should user be allowed
                 * to change it?
                 */
            } else if (strncmp(p, "cryo", 4) == 0) {
                type = CONF_CRYO;
                base = (char *)cryoSwitches;
                pItem = &cryoConfig[0];
            } else if (strncmp(p, "grating", 7) == 0) {
                type = CONF_GRATING;
                /* which grating */
                i = atoi(q);
                if ( (i < 0) || (i >= NUM_GRATINGS)) {
                    gnirsLogMessage(CICS_DB_FILE,
                            "Illegal grating number in %s", orig);
                    return VME_ERROR;
                }
                base = (char *)&gratingData[i];
                pItem = &gratConfig[0];
            } else {
                gnirsLogMessage(CICS_DB_ERROR,
                        "Section '%s' not recognized", q);
                return VME_ERROR;
            }
            /* Valid section header seen; get next line */
            continue;
        }
        if (!sectionSeen) {
            gnirsLogMessage(CICS_DB_FILE,
                    "Section header must be first: file %s", file);
            return VME_ERROR;
        }
        start = p;
        inString = FALSE;
        /* Replace ',' with space when not in string */
        while (*p) {
            switch (*p) {
                case '"':
                    if (inString == TRUE)
                        inString = FALSE;
                    else
                        inString = TRUE;
                    break;
                case ',':
                    if (inString == FALSE)
                        *p = ' ';
                    break;
            }
            p++;
        }
        /* Find the '=' and mark the end of the keyword */
        p = strchr(start, '=');
        if (p == NULL) {
            gnirsLogMessage(CICS_DB_FILE, "No '=' in %s", orig);
            return VME_ERROR;
        }
        *p++ = '\0';
        /* Skip any leading white space */
        while ((*p == ' ') || (*p == '\t'))
            p++;
        /* 'keyword' to lowercase and then find it */
        for(q = start; *q; q++)
            if isupper(*q)
                *q = tolower(*q);
        if (type == CONF_FILTER)
            rc = setFilters(start, p, orig);
        else
            rc = parseKeywordValue(start, pItem, base, p, orig);
        if (rc != VME_OK)
            return rc;
    }
    fclose(fd);
    /* Rerun finishMotorConfig on last motor if this wasn't done before */
    if (motor != -1)
        finishMotorConfig(motor);
    /* other cleanup */
    rc = finishMechConfig();
    if (rc != VME_OK)
        return rc;
    rc = finishFiltersConfig();
    if (rc != VME_OK)
        return rc;
    return VME_OK;
}

/* Parse a keyword = value line */
int parseKeywordValue(char *keyword, itemConfig *pItem, char * base,
                                            char *rhs, char *orig) {
    switchType *pSw;
    motion *pMotion;
    ioBit *iob;
    char *where;
    char *p;
    itemConfig *pI;
    int i, type;
    int port, bit, level, mask;
    BOOL found;
    double *dq;

    found = FALSE;
    for (pI = pItem; pI->item && !found; pI++) {
        if (strncmp(keyword, pI->item, strlen(pI->item))== 0) {
            found = TRUE;
            i = 0;
            where = pI->offset + base;
            type = pI->type;
            /* First deal with a special case */
            switch(type) {
                case CONF_FOCUS:
                    where = (char *)&focusZeroPoint;
                    type = CONF_INT;
                    break;
            }
            switch (type) {
                case CONF_INT:
                    i = sscanf(rhs, "%d", (int *)where);
                    break;
                case CONF_DOUBLE:
                    i = sscanf(rhs, "%lf", (double *)where);
                    break;
                case CONF_CHAR:
                    /* For quoted char constant, as in "L" */
                    i = 1;
                    if (*rhs == '"')
                        if (*(rhs+2) == '"')
                            *(char *)where = rhs[1];
                        else
                            i = 0;  /* multi char string not wanted */
                    else
                        *(char *)where = rhs[0];
                    break;
                case CONF_STRING:
                    /* Can store maxLen-1 char plus null; rhs has two '"'
                     * which don't get stored.  Hence  maxLen+1
                     */
                    if (strlen(rhs) >= (pI->maxLen + 1))
                        i = 0;
                    else {
                        i = 1;
                        p = strchr(rhs,'"');
                        /* skip a null string */
                        if (p && (*(p+1) != '"'))
                            i = sscanf(rhs, " \"%[^\"]\"", (char *)where);
                    }
                    break;
                case CONF_APPEND_STR:
                    if ((strlen(rhs) + strlen(where))
                                >= pI->maxLen)
                        i = 0;
                    else {
                        i = 1;
                        p = strchr(rhs,'"');
                        /* skip a null string */
                        if (*p && (*(p+1) != '"')) {
                            i = strlen(where);
                            i = sscanf(&rhs[i], " \"%[^\"]\"", (char *)where);
                        }
                    }
                    break;
                case CONF_TYPE:
                    /* ugly */
                    i = 1;
                    for (p = rhs; *p; p++)
                        if (isupper(*p))
                            *p = tolower(*p);
                    if (strncmp(rhs, "rotary", 6) == 0)
                        *(int *)where = ROTARY;
                    else if (strncmp(rhs, "linear", 6) == 0)
                        *(int *)where = LINEAR;
                    else if (strncmp(rhs, "binary", 6) == 0)
                        *(int *)where = BINARY;
                    else
                        i = 0;
                    break;
                case CONF_SWITCH:
                    pSw = (switchType *)where;
                    /* Home switches have 6 items; other switches have 2 */
                    i = sscanf(rhs, "%d %d %d %d %d %d", &(pSw->position),
                            &(pSw->offset), &port, &bit,
                            &(pSw->useAlt), &(pSw->type));
                    if ( (i != 6) && (i != 2))
                        i = 0;
                    else if (i == 6) {
                        pSw->control.port= port & 0xFF;
                        pSw->control.bit = bit & 0xFF;
                    }
                    break;
                case CONF_MOTION:
                    pMotion = (motion *)where;
                    i = sscanf(rhs, "%d %d", &(pMotion->acceleration),
                            &(pMotion->velocity));
                    if (i != 2)
                        i = 0;
                    break;
                case CONF_POSITION:
                    i = doPosition(base, rhs, orig);
                    break;
                case CONF_GBOUNDS:
                    i = 0;
                    dq = (double *)where;
                    if ((p = strtok(rhs, " ,")) &&
                                    sscanf(p, "%lf", dq) ) {
                        i++;
                        dq++;
                        while ((p = strtok(NULL, " ,")) &&
                                            sscanf(p, "%lf", dq)) {
                            i++;
                            dq++;
                            if (i >= NUM_GRAT_BOUNDS)
                                break;
                        }
                    }
                    for(; i < NUM_GRAT_BOUNDS; i++ )
                        *dq++ = 100000.;  /* an unlikely micron wavelength!! */
                    break;
                case CONF_BIT:
                    iob = (ioBit *)where;
                    i = sscanf(rhs, "%d %d %d", &port, &bit, &level);
                    if (i != 3)
                        i = 0;
                    else {
                        port &= 0xFF;
                        bit &= 0xFF;
                        iob->port= port;
                        iob->bit = bit;
                        /* Set output ports */
                        if ((1 << port) & xycomOutputs) {
                            mask = 1 << bit;
                            if (level)
                                xycomOutputInit[port] |= mask;
                            else
                                xycomOutputInit[port] &= ~mask;
                        }
                    }
                    break;

            }
            if ( i == 0) {
                gnirsLogMessage(CICS_DB_ERROR,
                        "Failed to convert rhs <%s>", rhs);
                return VME_ERROR;
            }
        }
    }
    if (!found) {
        gnirsLogMessage(CICS_DB_ERROR,
                "readConfig failed to find keyword in %s", orig);
        return VME_ERROR;
    }
    return VME_OK;
}

int doPosition(char *base, char *rhs, char *orig) {
    int i;
    char *p;
    char input[ITEM_ID_LEN];
    char buf[4];
    int position, index;
    NODE *pNode;
    motorVars *m;
    mechDescriptor *pMech;

    pMech = (mechDescriptor *)base;

    p = strchr(rhs,'"');
    if (!p) {
        gnirsLogMessage(CICS_DB_FILE, "string expected in <%s>", orig);
        return 0;
    }
    i = sscanf(rhs, " \"%[^\"]\" %d %d", input, &position, &index);
    if (i == 2) {
        index = -1;
        i = 3;
    }
    if (i != 3)
        i = 0;
    else { 
        /* validate position */
        m = motors[pMech->motor];
        if (!m)     /* if not defined, ignore for now --- testing XXX XXX */
            return i;
        if (m->type == ROTARY) {
            if (abs(position) > m->fullTravel) {
                gnirsLogMessage(CICS_DB_FILE,
                        "Invalid position for rotary mechanism <%s>",
                        orig);
                return 0;
            }
        } else if (m->type == BINARY) {
            /* Must be home (==0) or neg limit */
            if ((position != 0) && (position > BINARY_LIMIT)) {
                gnirsLogMessage(CICS_DB_FILE,
                        "Binary position not at a limit <%s>", orig);
                return 0;
            }
            if (position <= BINARY_LIMIT)
                position = m->negLimit.position;
        } else if ((position <= m->negLimit.position) ||
                   (position >= m->posLimit.position)) {
                gnirsLogMessage(CICS_DB_FILE,
                        "Invalid position for linear mechanism <%s>",
                        orig);
                return 0;
        }
        /* lookup name, and insert it along with position in list */
        pNode = nodeLookup(input, pMech->pTable, TRUE);
        if (pNode == NULL)
            return 0;
        /* insert values */
        strncpy(((mechNode *)pNode)->name, input, ITEM_ID_LEN);
        ((mechNode *)pNode)->position = position;
        /* If index given, it's another name for the position */
        if (index != -1) {
            /* Don't test for plausibility as index can be for a slit
             * position (say, 0 - 10) or a filter wheel ([12]00 - [12]99)
             * etc..
             */
            sprintf(buf, "%d", index);
            pNode = nodeLookup(buf, pMech->pTable, TRUE);
            if (pNode == NULL)
                return 0;
            /* insert values */
            strncpy(((mechNode *)pNode)->name, buf, ITEM_ID_LEN);
            ((mechNode *)pNode)->position = position;
        }
        i = 1;
    }
    return i;
}

/* Convert a string to lower case */
void strnlc(char *out, char *in, int len) {
    char *p, *q;
    int i;

    for (q = out, p = in, i = 0; (i < len) && *p ; i++) {
        if (isupper((int)*p))
            *q++ = (char) tolower((int)*p++);
        else
            *q++ = *p++;
    }
    *q = '\0';
}

/*
 *+
 * FUNCTION NAME:
 * healthString
 *
 * INVOCATION:
 * healthString(hp, health)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (<) hp (char *)    Where to store string
 * (>) health (int)   Health as an enum
 *
 * FUNCTION VALUE:
 * (void) Returns 'void' and hence has no STATUS value
 *
 * PURPOSE:
 * Converts health descriptor to string
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
void healthString(char *hp, int health)
{
    char *p;

    switch (health) {
    case GOOD:
        p = "GOOD";
        break;
    case WARNING:
        p = "WARNING";
        break;
    case BAD:
        p = "BAD";
        break;
    }
    strcpy(hp, p);
}


/* logging functions */

/*
 *   PURPOSE
 *   -------
 *   Taken from cicsLib.c.
 *   Source for logging functions and modified as little as possible from
 *   original.
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *   gnirsInitLogging   - Create semaphore for GNIRS logging (optional)
 *   gnirsLogMessage    - Log a message.
 *   gnirsSetDebug      - Set new debug level
 *
 *   DEPENDENCIES
 *   ------------
 *
 *   LIMITATIONS
 *   ------------
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
 *   while others are repeated. I have used a ring buffer in gnirsLogMessage()
 *   as a means of alleviating the problem, but it will still happen if
 *   there are sufficient messages to fill the ring buffer
 *
 *   AUTHOR
 *   ------
 *   Steven Beard  (smb@roe.ac.uk)
 *   Janet Tvedt   (tvedt@noao.edu)
 *   Richard Wolff (rwolff@noao.edu)
 *
 *   HISTORY
 *   -------
 *   04-Dec-1996: Original version with just gnirsLogMessage,
 *                gnirsLogLong and gnirsLogString, based on Michelle
 *                logging library (which did not conform to the
 *                Gemini SPS).                                        (smb)
 *   16-Dec-1996: gnirsLogFloat added.                                 (smb)
 *   10-Jan-1997: gnirsLogDouble added.                                (smb)
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
 *   27-Jan-1997: Modified gnirsLogMessage to use a ring buffer of
 *                messages to get around a deficiency of logMsg().    (smb)
 *   04-Jun-1997: Size of message buffer increased from 40 to 64.     (smb)
 *   17-Jun-1997: Modified to use a semaphore to ensure that two
 *                tasks do not attempt to use the same element of
 *                the message buffer at the same time.                (smb)
 *   17-Jun-1997: Set top level prefixes. Janet Tvedt's database
 *                access functions added.                             (smb,tvedt)
 *   26-Jun-1997: Channel Access get and put functions added, but
 *                then transferred to "gnirsLib2.c" because it was
 *                not possible to include "dbAccess.h" and
 *                "cadefs.h" in the same file..                       (smb)
 *   12-Aug-1997: Added check functions used by CAD functions.        (smb)
 *
 *   04-Oct-1999: Removed many functions, renamed cics to gnirs,
 *                but remaining code and comments are essentially what
 *                was provided for the cics library.                  (rjw)
 */


/*
 * The gnirsLogMessage() function uses a ring buffer to help prevent messages
 * displayed on the console by logMsg() being overwritten. This constant
 * defines the number of messages in the ring buffer.
 */

#define MESSAGE_BUFFERS   64

/*
 * This constant is used by gnirsLogLong, gnirsLogFloat and gnirsLogDouble
 * to determine the maximum length of the introductory message. This allows
 * (GNIRS_MESSAGE_LENGTH - MAX_INTRO_SIZE) characters left for the actual value.
 * MAX_INTRO_STRING is used by gnirsLogString.
 */

#define MAX_INTRO_SIZE    60
#define MAX_INTRO_STRING  50


/* Global Variables */

/* Not used (yet?)
 * Top level prefix and SAD prefix
 * static char gnirsTop[MAX_STRING_SIZE] = "gnirs:";
 * static char gnirsSadtop[MAX_STRING_SIZE] = "gnirs::";
 */

SEM_ID semMutex;        /* Mutual exclusion semaphore to prevent more than
                         * a single task from accessing the static message
                         * array in gnirsLogMessage
                         */

static short useSem = 0;/* Flag set to 1 if semaphore is to be used */

/*
 *   (storage allocated in gnirsGlobals.c)
 * long gnirsDebugLevel = 1;     Current debugging level 
 *                                 0 = NOLOG
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


void gnirsInitLogging(void)
{
    semMutex = semBCreate(SEM_Q_FIFO, SEM_FULL);
    useSem = 1;
}


/* ===================================================================== */


void gnirsLogMessage(const long debugLevel, const char *format, ...)
{
    va_list arglist;
    static char buffer[MESSAGE_BUFFERS][GNIRS_MESSAGE_LENGTH];
    static char composeBuffer[GNIRS_MESSAGE_LENGTH];
    long debug;
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

    if (useSem == 1)
        semTake(semMutex, WAIT_FOREVER);

    debug = debugLevel;

    /* if a serious error, the health can't be good */
    if (debug == CICS_DB_ERROR) {
        gnirsG.health = BAD;
    }
    if (debug > CICS_DB_MARK)
        debug = CICS_DB_ERROR;
/*
 * Only display the message if the debug value associated with it is less
 * than or equal to the current debug level.
 */

    if (debug <= gnirsDebugLevel) {

/* At this point the message can be written to the "historyLog" record.
 *  Using PvPut. This is still to be added.
 */

/* build message from input */
        va_start(arglist, format);
        vsprintf(composeBuffer, format, arglist);
        va_end(arglist);

/* Copy message to gnirsErrorMessage */
        strncpy(gnirsErrorMessage, composeBuffer, GNIRS_MESSAGE_LENGTH-1);

/* Copy that part of the message which can fit in the history log. */

        strncpy(histMsg, composeBuffer, MAX_STRING_SIZE-1);

/* Copy the message to the history log using Channel Access. */

/*
 * NOTE: THE FOLLOWING LINES ARE COMMENTED OUT, AS THEY HAVEN'T BEEN TESTED YET.
 *
 *
 *      sprintf( fieldName, "%shistoryLog.VAL", gnirsSadtop );
 *      status = gnirsCaPut( fieldName, errMess, DBF_STRING, histMsg );
 */

/* Copy the message to the next available location in the ring buffer. */

        strncpy(buffer[next], composeBuffer, GNIRS_MESSAGE_LENGTH-1);

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

    if (useSem == 1)
        semGive(semMutex);

}
/* ===================================================================== */


void gnirsSetDebug(const long debug)
{

    gnirsDebugLevel = debug;
}
