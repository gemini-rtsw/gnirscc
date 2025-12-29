static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: b014.c,v 1.2 2009/05/27 19:33:32 fkraemer Exp $"
};
#if	defined(vxWorks)
/*
 * Modify SunOS 4.1.3 device driver for vxWorks.
 */
#endif
/* b014 --- device driver for the INMOS B014                04/09/1990 */

/*
 *  Revision   : 1.0
 *
 *  Copyright (c) Inmos Ltd, 1988.
 *  All rights reserved.
 *
 *  DESCRIPTION
 *     This is the source of the S514C device driver.  It is intended
 *  to interface to the IMS B014 VME board.
 *
 *  NOTES
 *     This driver has been written for SUN-3 and SUN-4 workstations only.
 *  It has been compiled under SUNOs 4.0.3 only -
 *  it is not supported for SUNOs 3.x systems.
 *
 *  HARDWARE SUPPLEMENT
 *     The IMS B014 is accessed through the IMS C012 link adapter interface.
 *  It has the ability to interrupt the host on input, output and error.
 *  The interrupts are enabled or disabled through the Interrupt Mask Register
 *  this is an 8 bit register of which only the first four lsb bits are used ie
 *   
 *                7                  3  2  1  0
 *              ---------------------------------
 *              |                   |  |  |  |  | Interrupt Mask Register
 *              ---------------------------------
 *
 *      BIT_0         Unused
 *      BIT_1         MERROR interrupt
 *      BIT_2         OUTPUTINT interrupt
 *      BIT_3         INPUTINT interrupt
 *      BIT_4         Unused
 *      BIT_5         Unused
 *      BIT_6         Unused
 *      BIT_7         Unused
 *
 *  There are two STATUS REGISTERS available which have the value of
 *  one if there is data ready for transfer.There is also an interrupt
 *  enable bit (BIT_1 lsb) which is set when an interrupt has 
 *  occurred.
 *
 *  Data is passed to and from the board via two DATA REGISTERS.
 *  There is also an ERROR register containing the value of the error
 *  flag.
 *           
 *  HISTORY
 *     brwc 27-Jul-88 RJO - Initial coding
 *     brwc 23-Aug-88 RJO - Added extra test of the 'md' info pointer 
 *                          in B014Open().
 *     brwc 24-Aug-88 RJO - Added new IOCTL header files.
 *     brwc 31-Aug-88 RJO - Bit of an omission in B014Ioctl, forgot to
 *                          assign the 'md' pointer to B014Info.
 *     brwc 01-Sep-88 RJO - Discovered that the bp->b_flag field must be masked
 *                          with B_BUSY & B_PHYS to get the rw_flag.
 *                          The ERROR option must also have an 'iodone' call in
 *                          it to release the locked page of memory.
 *                          Check flags in OPEN call so that FREAD | FWRITE is 
 *                          accepted as legal.
 *     brwc 05-Sep-88 RJO - Changed the initialisation of the ISR and OSR registers 
 *                          to be 1 (BIT_1) ie interrupts on, so that the IMR is
 *                          used for Interrupt detection and setting.
 *     brwc 07-Sep-88 RJO - Took out the above initialisation in B014Attach, as
 *                          the BIT_1 goes when the C012 is reset.
 *     brwc 23-Sep-88 RJO - Changed the test of the status bit when reading.
 *                          Added PCATCH to sleep calls to trap CTRL-C.
 *     brwc 26-Sep-88 RJO - Altered the B014Read call to use my own physio.
 *     brwc 27-Sep-88 RJO - Added PCATCH to my physio (PhysIO) and it now
 *                          catches CTRL_C.Turned B014Busy OFF in Close.
 *     brwc 28-Sep-88 RJO - Added extra line to xxAttach() to set up the
 *                          vectored interrupt routine.
 *     brwc 03-Oct-88 RJO - Made the tracing of interrupt source better ie
 *                          getting the bloody thing correct !!
 *     brwc 04-Oct-88 RJO - Added timer stuff for link timeouts on read/write.
 *     brwc 05-Oct-88 RJO - Altered PhysIO() to quit if a timeout happens,
 *                          and found a cure for the common cold.
 *     brwc 06-Oct-88 RJO - Added 'unitimeout' to B014Read/Write to stop the
 *                          timeout happening if the transfer was OK.
 *     brwc 11-Oct-88 RJO - Bug in READFLAGS, returning NotVMEError.
 *     brwc 13-Oct-88 RJO - Discoverd well bad probs with my 'physio' routine
 *                          so in go the hacks ! Problem is that the number
 *                          of bytes in a transfer is not being returned to 
 *                          the system call in the user area.
 *     brwc 18-Oct-88 RJO - When an error is detected in PhysIO(), it must
 *                          turn off the Busy flags etc else it will sleep
 *                          until Ctrl-C is pressed in Strategy().
 *     brwc 28-Oct-88 RJO - Added Copyin and Copyout calls to PhysIO to
 *                          move data between data and user space.
 *           4-Nov-88 RJP - Fixed Error flag logic in ioctl.
 *           7-Nov-88 RJP - Changed semantics of ioctl call, to use struct
 *                          instead of bit fields in an int.
 *          14-Nov-88 RJP - Fixed request size > 4096, bus error problem.
 *          15-Nov-88 RJP - Changed name of sys/bcmd.h to sys/ims_bcmd.h
 *                          greater level of 'uniqueness'
 *          03-Sep-90 BJ  - Changes for S514C release:
 *                          made multiple boards work properly
 *                          fixed reset & analyse pulse widths
 */

#if	defined(vxWorks)
#include "b014includes.h"
#else
#include <sys/param.h>
#include <sys/buf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/ims_bcmd.h> /* The IOCTL definitions as defined by me */
#include <machine/psl.h>
#include <sundev/mbvar.h>

#include "bxiv.h"     /* File generated by config defines NBXIV */

#include "b014reg.h"  /* Register definintions                  */
#endif

#if	defined(vxWorks)
#ifdef	DEBUG
/* The print statements these defines enable will be conditional. */
#define DB 
#define DB1     /* Installation debugging */
#define DB2     /* Open/close debugging */
#define DB3 
#endif
#else
/* #define DB */
/* #define DB1 */    /* Installation debugging */
/* #define DB2 */    /* Open/close debugging */
/* #define DB3 */
#endif

#define YES       1
#define NO        0

#define READ_CHAN    0
#define WRITE_CHAN   1

#if	defined(vxWorks)
#define MAX_BOARDS      1                       /* Boards per system */
#define B014UNIT(dev)   (0)		/* always unit 0. */
#define TENTH           (sysClkRateGet() / 10)      /* Tenth of a second delay            */
#else
#define MAX_BOARDS      8                       /* Boards per system */

#define B014PRI         PZERO+1        /* Software sleep priority for B014   */
#define B014UNIT(dev)   (minor(dev))   /* Obtain the unit from device number */
#define TENTH           (hz / 10)      /* Tenth of a second delay            */
#endif

/* Macros for 'Reset' and 'Analyse' registers */
#define ASSERT(reg)     reg = 1
#define DEASSERT(reg)   reg = 0

