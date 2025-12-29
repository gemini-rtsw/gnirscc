#ifndef SAVER_H
#define SAVER_H


/* includes*/

#include "imhutils.h"
#include "gnDCADefs.h"
#include "dhs.h"

/* defines*/
#define FITS_SAVE 0
#define DHS_SAVE 1

#ifdef VXWORKS

typedef struct {
    char ip[MAXSTRING];           /* server ip*/
    char name[MAXSTRING];         /* dhs server name*/
    char lifetime[MAXSTRING];
    char observeID[MAXSTRING];
    DHS_DATA_TYPE format;
    char label[MAXSTRING];
    int initDone;
    char impName[MAXSTRING];
    char qlStream[MAXSTRING];
    long *data;
    int wait;
/*     DHS_TAG putTag; */
/*     DHS_BD_DATASET ds; */
    DHS_CONNECT connect;
}dhsParams;
typedef struct {
    int run;
    int bufNum;
    dhsParams dhs;
    roiParams roi;
    
}callBackStruct;
typedef struct {
    char ip[MAXSTRING];           /* server ip*/
    int sfd;
    long port;                    /* port for fits saver*/
    char host[MAXSTRING];
    char filename[MAXCAPTBUFS][MAXSTRING];     /* base filename for image */
    char pixeldir[MAXSTRING];     /* directory to place IRAF pixel file in */
   /*  char headerdir[MAXSTRING];  */   /* directory to put header/main file in */
    int  filetype;                /* SAVE_IRAF or SAVE_FITS */
    long format;
    void *pdata;
}fitsParams;

typedef union{
    dhsParams dhs;
    fitsParams fits;
}saverType;


typedef struct {
    saverType param;
    chipParams chip;
    long procMode;                    /* sep rdd, rrd*/
    roiParams roi;
    long server;                   /* dhs or fits */
    long imtype;                  /*image type (IMG_RESULT, etc) */
    int health;
    int disposition;
} saverParams;
/* function prototypes*/
int saveFitsData(int bufNum, fitsParams *fits, tRect *roiStruct,int roiNum);
int writeFits(header *hdr,fitsParams *fits, void *buf,int dx,int dy,int bufNum,int roiNum);
int saveDhsData(int bufNum, dhsParams *dhs, roiParams *roiStruct);

#endif
/* Saver message header */
typedef struct {
  long naxis1;                  /* # of pixels on X axis */
  long naxis2;                  /* # of pixels on Y axis */
  long imtype;                  /* Obj2Save image type (IMG_RESULT, etc) */
  int format;                   /* the datatype of image pixels */
  char filename[MAXSTRING];     /* base filename for image */
  char pixeldir[MAXSTRING];     /* directory to place IRAF pixel file in */
  char headerdir[MAXSTRING];    /* directory to put header/main file in */
  int  filetype;                /* SAVE_IRAF or SAVE_FITS */
} svrMsgHdr;


#endif
