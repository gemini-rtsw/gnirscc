/****************************************************************************
 * File: 	gnDQVars.h
 * Purpose:  Defines and declares variables used by all datacube procedures
 *
 * Author:      Jerry Heim
 * Copyright:   Aura Inc.  All rights reserved.
 * Date:	02 May 1996
 * History:  Modified - 16 Jul 1996 - ncb - began converting to new nameing
 *		scheme, began adding support for circular Capture buffer.
 *
 ***************************************************************************/

#undef TESTSYSTEM 
#include <cicsLib.h>
#ifdef MAIN

DqSystem	oSystem;		/* DataCube system object */

/* DataCube device objects on board 0 */
DqIPDev		oAb0, oAu_B0, oAp_B0, oAd_B0, oAg_B0;

/* DataCube device objects on board 1 */ 
DqIPDev		oAb1, oAu_B1, oAd_B1, oAg_B1; 

/* Board Zero Vsims */
DqIPDev		oAm0_B0, oAm1_B0, oAm2_B0, oAm3_B0, oAm4_B0, oAm5_B0;

/* Board One Vsims */
DqIPDev		oAm0_B1, oAm1_B1, oAm2_B1, oAm3_B1, oAm4_B1, oAm5_B1; 

/* define surfaces for DataCube processing using new naming scheme */
DqSurf		oSCaptBufHb, oSCaptBufLb;
DqSurf		oIntrmSurfHb,oIntrmSurfLb;
DqSurf		oTempSurfHb,oTempSurfLb;/* only created never used pbr*/

DqSurf		oRes1UwHb, oRes1UwLb, oRes1LwHb, oRes1LwLb; 
DqSurf		oRes2UwHb, oRes2UwLb, oRes2LwHb, oRes2LwLb; 

DqSurf	        oDeSUwHb,oDeSUwLb,oDeSLwHb,oDeSLwLb;
DqSurf		oFnlDataUwHb, oFnlDataUwLb, oFnlDataLwHb, oFnlDataLwLb; 
DqSurf	oFDUwHb, oFDUwLb, oFDLwHb, oFDLwLb;

/* duplicate surfaces defined for DataCube processing */
DqSurf		oIRRcvSurfHb, oIRRcvSurfLb;/* never used pbr*/
DqSurf		oIRXmtSurfHb, oIRXmtSurfLb;

/* surfaces used in testing and Display */
DqSurf		oTProcDstSurf;
DqSurf		oDispSrcSurf;
DqSurf		oDispDstSurf;

/* surface descriptor for AD input device */
DqSurf		oAdSurf = (DqSurf) NULL;
/* flag to track if oAdSurf has been created */
int		adSurfCreated = FALSE;	

DqSurf		poAcqMultiDst[3];
DqSurf		poAcqMvMultiDst[3];
DqSurf		poProcMvMultiDst[3];
DqSurf  	poMultiDst[3]; 
DqSurf  	poUSMultiDst[5]; 

DqRect		tPRect;            /* never set pbr*/
DqRect		pRect;         /* never set pbr*/
DqRect		tRect;
DqRect		cbRect;

/* 8 bit surfaces are read into these buffers*/
DqByte  *imgBuf[4][ARRAY_WIDTH];
DqByte imageData[4][ARRAY_SZ * ARRAY_SZ];

/* test buffers*/
#if 0
DqByte		ImgBuf[ARRAYWIDTH][ARRAYLEN];
DqByte		ImgBufHb[ARRAYWIDTH][ARRAYLEN];
DqByte		ImgBufLb[ARRAYWIDTH][ARRAYLEN];
#endif
DqByte		ImgBufUwHb[ARRAYWIDTH][ARRAYLEN];
DqByte		ImgBufUwLb[ARRAYWIDTH][ARRAYLEN];
DqByte		ImgBufLwHb[ARRAYWIDTH][ARRAYLEN];
DqByte		ImgBufLwLb[ARRAYWIDTH][ARRAYLEN];


DqByte		pointHb, pointLb;          /* not used pbr*/

DqVideo		oIRVideo, oClrVideo;

/* Input video object description structure */
DqSampleInfo	tSampleInfo;

char *dbTop = NULL;
char *dbSadTop;

/* Misc Testing and control variables */
int	cont;
int 	Dcube,dqtestrun;

/* variables for testSystem not normally used */
#if defined(TESTSYSTEM)
unsigned short urawbuffer[(long)1049600];
short rawbuffer[(long)1049600];
unsigned char rawRowHb[2048];
unsigned char rawRowLb[2048];
#endif