#if	defined(vxWorks)
extern	void bzero() ;
extern	void bcopy() ;
extern  void lb014( char * );
#else
/* External variables */

extern int hz;             /* Kernel clock rate */

extern void wakeup (/* caddr_t p */);
extern int sleep (/* caddr_t p, int pri */);
#endif

/* Global data */

/* 
 * One state table entry is provided for each device.
 * It contains enough information about a transfer to
 * enable its completion should an interrupt occur.
 */

struct ChanState {
   struct buf Buf;            /* Buffer for 'physio ()'            */
   int Timeout;               /* Channel timeout in ticks          */
   BYTE HoldingBuf[MAX_B014_BSIZE+10]; /* buffer to hold data in   */
   int Count;                 /* Number of bytes in the transfer   */
   BYTE *Cp;                  /* Pointer to next byte              */
   struct BoardState *Back;   /* Back-pointer for 'TimeOut ()'     */
};

struct BoardState {
   BYTE IntReg;               /* Copy of interrupt status register */
   struct B014Reg *Reg;       /* Virtual address of B014 registers */
   int IntPri;                /* Board's interrupt priority        */
   BOOL B014Busy;             /* TRUE, if device is in use         */
   BOOL B014Open;             /* TRUE, if device is open already   */
   BOOL B014Timed;            /* TRUE, if watchdog timer needed    */
   BOOL B014SignalEnabled;    /* Signal reqd for error             */
   int B014SignalLevel;       /* Signal level for use in error     */
#if	defined(vxWorks)
   int dev ;		      /* For SunOS driver. */
   int chan ;		      /* Current channel, read or write. */
   SEM_ID bufSem;	      /* Semaphore for struct buf use. */
   WDOG_ID wdId;	      /* Watchdog id */
#else
   struct proc *B014UProc;    /* Pointer to the per user process   */
#endif
   struct ChanState Chan[2];  /* Read and write channels           */
};

struct BoardState B014Board[MAX_BOARDS];

#if	!defined(vxWorks)
/* Auto configuration related declarations */
        
/* Kernel interface routines */

int B014Probe(), B014Attach (), bxivIntr(); 

struct mb_device *B014Info[MAX_BOARDS];

struct mb_driver bxivdriver = {
   B014Probe,             /* Probe routine entry               */
   0,                     /* Slave entry - no slaves           */
   B014Attach,            /* Attach routine entry              */
   0,                     /* Go entry - no go !                */
   0,                     /* Done entry - no done reqd         */
   bxivIntr,              /* Interrupt routine entry           */
   sizeof(struct B014Reg), /* Amount of memory space needed    */
   "bxiv",                /* Name of the device                */
   B014Info,              /* Backpointers to mbdinit structs   */
   0,                     /* Name of a controller              */
   0,                     /* Backpointers to mbcinit structs   */
   0,                     /* Want exclusive use of Main Bus    */
   0,                     /* Interrupt routine linked list     */
};
#endif


#if	defined(vxWorks)
static int TimeOut (/* caddr_t Args */);
static int Strategy (/* struct buf *bp */);
static int Start (/* struct BoardState *B014, int chan */);
static int physio (/* dev, buffer, len, cmd, B014, bp */);
/* TBA */
static struct	b014_stat	*pb014Stat ;
static int LogIt (/* int unit, int Value, BYTE *Message */);
#define	Doze(a)	taskDelay(sysClkRateGet()/10)
#else
static int MinPhys (/* struct buf *bp */);
static int TimeOut (/* caddr_t Args */);
static int PhysIO (/* dev, uio, cmd, B014, bp */);
static int Strategy (/* struct buf *bp */);
static int Start (/* struct BoardState *B014, int chan */);
static void Doze (/* caddr_t p */);
static int LogIt (/* int unit, int Value, BYTE *Message */);
        
/* B014Probe --- determine if the device exists at the address specified */

int B014Probe (Reg, Unit)
caddr_t Reg;   /* Address where the device lives */
int Unit;      /* Unit number of this device     */
{
/*
 * Output Parameters:
 *    (int) Result - Returns 0 if no device is there, otherwise it
 *                   returns the size of the device registers.
 * 
 * NOTES:
 * Uses kernel routine 'peekc' which accesses a given address, if the
 * address doesnt exist it can catch the 'bus error' returned and
 * returns a value of -1.
 */
   register struct B014Reg *r;
   
#ifdef DB1
   printf ("B014Probe: Unit = %d, Reg = %lx\n", Unit, (long int)Reg);
#endif

   r = (struct B014Reg *)Reg;           /* Point at the mapped registers */

   if (peekc ((BYTE *)&r->Isr) == -1) { /* Contact the device */
#ifdef DB1
      printf ("B014Probe: no device installed\n");
#endif
      return (0);                        /* There is no device here */
   }
   
   return (sizeof (struct B014Reg));    /* Return device register size   */
}
#endif

 
/* B014Attach --- perform device and global data initialisation */

#if	defined(vxWorks)
int b014Attach (pStat)
struct	b014_stat	*pStat ;
#else
int B014Attach (md)
struct mb_device *md;   /* Pointer to the slave device structure */
#endif
{
/*
 *    Obtains values for the Interrupt priority level and the 
 * status/ID. This information has to be taken out from the
 * mb_device structure and plugged into the registers of the B014
 */
   register struct B014Reg *r;
   register int unit;
  
#if	defined(vxWorks)
   unit = 0 ;
   pb014Stat = pStat ;
#else
   unit = md->md_unit;
#endif

#ifdef DB1
   if(pb014Stat->b014Dbg&DbgOpen)
      printf ("B014Attach: unit = %d\n", unit);
#endif
  
#if	defined(vxWorks)
   r = (struct B014Reg *)pStat->b014_address;
#else
   r = (struct B014Reg *)md->md_addr;
#endif
  
   /* Write interrupt number to the device */
#if	defined(vxWorks)
   r->IntStatusIDReg = B014_INTERRUPT_NUM;
   /* Get the value of the int priority */
   r->IntLevelReg = B014_INTERRUPT_LEVEL;
#else
   if (md->md_intr != NULL)
      r->IntStatusIDReg = md->md_intr->v_vec;
   else
      cicsLogMessage (0,"B014: No interrupt vector specified in config file\n");
    
   r->IntLevelReg = md->md_intpri; /* Get the value of the int priority */
#endif
   r->IntMaskReg = 0x00;           /* Clear all interrupts at start     */
   r->Isr = C012DISABLE;           /* Clear the ISR interrupts          */
   r->Osr = C012DISABLE;           /* Clear the OSR interrupts          */
   
#ifdef DB1
   if(pb014Stat->b014Dbg&DbgOpen) {
      printf ("Interrupt Level Reg : %x\n", r->IntLevelReg);
      printf ("Interrupt Mask  Reg : %x\n", r->IntMaskReg);
      printf ("Interrupt status ID : %x\n", r->IntStatusIDReg);
   }
#endif
  
   /* Initialise the device state */
   B014Board[unit].B014Busy = NO;        /* Device is not yet busy */
   B014Board[unit].B014Open = NO;        /* Device is not yet open */
   B014Board[unit].B014Timed = NO;       /* Timer not reqd yet     */
   B014Board[unit].B014SignalEnabled = NO;
   B014Board[unit].B014SignalLevel = 0;
   B014Board[unit].Reg = r;              /* Point to device registers */
#if	defined(vxWorks)
   B014Board[unit].IntPri = B014_INTERRUPT_LEVEL;
#else
   B014Board[unit].IntPri = md->md_intpri;
#endif
   B014Board[unit].Chan[READ_CHAN].Timeout = 0;  /* Reset read timer  */
   B014Board[unit].Chan[WRITE_CHAN].Timeout = 0; /* Reset write timer */
   B014Board[unit].Chan[READ_CHAN].Count = 0;    /* Reset count       */
   B014Board[unit].Chan[WRITE_CHAN].Count = 0;   /* Reset count       */
   B014Board[unit].Chan[READ_CHAN].Back = &B014Board[unit];
   B014Board[unit].Chan[WRITE_CHAN].Back = &B014Board[unit];

#if	defined(vxWorks)
   B014Board[unit].dev = 0;
   B014Board[unit].bufSem = semBCreate( SEM_Q_PRIORITY, SEM_EMPTY ) ;
   B014Board[unit].wdId = wdCreate() ;
#endif

#ifdef DB1
   if(pb014Stat->b014Dbg&DbgOpen)
      printf ("B014Attach: exit\n");
#endif
#if	defined(vxWorks)
   return(OK);
#endif
}
     
