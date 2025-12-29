static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: reader.c,v 1.2 2009/05/27 19:33:36 fkraemer Exp $"
};
/*
 * reader.c
 *
 * read from channel, write to stdout
 * (output socket).
 *
 */
#include <vxWorks.h>
#include <stdio.h>
#include <selectLib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <signal.h>
#include <intLib.h>
#include <logLib.h>
#include <taskLib.h>
#include	<unistd.h>
#include	"irstd.h"
extern int errno;
#include	"drvr_defs.h"
#include	"drvr_b014.h"
#define		NAACCIOV
#include	"drvr_vars.h"
#include	"drvrlink.h"
#include 	"fchanio.h"
#define BIOISR
#include        "bioIsr.h"

extern B014_MAP b014;
int cmnd_sock;
int control (int sock1);
VOID selsleep(int secs, int time);


VOID reader(socket,cmnd)
int	socket,cmnd;
{

    int	n;
    int nw;
    int readlock;


    cmnd_sock = cmnd;
    
/* enable read interrupts */

    *b014.pintrupt_enable |= R_INTERRUPT_ENABLE;
    *b014.pisr = STATUS_BIT_1;


    while (1)
    {
	errno = 0;
         
        n = IBcount;

        if ( n == 0){
            selsleep(0,TIMEREAD(250));
	  };

/* locking the data going into the buffer allows for a more uniform socket flow
 *          less dependence on amounts of data and overlapping problems
 */

        readlock = intLock();
        n = IBcount;

        if (IBcount > Buf_MAX){
	 logMsg("Too much data InBuff count = %d - IBin %d\n",n,IBin,0,0,0,0);
          n = Buf_MAX;
         };

        if ( n != 0 ){

           if ((nw = write(socket, (char *)InBuff, n)) <= 0)
	      {
	    logMsg("n=%d, nw=%d\n",n,nw,0,0,0,0);
	    perror("in reader after write to socket.");
	    break;
	     };
           IBin = IBcount = 0;
 	 };
         intUnlock(readlock);
/* re-open the interruptions 
 *    bioIsr will not accept interrupts for both reads and writes
 *    the following delay flips control to the writer
 */


        taskDelay(NO_WAIT);
    }

    close(socket);
    logMsg("closing reader\n",0,0,0,0,0,0);
    return;
}





