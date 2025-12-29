/* File:     imhutils.h
 * Purpose:  Header infomation for routines in imhutils.c
 *
 * Routines: 
 * Author:   Tad Morgan (tmorgan)
 * Copyright (C) 1996 AURA, Inc. All rights reserved.
 *
 * NOTE: #define IRAF (or -DIRAF on compile) to include IRAF support & link
 *       appropriate IRAF libraries.
 */
#ifndef IMHUTILS_H
#define IMHUTILS_H

#ifdef VXWORKS
#include <epCommon.h> 
#include "gnDCADefs.h"

#else
#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "saverCommon.h"
#include "svcontrol.h"
#include "saver.h"
#include "saveDefines.h"
#include <ctype.h>
#endif

#ifndef DHS_SAVER
#include <fitsio.h>

#endif


#define MAXLINE		1024

/* to take care of error from fortran library */
int MAIN_;

#define NUMCHAN 8        /* number of channels to be used to access EPICS DB */

/* IMFORT type values for imcreat function */
#define IRF_NONE        0
#define IRF_BOOL        1
#define IRF_STRING      2
#define IRF_SHORT       3
#define IRF_INT         4
#define IRF_LONG        5
#define IRF_REAL        6
#define IRF_DOUBLE      7
#define IRF_BYTE        8               /* not part of IMFORT */

/* Epics Obj2Save image types */
#define IMG_SKY         1
#define IMG_RESULT      2
#define IMG_RAW         3
#define IMG_DARK        4
#define IMG_BIAS        5
#define IMG_BAD         6

/* filename extensions for each Obj2Save image type */
#define EXT_SKY         "_sky"
#define EXT_RESULT      "_result"
#define EXT_RAW         "_raw"
#define EXT_DARK        "_dark"
#define EXT_BIAS        "_bias"
#define EXT_BAD         "_bad"

/* Data types of saved data for each Obj2Save image type */
#define IRF_RESULT_TYPE IRF_REAL
#define IRF_RAW_TYPE    IRF_REAL
#define IRF_DARK_TYPE   IRF_REAL
#define IRF_BIAS_TYPE   IRF_REAL
#define IRF_BAD_TYPE    IRF_REAL
#define IRF_SKY_TYPE    IRF_REAL
#define FIT_RESULT_TYPE TFLOAT
#define FIT_RAW_TYPE    TFLOAT
#define FIT_DARK_TYPE   TFLOAT
#define FIT_BIAS_TYPE   TFLOAT
#define FIT_BAD_TYPE    TFLOAT
#define FIT_SKY_TYPE    TFLOAT

/* Values for ImgCtrl:ImageFormat */
#define SAVE_IRAF       0
#define SAVE_FITS       1

/*Valid characters in a FITS keyword */
#define FITS_VALID "_-1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ"

#define IN_BUFFER_LEN   262144            /* buffer size in bytes */
#define CHUNK_SIZE 4096

/* linked list structure for image header information */
typedef struct  header{
	char name[MAX_STRING_SIZE];
	char comment[MAX_STRING_SIZE];     /* header's name, description    */
	int format;                     /* type for data, one of the T_* */
	char data[MAX_STRING_SIZE];      /* memory allocated for value    */
	struct header *next;            /* link to next header in list   */
} header;
typedef union dataType
{
  char string[MAXSTRING];
  long lval;
  long bval;
  long ival;
  float fval;
  char cval;
  double dval;
  short sval;
}dataType;

typedef struct sendHead
{
  char name[MAXSTRING];
  char comment[MAXSTRING];
  int format;
  dataType data;
  
}sendHead;
/* structure for an image */
typedef struct image{
    void *pixels;			/* pointer to pixel data             */
    int x,y;			/* size of the image, in pixels      */
    int format;			/* type of data: int, float ...      */
    int im_type;                    /* type of image: from Obj2Save list */
    char *pixel_dir;		/* pixels directory                  */
    char *header_dir;		/* header directory                  */
    char *im_list;			/* file to save image names in       */
    char *filename;	        	/* filename, numbers tacked on       */
    int filetype;                   /* format to save image, IRAF|FITS   */
    header *head;			/* header data                       */
    header *hk_data;		/* housekeeping info                 */
    struct image *next;
} image;


extern char *strdup(const char *);


#ifdef VXWORKS
int    imTypeSize(int format);
int    typeSize(int type);
void   imFreeHdrs(header **ptr);
void   imFreeImg(image *im);
char  *imFixName(char *name);
header *imAddHdr(header **hdr, int type, char *name, void *data, char *comment);

/* header *dcHdrSet(char *hdrarray[][2]); */
int dcNameGet(void *connect, char **label, int maxsize);

#else
STATUS imInit(int sfd, image *im);
int    imTypeSize(image *im);
STATUS imSave(int sFd);
STATUS imGetHeaders(header **ptr, int sfd);
STATUS imStatus(char *stat);
void   imFreeHdrs(header **ptr);
void   imFreeImg(image *im);
STATUS imRead(int sFd, image *im);
char *imMakeFilename(char *dir, char *base, int type, int filetype, char *filename);
STATUS write_Fits_header(fitsfile *fptr, header *p);
STATUS save_Fits_image(image *im);

#ifdef IRAF
STATUS write_Iraf_header(int fd, header *p);
STATUS save_Iraf_image(image *im);
#endif

STATUS save_image(image *im);
char *imFixName(char *name);
#endif

#ifndef DHS_SAVER
STATUS write_Fits_header(fitsfile *fptr, header *p);
#endif


#endif
