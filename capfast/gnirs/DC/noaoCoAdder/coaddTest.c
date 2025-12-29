#include "gnDCADefs.h"
#include "gnDCAVars.h"
#include <sysLib.h>
#include <taskLib.h>
#include <vme.h>
#include "DCA.h"

#include "coaddTest.h"

extern coAdSems sem;

long dmabuffer[1024][1024];
long *vmebuffer;

int transferBuffer();
int coaddBuffer();
int unscrambleBuffer();

struct framectrl 
{
    int add;
    int first;
    int last;
} fctl = {1, 1, 0};

int dcainit()
{
    return DCAInit();
}

int DCAInit()
{
/*      int  *buf,*buf1; */
    int result;
    sem.transFrame = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
    sem.coAdFrame = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
    sem.unScrambleFrame = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
#ifdef LONGIO
    if( (result = sysBusToLocalAdrs( VME_AM_EXT_USR_DATA, (void *)DCA_BASE,
				 (void *)    &oSystem.pDCARegs ) ) != OK )
#else  
      printf("\n\n\n\nOK\n");
    if( (result = sysBusToLocalAdrs( VME_AM_SUP_SHORT_IO, (void *)DCA_BASE,
				 (void *)    &oSystem.pDCARegs ) ) != OK )
   
#endif    
    {
	printf("DCAInit: Cannot convert VME address to local.\n");
	return( result ) ;
    } 
    printf("DCA_BASE = %x, vme = %x",DCA_BASE, (unsigned) oSystem.pDCARegs);
    /* start interrupts*/
    interruptInit();
    /*start tasks*/
     taskSpawn("tTransfer",60,VX_FP_TASK,10000,transferBuffer,NULL,0,0,0,0,0,0,0,0,0);
     taskSpawn("tCoadd",60,VX_FP_TASK,10000,coaddBuffer,NULL,0,0,0,0,0,0,0,0,0);

     taskSpawn("tunscramble",60,VX_FP_TASK,10000,unscrambleBuffer,NULL,0,0,0,0,0,0,0,0,0);
     /*     buf = (int *)memalign(sizeof(long),2000000);*/
/*      sysLocalToBusAdrs(VME_AM_EXT_USR_DATA,buf,&buf1); */
/*      if(sysLocalToBusAdrs(VME_AM_EXT_USR_DATA,(char *)buf,(char **)&buf1) == OK) 
	 
	 printf("local = %x   address = %x\n",buf,buf1);
     else
	 printf("ERROR\n");
*/

     if(sysLocalToBusAdrs(VME_AM_EXT_USR_DATA,(char *)dmabuffer,(char **)&vmebuffer) == OK)
	 
	 printf("local = %x   vmeaddress = %x\n",(unsigned long)dmabuffer,(unsigned long)vmebuffer);
     else
	 printf("ERROR\n");
     
    return OK;
}

int transferBuffer()
{
   while (1)
    {
	semTake(sem.transFrame,WAIT_FOREVER);
	printf ("Got transfer  interrupt\n");
    }
}

int coaddBuffer()
{   while (1)
    {
	semTake(sem.coAdFrame,WAIT_FOREVER);
	printf ("Got coadd interrupt\n");
    }

}

int unscrambleBuffer()
{
    while (1)
    {
	semTake(sem.unScrambleFrame,WAIT_FOREVER);
	printf ("Got unscramble interrupt\n");
    }

}

int dcapeek(int offset)
{
    return DCAPeek(offset);
}

int DCAPeek(int offset)
{
    int *p;
    p = (int *)oSystem.pDCARegs + offset;
    printf("offset %d = %d\n",offset,*p);
    return *p;
}

int dcapoke(int offset, int val)
{
    
    return DCAPoke(offset,val);
}

int DCAPoke(int offset,int val)
{
    int *p;
    p = (int *)oSystem.pDCARegs + offset;
    *p = val;
    return OK;
}

int setup(void)
{
    static int blah = 0;

    int retval;

    if (blah == 0) {
	dcainit();
	blah = 1;
    }

    /* coadd buffer start location: */
    setcbase(0x0);
    

    /* coadd a big buffer: */
    setcoaddbufsize(0x10000);
    
    /* add this, and it's the first and last frame: */
    add();
    both();
    
    /* interrupt vector */
    retval = dcapoke(3, COADD_INT_NUM);

    /* descramble start address */
    setdbase(0x0);
    
    /* # of rows: */
    setdsnumrows(0x3ff);
    
    /* # of cols: */
    setdsnumcols(0x3ff);
    
    /* descramble interrupt vector: */
    retval = dcapoke(7, UNSCRAMBLE_INT_NUM);
    
    /* transfer start address: */
    setxbase(0x0);
    
    /* # of rows in ROI */
    setxnumrows(0x1ff);  /* half of a frame */
    
    /* # of cols: */
    setxnumcols(0x3ff);
    
    /* transfer done interrupt vector: */
    retval = dcapoke(11, TRANS_INT_NUM);
    
    /* VMEbus address where we DMA data to: */
    retval = dcapoke(15, (int) vmebuffer);
    
    /* # of words to transfer: */
    setdmasize(0x40000);
    printf("Setup done!\n");
    return OK;
}

