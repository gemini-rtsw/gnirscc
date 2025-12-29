/****************************************************************************
 * File: 	gnDCAVars.h
 * Purpose:  Defines and declares variables used by all data coadder procedures
 *
 * Author:      Jerry Heim
 * Copyright:   Aura Inc.  All rights reserved.
 * Date:	02 May 1996
 * History:  Modified - 16 Jul 1996 - ncb - began converting to new nameing
 *		scheme, began adding support for circular Capture buffer.
 *
 ***************************************************************************/
#ifndef GNDCAVARS_H
#define GNDCAVARS_H
#undef TESTSYSTEM 
#include <cicsLib.h>
#include "gnDCADefs.h"
#ifdef MAIN

tDCASystem oSystem;		/* data CoAdder system object */
/* these registers will have the default values for the registers */
tDCARegs desRegs;		/* desired register values data taking routines */

tRect	   oDR_Rect;		/* rectangle for current DRROI description */

/* array of pointers to rectangles to hold current DRROI list */
tRect	  *pDRoiRect[4] = { NULL, NULL, NULL, NULL };

/* floating point image buffers */
/* float    **imgBuf; */
/* float    imageData[ARRAY_SZ * ARRAY_SZ]; */

/* char *dbTop = NULL; */
/* char *dbSadTop; */

/* Misc Testing and control variables */
int	cont;
int 	Dcube,dqtestrun;

int dcaDebug;

/* float  imgBuf_tst[1024][1024]; */

long *dqPatResult;
long *roiResult;


/* Variables needed to configure and control the piplines and data capture and 
 * processing pipes
 */
int arSizeIdx;          /* index into video config and detector size arrays */
int arSize;             /* number of rows and columns being read out */
char arType[MAX_STRING_SIZE];             /* array type, alladin 2 or 3*/
int numLNRs;            /* number of low noise read frames expected */
int numCoAdds;          /* number of images to be coadded */
int framesPerCycle;     /* number of frames for each image */
int numPics;            /* number of pictures to send to DHS */
int numObs;             /* number of packages to create in DHS for data */
int procMode;           /* data processing mode to use SEP, STARE, CHOP, ETC.*/
char *procModeNames[10] = {"STARE", "SEP", "CHOP", "CHOP3", "TEST" };
int uCodeType;          /* type of ucode RDD, RD, SRB, SUR, ETC, */
char *uCodeTypeNames[10] = {"","RRD", "RDD", "RD",  "SRB" };
int disposition;	/* disp of data, (S)ave, (D)isplay, (B)oth, (T)oss,
			 * (O)thers*/

int numAcq;
int hdrTiming;
int hdrDetail;

/* char saverFileName[MAX_STRING_SIZE]; */

double tDetAbs, tMount; /* detector and mount temperature for headers */ 

int rddSetupDone;
int rrdSetupDone;
int testSetupDone;
int sepSetupDone;
int DCA_Abort = FALSE;
int DCA_Save = TRUE;

char detType[MAX_STRING_SIZE];
int arsize[4] = { 256, 512, 768, 1024 };
int a3rsize[4] = {258,514,770,1026};
int a3csize[4] = {256, 512, 768, 1024};
int transsize[4] = {128,256,384,512};
int quadSize[4] = { 128, 256, 384, 512 };

/* Variables needed to handle the circular capture buffer */
/* int     nextInBuf, nextOutBuf;    */                           
int     numBufs; 
int emptyBufs;
                                        
int     captBufCol[MAXCAPTBUFS],captBufRow[MAXCAPTBUFS], captBufAddr[MAXCAPTBUFS],captBufTAddr[MAXCAPTBUFS];  
int     captBufEmpty[MAXCAPTBUFS], captBufFull[MAXCAPTBUFS];

tPoint  oRcvInPoint[16]; 
tPoint  oRcvOutpoint[16];

int    	iHoriz, indx;

/* These arrays hold the values for the various timing parameters needed to
 * setup the Datacube input video object. These parameters are drived from 
 * expermental data using the actual Datacube system and the wfire sequencer
 */