/* B014Open --- open up the device and get it ready */

	int
b014Open( pStat, name, flag )
	struct	b014_stat	*pStat ;
	char	*name ;
	int	flag;
{
	register struct BoardState *B014;

	B014 = &B014Board[0];       /* Get the state table for this device */

	pStat->openError = NO ;
	if (B014->B014Open) {       /* This device is open already */
	  /* According to the VxWorks Programmer's Guide, 4.9.3.2, the open
	   * routine can return any value to identify the newly opened file.
	   * That seems not to allow for an error here.
	   */
	  cicsLogMessage(0, "b014Open() called with the device already open.\n" ) ;
	  pStat->openError = YES ;
	}

	B014->B014Open = YES;       /* device is now open          */

	if( pStat->b014Dbg&DbgOpen )
	    lb014( "Open" ) ;

	return( (int)pStat );
} 

#if	!defined(vxWorks)
/* B014Open --- open up the device and get it ready */

int B014Open (dev, flags)
dev_t dev;  /* Major and minor number of the device */
int flags;  /* Flag passed by the user to the open  */
{
#ifdef DB2
   register struct B014Reg *r;
#endif
   register int Unit = B014UNIT(dev);  /* get the major/minor device number */
   register struct mb_device *md;
   register struct BoardState *B014;

#ifdef DB2
   printf ("B014Open: Unit = %d, Flags = %d\n", Unit, flags);
#endif

   if (Unit >= MAX_BOARDS)                /* Validate the Unit number    */
      return (ENXIO);  

   md = B014Info[Unit];                   /* Get the device data         */
  
   if (md == NULL || md->md_alive == 0)
      return (ENXIO);                     /* There is no device there    */
  
   B014 = &B014Board[Unit];       /* Get the state table for this device */
  
   if (B014->B014Open)                     /* This device is open already */
      return (EBUSY);
  
   B014->B014Open = YES;                   /* device is now open          */

#ifdef DB2
   r = B014->Reg;
   printf ("B014Open: IntMaskreg = %x, IntStatusIDReg = %x\n", r->IntMaskReg, r->IntStatusIDReg);
#endif

   return (0);                              /* Get the hell out of here   */
}

/* MinPhys --- determine the maximum block size */

static int MinPhys (bp)
struct buf *bp;      /* Pointer to a buffer queue */
{
   if (bp->b_bcount > MAX_B014_BSIZE)
      bp->b_bcount = MAX_B014_BSIZE;
}
#endif

          
/* TimeOut --- watchdoggy timer for the link */

static int TimeOut (Args)
caddr_t Args;  /* Pointer to board state structure */
{
/*
 *    This function is called after the timeout period has expired.
 * A check is made to see if the device has finished its transfer
 * i.e. it should not be busy and its byte count should be zero. 
 */
   struct ChanState *cp = (struct ChanState *)Args;
   register struct BoardState *B014;
   int Level;
  
   B014 = cp->Back;

#ifdef DB
   if(pb014Stat->b014Dbg&DbgIntr)
      logMsg ("TimeOut: unit = %d, Count %d, Busy %d\n", B014 - B014Board, cp->Count, B014->B014Busy,0,0,0);
#endif
  
   /* Start critical section */
   Level = splx (pritospl (B014->IntPri));
  
   if (!B014->B014Open) {              /* Test if the device is open already */
#if	defined(vxWorks)
      (void)splx (Level);
      return(OK);
#else
      splx (Level);
      return;
#endif
   }
    
   if ((B014->B014Busy) && (cp->Count != 0)) {  /* Not currently active */
      /*
       * The transfer has failed to complete within the
       * timeout period.So , set an error (73) and return 
       * to the user.
       */ 
       
      cp->Buf.b_flags |= B_ERROR;
#if	defined(vxWorks)
      cp->Buf.b_error = ETIMEDOUT;
      /* Reset B_BUSY and set B_DONE. */
      cp->Buf.b_flags &= ~B_BUSY;
      cp->Buf.b_flags |= B_DONE;
      /* Give the semaphore so that anyone waiting on the buffer will
       * wake up.
       */
      (void)semGive( B014->bufSem ) ;
#else
      cp->Buf.b_error = ETIME;
      iodone (&(cp->Buf));
      wakeup ((caddr_t) &(cp->Buf));
#endif
   }
    
#ifdef DB
   if(pb014Stat->b014Dbg&DbgIntr)
      logMsg ("TimeOut: exit b_error 0x%x\n", cp->Buf.b_error,0,0,0,0,0);
#endif
  
#if	defined(vxWorks)
      (void)splx (Level);
      return(OK);
#else
   splx (Level);    /* End critical section */
#endif
}

/* Strategy --- deal with requests for I/O */

static int Strategy (bp)
register struct buf *bp;   /* Pointer to a buffer queue */
{
   register struct BoardState *B014;
   int Level;
   int chan;
   int unit;
   struct ChanState *cp;
  
#ifdef DB
   if(pb014Stat->b014Dbg&DbgStrat)
      printf ("Strategy: entry\n");
#endif
    
   unit = B014UNIT(bp->b_dev);
   B014 = &B014Board[unit];

   if (bp->b_flags & B_READ)
      chan = READ_CHAN;
   else 
      chan = WRITE_CHAN;
      
   cp = &(B014->Chan[chan]);
  
   /* Begin that critical section */
   Level = splx (pritospl(B014->IntPri));

   /* Set up the first I/O Operation */
   B014->B014Busy = YES;                 /* This device is now busy          */
   cp->Cp = cp->HoldingBuf;              /* Tell B014 where to get data from */
   cp->Count = bp->b_bcount;             /* Local B014 count we decrement    */

#ifdef DB
   if(pb014Stat->b014Dbg&DbgStrat)
      printf ("B014Busy %d, Count %d\n", B014->B014Busy, cp->Count);
#endif

   Start (B014, chan);

   /* End of critical section */
   (void) splx(Level);  
  
#ifdef DB
   if(pb014Stat->b014Dbg&DbgStrat)
      printf ("Strategy: exit\n");
#endif
#if	defined(vxWorks)
    return( OK ) ;
#endif
}        


