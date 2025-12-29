static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: naacsrvr.c,v 1.2 2009/05/27 19:33:36 fkraemer Exp $"
};
/*******************************************************************************
 * PROGRAM:     naacsrvr
 * File:	naacsrvr.c
 * Purpose:	server for handling io from transputer network run as part of
 *		   naac_cntrl program.  
 * Author:	Diana Kennedy
 * History:
 *	11-Jan-1996 - created file - djk
 *
 ******************************************************************************/
#define __PROTOTYPE_5_0
#include        <vxWorks.h>
#include        <vme.h>
#include	<fcntl.h>
#include        <selectLib.h>
#include        <intLib.h>
#include        <sysLib.h>
#include        <taskLib.h>
#include	<sys/types.h>
#include	<sys/times.h>
#include	<stdio.h>
#include	<errno.h>
extern int errno;
#include	<string.h>
#include 	<unistd.h>
#include        <logLib.h>
#include	"irstd.h"
#include	"drvr_defs.h"
#include        "drvr_b014.h"
#define		NAACCIOV
#include	"drvr_vars.h"
#include	"drvrlink.h"

#include <timers.h>
int reset;
int dofr;
int dofw;
B014_MAP b014;
extern int closeFLAG;
extern int OBflag;
extern int IBin;
extern int IBout;
extern int trid;
extern int twid;
void b014_save();
void b014_restore();
VOID writer(int sock1, int timeout);
VOID reader(int sock1, int sock2);
VOID bioIsr();

void nsleep (int a, int b)
{
   struct timespec to;
    to.tv_sec = a;
    to.tv_nsec = b;
    nanosleep(&to, NULL);
    return;
}

/*******************************************************************************
 * Routine:	naacsrvr
 * Purpose:	starts reader and writer processes for client to b014 comm.
 * Parameters:  cmskt - int - a socket to communicate over
 * Returns:	Error or OK
 *
 ******************************************************************************/
int
naacSrvr(cmmskt, cmdskt,clientAddr)
int cmmskt;
int cmdskt;

struct sockaddr_in * clientAddr;
{
    static int oldcmm;

    static int oldcmd;

    prtdebug(0xFFFFFFFF, "mapping b014\n");
    if (b014.initialized == FALSE){
       oldcmm = 0;
       oldcmd = 0;
     };

/* initialize the b014 and interrupt levels in b014 and vxworks */
  /*   nsleep (2,0); */
    if (b014_init(INTERRUPT_LEVEL, INTERRUPT_NUM) == ERROR)
    {
	LOG("can't map b014 (55,naacsrvr.c)");
	return (ERROR);
    }
    logMsg("going to spawn tasks \n",0,0,0,0,0,0);

/* delete any tasks left from a previous client */
    closeFLAG = FALSE;
    IBin = IBout = 0;

    if (trid != 0){
         taskDelete(trid);
         close(dofr);
         logMsg("deleted reader task \n",0,0,0,0,0,0);
       };

    if (twid != 0){
         taskDelete(twid);
         close(dofw);
         logMsg("deleted writer task \n",0,0,0,0,0,0);
       };

    if (oldcmm != cmmskt && oldcmm != 0){
        close(oldcmm);
        oldcmm = cmmskt;
      };

    if (oldcmd != cmdskt && oldcmd != 0){
        close(oldcmd);
        oldcmd = cmdskt;
      };


	dofr = open("/source/irxp/fire/sun/reader.file",O_RDWR,0644);

 	if ((trid = taskSpawn("treader",SERVER_WORK_PRIORITY_R,0, 
             SERVER_STACK_SIZE,(FUNCPTR)reader,cmmskt,0,0,0)) == ERROR){ 
           perror("reader taskSpawn"); 
           close((int)dofr); 
           return (ERROR); 
         }; 

 	dofw = open("/source/irxp/fire/sun/writer.file", O_RDWR,0644); 

/* spawn the writer task */
       if ((twid = taskSpawn("twriter",SERVER_WORK_PRIORITY_R,0, 
           SERVER_STACK_SIZE, 
          (FUNCPTR)writer,cmmskt,cmdskt,100,trid)) == ERROR){ 
           perror("writer taskSpawn"); 
           close((int)dofw); 
           return (ERROR); 
         }; 
        logMsg("spawned reader and writer \n",0,0,0,0,0,0); 
  
    return(OK) ;

}

int
selsleep(secs,time)
int     secs;
int	time;
{
    struct timeval tv;
    
    tv.tv_sec = secs;
    tv.tv_usec = time;
    if (select(0, 0, 0, 0, &tv) < 0)
      {
	perror("select");
        return (ERROR);
       };
 return (OK);
}

