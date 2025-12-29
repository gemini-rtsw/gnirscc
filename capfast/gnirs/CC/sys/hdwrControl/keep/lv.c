static struct
  {
      void *v;
      char *c;
  }
sccsid =
{
    &sccsid,
        "%W% %G%"
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
#include "gnirsCC.h"
#include "sockutil.h"

/* Labview communication support, derived from GMOS fits
 * support.
 */


/*
 *+
 * FUNCTION NAME:
 * fitsKeywordTime
 *
 * INVOCATION:
 * fitsKeywordTime(label, value, comment)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) label (char *)
 * (>) value (double)
 * (>) comment (char *)
 *
 * FUNCTION VALUE:
 * (void) Returns 'void' and hence has no STATUS value
 *
 * PURPOSE:
 * Builds a FITS header entry for a time value (H:M:S)
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
void fitsKeywordTime(char *label, double value, char *comment)
{
    char outLine[85];
    int min,
         sec,
         tenths;
    int ival;

    ival = (int) value;
    tenths = (value - ival) * 10. + 0.5;
    sec = ival % 60;
    ival /= 60;
    min = ival % 60;
    ival /= 60;
    ival %= 24;         /* make day's hours, else get hours since epoch */

    sprintf(outLine, "%02d:%02d:%02d.%1d", ival, min, sec, tenths);
}



/* Labview support routines */

static SVMSG svout = {SVFLAG, 0, 0};
static SVMSG svreply = {0, 0, 0};
static FILE *lvFp = NULL;

/*
 *+
 * FUNCTION NAME:
 * getLvCode
 *
 * INVOCATION:
 * getLvCode()
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None
 *
 * FUNCTION VALUE:
 * (int) Returns message count field
 *
 * PURPOSE:
 * Parse labview message to find count field and return it
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
int getLvCode()
{
    if (socknRead(gnirsG.svFd, (char *) &svreply, SVMSG_LEN) != SVMSG_LEN)
        return VME_ERROR;
    if (svreply.magic != SVFLAG)
        return VME_ERROR;
    return (svreply.count);
}

/*
 *+
 * FUNCTION NAME:
 * sendMsg
 *
 * INVOCATION:
 * sendMsg(data)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) data (char *)
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Send labview message
 *
 * DESCRIPTION:
 * Input char* points to byte data to send
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
int sendMsg(char *data)
{
    int rc;

    if (gnirsG.lvFileType == LVSERVER)
        rc = sockWrite(gnirsG.svFd, (char *) &svout, SVMSG_LEN);
    else {
        rc = fwrite((const void *)&svout, SVMSG_LEN, 1, lvFp);
        if (rc != 1)
            rc = ERROR;
        else
            rc = OK;
    }
    if (rc == ERROR)
        return VME_ERROR;
    if (svout.count) {
        if (gnirsG.lvFileType == LVSERVER)
            rc = sockWrite(gnirsG.svFd, data, svout.count);
        else {
            rc = fwrite(data, 1, svout.count, lvFp);
            if (rc != svout.count)
                rc = ERROR;
            else
                rc = OK;
        }
    }
    if (rc == OK)
        return VME_OK;
    else
        return VME_ERROR;
}


/*
 *+
 * FUNCTION NAME:
 * sendLvData
 *
 * INVOCATION:
 * sendLvData(op, count, buffer)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) op (int)
 * (>) count (int)
 * (>) buffer (char *)
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Send data tagged with op code to labview
 *
 * DESCRIPTION:
 * Sends a buffer of data to the labview program, tagged with the
 * specified op code.
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
int sendLvData(int op, int count, char *buffer)
{

    svout.opcode = op;
    svout.count = count;
    return (sendMsg(buffer));
}

/*
 *+
 * FUNCTION NAME:
 * sendLvString
 *
 * INVOCATION:
 * sendLvString(op, string)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) op (int)
 * (>) string (char *)
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Sends a string, with NULL, to labview
 *
 * DESCRIPTION:
 * Calls lvSendData to do the work
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
int sendLvString(int op, char *string)
{
    int len;

    len = strlen(string) + 1;
    return (sendLvData(op, len, string));
}

/* status files rooted at '/lv' */
static char lvRoot[40] = "/lv";