#if	defined(vxWorks)
/* VxWorks version of physio(). */
/* physio --- execute the READ or WRITE command.
 */

static int physio (dev, buffer, len, cmd, B014, bp) 
DEV_HDR *dev;              /* Device header */
caddr_t	buffer;
int	len;
int cmd;                   /* Command to be used */
struct BoardState *B014;   /* State table entry for this device */
struct buf *bp;   
{
/*
 * NOTES:
 * Based on PhysIO() in this file.
 */
   int	iov_len = len ;
   register struct B014Reg *r;   /* Our memory mapped registers   */
   register struct ChanState *cp;
   int Level;                      
   BOOL IOError = NO;            /* Check if anything has gone wrong  */

#ifdef DB
   if(pb014Stat->b014Dbg&DbgPhysio)
      printf ("physio: cmd = %d, iovlen %d\n",
            cmd, iov_len);
#endif
       
   r = B014->Reg;

   if (cmd == B_READ)
      cp = &(B014->Chan[READ_CHAN]);
   else
      cp = &(B014->Chan[WRITE_CHAN]);
   
   bzero( (char *)bp, sizeof( struct buf ) ) ;

   /* Loop until all bytes are transferred or an error occurs. */
   while (iov_len > 0) {

      /*  Set up the buffer for I/O */
      bp->b_bcount  = (iov_len < MAX_B014_BSIZE) ? iov_len : MAX_B014_BSIZE;
      bp->b_dev = B014->dev;           /* Plug in the device unit info     */
      bp->b_un.b_addr = buffer; /* Point to users space         */
  
      if (cmd == B_WRITE) {
	 /* The SunOS version of this routine uses copyin() to move the
	  * buffer contents from user space to kernel space.  The vxWorks
	  * version just moves the data from one buffer to another so as
	  * to minimize changes elsewhere.
	  */
         bcopy (bp->b_un.b_addr, cp->HoldingBuf, bp->b_bcount) ;
      }
      
      /*
       * Begin the actual data transfer sequence.
       * The loop calls Strategy until all bytes sent or recieved,
       * it will stop if an error or a CTRL-C occurs.
       */
     
      /* OR in the READ or WRITE flag bit */
      bp->b_flags = (B_BUSY | cmd) & ~B_DONE;
      
      Level = spl6();

      Strategy (bp);

      while ((bp->b_flags & B_DONE) == 0) {
#ifdef DB3
      if(pb014Stat->b014Dbg&DbgPhysio)
         printf ("Sleeping on B_DONE: flags 0x%x, Count %d, bp 0x%x\n", bp->b_flags, cp->Count, (int)bp);
#endif

	      /* Wait on struct buf. */
	      (void)semTake( B014->bufSem, sysClkRateGet()/10 ) ;

#ifdef DB3
      if(pb014Stat->b014Dbg&DbgPhysio)
	 printf ("Awakened from B_DONE\n");
#endif
      }
       
      (void) splx(Level);  

      if (bp->b_flags & B_ERROR) {
          /* 
           * There has been an error, so report it and
           * say how many bytes we mangaged to transfer 
           * before the failure.We must also turn off
           * the interrupts and BUSY flag just in case
           * read or write is called again.
           */
                        
          IOError = YES;
          iov_len -= cp->Count;
          B014->B014Busy = NO;

          if (cmd == B_READ) {
              /* Turn off the read interrupts  */

              B014->IntReg &= ~B014INPUTINT;
              r->IntMaskReg = B014->IntReg;  
              r->Isr = C012DISABLE;  
          }
          else {
              /* Turn off the write interrupts */
                          
              B014->IntReg &= ~B014OUTPUTINT;
              r->IntMaskReg = B014->IntReg;
              r->Osr = C012DISABLE;
          }
	  /* while( iov_len > 0 ) */
          break;
      }
      else {
          /*
           * Decrement the user io structure so that it knows where and
           * what it is doing !!
           */
       
          iov_len -= bp->b_bcount;
      }    
      
#ifdef DB
   if(pb014Stat->b014Dbg&DbgPhysio)
      printf ("bp_flags 0x%x, b_error 0x%x, iovlen %d\n",
               bp->b_flags, bp->b_error, iov_len);
#endif
   }

   if (!IOError || bp->b_error != EINTR) {
     if (cmd == B_READ) {
          /* 
           * We must send back the buffer to user 
           * space.
           */
           
          bcopy (cp->HoldingBuf, bp->b_un.b_addr, bp->b_bcount) ;
      } 
   }
                                
   /*
    * More housekeeping - a drivers job is never done.
    * Basically, turn off the BUSY bit in the flags and
    * move to the next uio_iov record.
    */
   
   /* Clear the B_BUSY flag           */
   bp->b_flags &= ~(B_BUSY);

   return( len - iov_len ) ;
    
}
#else
/* PhysIO --- execute the READ or WRITE command. Rich's version of physio */

static int PhysIO (dev, uio, cmd, B014, bp)
dev_t dev;                 /* Major and minor device numbers of the device */
struct uio *uio;           /* Reference to the user buffer and device offset */
int cmd;                   /* Command to be used */
struct BoardState *B014;   /* State table entry for this device */
struct buf *bp;   
{
/*
 * NOTES:
 *    I had to write my own physio because Suns version wouldnt let
 * me do CTRL-C interruptions because the priority was too high.
 */
   register struct B014Reg *r;   /* Our memory mapped registers   */
   register struct ChanState *cp;
   int Level;                      
   BOOL IOError = NO;            /* Check if anything has gone wrong  */

#ifdef DB
   printf ("PhysIO: cmd = %d, iovcnt %d, resid %d, iovlen %d\n",
            cmd, uio->uio_iovcnt, uio->uio_resid, uio->uio_iov->iov_len);
#endif
       
   r = B014->Reg;

   if (cmd == B_READ)
      cp = &(B014->Chan[READ_CHAN]);
   else
      cp = &(B014->Chan[WRITE_CHAN]);
   
gotolabel:                      /* Disgusting isnt it !!             */

   /* Perform error and termination checking                          */
     
   if (uio->uio_iovcnt == 0)
      return (0);

   Level = spl6();
  
   while (bp->b_flags & B_BUSY) {
      bp->b_flags |= B_WANTED;

#ifdef DB3
      printf ("Strategy: 1st sleep, cmd = %d\n", cmd);
#endif

      if (sleep ((caddr_t) bp, B014PRI | PCATCH) == 1) {
         return (1);          /* We've been interrupted */
      }
   }
    
