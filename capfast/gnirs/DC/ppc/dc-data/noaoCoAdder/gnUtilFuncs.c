
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnUtilFuncs.c,v 1.2 2009/05/27 19:32:45 fkraemer Exp $"
};
/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename:
 *	gnUtilFuncs.c

 * Description:
 *	Defines and declares variables special functions used by many
 *		data coadder routines
 *
 * Function Names:
 *
 *	long setRegisters(tDCARegs *preg,int type)
 *		load all coadder registers depending on the type
 *	 	only setup for dma currently
 *
 *	void loadVarRegs(tDCARegs *dregs)
 *		LOad a set of registers to values needed for Data taking
 *
 *	int dcaLoadRegister(int *BaseAddr, int Offset, int Value)
 *		Loads a datacCoAdder register at address BaseAddr+Offset
 *		    with value directly. Does no checking that the  
 *		    corresct value got loaded, returns OK
 *
 *	int dcaInqRegister(int *BaseAddr, int Offset)
 *		Reads the value from a dataCoadder register at address
 *		    BaseAddr+Offset. Does no checking to see if the
 *		    address is valid. Returns the value read.
 *      void read32_surface()
 *                 Description: Reads a portion of image memory and 
 *                 prints data to console.
 *      read32Surface() 
 *                 Read an image off the image memory and store it in a local
 *                 array
 *      sleep (int a,int b)
 *                pause for a specified time
 * Dependencies:
 *	None
 *
 * Author: Nick C Buchholz
 * 
 * History:
 *	02-May-1996 - Original version - gh
 *	17-Jul-1996 - added comments and #include files - ncb
 *	17-Novl-1999- stripped rotuines not needed for NAOA coadd baoard control - ncb
 *
 ***************************************************************************/
#include <stdio.h>
#include <time.h>

#include <sys/types.h>
#include <sys/times.h>
#include <semLib.h>
#include <vxWorks.h>
#include <taskLib.h>

#include "gnDCADefs.h"
#undef	MAIN
#include "gnDCAVars.h"

#include <irstd.h>
#include <bc350Time.h>
char tmp[80];
extern SEM_ID dcaRegLock;

long setRegisters(tDCARegs *preg,int type)
{
    int i;
    switch (type)
    {
      case COADD:
	break;
      case DESCRAMBLE:
	break;
      case DMA:
	
	dcaLoadRegister(PXFERSTART,preg->pXferStart);
	dcaLoadRegister(NUMROWS,preg->numRows-1);
	dcaLoadRegister(NUMCOLS,preg->numCols-1);
	dcaLoadRegister(PDMASTART,preg->pDMAStart);
	dcaLoadRegister(SZDMABLOCK,preg->szDMABlock); 

#ifdef TRACE
	printf("PXFERSTART 0x%x\n",preg->pXferStart);
	printf("NUMROWS    %d\n",preg->numRows-1);
	printf("NUMCOLS    %d\n",preg->numCols-1);
	printf("PDMASTART  0x%x\n",preg->pDMAStart);
	printf("SZDMABLOCK %d\n",preg->szDMABlock);
#endif	

	/* start transfer*/
	dcaLoadRegister(CMNDSTARTDMA,1);
	break;
      default:
	i++;
    }
    return OK;

}


/*************************************************************************
 * loadVarRegs() - LOad a set of registers to values needed for Data taking
 *
 * Parameters -
 *	tDCARegs *dRegs - a pointer to the desired Register set values
 *
 * Returns - 
 *	void - it ought to return status - OK or error number but there is 
 *		no readback of the registers
 *
 *************************************************************************/
#if 0
void loadVarRegs(tDCARegs *dRegs)
{
    tDCARegs *aRegs = oSystem.pDCARegs;

    DPRINT(dcaDebug,"in loadVarRegs\n");
    
    semTake(dcaRegLock, WAIT_FOREVER);
    aRegs->pCoAddStart = dRegs->pCoAddStart;
    oSystem.regCopy.pCoAddStart = dRegs->pCoAddStart;
    aRegs->pixelCnt = dRegs->pixelCnt;
    oSystem.regCopy.pixelCnt = dRegs->pixelCnt;

    aRegs->pDesStart = dRegs->pDesStart;
    oSystem.regCopy.pDesStart = dRegs->pDesStart;
    aRegs->rowCnt = dRegs->rowCnt;
    oSystem.regCopy.rowCnt = dRegs->rowCnt;
    aRegs->colCnt = dRegs->colCnt;
    oSystem.regCopy.colCnt = dRegs->colCnt;

    aRegs->pXferStart = dRegs->pXferStart;
    oSystem.regCopy.pXferStart = dRegs->pXferStart;
    aRegs->numRows = dRegs->numRows;
    oSystem.regCopy.numRows = dRegs->numRows;
    aRegs->numCols = dRegs->numCols;
    oSystem.regCopy.numCols = dRegs->numCols;

    semGive(dcaRegLock);
}
#endif
/*************************************************************************
 * dcaLoadRegister() - modify a dataCoAdder register
 *
 *Inputs - baseaddr 
            offset
	    value
 *return - status
 *
 *************************************************************************/

int
dcaLoadRegister(int Offset, int Value)
{
#ifdef DEBUG
    int t1 = 0;
#endif
    int *Addr;
    int status;
    
    semTake(dcaRegLock, WAIT_FOREVER);
	/* calculate Hardware Address and load hardware register */
    Addr = ((int *) (((int *)(oSystem.pDCARegs )) + Offset));     
    *Addr = Value;			  
 
/*
 *  	status = vxMemProbe((char *)Addr, VX_WRITE, 4, (char *) &Value);
 * 	if (status != OK ) {
 * 		printf("vxMemProbe failed addr=0x%x - val=0x%x\n\n",(unsigned int)Addr,Value);
 * 		}
 */

 
    /* calculate Hardware Address and load hardware register */
    Addr = ((int*) (((int *)(&(oSystem.regCopy) ))+Offset)); 
    *Addr = Value;

    semGive(dcaRegLock);
    return (status);

}

/*************************************************************************
 * dcaInqReister() - read a dataCoAdder register
 *
 *Inputs - Offset - register number 0-N
	   
 *return - Value in Register
 *
 *************************************************************************/
int dcaInqRegister(int Offset)
{
    int Val;
    int *Addr;

    Addr = ((int *)(((int *)&oSystem.regCopy) + Offset));
    
    Val = *Addr;

    return (Val);

}


