/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * vximhutils.h
 *
 * DESCRIPTION
 * Routines for handling images, headers and associated data. This version is
 * more limited that that used on the Unix side. This one does not save images.
 * It instead sets of the data structures for other routines to handle.
 * 
 * FUNCTION NAME(S):
 * imTypeSize - return the size of the image's pixels' data
 * typeSize - return the size of the specified type
 * imFreeImg - free memory allocated to image
 * imAddHdr - add a header to a header list structure
 * imFreeHdrs - free memory allocated to a header list structure
 * imFixName - replace any FITS illegal characters in string with valid ones
 * 
 * DEPENDENCIES
 * None
 *
 *INDENT-OFF*
 * $Log: vximhutils.h,v $
 * Revision 1.2  2009/05/27 19:32:29  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:21  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:14:19  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:39:43  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */
#ifndef VXIMHUTILS_H
#define VXIMHUTILS_H

#include <epCommon.h> 
#include <fitsio.h>
/* IMFORT type values for imcreat function */
#define IRF_NONE        0
#define IRF_BOOL        1
#define IRF_STRING      2
#define IRF_SHORT       3
#define IRF_INT         4
#define IRF_LONG        5
#define IRF_REAL        6
#define IRF_DOUBLE      7
#define IRF_BYTE        8           /* not an IMFORT type */

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

#define MAXLINE              1024

/* Values for ImgCtrl:ImageFormat */
#define SAVE_IRAF       0
#define SAVE_FITS       1

/*Valid characters in a FITS keyword */
#define FITS_VALID "_-1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ"

#define IN_BUFFER_LEN   262144            /* buffer size in bytes */
#define CHUNK_SIZE 4096

/* linked list structure for image header information */
typedef struct header {
	char *name, *comment;           /* header's name, description    */
	int format;                     /* type for data, one of the T_* */
	void *data;                     /* memory allocated for value    */
	struct header *next;            /* link to next header in list   */
} header;

/* structure for an image */
typedef struct image {
    float *pixels;			/* pointer to pixel data             */
    int x,y;			/* size of the image, in pixels      */
    int format;			/* type of data: int, float ...      */
    int im_type;                    /* type of image: from Obj2Save list */
    char pixel_dir[MAX_STRING_SIZE];	/* pixels directory                  */
    char header_dir[MAX_STRING_SIZE];	/* header directory                  */
    char *im_list;	                /* file to save image names in       */
    char filename[MAX_STRING_SIZE];	/* filename, numbers tacked on       */
    int filetype;                   /* format to save image, IRAF|FITS   */
    header *head;			/* header data                       */
    header *hk_data;		/* housekeeping info                 */
    struct image *next;
} image;


extern char *strdup(const char *);

/* routines in imhutils.c */
int    imTypeSize(image *im);
int    typeSize(int type);
void   imFreeHdrs(header **ptr);
void   imFreeImg(image *im);
char  *imFixName(char *name);
header *imAddHdr(header **hdr, int type, char *name, void *data, char *comment);
STATUS write_Fits_header(fitsfile *fptr, header *p);

#endif
