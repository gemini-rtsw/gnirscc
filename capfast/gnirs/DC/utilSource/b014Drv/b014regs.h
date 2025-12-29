/* B014 device driver for VxWorks, 21Mar97, jan@noao.edu. */

/* The structure below is the B014 device descriptor.
 * It is used by the driver to keep track of things.
 */
struct b014_stat {
	DEV_HDR	b014_hdr ;		/* First! */
	int	openError ;		/* A flag */
	int	b014Dbg ;		/* Debug bits. */
#define	DbgOpen		0x1
#define	DbgIntr		0x2
#define	DbgStrat	0x4
#define	DbgPhysio	0x8
#define	DbgRdWr		0x10
#define	DbgIoctl	0x20
	int	nRdIntrs ;		/* Counter. */
	int	nWrIntrs ;		/* Counter. */
	char	*b014_address ;		/* B014 base address. */
} ;

/* Device name. */
#define	B014_DEV_NAME	"/dev/bxiv0"

/* VME address of board; address modifier standard supervisory short io.
 */
#define	B014_BASE	(char *)(0x9900)
#define	B014_AM		VME_AM_SUP_SHORT_IO

/* Interrupt stuff.
 */
#define B014_INTERRUPT_LEVEL	0x04	/* interrupt level */
#define B014_INTERRUPT_NUM	0xCA	/* interrupt number */

/*
 * Some useful typedefs.
 */
   
typedef unsigned char BYTE;

/*
 * Constants
 */

#define BIT_0            0x01
#define MAX_B014_BSIZE   4096

/*
 * Reset count constants.
 */

#define RESET_COUNT      1000

/*
 * Bit Mask Operations.
 */

#define ERRORBIT         0x01
#define TIMEOUTBIT       0x02

/*
 * IMS C012 Interrupt Mask Register Values.
 */

#define C012ENABLE       0x2   /* Enabling BIT_1 turns C012 interrupts on    */
#define C012DISABLE      ~C012ENABLE

/*
 * IMS B014 Interrupt Mask Register Values.
 */

#define B014INPUTINT      0x08  /* Enable INPUTINT interrupts  Recieve data  */
#define B014OUTPUTINT     0x04  /* Enable OUTPUTINT interrupts Send data     */
#define B014ERROR         0x02  /* Enable MERROR interrupts subsys error active  */
 
/*
 * IMS B014 Interface.
 */

struct B014Reg {
   BYTE Dummy0;          /* Location 0 not used                              */
   BYTE Idr;             /* Input data regsiter                              */
   BYTE Dummy1;          /* Location 2 not used                              */
   BYTE Odr;             /* Output data register                             */
   BYTE Dummy2;          /* Location 4 not used                              */
   BYTE Isr;             /* Input status register only lsb 2 bits used       */
   BYTE Dummy3;          /* Location 6 not used                              */
   BYTE Osr;             /* Output status register only lsb 2 bits used      */
   BYTE Dummy4;          /* Location 8 not used                              */
   BYTE ResetError;      /* Reset and error register only first lsb bit used */
   BYTE Dummy5;          /* Location 10 not used                             */
   BYTE Analyse;         /* Analyse register                                 */
   BYTE Dummy6;          /* Location 12 not used                             */
   BYTE IntMaskReg;      /* Interrupt mask register only 3 lsb bits used     */
   BYTE Dummy7;          /* Location 14 not used                             */
   BYTE IntLevelReg;     /* Interrupt level register                         */
   BYTE Dummy8;          /* Location 16 not used                             */
   BYTE IntStatusIDReg;  /* Interrupt status ID register                     */
   BYTE Dummy9;          /* Location 18 not used                             */
   BYTE TramError;       /* TRAM error register                              */
};

#if	defined(vxWorks)
/* Define a bunch of things we need to fool the SunOS device driver. */

/* Some macros. */
#define minor(k)        (k&0xf)
#define pritospl(k)     (k)
#define splx(k) (k)
#define spl6() (6)

/* Enough of the buf structure for present purposes. */
struct buf {
	long    b_flags;                /* too much goes here to describe */
	long    b_bcount;               /* transfer count */
	long    b_bufsize;              /* size of allocated buffer */
	short   b_error;                /* returned after I/O */
	dev_t   b_dev;                  /* major+minor device name */
	union {
	    caddr_t b_addr;             /* low order core address */
	} b_un;
};

/*
 * These flags are kept in b_flags.
 */
#define B_WRITE         0x00000000      /* non-read pseudo-flag */
#define B_READ          0x00000001      /* read when I/O occurs */
#define B_DONE          0x00000002      /* transaction finished */
#define B_ERROR         0x00000004      /* transaction aborted */
#define B_BUSY          0x00000008      /* not on av_forw/back list */

#endif