   (void) splx (Level);          /* restore the priority */
  
   /*  Set up the buffer for I/O */
   while (uio->uio_iov->iov_len > 0) {
      bp->b_bcount  = (uio->uio_iov->iov_len < MAX_B014_BSIZE) ? uio->uio_iov->iov_len : MAX_B014_BSIZE;
      bp->b_dev = dev;           /* Plug in the device unit info     */
      bp->b_un.b_addr = uio->uio_iov->iov_base; /* Point to users space         */
  
      if (cmd == B_WRITE) {
         if (copyin (bp->b_un.b_addr, cp->HoldingBuf, bp->b_bcount)) {
            /*
             * An error has occurred trying to write the
             * buffer into kernel space.Report then 
             * exit.
             */
                 
            bp->b_flags |= (B_ERROR | B_DONE);
            bp->b_error = EFAULT;
            wakeup ((caddr_t) bp);
            bp->b_flags &= ~B_BUSY;
            return (1);
         }
      }
      
      /*
       * Begin the actual data transfer sequence.
       * The loop calls Strategy until all bytes sent or recieved,
       * it will stop if an error or a CTRL-C occurs.
       */
     
      bp->b_flags = (B_BUSY | B_PHYS | cmd) & ~B_DONE;   /* OR in the READ or WRITE flag bit */
      
      Level = spl6();

      Strategy (bp);

      while ((bp->b_flags & B_DONE) == 0) {
#ifdef DB3
         printf ("Sleeping on B_DONE: flags 0x%x, Count %d, bp 0x%x\n", bp->b_flags, cp->Count, bp);
#endif

         if (sleep ((caddr_t) bp, B014PRI | PCATCH) == 1) {
            bp->b_flags |= (B_ERROR | B_DONE);
            bp->b_error = EINTR;
         }

#ifdef DB3
      printf ("Awakened from B_DONE\n");
#endif
      }
       
      if (bp->b_flags & B_ERROR) {
          /* 
           * There has been an error, so report it and
           * say how many bytes we mangaged to transfer 
           * before the failure.We must also turn off
           * the interrupts and BUSY flag just in case
           * read or write is called again.
           */
                        
          IOError = YES;
          uio->uio_iov->iov_len -= cp->Count;
          uio->uio_resid = cp->Count;
          bp->b_resid = cp->Count;

          B014->B014Busy = NO;
          if (cmd == B_READ) {
              /* Turn off the read interrupts  */

              B014->IntReg &= ~B014INPUTINT;
              r->IntMaskReg = B014->IntReg;  
              r->Isr = C012DISABLE;  
          }
          else {
              /* Turn off the write interrupts */
                          
              B014->IntReg &= ~B014OUTPUTINT;
              r->IntMaskReg = B014->IntReg;
              r->Osr = C012DISABLE;
          }
          break;
      }
      else {
          /*
           * Decrement the user io structure so that it knows where and
           * what it is doing !!
           */
       
          uio->uio_iov->iov_len -= bp->b_bcount;
          uio->uio_iov->iov_base += bp->b_bcount;
          uio->uio_resid -= bp->b_bcount;
      }    
      
#ifdef DB
      printf ("bp_flags 0x%x, b_error 0x%x, iovcnt %d, resid %d, iovlen %d\n",
               bp->b_flags, bp->b_error, uio->uio_iovcnt, uio->uio_resid, uio->uio_iov->iov_len);
#endif
   }          

   if (bp->b_flags & B_WANTED)
      wakeup ((caddr_t) bp);

   (void) splx(Level);  

   if (!IOError || bp->b_error != EINTR) {
     if (cmd == B_READ) {
          /* 
           * We must send back the buffer to user 
           * space.
           */
           
          if (copyout (cp->HoldingBuf, bp->b_un.b_addr, bp->b_bcount)) {
              /*
               * An error has occurred trying to write the
               * buffer back to user space.Report then 
               * exit.
               */
               
              bp->b_flags |= (B_ERROR | B_DONE);
              bp->b_error = EFAULT;
              wakeup ((caddr_t) bp);
              bp->b_flags &= ~B_BUSY;
              return (1);
          }
      } 
   }
                                
   /*
    * More housekeeping - a drivers job is never done.
    * Basically, turn off the BUSY bit in the flags and
    * move to the next uio_iov record.
    */
   
   bp->b_flags &= ~(B_BUSY | B_WANTED | B_PHYS);   /* Clear the B_BUSY flag           */
   --uio->uio_iovcnt;

   if (uio->uio_iovcnt != 0)
      uio->uio_iov++;          
    
   if (IOError && bp->b_error == EINTR)
      return(1);
    
   goto gotolabel;               /* Whoops sorry about this, but all while loops
                                   are gotos really ! */
}  
#endif


/* B014Read --- Transfer data from device to user space */

#if	defined(vxWorks)
	int
b014Read( pStat, buffer, len )
    struct	b014_stat	*pStat ;
    caddr_t	buffer;
    int	len;
#else
int B014Read (dev, uio)
dev_t dev;        /* Major and minor device numbers of the device   */
struct uio *uio;  /* Reference to the user buffer and device offset */
#endif
{
   int unit = B014UNIT(dev); 
   int ret = 0;
   register struct BoardState *B014;
   struct ChanState *cp;
   register struct buf *bp;
       
#ifdef DB
   if(pb014Stat->b014Dbg&DbgRdWr)
      printf ("B014Read: unit = %d\n", unit);
#endif
  
#if	defined(vxWorks)
   if( pStat->openError )
      /* Note that we cannot return ENXIO (or any other POSIX error code)
       * because a positive number cannot be distinguished from a byte
       * count.
       */
      return( ERROR ) ;
#else
   if (unit >= MAX_BOARDS)     /* Big Bad unit */
      return (ENXIO);
#endif

   B014 = &B014Board[unit];   /* Get the state record for this entry */
   cp = &B014Board[unit].Chan[READ_CHAN];
   bp = &(cp->Buf);            /* Get the block buffer */
        
#if	defined(vxWorks)

   B014->chan = READ_CHAN ;

   if (B014->B014Timed)
      (void)wdStart( B014->wdId, cp->Timeout, TimeOut, (int)cp ) ;
    
    /* In vxWorks lots of the stuff done in physio() is not needed.
     * We will write a vxWorks version.
     */
   ret = physio ((DEV_HDR *)pStat, buffer, len, B_READ, B014, bp); 
    
   if (B014->B014Timed)        /* Turn off the previous timeout      */
      (void)wdCancel( B014->wdId ) ;

#else

   if (B014->B014Timed)
      timeout (TimeOut, (caddr_t)cp, cp->Timeout);
    
   ret = PhysIO (dev, uio, B_READ, B014, bp); 
    
   if (B014->B014Timed)        /* Turn off the previous timeout      */
      untimeout (TimeOut, (caddr_t)cp);

#endif

#ifdef DB
   if(pb014Stat->b014Dbg&DbgRdWr)
      printf ("B014Read: ret = %d\n", ret);
#endif
  
  return (ret);
}                       


