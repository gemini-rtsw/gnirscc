/**************************************************************************
 * File:        gnDCADefs.h
 * Purpose:     Defines structures and constants 
 *
 * Author:      Jerry Heim
 * Copyright:   Aura Inc.  All rights reserved.
 * Date:        02 May 1996
 * History:    
 *	Modified - 16 Jul 1996 - ncb - began adding stuff needed for Circular 
 *		capture buffer implementation
 *	Modified - 30 Nov 1999 - ncb - removed Datacube stuff replaced with 
 *		NOAO CoAdd board stuff
 *
 **************************************************************************/
#ifndef GNDCADEFS_H
#define GNDCADEFS_H
#include <debug.h> 
#include <epicsTypes.h>
#ifdef VXWORKS
#include <msgQLib.h>
#include <semLib.h>
#include "imageHdr.h"
typedef struct HeaderVals
{
    char ds[NUM_HDR][2][MAX_STRING_SIZE];
    char frame[NUM_HDR][2][MAX_STRING_SIZE];
}HeaderVals;
#endif 
/* redefine this to 0 to compile the gnaac production version */
#define PROTOTEST 1
#define STATUS int

/* coadder start address*/
#define DCA_BASE 0x8000
#define SET_REG(x,y,val) *((int *) (((int *)(x)) + y))=val
/**/
#define COADD 0
#define DESCRAMBLE 1
#define DMA 2

/* stuff needed to handle capture buffer */
#define MAXCAPTBUFS 256		/* num of 256x256 ROIs in Buffer */
#define MAXCAPTCOLS 1024	/* num of Columns in Buffer */
#define MAXCAPTROWS 4096	/* num of Rows of MAXCAPTCOLS cols in Buffer */
#define MAXCAPTBANKS 4

#define   MESSAGE_LENGTH   240
#define MAXSTRING MAX_STRING_SIZE	/* maximum length of Epics strings */
#define MESSAGE_BUFFERS   64
#define MAX_NCCDS 1 

/* capt buf macros*/
#define COL_MASK 0x3ff
#define ROW_MASK 0x3fff
#define ADDR_MASK 0xffffffff
#define COL_BITS 10
#define MAKE_TRANS_ADDR(a,col,row) (captBufAddr[a] + arsize[arSizeIdx]*row + col )
#define ADD_TRANS_ADDR(a,col,row) ( MAKE_TRANS_ADDR(captBufAddr[a], (col) ,(row))) 

/* interrupt vectors   (64-255 are available)*/
#define COADD_INT_NUM 200
#define UNSCRAMBLE_INT_NUM 202
#define TRANS_INT_NUM 205
#define COADD_INT_LEVEL 5
#define INTERRUPT_LEVEL 5

/* dca socket types*/
#define DCAREAD 1
#define DCAWRITE 2
#define DCALOG 3
#define DCAWCS 4
#define DCATIME 5
#define DCADATE 6
#define CAREAD 7
#define CAWRITE 8
#define DCACLOSESOCKET 99

/* types for fits saver*/
#define DCALONG 1
#define DCADOUBLE 2
#define DCASTRING 3
#define DCAUSHORT 4
     
#define CICS_DB_NOLOG 0
#define CICS_DB_NONE  1
#define CICS_DB_MIN   2
#define CICS_DB_FULL  3
#define CICS_DB_MARK   100

#define CICS_DB_ERROR1 101
#define CICS_DB_ERROR2 102
#define CICS_DB_ERROR3 103
#define CICS_DB_ERROR4 104
#define CICS_DB_ERROR5 105
#define CICS_DB_ERROR 0

/* Offset values for register setting in load register routine */
#define PCOADDSTART	0  	/*0*/
#define PIXELCNT	1      	/*4*/
#define CNTRLFLAGS	2	/*8*/
#define IVCOADDDONE	3	/*c*/
#define PDESSTART	4	/*10*/
#define ROWCNT		5	/*14*/
#define COLCNT		6	/*18*/
#define IVDESDONE	7	/*1c*/
#define PXFERSTART	8	/*20*/
#define NUMROWS		9	/*24*/
#define NUMCOLS		10	/*28*/
#define IVXFERDONE	11	/*2c*/
#define TESTENBLNSEL	12	/*30*/
#define TESTSTART	13	/*34*/
#define PDMASTART	15	/*3c*/
#define SZDMABLOCK	16	/*40*/
#define CMNDSTARTDMA	17	/*44*/

#define COUNT 1

#define ARRAYWIDTH	16
#define ARRAYLEN	16

