static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: writer.c,v 1.2 2009/05/27 19:33:36 fkraemer Exp $"
};
/*
 * writer.c
 *
 * read from stdin (input socket),
 * write to channel
 *
 */
#include <vxWorks.h>
#include <stdio.h>
#include <in.h>
#include <inetLib.h>
#include <selectLib.h>
#include <sockLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <string.h>
#include <fioLib.h>
#include <sys/types.h>
#include <sys/times.h>
#include <signal.h>
#include	<unistd.h>
#include <intLib.h>
#include <logLib.h>
#include	"irstd.h"
#define          BIOISR
#include        "bioIsr.h"
extern int errno;
#include	"drvr_defs.h"
#include	"drvr_b014.h"
#define		NAACCIOV
#include	"drvr_vars.h"
#include	"drvrlink.h"
#include	"fchanio.h"
#include        "netConsts.h"
extern int dofr;
extern int dofw;
extern int twid;
extern int trid;

extern B014_MAP b014;
int	timeout = 50;
int senddown(int sock);
int control (int sock1);



VOID writer(socket,cmnd_sock,tout,tridd)

int	socket;
int     cmnd_sock;
int	tout;
int     tridd;


{

        fd_set ready;
        struct timeval to;
        int width;
        int status;




/* setup the FD masks for the select */

        width = (cmnd_sock > socket) ? cmnd_sock : socket;
        width++;

/* when a fd is ready then go and process it */

        while(1) {

/* check if the sockets are still alive */
       if (closeFLAG == TRUE){
          close (cmnd_sock);
          close (socket);
          taskDelete(trid);
          close (dofr);
          close (dofw);
          closeFLAG = FALSE;
          twid = 0;
          trid = 0;
          exit(1);
	};
        
          errno = 0;
          OBloop = FALSE;
	
          if (OBin == Buf_MAX) {
             while (OBcount > 0 )
                 OBloop = TRUE;
             OBloop = FALSE;
             OBin = OBout = 0;
	   };

     
          taskDelay(NO_WAIT);


/* see if there is anything ready to be processed */

          FD_ZERO(&ready);
          FD_SET(cmnd_sock,&ready);
          FD_SET(socket,&ready);
          to.tv_sec = 0;
          to.tv_usec = tout;
          timeout = tout;

          status = select(width, &ready, (fd_set *) 0, (fd_set *) 0, &to);

          if (status < 0) {
              logMsg("select status bad \n",0,0,0,0,0,0);
	    };

          if (FD_ISSET(cmnd_sock,&ready))
              control(cmnd_sock);

          if (FD_ISSET(socket,&ready)) {
              if((senddown(socket)) == ERROR){
                 break;
              };
           };

        }; /* end of while */

        close(cmnd_sock); 
  	close(socket);
	logMsg("closing writer\n",0,0,0,0,0,0);
	return;
 }

int  senddown(socket)
int  socket;
  {

    int n;
    int lock;

  
     if ((n = read(socket, (char *)(OutBuff + OBin),(Buf_MAX - OBin)))
               < 0) 
	   {
      	perror("read from socket failed in writer\n");
        return ERROR;
	    };

          
     OBin = OBin + n;
     OBcount = OBcount + n;

     while (OBcount >= Buf_MAX && timeout > 0) {
          taskDelay(TIMEWRITE(250));
          timeout -= TIMEWRITE(250);
     };

/* if the status is set and have something to give - write it 
      and activate interrupts
*/
           if (OBcount > 0){
              if (*(b014.posr) & STATUS_BIT_0 ){
              
               lock = intLock();            /* protect the update of data */

                *b014.podr = OutBuff[OBout];
                *b014.posr = STATUS_BIT_1;    
                *b014.pintrupt_enable |= W_INTERRUPT_ENABLE;                

               OutBuff[OBout] = 0x0000;
               OBout++;
               OBcount--;


               if (OBout == Buf_MAX) 
                   OBout = 0;

               intUnlock(lock);
	     };
           };
	
    return OK;
  }