/* B014Write --- transfer data from user space to device */

#if	defined(vxWorks)
	int
b014Write( pStat, buffer, len )
    struct	b014_stat	*pStat ;
    caddr_t	buffer;
    int	len;
#else
int B014Write (dev, uio)
dev_t dev;        /* Major and minor device numbers of the device   */
struct uio *uio;  /* Reference to the user buffer and device offset */
#endif
{
   int unit = B014UNIT(dev); 
   register struct BoardState *B014;
   register struct buf *bp;
   struct ChanState *cp;
   int ret = 0;

#ifdef DB
   if(pb014Stat->b014Dbg&DbgRdWr)
      printf ("B014Write: unit = %d\n", unit);
#endif
      
#if	defined(vxWorks)
   if( pStat->openError )
      /* Note that we cannot return ENXIO (or any other POSIX error code)
       * because a positive number cannot be distinguished from a byte
       * count.
       */
      return( ERROR ) ;
#else
   if (unit >= MAX_BOARDS)     /* Big Bad unit */
      return (ENXIO);
#endif

   B014 = &B014Board[unit];  /* Get the state record for this entry */
   cp = &B014Board[unit].Chan[WRITE_CHAN];
   bp = &(cp->Buf);            /* Get the block buffer */
      
#if	defined(vxWorks)

   B014->chan = WRITE_CHAN ;

   if (B014->B014Timed)
      (void)wdStart( B014->wdId, cp->Timeout, TimeOut, (int)cp ) ;
    
    /* In vxWorks lots of the stuff done in physio() is not needed.
     * We will write a vxWorks version.
     */
   ret = physio ((DEV_HDR *)pStat, buffer, len, B_WRITE, B014, bp); 
    
   if (B014->B014Timed)        /* Turn off the previous timeout      */
      (void)wdCancel( B014->wdId ) ;

#else

   if (B014->B014Timed)
      timeout (TimeOut, (caddr_t)cp, cp->Timeout);
    
   ret = PhysIO (dev, uio, B_WRITE, B014, bp);
  
   if (B014->B014Timed)
      untimeout (TimeOut, (caddr_t)cp);
#endif

#ifdef DB
   if(pb014Stat->b014Dbg&DbgRdWr)
      printf ("B014Write: ret = %d\n", ret);
#endif
      
   return (ret);
}                       


/* B014Close --- close down this device */
#if	defined(vxWorks)
	STATUS
b014Close( pStat )
	struct	b014_stat	*pStat ;
#else
int B014Close (dev, flags)
dev_t dev;     /* Major and minor device number for this device */
int flags;     /* Read/write access flags                       */
#endif
{
   register int Unit = B014UNIT(dev);
   register struct B014Reg *r;
   register struct BoardState *B014;

#ifdef DB2
   if(pb014Stat->b014Dbg&DbgOpen)
      printf ("B014Close: Unit = %d\n", Unit);
#endif
  
   B014 = &B014Board[Unit];  /* Get the state table entry for this device */
    
   r = B014->Reg;
 
   /* Restore the state table entries */
   B014->B014Open = NO;             /* device is closed         */
   B014->B014Busy = NO;             /* Device is no longer busy */  
   B014->B014Timed = NO;            /* No future for the timers */
   B014->B014SignalEnabled = NO;    /* Turn off signals         */
   B014->B014SignalLevel = 0;       /* Reset the level          */
   B014->Chan[WRITE_CHAN].Timeout = 0;
   B014->Chan[READ_CHAN].Timeout = 0;
       
   /* 
    * Disable all interrupts, try and leave the device in the
    * state we originally found it !
    */
   r->Isr = C012DISABLE;
   r->Osr = C012DISABLE;
   B014->IntReg = 0x00;
   r->IntMaskReg = B014->IntReg;
  
#ifdef DB2
   if(pb014Stat->b014Dbg&DbgOpen)
      printf ("B014Close: exit\n");
#endif

   return (0);               /* were out of here.... */
}  

/* Start --- start the transfer of data */

static int Start (B014, chan)
struct BoardState *B014;   /* Pointer to the device state table entry for this device */
int chan;                  /* What channel are we starting */
{
/*
 * NOTES:
 *    This is called from the interrupt routine as well as the
 * Strategy routine.
 */
   struct B014Reg *r;         /* Pointer to the device registers */
   struct ChanState *cp;
 
   r = B014->Reg;
   cp = &(B014->Chan[chan]);

#ifdef DB
   if(pb014Stat->b014Dbg&DbgIntr)
      logMsg ("Start: chan = %d, Count =%d\n", chan, cp->Count,0,0,0,0);
#endif
    
   switch (chan) {
   case READ_CHAN:
      while (cp->Count > 0) {
         if (!(r->Isr & BIT_0))
            break;                   
         else {
            *cp->Cp = r->Idr;
            cp->Cp++;
            cp->Count--;
         }
      }

      if (cp->Count > 0) {
         /* 
          * Still more characters to read, so enable interrupts.
          * Note we have to OR in the existing interrupts that
          * have been set.
          */
         r->Isr = C012ENABLE;  /* Enable the IMS C012 interrupts */
         B014->IntReg |= B014INPUTINT;
         r->IntMaskReg = B014->IntReg;
#ifdef DB
	 if(pb014Stat->b014Dbg&DbgIntr)
	    logMsg ("READ interrupts set MaskReg %x,Count %d\n",r->IntMaskReg, cp->Count,0,0,0,0);
#endif
      }
      else {
         /* Disable interrupts, transfer is complete */
         B014->IntReg &= ~B014INPUTINT;
         r->IntMaskReg = B014->IntReg;
         r->Isr = C012DISABLE; 
         B014->B014Busy = NO;
         cp->Buf.b_flags |= B_DONE;

         /* Free device to sleeping strategy routine */

         /* Free buffer to waiting physio */
#if	defined(vxWorks)
	 (void)semGive( B014->bufSem ) ;
#else
         wakeup (&(cp->Buf));
#endif
#ifdef DB3
	 if(pb014Stat->b014Dbg&DbgIntr)
	    logMsg ("Start: unit %d, READ, wakeup 0x%x, Count %d\n", B014 - B014Board, (int)&(cp->Buf), cp->Count,0,0,0);
#endif
      }
      break;

   case WRITE_CHAN:
      while (cp->Count > 0) {
         if (!(r->Osr & BIT_0))
            break;
         else {
            r->Odr = *cp->Cp; 
            cp->Cp++;
            cp->Count--;
         }
      }
      
      if (cp->Count > 0) {
         /* Still more characters to write, so enable interrupts */
         r->Osr = C012ENABLE; /* Enable the IMS C012 interrupt */
         B014->IntReg |= B014OUTPUTINT;
         r->IntMaskReg = B014->IntReg;
#ifdef DB
	 if(pb014Stat->b014Dbg&DbgIntr)
	    logMsg ("WRITE interrupts set MaskReg %x,Count %d\n",r->IntMaskReg,cp->Count,0,0,0,0);
#endif
      }
      else {
         /* Disable interrupts, transfer is complete */
         B014->IntReg &= ~B014OUTPUTINT;
         r->IntMaskReg = B014->IntReg;
         r->Osr = C012DISABLE;   /* Turn off the IMS C012 interrupts */
         B014->B014Busy = NO;
         cp->Buf.b_flags |= B_DONE;

         /* Free device to sleeping strategy routine */
         /* Free buffer to waiting physio */
#if	defined(vxWorks)
	 (void)semGive( B014->bufSem ) ;
#else
         wakeup (&(cp->Buf));
#endif
#ifdef DB3
	 if(pb014Stat->b014Dbg&DbgIntr)
	    logMsg ("Start: unit %d, WRITE, wakeup 0x%x, Count %d\n", B014 - B014Board, (int)&(cp->Buf), cp->Count,0,0,0);
#endif
      }
      break;

   default:
      /* do something */
      break;
   }
  
#ifdef DB
   if(pb014Stat->b014Dbg&DbgIntr)
      logMsg ("Start: exit\n",0,0,0,0,0,0);
#endif
#if	defined(vxWorks)
    return( OK ) ;
#endif
}
        