/* The AdSurf needs to be 4 pixels wider than the incoming image to compensate
 * for some problem in the hardware which causes the last pixel value to be
 * copied into the second to last pixel location in each row.  By making the
 * AdSurf two pixels longer, the second to last pixel and last pixel are both
 * zero so this hardware "bug" is no longer a problem. however all attempts to
 * make the surface only two pixels longer failed. setting the value to 2048-51
 * gives a surface which is 2048 wide. */
int	arCols[] = { 512, 1024, 1536, 2048 };
int	arRows[] = { 128, 256, 384, 512 };
 
int	PixelsPerLine[] = { 516, 1028, 1540, 2052 };
int	LinesPerFrame[] = { 128, 256, 384, 512 };


/* stuff related to saving images */
char dhsIDString[40];

 /* not MAIN*/ 
#else          
/* define externs of all above here */

extern  tDCASystem oSystem;
extern  tDCARegs desRegs;	/* desired register values data taking routines */


extern	tRect	oDR_Rect;
extern	tRect	*pDRoiRect[4];

/* 8 bit surfaces are read into these buffers*/
extern  float  **imgBuf;  
extern  float    imageData[ARRAY_SZ * ARRAY_SZ];

extern char *dbTop;
extern char *dbSadTop;
										
/* Misc Testing and control variables */
extern	int	cont;  /* temp variable for testing*/
extern  int	Dcube,dqtestrun;
extern  int	dcaDebug;


extern float  imgBuf_tst[1024][1024];



/* Variables needed to configure and control the piplines and data capture and 
 * processing pipes
 */
extern	int arSizeIdx;          
extern	int arSize;             
extern char arType [MAX_STRING_SIZE];             
extern	int numLNRs;            
extern	int numCoAdds;          
extern	int framesPerCycle;     
extern	int numPics;            
extern	int numObs;             
extern	int procMode;           
extern  char *procModeNames[10];
extern	int uCodeType;          
extern  char *uCodeTypeNames[10];
extern  int disposition; /* disp of data, (S)ave, (D)isplay, (B)oth, (T)oss,
			  * (O)thers*/

extern  int numAcq;
extern  int hdrTiming;
extern  int hdrDetail;

/* extern char saverFileName[]; */

extern  double tDetAbs, tMount; /* detector and mount temps for headers */ 

extern SEM_ID semRDD;
extern SEM_ID semRRD;
extern SEM_ID semTEST;
extern SEM_ID semSEP;
extern SEM_ID semSetupRDD;	/* semaphore to start the RDD setup task */
extern SEM_ID semSetupRRD;	/* semaphore to start the RRD setup tasks */
extern SEM_ID semSetupTEST;	/* semaphore to start the TEST setup tasks */
extern SEM_ID semSetupSEP;	/* semaphore to start the SEP setup tasks */
extern SEM_ID obsDone;
extern SEM_ID semSetup; /* semaphore for return from the setup tasks */
extern SEM_ID semDMA;

extern	int rddSetupDone;
extern	int rrdSetupDone;
extern  int testSetupDone;
extern  int sepSetupDone;
extern	int DCA_Abort;
extern	int DCA_Save;

extern char detType[MAX_STRING_SIZE];
extern	int arsize[];
extern int a3rsize[] ;
extern int a3csize[] ;
extern int transsize[] ;
extern	int quadSize[];

/* Variables needed to handle the circular capture buffer */
/* extern	int	nextInBuf, nextOutBuf;    */                            
extern	int	numBufs; 
extern int emptyBufs;
                                            
extern	int	captBufCol[MAXCAPTBUFS],captBufRow[MAXCAPTBUFS];   
extern	int	captBufEmpty[MAXCAPTBUFS], captBufFull[MAXCAPTBUFS],captBufAddr[MAXCAPTBUFS],captBufTAddr[MAXCAPTBUFS]; 

extern  tPoint  oRcvInPoint[16]; 
extern  tPoint  oRcvOutpoint[16];

extern	int	iHoriz, indx;

extern int	arCols[]; 
extern int	arRows[];
extern int	PixelsPerLine[];
extern int	LinesPerFrame[];