#define ARRAY_WIDTH 1024
#define ARRAY_LENGTH 1024
#define ARRAY_SZ 1024

/* defines for disposition of data codes */
#define TOSS 		0
#define SAVE		1
#define DISPLAY		2
#define SVANDDISP	3

/* defines for processing modes */
#define STARE		0
#define SEP		1
#define CHOP	        2
#define CHOP3		3
#define TEST		4

/* defines for various array size choices */
#define	AR256x256  	0
#define AR512x512       1
#define AR768x768	2
#define AR1024x1024	3


/* ucode Types to be taken determines which Board setup to use also 
 * determines the processing algorithm to use 
 */
#define		NOCODE		0	/* no code loaded */
#define		RRD		1	/* (row) reset read fast microcode */
#define		RDD		2	/* reset read read slow microcode */
#define		RD		3	/* global reset one read ucode */

#define DCA_ADD 1
#define DCA_SUB 0
#define DCA_FIRSTF 2
#define DCA_LASTF 4

enum {GOOD = 0, WARNING, BAD};


/* alignment point structure definition */
typedef struct tPoint 
{
    int iX, iY; /* column nd row values for the alignment point of a surface*/
} tPoint;

typedef struct  
{
    long lowX, lowY;  	/* column and row values inclusive for the lower left point in a rectangle */
    long hiX, hiY; 	/* column and row values exclusive for the upper right point of a rectangle*/
    long rows, cols;	/* the count of rows and columns equal to hi - low */
} tRect;

typedef struct dcaRegisters 
{
    int pCoAddStart;  
    int pixelCnt;     
    int cntrlFlags;     	/* three flags stored in bit0, bit1, bit2 */
    int ivCoAddDone;  
    int pDesStart;    
    int rowCnt;       
    int colCnt;       
    int ivDesDone;    
    int pXferStart;   
    int numRows;      
    int numCols;      
    int ivXferDone;   
    int testEnblNSel; 
    int testStart;   
    int blank;
    int pDMAStart;    
    int szDMABlock;   
    int cmndStartDMA;  
} tDCARegs;              
    
/* semaphores */
#ifdef VXWORKS
typedef struct
{
    SEM_ID coAdFrame;  
    SEM_ID unScrambleFrame;
    SEM_ID transFrame;
    SEM_ID watchDog;
}coAdSems;
#endif
typedef struct dcaSystem
{
    tDCARegs *pDCARegs;		/* the base address of the NOAO coadder board register set */
    tDCARegs regCopy;		/* A copy of the current register setup */

} tDCASystem;		/* DataCoAdder system object */
typedef struct
  {
      int physRows;             /* number of physical rows */
      int physCols;             /* and columns */
      int overscan;             /* size of overscan */
      int leadin;               /* size of leadin */
      int garbage;              /* number of garbage pixels to drop */
      char name[MAX_NCCDS][MAXSTRING];   /* chip names */
  }chipParams;
#define MAXROIS 4
typedef struct
{
    tRect roi[MAXROIS];
    int numRois;
}roiParams;

typedef struct 
{
    char ctype1[MAXSTRING];
    double crpix1;
    double crval1;
    char ctype2[MAXSTRING];
    double crpix2;
    double crval2;
    double cd1_1;
    double cd1_2;
    double cd2_1;
    double cd2_2;
    char radecsys[MAXSTRING];
    double equinox;
    double mjdobs;

}wcsParams;
typedef void *tAddr;

/* function prototypes*/
void logMessage(const long, const char *, ...);
long setRegisters(tDCARegs *preg,int type);
int interruptInit(void);
void coAddIntHndlr(int dummy);
void unscrambleIntHndlr(int dummy);
void transIntHandlr(int dummy);
int openEpicsSocket();
int closeEpicsSocket();

#ifdef DHS_SAVER
/* dhs functions*/
void dhsWriteCallback(DHS_CONNECT, DHS_TAG, DHS_CMD_STATUS, char *, char *, void *);
void dhsErrorCallback(DHS_CONNECT, DHS_STATUS, DHS_ERR_LEVEL, char *, DHS_TAG, void *);
int avAddString(DHS_BD_FRAME, char *, char *);
void dropDHS();
int dhsNewDs();
int avAddInt(DHS_BD_FRAME, char *, int);
int avAddFloat(DHS_BD_FRAME, char *, float);
int avAddDouble(DHS_BD_FRAME, char *, double);
int avAddSection(DHS_BD_FRAME, char *, int, int, int, int);
int avAddTime(DHS_BD_FRAME, char *, double);
#endif

#endif