/* bxivIntr --- the interrupt handler */

#if	defined(vxWorks)
int bxivIntr (pStat)
    struct	b014_stat	*pStat ;
#else
int bxivIntr (unit)
int unit;   /* Unit number of the device */
#endif
{
   register struct B014Reg *r;
   register struct BoardState *B014;
   struct ChanState *cp;
   int chan;
#if	defined(vxWorks)
   int unit = B014UNIT(0); 
#endif
       
#ifdef DB
   if(pb014Stat->b014Dbg&DbgIntr)
      logMsg ("bxivIntr: unit = %d\n", unit,0,0,0,0,0);
#endif
  
   B014 = &B014Board[unit];  /* get the state table entry for this device */
   r = B014->Reg;
  
   /*
    * Find out exactly who triggered this interrupt.This is done
    * by seeing if the interrupt bit is set in the status
    * registers of the IMS B014.
    *
    * NOTE: This got me confused ! The ERROR register is actually NOT ERROR
    * ie a zero means error and one is not.
    */
  
   if ((r->IntMaskReg & B014INPUTINT) &&
       (r->Isr & (C012ENABLE | BIT_0))) {
      chan = READ_CHAN;
#if	defined(vxWorks)
      pStat->nRdIntrs++ ;
#endif
   }
   else if ((r->IntMaskReg & B014OUTPUTINT) &&
            (r->Osr & (C012ENABLE | BIT_0))) {
      chan = WRITE_CHAN;
#if	defined(vxWorks)
      pStat->nWrIntrs++ ;
#endif
   } 
   else if ((r->IntMaskReg & B014ERROR) &&
            (!(r->ResetError & BIT_0))) {
      /*
       * The transputer error flag is set. Check to see if the
       * user needs informing about this.
       */
#ifdef DB
      if(pb014Stat->b014Dbg&DbgIntr)
	 logMsg ("ERROR interrupt\n",0,0,0,0,0,0);
#endif
             
#if	!defined(vxWorks)
      if (B014->B014SignalEnabled)
         psignal (B014->B014UProc, B014->B014SignalLevel);
#endif

      /*
       * Clear the error interrupt bit, so it wont
       * bother us again.
       */
      B014->IntReg &= ~B014ERROR;
      r->IntMaskReg = B014->IntReg;
      
#ifdef DB
      if(pb014Stat->b014Dbg&DbgIntr)
	 logMsg ("bxivIntr: Int Mask = %x\n", r->IntMaskReg,0,0,0,0,0);
#endif
           
      return (1);
   }
   else {
#if	defined(vxWorks)
      logMsg ("bxiv%d: spurious interrupt\n", unit,0,0,0,0,0);
#else
      printf ("bxiv%d: spurious interrupt\n", unit);
#endif
      return (1);
   }

   cp = &(B014->Chan[chan]);
   
#ifdef DB
   if(pb014Stat->b014Dbg&DbgIntr)
      logMsg ("bxivIntr: Count %d, MaskReg 0x%x, chan %d\n", cp->Count, r->IntMaskReg, chan,0,0,0);
#endif
   /* 
    * Has the I/O transfer operation completed yet ? This 
    * info is in our state record.
    */
  
   if (cp->Count == 0) {
      /* 
       * Transfer is completed. We dont need those interrupts
       * to bother us again, so we clear the enabled interrupts.
       */
      switch (chan) {
      case READ_CHAN:
         /* 
          * Clear the INPUTINT interrupt and
          * the IMS C012 interrupt.
          */
         B014->IntReg &= ~B014INPUTINT;
         r->IntMaskReg = B014->IntReg;  
         r->Isr = C012DISABLE;  
         break;
      case WRITE_CHAN:
         /* 
          * Clear the OUTPUTINT interrupt and
          * the IMS C012 interrupt.
          */
         B014->IntReg &= ~B014OUTPUTINT;
         r->IntMaskReg = B014->IntReg;
         r->Osr = C012DISABLE;
         break;
      default:
         /* do something */
#if	defined(vxWorks)
         logMsg ("bxiv%d: bad channel number: %d\n", unit, chan,0,0,0,0);
#else
         printf ("bxiv%d: bad channel number: %d\n", unit, chan);
#endif
         break;
      }
         
      B014->B014Busy = NO;
       
      /* Free buffer to waiting physio */
      if (cp->Buf.b_flags & B_DONE) {
#if	defined(vxWorks)
         logMsg ("bxiv%d: dup biodone: chan = %d\n", unit, chan,0,0,0,0);
#else
         printf ("bxiv%d: dup biodone: chan = %d\n", unit, chan);
#endif
      }
      else
#if	defined(vxWorks)
      {
	 /* Reset B_BUSY and set B_DONE. */
	 cp->Buf.b_flags &= ~B_BUSY;
	 cp->Buf.b_flags |= B_DONE;
      }
#else
         iodone (&(cp->Buf));
#endif

      /* Free device to sleeping strategy routine */
#if	defined(vxWorks)
      (void)semGive( B014->bufSem ) ;
#else
      wakeup ((caddr_t) &(cp->Buf));
#endif
      
#ifdef DB3
      if(pb014Stat->b014Dbg&DbgIntr)
	 logMsg ("Intr: awakened 0x%x, unit = %d\n", (int)&(cp->Buf), unit,0,0,0,0);
#endif
   }
   else
      Start (B014, chan);  /* Start the transfer */

#ifdef DB
   if(pb014Stat->b014Dbg&DbgIntr)
      logMsg ("bxivIntr: exit: Count %d, MaskReg 0x%x\n", cp->Count, r->IntMaskReg,0,0,0,0);
#endif
  
   return (1);
}    


/* B014Ioctl --- perform device specific functions not covered by read, write, open, close */
#if	defined(vxWorks)
	STATUS
b014Ioctl( pStat, cmd, data )
	struct	b014_stat	*pStat ;
	int	cmd;
	caddr_t	data;