int dqdebug;
#if 0
DqByte  imgBufUwHb[1024][1024];
DqByte	imgBufUwLb[1024][1024];
DqByte	imgBufLwHb[1024][1024];
DqByte	imgBufLwLb[1024][1024];
#endif
long *dqPatResult;
long *roiResult;


/* Variables needed to configure and control the piplines and data capture and 
 * processing pipes
 */
int arSizeIdx;          /* index into video config and detector size arrays */
int arSize;             /* number of rows and columns being read out */
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
char *ObsrvID;
char saverFileName[MAX_STRING_SIZE];
int numDROIs;
DqRect  pDRoiRect[4];
double tDetAbs, tMount; /* detector and mount temperature for headers */ 

int rddSetupDone;
int rrdSetupDone;
int testSetupDone;
int sepSetupDone;
int DQ_Abort = FALSE;
int DQ_Save = TRUE;

int arsize[4] = { 256, 512, 768, 1024 };
int quadSize[4] = { 128, 256, 384, 512 };

/* Variables needed to handle the circular capture buffer */
int     nextInBuf, nextOutBuf;                              
int     numBufs;                                            
int     captBufCol[MAXCAPTBUFS],captBufRow[MAXCAPTBUFS];  
int     captBufEmpty[MAXCAPTBUFS], captBufFull[MAXCAPTBUFS];

tPoint  tRcvInPoint[16]; 
tPoint  tRcvOutpoint[16];
DqRect	tCaptRect;

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
 
int	ActivePixelsPerLine[] = { 516, 1028, 1540, 2052 };
int	BlankPixelsPerLine[] =  { 40, 36, 28, 28 };


int	ActiveLinesPerFrame[] = { 128, 256, 384, 512 };
int	BlankLinesPerFrame[] =  { 8, 8, 8, 8 };

/*  Note:  To set the HORIZ_LINE_TIME measure the time between H Sync
    signals using the oscilloscope connected to the H sync pin (pin 
    21 of the AD module). Now set the time below such that the horizontal
    counter chip is not over clocked i.e. <= 10 MHz check the prescaler to
    see the actual clock divider value AD_K4 */
/* each entry is for a different array size i.e. varying ROI's */
double	HorizLineTime[] = { .000050, .000100, .000200, .000200 };

int	VSyncToActive[] = { 3, 3, 3, 3 };
int	HSyncToActive[] = { 9, 9, 14, 14 };

/* pipe descriptor variables */
DqPipe		oAcqPipe;	/* IR acquire pipe */
DqPipe		oAcqMvPipe;
DqPipe		oInt2DesMvPipe;
DqPipe		oRes1_2DesMvPipe;
DqPipe		oRes2_2DesMvPipe;
DqPipe		oAddPipe;
DqPipe		oSubPipe;
DqPipe		oUnSPipe;
DqPipe		oUnS16Pipe;
DqPipe		oInt2DesPipe, oInt2DesMvPipe;
DqPipe		oDispPipe, oDispMvPipe;

/* pipe event variables */
int	iAcqPipeEvnt;
int	iAcqMvPipeEvnt;
int	iInt2DesMvPipeEvnt;
int	iRes1_2DesMvPipeEvnt;
int	iRes2_2DesMvPipeEvnt;
int	iAddPipeEvnt;
int	iSubPipeEvnt;
int	iUnSPipeEvnt;
int	iUnS16PipeEvnt;
int	iProcPipeEvnt, 	iInt2DesMvPipeEvnt;
int 	iDispPipeEvnt,	iDispMvPipeEvnt;

/*pat variables and events*/
int	oAcqPat, iAcqPatDone;
int	oAcqXYPat[2], iAcqXYPatDone[2];
int	oAcqMvPat, iAcqMvPatDone;
int	oAcqMvXYPat[2], iAcqMvXYPatDone[2];
int     oInt2DesMvPat, iInt2DesMvPatDone;          
int	oRes1_2DesMvPat, iRes1_2DesMvDone;
int	oRes2_2DesMvPat, iRes2_2DesMvDone;
int	oAddPat, iAddPatDone;
int	oAddXYPat[2], iAddXYPatDone[2];
int	oSubPat, iSubPatDone;
int	oSubXYPat[2], iSubXYPatDone[2];
int	oUnSProcPat, iUnSProcPatDone;
int	oUnS16ProcPat, iUnS16ProcPatDone;
int	oProcPat, iProcPatDone;
int	oTProcPat, iTProcPatDone;
int	oDispMvPat, iDispMvPatDone; 