/* stuff related to saving images */
extern 	char	dhsIDString[40];

#endif

/**********************************************************************/
/*  prototypes for Data coadder Access functions written by NOAO          */

/* in gnStartupDQ.c */
int gnDCAStart( void ); 
#if defined(TASKINIT)
int initTask( SEM_ID *semId, SEM_B_STATE semState, int priority, int options,
	      	int stack, FUNCPTR func, char *name);
#endif
int doRDD (int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);
int doRRD (int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);
int doTEST(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);
int doSEP (int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);

/* in gnUtilFuncs.c */
void    loadVarRegs(tDCARegs *dRegs);
int 	dcaLoadRegister( int Offset, int Value);
int 	dcaInqRegister( int Offset);
void	read32_surface(int hex, tRect oRect, tAddr oAddr);
void    read32Surface(tAddr oAddr, tRect *pRect);
void    sleep (int a, int b);
tAddr   calcStartAddr(int saddr, tRect *roiRect);
void    dcaSetRect(tRect pSRect, tRect *pDRect);

/* in gnInitStdSys.c */
int	gnCreateStdSys(void);
int	gnMakeStdSys(tDCASystem *pSystem);
int 	gnDisposeSys(void);
int	gnDCASurfInit(void);

/* in gnCaptBufs.c */
int	initCaptBufs(int arSizeIdx);
int	getEmptyBuf(void);
void	putEmptyBuf(int emptyBuf);
int	getFullBuf(void);
void	putFullBuf(int emptyBuf);

/* in gnTestProg.c test program for q&d hardware test */
int 	gndca(int numADs, int numQuads);


/* in gnTakeData.c */
int	DCArdd( void );
int	DCArrd( void );
int 	DCAsep();
int 	DCAtest(void);
int 	captAndAddFrame( void );
int 	captAndSubFrame( void );


/* in gnDqRpcProc.c */
int 	*dqObsSetupRPC_1(int i);
int 	*dqObserveRPC_1(int i);
int 	*dqObsStatusRPC_1(int i);
int 	*dqObsAbortRPC_1(int i);
int 	*dqObsStopRPC_1(int i);

/* in gnEPICSIntrfc.c */
int 	gnGetGNAACParams(int *setupNeeded,int *acqSetupNeeded);
int 	gnGetROIVal(char *N, tRect *Rect); 
int	setObsFlags(int prep, int acq, int rdout);

STATUS 	gnGetEpics (char* name,unsigned short type, void *val);
STATUS  gnPutEpics (char* name,unsigned short type, void *val);
STATUS 	gnGetEpicsT (char *top,char* name,unsigned short type, void *val);
STATUS  gnPutEpicsT (char *top,char* name,unsigned short type, void *val);

#if defined(EPICS)
STATUS 	gnGetEpicsEnum(char *name, dbr_int_t *val);
STATUS 	gnPutEpicsEnum(char *name, dbr_int_t *val);
STATUS 	gnGetEpicsChanArray (char **name, chid **chTmp, int n);
STATUS 	gnGetEpicsTypeArray(chid *chTno, int **ctype, int n);
STATUS  gnGetEpicsValArray(int *ctype, chid *chTmp, void **val, int n);
STATUS 	gnPutEpicsValArray(int *ctype, chid *chTmp, void **val, int n);
STATUS 	gnClearEpicsChanArray ();
#endif

#if 0 
void 	gnPutEpicsVar(char *Var,  dbr_int_t Value);
void change_notify(struct event_handler_args args);
STATUS 	put_EPICS_int(char *Var, dbr_int_t Value);
#endif

int gnRDDStareSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);
int gnRRDStareSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);
int gnTestSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);
int gnSepSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);

/* in crFitsHeaders.c*/
STATUS getHeaderInfo( int timing );
STATUS setHeaderROI(int roiNum,tRect *proi,int captBuf);
STATUS getWCSInfo(int roiNum,int captBuf);
int    getDsHeader(int timing,int captBuf);
void  initHeader(int captBuf);

/* in gnSaveData.c*/
void saveFrame(int scrambled, int dataSize);
#endif
 