#else
int B014Ioctl (dev, cmd, data, flag)
dev_t dev;     /* Major and minor device number for device */
int cmd;       /* Action to be carried out */
caddr_t data;  /* User address of a buffer */
int flag;      /* Hardly ever used but there to make up the numbers */
#endif
{
   register struct B014Reg *r;
   register struct BoardState *B014;
   int unit = B014UNIT(dev);
   long int *Status;
   long int Args;
   int Action, Param;
#if	!defined(vxWorks)
   register int i;
#endif

#if	defined(vxWorks)
   if( pStat->openError )
      /* Note that we cannot return ENXIO (or any other POSIX error code)
       * because a positive number cannot be distinguished from a byte
       * count.
       */
      return( ERROR ) ;
#else
   if (unit >= MAX_BOARDS)     /* Big Bad unit */
      return (ENXIO);
#endif

   B014 = &B014Board[unit];    /* get the state table entry for this device */  
   r = B014->Reg;               /* Map registers to physical locations */
     
   if (B014->B014Open == NO) {
      LogIt (unit, 0, "Ioctl: This device is not open yet");
      return (ENODEV);
   }

   switch (cmd) {
   case READFLAGS:
   /* 
    * This ignores its aruments and returns a word to the
    * user containing the result.
    * The parameter that is passed (via the data pointer)
    * consists of a 32 bit (ie short) integer.It has the
    * following format :-
    *
    *      31       Unused        1   0
    *     ------------------------------
    *     |  |                   |  |  |
    *     ------------------------------
    *      
    *  Where ...
    *   BIT_0       represents the ErrorFlag
    *   BIT_1       represents the TimeOutFlag
#if	defined(vxWorks)
    *   BIT_2       is write status, see code below.
    *   BIT_3       is read status, set indicates there is a byte to read.
#else
    *   BIT_2       Unused (inclusive)
#endif
    *   BIT_31      Unused
    *
    */
      
      Status = (long int *)data;      /* Point to the argp parameter */
      *Status = 0;                    /* ensure a clean start        */
      *Status =  (long int)((r->ResetError ^ BIT_0) & BIT_0); /* Send back the error value   */
      *Status |= ((long int) B014->B014Timed << 1);          /* Shove the TimeOut value in BIT_1 */
      *Status |= ((long int) (r->Osr & BIT_0) << 2);   /* Send back write status        */
      *Status |= ((long int) (r->Isr & BIT_0) << 3);   /* Send back read status         */                 
#if	defined(vxWorks)
      if(pb014Stat->b014Dbg&DbgIoctl)
	 printf ("READFLAGS: 0x%x\n", *Status);
#endif
      break;

   case SETFLAGS:
      /*
       * This takes a 32-bit integer (via the data parameter) and 
       * contains the actions required.
       * The format of the word is as follows....
       *
       *      31            16 15           0
       *      -------------------------------
       *      |               |             |
       *      -------------------------------
       *          Action          Parameter
       *
       *   BITs  0 - 15  Contain the parameters.
       *   BITs 16 - 31  Contain the action or function number.
       *
       *   Function Number             Parameter
       *   0    RESET                    None.
       *   1    ANALYSE                  None.
       *   2    SET TIMEOUT              Time out value (0 means reset timeout).
       *   3    SET ERROR SIGNAL         Signal number.
       *   4    RESET ERROR SIGNAL       None.
       */
      
      Args = *(long int *)data;
      Action = (int)(Args >> 16);
      Param = (int)(Args & 0xFFFF);

      switch (Action) {
      case RESET:             /* Reset the root transputer */
         DEASSERT (r->Analyse);
         DEASSERT (r->ResetError);
         Doze ((caddr_t)B014);
         ASSERT (r->ResetError);
         Doze ((caddr_t)B014);
         DEASSERT (r->ResetError);
         Doze ((caddr_t)B014);
         break;

     case ANALYSE:             /* Analyse the root transputer */
         DEASSERT (r->Analyse);
         DEASSERT (r->ResetError);
         Doze ((caddr_t)B014);
         ASSERT (r->Analyse);
         Doze ((caddr_t)B014);
         ASSERT (r->ResetError);
         Doze ((caddr_t)B014);
         DEASSERT (r->ResetError);
         Doze ((caddr_t)B014);
         DEASSERT (r->Analyse);
         Doze ((caddr_t)B014);
         break;

      case SETTIMEOUT:             /* Set up the data for a timeout */
         if (Param == 0) {
             B014->B014Timed = NO;      /* Reset timer */
             B014->Chan[READ_CHAN].Timeout = 0;
             B014->Chan[WRITE_CHAN].Timeout = 0;
         }
         else {
             B014->B014Timed = YES;     /* Timeout wanted */
             B014->Chan[READ_CHAN].Timeout = Param * TENTH;   /* Timeout period */
             B014->Chan[WRITE_CHAN].Timeout = Param * TENTH;   /* Timeout period */
#ifdef DB
	     if(pb014Stat->b014Dbg&DbgIoctl)
		printf ("Timeout: %d\n", B014->Chan[READ_CHAN].Timeout);
#endif
         }
         break;

      case SETERRORSIGNAL:
         /*
          * Make sure that the user is signalled when an
          * error occurs.The SignalLevel determines which
          * signal is to be caught by the user.The ERROR 
          * interrupts are also enabled, so that when an
          * ERROR interrupt occurs we can catch it and then
          * signal the user.
          */
         B014->B014SignalEnabled = YES;
         B014->B014SignalLevel = Param;
         B014->IntReg |= B014ERROR; /* Turn on ERROR interrupts */
         r->IntMaskReg = B014->IntReg;
#if	!defined(vxWorks)
         B014->B014UProc = u.u_procp;        /* Point to the users context */
#endif
         break;

      case RESETERRORSIGNAL:             /* Turn off the signalling and ERROR interrupts */
         B014->B014SignalEnabled = NO;
         B014->B014SignalLevel = 0;
         B014->IntReg &= ~B014ERROR;
         r->IntMaskReg = B014->IntReg;
         break;

      default:
         LogIt (unit, Action, "Ioctl: Unknown ioctl action passed");
         return (EINVAL);
         break;
      }   /* end switch */
      break;

   default:
      LogIt (unit, cmd, "Ioctl: Unrecognised command parameter");
      return (EINVAL);
      break;
   }  /* end switch */
   
   return (0);       /* Success code */
}     


#if	!defined(vxWorks)
/* Doze --- sleep for 100ms */

static void Doze (p)
caddr_t p;
{
   /* 'hz' gives ticks per second. We need a tenth of a second, so... */
   timeout (wakeup, p, hz / 10);
   sleep (p, B014PRI | PCATCH);
}
#endif


/* LogIt --- get error information out of the driver to the user */

static int LogIt (unit, Value, Message)
int unit;      /* Where did you come from ?            */
int Value;     /* What was the dodgy value             */
BYTE *Message; /* What do you want to tell the world ? */
{
   printf ("B014%d: %s %d\n", unit, Message, Value);
#if	defined(vxWorks)
   return(OK);
#endif
}       
