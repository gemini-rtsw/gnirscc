
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnCaptBufs.c,v 1.2 2009/05/27 19:32:46 fkraemer Exp $"
};




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
#include <irstd.h>
char tmp[80];
static int nextInBuf, nextOutBuf;
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
    int i, numInColDir, numInRowDir;
    int row, col, rowPos, colPos;
    int pixelCnt;


    numInRowDir = (MAXCAPTROWS) / arsize[arSizeIdx];
    numInColDir = (MAXCAPTCOLS) / arsize[arSizeIdx];
    numBufs = numInRowDir * numInColDir; 

    pixelCnt = arsize[arSizeIdx] * arsize[arSizeIdx];
    sprintf(tmp, " numbufs %d, numInRowDir %d, numInColDir %d, pixelCnt %d\n",
				numBufs, numInRowDir, numInColDir, pixelCnt);
  
    rowPos = colPos = 0; 	
    row = col = 0; 

    for (i=0; i<MAXCAPTBUFS; i++)
    {
	col = colPos * arsize[arSizeIdx];
	row = rowPos * arsize[arSizeIdx];
	if (i >= numBufs )
	{
	    captBufEmpty[i] = FALSE;
	    captBufFull[i] = TRUE;
	    captBufCol[i] = -1;
	    captBufRow[i] = -1;
	    captBufAddr[i] = -1;
	}
	else
	{
	    captBufEmpty[i] = TRUE;
	    captBufFull[i] = FALSE;
	    captBufCol[i] = col;
	    captBufRow[i] = row;
	    captBufAddr[i] = pixelCnt * i;

	    if ((colPos = ((colPos + 1) % numInColDir)) == 0)
		rowPos = ((rowPos + 1) % numInRowDir);

	    sprintf(tmp, "Buf=%d, col=%d, row=%d, colPos=%d, rowPos=%d addr %d\n", 
 				i, col, row, colPos, rowPos, captBufAddr[i]);
 
	}
    }
    nextInBuf = nextOutBuf = 0;
    return (OK);
    
}
void printCapt()
{
    int i;
    for (i=0;i<16;i++)
    {
	if(nextInBuf == i)
	    printf("%d, empty = %d, full = %d nextEmptyBuf\n",i,captBufEmpty[i],captBufFull[i]);
	if(nextOutBuf == i)
	    printf("%d, empty = %d, full = %d nextFullBuf\n",i,captBufEmpty[i],captBufFull[i]);
	else if(nextInBuf != i)
	    printf("%d, empty = %d, full = %d \n",i,captBufEmpty[i],captBufFull[i]);
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
    if (captBufEmpty[nextInBuf]==TRUE) 
    { 
	/* mutex begin Capt buffer */
	ret = nextInBuf;
	captBufEmpty[nextInBuf] = FALSE;
	nextInBuf = ((++nextInBuf) % numBufs);
	/* mutex end Capt buffer */

	return (ret);
    }
    else 
    {
	    DPRINT(t1,"No Empty buffers");
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
    captBufEmpty[emptyBuf] = TRUE;

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

    if (captBufFull[nextOutBuf])
    {
	/* mutex begin Capt buffer out */
	ret = nextOutBuf;
	captBufFull[nextOutBuf] = FALSE; /*  added by pbr 10-24-96*/
	nextOutBuf = ((++nextOutBuf) % numBufs);
	/* mutex end Capt buffer out */
	return (ret);
    }
    else
    {
	printf("No full buffers\n");
	return (ERROR);
    }

}
int clearBuf(int buf)
{
    while(nextOutBuf != buf)
	sleep(1,100000000);
    captBufFull[buf] = FALSE; 
    nextOutBuf = (++nextOutBuf) % numBufs; 
    captBufEmpty[buf] = TRUE;
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
    captBufFull[fullBuf] = TRUE;

}