void
setflags(void)
{
    int val = 0;
    
    val |= fctl.add;
    val |= (fctl.first << 1);
    val |= (fctl.last << 2);
    
    printf("In setflags: val = %d\n", val);

    dcapoke(2, val);
}


void
first(int first)
{
    fctl.first = first;
    if (first)
	printf("first frame...\n");
    else
	printf("not first frame...\n");

    setflags();
}

void
last (int last)
{
    fctl.last = last;
    if (last)
	printf("last frame...\n");
    else
	printf("not last frame...\n");
    setflags();
}

void
both (void)
{
    fctl.first = 1;
    fctl.last = 1;
    
    printf("Both first and last frame...\n");
    setflags();
}

void
neither (void)
{
    fctl.first = 0;
    fctl.last = 0;
    printf("neither first nor last frame...\n");
    setflags();
}

void
add (void)
{
    fctl.add = 1;
    printf("adding...\n");
    setflags();
}

void
subtract (void)
{
    fctl.add = 0;
    printf("subtracting...\n");
    setflags();
}
int
teststart(void)
{
    dcapoke(12,1);  /* master counter pattern */
    dcapoke(13,1);
    printf("Test pattern running...\n");
    return OK;
}

int
slavestart(void)
{
    dcapoke(12,1);  /* slave counter pattern */
    dcapoke(13,1);
    printf("Test pattern running...\n");
    return OK;
}
int
masterstart(void)
{
    dcapoke(12,3);  /* master counter pattern */
    dcapoke(13,1);
    printf("Test pattern running...\n");
    return OK;
}

int
teststop(void)
{
    dcapoke(13,0);
    dcapoke(12,0);
    printf("Test pattern halted.\n");
    return OK;
}

int
setcbase(int addr)
{
    dcapoke(0, addr);
    printf("Coadd buffer start set to: %x\n",addr);
    return addr;
}

void
setcoaddbufsize(int size)
{
    dcapoke(1, size);
    printf("Coadd buffer size set to: %x\n", size);
}


int
setdbase(int addr)
{
    dcapoke(4, addr);
    printf("Descramble buffer start set to: %x\n",addr);
    return addr;
}

void
setdsnumrows(int rows)
{
    dcapoke(5, rows);
    printf("Number of rows to descramble: %x\n", rows);
}

void
setdsnumcols(int cols)
{
    dcapoke(6, cols);
    printf("Number of columns to descramble: %x\n", cols);
}


int
setxbase(int addr)
{
    dcapoke(8, addr);
    printf("Transfer buffer start set to: %x\n", addr);
    return addr;
}

void
setxnumrows(int rows)
{
    dcapoke(9, rows);
    printf("Number of rows in ROI to transfer: %x\n", rows);
}

void setxnumcols(int cols)
{
    dcapoke(10, cols);
    printf("Number of columns in ROI to transfer: %x\n", cols);
}


int
zerobase(void)
{
    setxbase(0);
    setdbase(0);
    setcbase(0);
    printf("All buffer base addresses set to zero.\n");
    
    return OK;
}

int
dmago(void)
{
    dcapoke(17,1);
    printf("Transfer started!\n");
    return OK;
}

int
setdmasize(int size)
{
    dcapoke(16, size);
    printf("Size of DMA transfer: %d words\n", size);
    return size;
    
}

/* readbuffer lets us display numwords words from the DMA buffer after doing the DMA transfer.
 * All words are thirty-two bits.  Added "gotcha" - dmabuffer is a 2D array! 
 */
void
readbuffer(int numwords, int firstrow)
{
    int i;
    int row = firstrow;
    int col = 0;


    for (i = 0; i < numwords; i++) {
	printf("Address: %p [%3d,%3d] = %08lx\n", &(dmabuffer[row][col]), row, col, dmabuffer[row][col]);
	if (++col == 1024) {
	    col = 0;
	    ++row;
	}
    }
}

int
mempoke(int row, int col)
{
    printf("Address: %p = ",  &(dmabuffer[row][col]));
    return dmabuffer[row][col];
}
    