/*
 *+
 * FUNCTION NAME:
 * lvFileOpen
 *
 * INVOCATION:
 * lvFileOpen(name)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) name (char *)
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Opens a file for labview data
 *
 * DESCRIPTION:
 * Opens the file whose name is given.  Depending on the value of lvFileType,
 * this will be a file in /lv (see lvRoot just above) or a remote
 * connection to the labview program.
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
int lvFileOpen(char *name)
{
    int rc;
    char buffer[120];

    if (gnirsG.lvFileType == LVSERVER) {
        rc = sendLvString(SVOPEN, name);
    } else {
        sprintf(buffer, "%s/%s", lvRoot, name);
        lvFp = fopen(buffer, "wb");
        if (lvFp == NULL) {
            rc = VME_ERROR;
        } else
            rc = VME_OK;
    }
    if (rc != VME_OK) {
        gnirsLogMessage(CICS_DB_ERROR5, "Error creating data file.");
    }
    return rc;
}

/*
 *+
 * FUNCTION NAME:
 * lvFileRemove
 *
 * INVOCATION:
 * lvFileRemove(name)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) name (char *)
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Deletes an existing (labview status) file
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
int lvFileRemove(char *name)
{
    int rc;
    char buffer[120];

    if (gnirsG.lvFileType == LVSERVER) {
        rc = sendLvString(SVDELETE, name);
    } else {
        sprintf(buffer, "%s/%s", lvRoot, name);
        remove(name);
        rc = VME_OK;
    }
    return rc;
}

/*
 *+
 * FUNCTION NAME:
 * lvFileSeek
 *
 * INVOCATION:
 * lvFileSeek(seek, whence)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) seek (off_t)
 * (>) whence (int)
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Seek to an offset in a LV file
 *
 * DESCRIPTION:
 * 
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
int lvFileSeek(off_t seek, int whence)
{
    int rc;
    int argv[2];

    if (gnirsG.lvFileType == LVSERVER) {
        argv[0] = (int) seek;
        argv[1] = whence;
        rc = sendLvData(SVSEEK, 2 * sizeof(int), (char *) argv);
    } else {
        fseek(lvFp, seek, whence);
        rc = VME_OK;
    }
    return rc;
}

/*
 *+
 * FUNCTION NAME:
 * lvFileTell
 *
 * INVOCATION:
 * lvFileTell()
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None
 *
 * FUNCTION VALUE:
 * (int) Returns current seek pointer in file
 *
 * PURPOSE:
 * Determine value of file seek pointer
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
int lvFileTell()
{
    int where;

    if (gnirsG.lvFileType == LVSERVER) {
        sendLvData(SVTELL, 0, (char *) NULL);
        where = getLvCode();
        /* if error, get back VME_ERROR, which is 1, which is a possible
         * result of a ftell, but not one expected anywhere in this system.
         */
    } else
        where = ftell(lvFp);
    return where;
}

/*
 *+
 * FUNCTION NAME:
 * lvFileClose
 *
 * INVOCATION:
 * lvFileClose()
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Close a (LV) file
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
int lvFileClose()
{
    int rc;

    if (gnirsG.lvFileType == LVSERVER) {
        rc = sendLvData(SVCLOSE, 0, (char *) NULL);
    } else {
        rc = fclose(lvFp);
        if (rc != OK)
            rc = VME_ERROR;
        else {
            lvFp = NULL;
            rc = VME_OK;
        }
    }
    return rc;
}

/*
 *+
 * FUNCTION NAME:
 * lvFileWrite
 *
 * INVOCATION:
 * lvFileWrite(count, buffer)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) count (int)
 * (>) buffer (char *)
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Write data to a (LV) file
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
int lvFileWrite(int count, char *buffer)
{
    int rc;

    rc = sendLvData(SVWRITE, count, (char *) buffer);
    if (rc == VME_OK)
        rc = count;
    return rc;
}

/* Don't write the null */
/*
 *+
 * FUNCTION NAME:
 * lvFileString
 *
 * INVOCATION:
 * lvFileString(string)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) string (char *)
 *
 * FUNCTION VALUE:
 * (void) Returns 'void' and hence has no STATUS value
 *
 * PURPOSE:
 * Write a string to a (LV) file
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
void lvFileString(char *string)
{
    int len;

    len = strlen(string);
    sendLvData(SVWRITE, len, (char *) string);
}

/*
 *+
 * FUNCTION NAME:
 * lvFileSever
 *
 * INVOCATION:
 * lvFileSever()
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None
 *
 * FUNCTION VALUE:
 * (void) Returns 'void' and hence has no STATUS value
 *
 * PURPOSE:
 * Terminate the labview program on workstation
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
void lvFileSever()
{

    sendLvData(SVEXIT, 0, (char *) NULL);
}

/* Dump the system status to Labview */

BOOL lvDumpAllowed = FALSE;

/* toggles dumping of data to labview */
void lvD() {
    if (lvDumpAllowed)
        lvDumpAllowed = FALSE;
    else
        lvDumpAllowed = TRUE;
}

void lvFakeTemp();
/* Task which is spawned to dump the status */
int lvDump(int i1, int i2, int i3, int i4, int i5, int i6, int i7,
            int i8, int i9, int i10) {

    char buffer[LV_MAX_MSG_LEN];

    /* confined to seconds until we change the time handling */
    (void) time((time_t *)NULL);    /* be sure timer is started */

    for (;;) {
        taskDelay(gnirsG.clockRate);
        if (lvDumpAllowed ) {
            lvMotorDump();
            /* Dump pressure and temp as well */
            lvFakeTemp();
            /* Time must come last */
            sprintf(buffer, "Wt%6d\n", (int)time((time_t *)NULL));
            lvFileString(buffer);
        }
    }
}

    