/* PAT handle for PAT Variable storage */
int	oVar;

/* stuff related to saving images */
char dhsIDString[40];

 /* not MAIN*/ 
#else          
/* define externs of all above here */

extern	DqSystem	oSystem;
extern	DqIPDev		oAb0, oAu_B0, oAp_B0, oAd_B0, oAg_B0;
extern	DqIPDev		oAb1, oAu_B1, oAd_B1, oAg_B1 ; 
extern	DqIPDev		oAm0_B0, oAm1_B0, oAm2_B0, oAm3_B0, oAm4_B0, oAm5_B0;
extern	DqIPDev		oAm0_B1, oAm1_B1, oAm2_B1, oAm3_B1, oAm4_B1, oAm5_B1; 
/* define surfaces for DataCube processing using new naming scheme */

extern DqSurf	oSCaptBufHb, oSCaptBufLb;
extern DqSurf	oIntrmSurfHb,oIntrmSurfLb;
extern DqSurf	oTempSurfHb,oTempSurfLb;

extern DqSurf	oRes1UwHb, oRes1UwLb, oRes1LwHb, oRes1LwLb; 
extern DqSurf	oRes2UwHb, oRes2UwLb, oRes2LwHb, oRes2LwLb; 

extern DqSurf   oDeSUwHb,oDeSUwLb,oDeSLwHb,oDeSLwLb;
extern DqSurf	oFnlDataUwHb, oFnlDataUwLb, oFnlDataLwHb, oFnlDataLwLb;
extern DqSurf	oFDUwHb, oFDUwLb, oFDLwHb, oFDLwLb;

/* duplicate surfaces defined for DataCube processing */
extern	DqSurf	oIRRcvSurfHb, oIRRcvSurfLb;
extern	DqSurf	oIRXmtSurfHb, oIRXmtSurfLb;

/* surfaces used in testing and Display */
extern	DqSurf	oTProcDstSurf;
extern	DqSurf	oDispSrcSurf;
extern	DqSurf	oDispDstSurf;

extern	DqSurf	oAdSurf;
extern	int	adSurfCreated;

extern	DqSurf	poAcqMultiDst[3];
extern	DqSurf	poAcqMvMultiDst[3];
extern	DqSurf	poProcMvMultiDst[3];
extern  DqSurf  poMultiDst[3]; 
extern	DqSurf	poUSMultiDst[5]; 

extern	DqRect	tPRect;
extern	DqRect	pRect;
extern	DqRect	tRect;
extern	DqRect	cbRect;

/* 8 bit surfaces are read into these buffers*/
extern DqByte  *imgBuf[4][ARRAY_WIDTH ];  
extern DqByte imageData[4][ARRAY_SZ * ARRAY_SZ];
#if 0
extern	DqByte		ImgBuf[ARRAYWIDTH][ARRAYLEN];
extern	DqByte		ImgBufHb[ARRAYWIDTH][ARRAYLEN];
extern	DqByte		ImgBufLb[ARRAYWIDTH][ARRAYLEN];
#endif
extern	DqByte		ImgBufUwHb[ARRAYWIDTH][ARRAYLEN];
extern	DqByte		ImgBufUwLb[ARRAYWIDTH][ARRAYLEN];
extern	DqByte		ImgBufLwHb[ARRAYWIDTH][ARRAYLEN];
extern	DqByte		ImgBufLwLb[ARRAYWIDTH][ARRAYLEN];

extern	DqByte		pointHb, pointLb;
extern	DqVideo		oIRVideo, oClrVideo;

/* Input video object description structure */
extern	DqSampleInfo	tSampleInfo;

extern char *dbTop;
extern char *dbSadTop;

/* Misc Testing and control variables */
extern	int	cont;  /* temp variable for testing*/
extern  int	Dcube,dqtestrun;

/* variables for testSystem not normally used */
#if defined(TESTSYSTEM)
extern  unsigned short urawbuffer[];
extern  short rawbuffer[];
extern  unsigned char rawRowHb[];
extern  unsigned char rawRowLb[];
#endif

extern int dqdebug;
extern DqByte  imgBufUwHb[1024][1024];
extern DqByte	imgBufUwLb[1024][1024];
extern DqByte	imgBufLwHb[1024][1024];
extern DqByte	imgBufLwLb[1024][1024];
extern long *dqPatResult;
extern long *roiResult;
/* extern float result1024[1024][1024]; */
/* extern float result512[512][512]; */
/* extern float result256[256][256]; */

/* Variables needed to configure and control the piplines and data capture and 
 * processing pipes
 */
