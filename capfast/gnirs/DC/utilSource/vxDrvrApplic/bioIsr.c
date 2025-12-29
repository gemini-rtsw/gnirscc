static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: bioIsr.c,v 1.2 2009/05/27 19:33:35 fkraemer Exp $"
};
/*
 * bioIsr.c
 *
 * catch interrupts from the B014 - either into or out of B014
 *
 */
#include <vxWorks.h>
#include <stdio.h>
#include <selectLib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sysLib.h>
#include <logLib.h>
#include <signal.h>
#include	<unistd.h>
#include <intLib.h>
#include	"irstd.h"
extern int errno;
#include	"drvr_defs.h"
#include	"drvr_b014.h"
#define		NAACCIOV
#include	"drvr_vars.h"
#include	"drvrlink.h"
#include 	"fchanio.h"
#define BIOISR
#include         "bioIsr.h"

extern B014_MAP b014;

VOID bioIsr( )
{
   int lock;

/* check the read status bit */

   if (*b014.pisr & STATUS_BIT_0)
     {

      lock = intLock();
     *b014.pintrupt_enable &= R_INTERRUPT_DISABLE;
     *b014.pisr = STATUS_NOT_BIT_1;
 
      InBuff[IBin] = *b014.pidr;
      IBcount++;
      IBin++;

      if (IBin >= Buf_MAX){
         IBin = 0;
       };

     *b014.pintrupt_enable |= R_INTERRUPT_ENABLE;
     *b014.pisr = STATUS_BIT_1;

      intUnlock(lock);
    };

/* check the write status bit */

    if (*b014.posr & STATUS_BIT_0) {

          lock = intLock();
          *b014.pintrupt_enable &= W_INTERRUPT_DISABLE;
          *b014.posr = STATUS_NOT_BIT_1;

         if (OBcount > 0 ){
           *b014.podr = OutBuff[OBout];
            OutBuff[OBout] = 0x0000;
            OBout++;
            OBcount--;
            if (OBout >= Buf_MAX)
                OBout = 0;
           if (OBcount >= 0) {
             *b014.pintrupt_enable |= W_INTERRUPT_ENABLE;
             *b014.posr = STATUS_BIT_1;
 	    };
            if (OBcount == 0){
              OBflag = TRUE;
            };
	  }else{
            OBout = 0;
            OBin = 0;
            OBcount = 0;
	  }
          intUnlock(lock);
       };

 
 return;
}  











