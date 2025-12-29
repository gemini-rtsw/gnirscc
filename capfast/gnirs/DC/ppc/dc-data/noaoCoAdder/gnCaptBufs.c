
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnCaptBufs.c,v 1.2 2009/05/27 19:32:44 fkraemer Exp $"
};


#define DEBUG

extern int dqdebug;
/****************************************************************************
 * File: 	NaacCaptureBufs.c
 * Purpose: 	Creates and Initializes the Datacube Capture buffer routines
 *	       	    Initializes and controls the circular capture buffer used 
 *		    by Naac to get data into the system.  
 *
 * Routines:	initCaptBufs -Initializes the Capture Buffer system 
 *                            for a data taking run 
 *		getEmptyBuf -gets the number of the next empty 
 *                           location in the circular capture buffer
 *		getFullBuf -	Gets the number of the next full location 
 *                              in the circular capture buffer
 *		putEmptyBuf - mark a buffer as empty
 *		putFullBuf - mark a buffer as used
 *
 * Author:      Nick C Buchholz
 * Copyright:   Aura Inc.  All rights reserved.
 * Date:  	13 Aug 1996
 * History:
 *	Created - 12 Aug 1996 - ncb - started file to handle capture buffer ring
 *	modified - 29 Nov 1999 - redone to handle buffers in new CoAdder Board
 *
 ***************************************************************************/
#include <sys/types.h>
#include <sys/times.h>

#include <stdio.h>

#include <gnerrno.h>

#include "gnDCADefs.h"

#undef MAIN
#include "gnDCAVars.h"
#include "semLib.h"
#include <irstd.h>
static SEM_ID mutex=NULL;
char tmp[80];
static int nextInBuf, nextOutBuf;
void gemLogMsg(int level, const char *pFormat, ...) ;
/****************************************************************************
 * Routine:	initCaptBufs
 * Purpose:	Initializes the Capture Buffer system for a data taking run 
 *		This routine should be run once for each data run. if the ROI
 *		size has changed from the previous data run it will
 *		reinitialize the buffer sizes and locations otherwise it does
 *		nothing . It uses a single large memory buffer to implement a
 *		circular capture buffer.   
 * Parameters:	arSizeIdx - int - the index into the detector ROI size array
 * Returns:     int - (OK) if all is correct
 *		      (ERROR) if any failures
 *****************************************************************************/

int initCaptBufs(int arSizeIdx)
{
    int bank,colMax,rowMax,c,r,rowAddr,buf;
    int i;
    int pixelCnt;
    int detector;
    int time = 0;
    int status = OK;
    int imageRows,imageCols;

   /*  wait for coadder to clear out */
    if(mutex)
      {
	while ((emptyBufs < numBufs) && (time < 60))
	  {
	    sleep(1,0);
	    time ++;
/* 	    gemLogMsg(3,"waiting for capt bufs\n"); */
	  }
	if (time >= 60)
	  status = ERROR;
	  
      }
    else
      mutex = semMCreate(SEM_Q_PRIORITY);

    if((strcmp(A2,detType) == 0) || (strcmp(A3_A,detType) == 0))
      { 
	imageCols = arsize[arSizeIdx] ;
	imageRows =  arsize[arSizeIdx];
	if(strcmp(A2,detType) == 0)
	  detector = 2;
	else	  
	  detector = 3;
      }
    else if (strcmp(A3_B,detType) == 0)
      {
	imageCols =  a3csize[arSizeIdx];
	imageRows = a3rsize[arSizeIdx] ;
	detector = 3;
      }	
    else
      {
	gemLogMsg(0,"wrong array type\n");
	gemLogMsg(10,"a2 = %s, a3a = %s, a3b = %s, detType = %s",A2,A3_A,A3_B,detType);
	status = ERROR;
      }	

    if(status == OK)
      {
	semTake(mutex,WAIT_FOREVER);
	
	/* coadder setup depends on which array*/
/* 	printf("initCaptBufs detector = %s=n",detType); */

	    rowMax = (MAXCAPTROWS) / imageRows;
	    colMax = (MAXCAPTCOLS) / imageCols;
	      gemLogMsg(10,"1024 x 1024 rowmax = %d colmax = %d  MAXCAPTROWS = %d, MAXCAPTCOLS %d cols %d rows %d\n",rowMax,colMax,MAXCAPTROWS,MAXCAPTCOLS,imageCols,imageRows);
	    pixelCnt = imageRows * imageCols;
	    c = 0;
	    buf = 0;
	    for (bank=0; bank<MAXCAPTBANKS; bank++)
	      {
		r=0;
		for (r=0 ;r<rowMax; r++)
		  {
		    rowAddr = bank*MAXCAPTCOLS*MAXCAPTROWS + 
		      r * imageRows * MAXCAPTCOLS;
				/*  for(c=0; c<colMax; c++) */
				/* 	    {		 */	
		    captBufEmpty[buf] = TRUE;
		    captBufFull[buf] = FALSE;
		    captBufAddr[buf] = pixelCnt * r + bank*MAXCAPTCOLS*MAXCAPTROWS ;
		    captBufTAddr[buf] = rowAddr + c*imageCols;
		    buf ++;
		    
				/* 	    } */
		   }
	      }


	emptyBufs = numBufs = buf;
	for (i = buf;i<MAXCAPTBUFS;i++)
	  {
	    captBufEmpty[i] = FALSE;
	    captBufFull[i] = TRUE;
	    captBufAddr[i] = -1;
	    captBufTAddr[i] = -1;
	    
	  }
	nextInBuf = nextOutBuf = 0;
	semGive(mutex);
      }
  /*   printCapt(); */
    return status;
    
}
int emptyBuffers()
{
  return emptyBufs;
}
void printCapt()
{
  int i;
  gemLogMsg(10,"numBufs = %d, emptyBufs = %d\n",numBufs, emptyBufs);
  for (i=0;i<numBufs;i++)
    {
	if(nextInBuf == i)
	    printf("%d, empty = %d, full = %d add1 %d, add2 %d nextEmptyBuf\n",i,captBufEmpty[i],captBufFull[i],captBufAddr[i],captBufTAddr[i]);
	if(nextOutBuf == i)
	    printf("%d, empty = %d, full = %d add1 %d, add2 %d nextFullBuf\n",i,captBufEmpty[i],captBufFull[i],captBufAddr[i],captBufTAddr[i]);
	else if(nextInBuf != i)
	    printf("%d, empty = %d, full = %d  add1 %d, add2 %d \n",i,captBufEmpty[i],captBufFull[i],captBufAddr[i],captBufTAddr[i]);
    }
}
/****************************************************************************
 * Routine:	getEmptyBuf
 * Purpose:	gets the number of the next empty location in the circular
 *		capture buffer. uses four or sisxteen locations depending 
 *		on the size of the incoming buffer. returns -1 if no buffer 
 * 		is available and empty.  The caller is responsible for waiting
 * 		and trying again later. returns the index into the location
 *		list of the next empty buffer if successful. 
 * Parameters: none
 * Returns:  	the index of the next location or -1 if no location is
 *		available.
 *****************************************************************************/