extern	int arSizeIdx;          
extern	int arSize;             
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
extern int numAcq;
extern  int hdrTiming;
extern  int hdrDetail;
extern	char *ObsrvID;
extern char saverFileName[];
extern  int numDROIs;
extern  DqRect  pDRoiRect[];
extern  double tDetAbs, tMount; /* detector and mount temps for headers */ 

#if defined(TASKINIT)
extern SEM_ID semRDD;
extern SEM_ID semRRD;
extern SEM_ID semTEST;
extern SEM_ID semSEP;

extern SEM_ID semSetupRDD;	/* semaphore to start the RDD setup task */
extern SEM_ID semSetupRRD;	/* semaphore to start the RRD setup tasks */
extern SEM_ID semSetupTEST;	/* semaphore to start the TEST setup tasks */
extern SEM_ID semSetupSEP;	/* semaphore to start the SEP setup tasks */

extern SEM_ID semSetup; /* semaphore for return from the setup tasks */
#endif

extern	int rddSetupDone;
extern	int rrdSetupDone;
extern  int testSetupDone;
extern  int sepSetupDone;
extern	int DQ_Abort;
extern	int DQ_Save;

extern	int arsize[];

extern	int quadSize[];

/* Variables needed to handle the circular capture buffer */
extern	int	nextInBuf, nextOutBuf;                               
extern	int	numBufs;                                             
extern	int	captBufCol[MAXCAPTBUFS],captBufRow[MAXCAPTBUFS];   
extern	int	captBufEmpty[MAXCAPTBUFS], captBufFull[MAXCAPTBUFS]; 

extern  tPoint  tRcvInPoint[16]; 
extern  tPoint  tRcvOutpoint[16];
extern  DqRect	tCaptRect;

extern	int	iHoriz, indx;

extern int	arCols[]; 
extern int	arRows[];
extern int	ActivePixelsPerLine[];
extern int	BlankPixelsPerLine[];
extern int	ActiveLinesPerFrame[];
extern int	BlankLinesPerFrame[];
extern double	HorizLineTime[];
extern int	VSyncToActive[];
extern int	HSyncToActive[];  

extern	DqPipe		oAcqPipe;	/* IR acquire pipe */
extern	DqPipe		oAcqMvPipe;
extern  DqPipe		oInt2DesMvPipe;
extern  DqPipe		oRes1_2DesMvPipe;
extern  DqPipe		oRes2_2DesMvPipe;
extern	DqPipe		oAddPipe;
extern  DqPipe		oSubPipe;
extern	DqPipe		oUnSPipe;
extern	DqPipe		oUnS16Pipe;
extern  DqPipe		oInt2DesPipe, oInt2DesMvPipe;
extern	DqPipe		oDispPipe, oDispMvPipe; 

extern  int	iAcqPipeEvnt;
extern  int	iAcqMvPipeEvnt;
extern	int	iInt2DesMvPipeEvnt;
extern  int	iRes1_2DesMvPipeEvnt;
extern  int	iRes2_2DesMvPipeEvnt;
extern	int	iAddPipeEvnt;
extern  int	iSubPipeEvnt;
extern	int	iUnSPipeEvnt;
extern	int	iUnS16PipeEvnt;
extern	int	iProcPipeEvnt, 	iInt2DesMvPipeEvnt;
extern	int	iDispPipeEvnt,	iDispMvPipeEvnt;

/*  pat variables*/
extern  int	oAcqPat, iAcqPatDone;
extern  int	oAcqXYPat[], iAcqXYPatDone[];
extern  int	oAcqMvPat, iAcqMvPatDone;
extern  int	oAcqMvXYPat[2], iAcqMvXYPatDone[2];
extern  int     oInt2DesMvPat, iInt2DesMvPatDone;          
extern  int	oRes1_2DesMvPat, iRes1_2DesMvDone;
extern  int	oRes2_2DesMvPat, iRes2_2DesMvDone;
extern	int	oAddPat, iAddPatDone;
extern	int	oAddXYPat[], iAddXYPatDone[];
extern  int	oSubPat, iSubPatDone;
extern  int	oSubXYPat[], iSubXYPatDone[];
extern	int	oUnSProcPat, iUnSProcPatDone;
extern	int	oUnS16ProcPat, iUnS16ProcPatDone;
extern	int	oProcPat, iProcPatDone;
extern	int	oTProcPat, iTProcPatDone;
extern	int	oDispMvPat, iDispMvPatDone; 