/*
 * This procedure maps the b014 registers
 * into user local memory space
 */
int
b014_init(intLevel, intNumber)
   int   intLevel;
   int   intNumber;

{
    extern int	errno;

    int lockKey;
    UINT8 intLevel8;
    UINT8 intNumber8;

   if (b014.initialized == TRUE){
      return (OK);
    };

    errno = 0;

/* map the b014 registers into low space in vxworks*/

   if (sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO,(char *)DEFAULT_BASE, 
       &b014.baseAddr) == ERROR){
	perror("b014_init: bus to local address");
	return (ERROR);
    }

/* establish addressibility to registers */

    b014.pisr = (UINT8 *) b014.baseAddr + ISR_OFF;
    b014.posr = (UINT8 *) b014.baseAddr + OSR_OFF;
    b014.pidr = (UINT8 *) b014.baseAddr + IDR_OFF;
    b014.podr = (UINT8 *) b014.baseAddr + ODR_OFF;
    b014.preset_error = (UINT8 *) b014.baseAddr + RESET_ERROR_OFF;
    b014.panalyse = (UINT8 *) b014.baseAddr + ANALYSE_OFF;
    b014.ptram_errs = (UINT8 *) b014.baseAddr + TRAM_ERRORS_OFF;
    b014.pintrupt_enable = (UINT8 *)b014.baseAddr + INTRUPT_ENABLE_OFF;
    b014.pintrupt_level = (UINT8 *)b014.baseAddr + INTRUPT_LEVEL_OFF;
    b014.pintrupt_statid = (UINT8 *)b014.baseAddr + INTRUPT_STATID_OFF;


/* set the interrupt level and number(index into vector table) for
   both the b014 and vxworks
*/

    intLevel8 =  intLevel & INTERRUPT_MASK;
    intNumber8 = intNumber;

    if (intLevel) {

/* put the interrupt routine into the vector table */

       if (intConnect (INUM_TO_IVEC(intNumber), bioIsr, 0) == ERROR){
           perror("b014_init: installing ISR");
           return (ERROR);
	 };

       intLevel &= 0x07; /* make sure the interrupt level is valid */

       logMsg("level %d %c, number %d %c \n",intLevel,intLevel8,
                 intNumber,intNumber8,0,0);

/* enable the interrupt level */

       if (sysIntEnable (intLevel) == ERROR){
           perror ("b014_int: enabling interrupt");
           return (ERROR);
         };

       logMsg("level %d %c, number %d %c \n",intLevel,intLevel8,
                 intNumber,intNumber8,0,0);

/* set the interrupt level and number into the b014 registers */

       *b014.pintrupt_level = intLevel8;
       *b014.pintrupt_statid = 0x00;
       *b014.pintrupt_statid |= intNumber8;

       logMsg("level %d %c, number %d %c \n",intLevel,intLevel8,
                 intNumber,intNumber8,0,0);
       lockKey = intLock();


/* disable read & write interrupts */  
          
       *b014.pintrupt_enable = 0x00;    
       *b014.posr &= STATUS_NOT_BIT_1;
       *b014.pisr &= STATUS_NOT_BIT_1;

       intUnlock (lockKey);
       
       logMsg("reset interrupt enable bits \n",0,0,0,0,0,0);

     };

    OBflag = FALSE;
    closeFLAG = FALSE;

    b014.initialized = TRUE;
    logMsg("returning \n",0,0,0,0,0,0);

    return (OK);
}


/*
 * This procedure saves b014 registers
 */

void b014_save( )

{

/* save b014 registers */

    b014.isr = *b014.pisr;
    b014.osr = *b014.posr;
    b014.intrupt_enable = *b014.pintrupt_enable;
    b014.intrupt_level = *b014.pintrupt_level;
    b014.intrupt_statid = *b014.pintrupt_statid;

    return;
  }

/*
 * This procedure restores b014 registers
 */

void b014_restore( )

{

/* restore b014 registers */

    

    if (*b014.pintrupt_level != b014.intrupt_level)
      *b014.pintrupt_level = b014.intrupt_level;

    if ((*b014.pintrupt_statid & (STATUS_BIT_1 | STATUS_BIT_0)) != 
         INTERRUPT_NUM)
      *b014.pintrupt_statid = INTERRUPT_NUM;

    *b014.pisr = b014.isr;
    *b014.posr = b014.osr;
    *b014.pintrupt_enable = b014.intrupt_enable;

    return;
  }