int getEmptyBuf(void)
{
    int ret;
#ifdef DEBUG
    int t1 = 1;
#endif
   /*  printf("getEmptyBuf waiting for mutex\n"); */
	semTake(mutex,WAIT_FOREVER);
  /*   printf("getEmptyBuf got mutex\n"); */
    if (captBufEmpty[nextInBuf]==TRUE) 
    { 
		/* mutex begin Capt buffer */
		ret = nextInBuf;
		captBufEmpty[nextInBuf] = FALSE;
		nextInBuf = ((++nextInBuf) % numBufs);
		/* mutex end Capt buffer */
		emptyBufs--;
		semGive(mutex);
		return (ret);
    }
    else 
    {
	    DPRINT(t1,"No Empty buffers");
		semGive(mutex);
		return (ERROR);
    }

}

/****************************************************************************
 * Routine:	putEmptyBuf
 * Purpose:	returns an empty buffer to the list of available buffers 
 * Parameters:  emptyBuf - int - a pointer to the buffer to be returned.
 * Returns:  	void
 *****************************************************************************/
void putEmptyBuf(int emptyBuf)
{
	/*     printf("\t\tputEmptyBuf %d\n",emptyBuf); */ 
	semTake(mutex,WAIT_FOREVER);
	captBufEmpty[emptyBuf] = TRUE;
	emptyBufs++;
	semGive(mutex);
}

/****************************************************************************
 * Routine:	getFullBuf
 * Purpose:	Gets the number of the next full location in the circular
 *		capture buffer. uses four or sixteen locations depending 
 *		on the size of the incoming data. returns -1 if no buffer 
 * 		is available and Full.  The caller is responsible for waiting
 * 		and trying again later. returns the index into the location
 *		list of the next full buffer if successful. 
 * Parameters: none
 * Returns:  	the index of the next location or -1 if no location is
 *		available.
 *****************************************************************************/
int getFullBuf(void)
{
    int ret;

  semTake(mutex,WAIT_FOREVER);
    if (captBufFull[nextOutBuf])
    {
	/* mutex begin Capt buffer out */
	ret = nextOutBuf;
	captBufFull[nextOutBuf] = FALSE; /*  added by pbr 10-24-96*/
	nextOutBuf = ((++nextOutBuf) % numBufs);
	/* mutex end Capt buffer out */ 
	semGive(mutex);
	return (ret);
    }
    else
    {
	gemLogMsg(1,"No full buffers\n");
	  semGive(mutex);
	return (ERROR);
    }

}
int clearBuf(int buf)
{
    while(nextOutBuf != buf)
	sleep(1,100000000);
	semTake(mutex,WAIT_FOREVER);
    captBufFull[buf] = FALSE; 
    nextOutBuf = (++nextOutBuf) % numBufs; 
    captBufEmpty[buf] = TRUE;
  semGive(mutex);
    return OK;
}

/****************************************************************************
 * Routine:	putFullBuf
 * Purpose:	puts a buffer into the list of Full buffers
 * Parameters:  fullBuf - int - a pointer to the buffer to be put on the list.
 * Returns:  	void
 *****************************************************************************/
void putFullBuf(int fullBuf)
{
/*     printf("\t\tputFullBuf %d\n",fullBuf); */
  semTake(mutex,WAIT_FOREVER);
    captBufFull[fullBuf] = TRUE;
	semGive(mutex);

}

