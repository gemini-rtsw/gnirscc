/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * dcvx.h
 *
 * DESCRIPTION
 * This header file describes the "Detector Controler" (DC) API for VxWorks 
 * using routines from imhutil.c, which communicate with the sunSaver program 
 * on a Solaris machine. A number of support and test routines are also 
 * included.
 * 
 * FUNCTION NAME(S):
 * dcInit - initalize the DC library (API)
 * dcExit - close the DC library (API)
 * dcDsNew - create a new dataset (API)
 * dcFrameNew - create a new frame (e.g. image) in a dataset (API)
 * dcAvNew - create an attribute/value (e.g. image header) to a [dataset] or frame (API)
 * dcDsFree - destroy a dataset and its substructures (API)
 * dcNameGet - get a unique name from server (API)
 * dcPut - send the dataset to the server (API)
 * dcSetPort - set the port number of saver server program
 * dcGetPort - return the port number of saver server program
 * dcInitSaver - initalize address, port, etc of saver server program
 * strndup - return a duplicate string N characters long
 * dcImSave - save an image using DC libraries
 * dcHdrSet - create a header from name/value pairs of strings
 * dcImSaveTest - save a test image with headers
 * 
 * DEPENDENCIES
 *
 *INDENT-OFF*
 * $Log: dcvx.h,v $
 * Revision 1.2  2009/05/27 19:32:26  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.2  1998/11/20 17:14:13  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:39:44  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */
#ifndef DCVX_H

#include "imhutils.h"   /* for IRAF types */
#include "saverCommon.h"
#include "saveDefines.h"
#define DHS_OK     0
#define DHS_ERROR -1
#define CHUNK_SIZE 4096
#define	CARD		(80)     /* Bytes per FITS card */

/* default locations to put images on Sun */
#define PIXEL_DIR "/tmp/pixels"
#define HDR_DIR   "/tmp"

/* mockups for Data Handling System data types, mapped to IRAF type */
#define DHS_DATA_TYPE int
#define STRING_DT IRF_STRING
#define BOOLEAN_DT  IRF_BOOL
#define INT8_DT  IRF_BYTE
#define INT16_DT  IRF_SHORT
#define INT32_DT  IRF_LONG
#define FLOAT_DT  IRF_REAL
#define DOUBLE_DT IRF_DOUBLE
#define SHORT_DT     100          /* not in DHS, but output by DQ - convert */

typedef image dataSet;


/* typedef union dataType */
/* { */
/*   char string[MAXSTRING]; */
/*   long lval; */
/*   long bval; */
/*   long ival; */
/*   float fval; */
/*   char cval; */
/*   double dval; */
/*   short sval; */
/* }dataType; */
/* typedef struct sendHead */
/* { */
/*   char name[MAXSTRING]; */
/*   char comment[MAXSTRING]; */
/*   int format; */
/*   dataType data; */
  
/* }sendHead; */

#if 0
/* Functions part of the Detector Controller API (DC API) */
void *dcInit(char *server, char *name);
void dcExit(void *connect);
void *dcDsNew(void *connect);
void *dcFrameNew( void *container, char *name, 
		 DHS_DATA_TYPE type, int index,int ndims, int dims[], void *data);
void *dcAvNew( void *container, char *name, DHS_DATA_TYPE type,
	      int ndims, int dims[], void *data);
void dcDsFree(void *connect, void *container);
int dcNameGet(void *connect, char **label, int maxsize);
int dcPut(void *connect, void *container);
int saveSFrame(int dataSize,int format,void *dsp, void *connect,int last,int index);

int saveUSFrame(int dataSize,int format,void *dsp, void *connect, int last,int index);
STATUS DHSaddFrame(int scrambled,int dataSize,void *connect,void *dsp,char **dsName,int *index);
STATUS DHSaddSFrame(int format,void *connect,void *dsp,char **dsName,int *index);
STATUS DHSaddUSFrame(int format,void *connect,void *dsp,char **dsName,int *index);
#endif
/* support functions for DC implementation (not part of API) */
void dcSetPort(int port);
int dcGetPort();
STATUS dcInitSaver(char *server, int port, char *name);
char *strndup(const char *s1, int size);
STATUS dcImSave(int xdim, int ydim, DHS_DATA_TYPE format, void *data, header *hdr);STATUS dcimSave(int xdim, int ydim, DHS_DATA_TYPE format, void *data, header *hdr);
header *dcHdrSet(char hdrarray[255][2][MAX_STRING_SIZE],header *curr);
STATUS dcImSaveTest();
int saveHeaders(FILE *fd,header *hdr);
int dcput(void *connect, void *container);
#define DCVX_H
#endif
