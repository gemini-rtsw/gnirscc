static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: drvrlink.c,v 1.2 2009/05/27 19:33:36 fkraemer Exp $"
};
/******************************************************************************
 * File: 	drvrlink.c
 * Purpose:	implementation of the standard INMOS link interface using the 
 *		VxWorkssocket Facilties
 * Author:	Diana Kennedy
 * Copyright:   Aura Inc - 1996
 * History:	
 *	11-Jan-1996 - created file - djk
 ******************************************************************************/
#include	<stdio.h>		/* Standard include file */
#include	<ctype.h>		/* Character classification stuff */
#include	<string.h>		/* String functions */
#include	<sys/types.h>		/* needed by gethostbyname */
#include	<sys/socket.h>		/* needed by gethostbyname */
#include	<netinet/in.h>	        /* needed for sockaddr defines */
#include	<errno.h>		/* needed by perror */
extern int errno;
#include        <logLib.h>              /* log messages */
#include "irstd.h"
#include "drvr_b014.h"

#define SERVER
#include "drvrlink.h"
extern int reset;
extern B014_MAP b014;
extern int selsleep(int secs,int usecs);
int b014_reset();
int b014_analyse();
UINT8 b014_error();
UINT8 b014_readstat();
UINT8 b014_writestat();

/*******************************************************************************
 * Routine:	ResetLink()
 * Purpose:	Reset the transputer system associated with this server.
 * Parameters:	void
 * Returns:	OK if the reset is successful, -1 otherwise.
 *
 ******************************************************************************/
int ResetLink()
{
      logMsg("Entering ResetLink\n",0,0,0,0,0,0);
      if (b014_reset() == ERROR){
        logMsg("ERROR in ResetLink\n",0,0,0,0,0,0);
        return ERROR;
      }else{
        reset = 0;
        logMsg("Leaving ResetLink\n",0,0,0,0,0,0);
        return OK;
      };
}

/*******************************************************************************
 * Routine:	TestError()
 * Purpose:	Check to see if error is set.
 * Parameters:	void
 * Returns:	zero if not set , one if set
 *
 ******************************************************************************/
UINT8 TestError()
{
    logMsg("Entering TestError\n",0,0,0,0,0,0);
    return (b014_error());
}

/*******************************************************************************
 * Routine:	WriteStat()
 * Purpose:	Check to see if write status is set.
 * Parameters:	void
 * Returns:	zero if not set , one if set
 *
 ******************************************************************************/
UINT8 WriteStat()
{
    logMsg("Entering WriteStat\n",0,0,0,0,0,0);
    return (b014_writestat());
}

/*******************************************************************************
 * Routine:	ReadStat()
 * Purpose:	Check to see if read status is set.
 * Parameters:	void
 * Returns:	zero if not set , one if set
 *
 ******************************************************************************/
UINT8 ReadStat()
{
    logMsg("Entering ReadStat\n",0,0,0,0,0,0);
    return (b014_readstat());
 
}

/*******************************************************************************
 * Routine:	AnalyzeLink()
 * Purpose:	Analyze the transputer system associated with this server.
 * Parameters:	void
 * Returns:	OK if the Analyze is successful, -1 otherwise.
 *
 ******************************************************************************/
int
AnalyzeLink()
{
    return (b014_analyse());

}

int b014_analyse()
{
    if (b014.baseAddr == 0)
	return ERROR;
    *b014.panalyse = 0;
    *b014.preset_error = 0;
    selsleep(0,100);
    *b014.panalyse = 1;
    selsleep(3,0);
    *b014.preset_error = 1;
    selsleep(3,0);
    *b014.preset_error = b014.reset_error = 0;
    selsleep(0,100);
    *b014.panalyse = b014.analyse = 0;
    selsleep(0,100);
    return OK;
}

int b014_reset()
{
    if (b014.baseAddr == 0)
	return ERROR;
    *b014.panalyse = b014.analyse = 0;
    *b014.preset_error = b014.reset_error = 0;
    selsleep(0,100);
    *b014.preset_error = 1;
    selsleep(3,0);
    *b014.preset_error = b014.reset_error = 0;
    selsleep(0,100);
    return OK;
}

UINT8 b014_error()
{
   UINT8 error_bit;

    error_bit = (UINT8)(*b014.preset_error & STATUS_BIT_0);
    return (error_bit);
 }

UINT8 b014_readstat()
{
   UINT8 rstat_bit;

    rstat_bit = (UINT8)(*b014.pisr & STATUS_BIT_0);
    return (rstat_bit);
 }

UINT8 b014_writestat()
{
   UINT8 wstat_bit;

    wstat_bit = (UINT8)(*b014.posr & STATUS_BIT_0);
    return (wstat_bit);
 }