/* PAT  handle for PAT Variable storage */
extern int oVar;

/* stuff related to saving images */
extern 	char	dhsIDString[40];

#endif

/**********************************************************************/
/*  prototypes for DataCube Access functions written by NOAO          */

/* in crFitsHeaders.c*/
STATUS getHeaderInfo( int timing );
STATUS setHeaderROI(int roiNum);
STATUS getWCSInfo(void);
int rdBanCom635Time(int timing);

/* in gnUtilFuncs.c */
void	SendSurf20Mhz(DqSurf oSurf);
void	RcvSurf20Mhz(DqSurf oSurf);
void	SendSurf40Mhz(DqSurf oSurf);
int 	DqLoadRegister(int *BaseAddr, int Offset, int Value);
int 	DqInqRegister(int *BaseAddr, int Offset);
void	read_surface(DqSurf oSurface);
void	read16_surface(DqSurf oSurfaceMS, DqSurf oSurfaceLS);
void	read32_surface(int hex, DqRect tRect, DqSurf oSurfaceUwMS, DqSurf oSurfaceUwLS,
		       DqSurf oSurfaceLwMS, DqSurf oSurfaceLwLS);
void	setTP(int);
void	clrTP(int);
void	loadRawBuf(DqSurf oSurfaceMS, DqSurf oSurfaceLS, int startrow, int startcol);

/* in gnInitStdSys.c */
int	gnCreateStdSys(void);
int	gnDisposeSys(void);
int	gnDQSurfInit(void);
int	gnDQStart(void);

/* in gnStartupDQ.c */
#if defined(TASKINIT)
int initTask( SEM_ID *semId, SEM_B_STATE semState, int priority, int options,
	      	int stack, FUNCPTR func, char *name);
#endif
int doRDD(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
      	int n9, int n10);
int doRRD(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
      	int n9, int n10);
int doTEST(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
      	int n9, int n10);
int doSEP(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
      	int n9, int n10);
int gnRDDStareSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		    int n8, int n9, int n10);
int gnRRDStareSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		    int n8, int n9, int n10);
int gnTestSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		int n8, int n9, int n10);
int gnSepSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7,
	       int n8, int n9, int n10);

/* in gnVideoInit.c */
int	gnSetVideoStruct(int var);
int	gnCreate32AD_4QVideo(void);
int	gnCreateVideo(void);
int	gnDisposeVideo( void );
int	PrintVideoResults( void );

/* in gnDispPipeSetup.c */
int	LgeDispPipeSetup(DqSurf oSurf);
int	SmlDispPipeSetup(DqSurf oSurf);
int	gnMakeDispPipe (DqSurf oSurf );
int	gnDisposeDispPipe( void );

/* in gnAcqPipeSetup.c */
int 	gnAcqPATSetup( void );

/* in gnAddPat.c */
int gnSetupAddPat(DqSurf oResUwHb, DqSurf oResUwLb,
		 DqSurf oResLwHb, DqSurf oResLwLb);

/* in gnTestProcs.c */
int 	gnTProcPatSetup( void );
int 	gnTUnSPatInit( void );
int  	gnDispMvPipeInit(void);

/* in gnCaptBufs.c */
int	initCaptBufs(int arSizeIdx);
int	getEmptyBuf(void);
void	putEmptyBuf(int emptyBuf);
int	getFullBuf(void);
void	putFullBuf(int emptyBuf);

/* in gnSubPat.c */
int gnSetupSubPat(DqSurf oResUwHb, DqSurf oResUwLb,
		 DqSurf oResLwHb, DqSurf oResLwLb);

/* in gnTestProg.c test program for q&d hardware test */
int 	gndq(int numADs, int numQuads);

/* in gnUnScramblePat.c */
int 	gnUnScrmbl16PatSetup( void );
int 	gnUnScramblePatSetup( void );
int gnUnScramblePat( );


/* in gnTakeData.c */
int	DQrdd( void );
int	DQrrd( void );
int DQsep();
int 	DQtest(void);
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
int 	gnGetROIVal(char *N, DqRect *Rect); 
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

/* guess on prototype for this datacube internal function */
void _amFlipGateZoom(DqIPDev mem, int gate, int xfilp, int yflip);     

void read32Surface(int scrambled,DqRect tRect,DqSurf oSurfaceUwMS,DqSurf oSurfaceUwLS,DqSurf oSurfaceLwMS,DqSurf oSurfaceLwLS);
void read16Surface(int scrambled,DqRect tRect,DqSurf oSurfaceLwMS,DqSurf oSurfaceLwLS);
