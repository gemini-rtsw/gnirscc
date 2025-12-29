/**************************************************************************
 * File:        gnIpsDefs.h
 * Purpose:     Defines structures and constants for Imageflow communications
 *              Defines prototypes for Datcube control functions
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

#include <debug.h>
/* redefine this to 0 to compile the gnaac production version */
#define PROTOTEST 1
#define STATUS int

/* stuff needed to handle capture buffer */
#define MAXCAPTBUFS 128		/* num of 256x256 ROIs in Buffer */
#define MAXCAPTCOLS 8192	/* num of Columns in Buffer */
#define MAXCAPTROWS 512		/* num of Rows of MAXCAPTCOLS cols in Buffer */
#define MAX_STRING_SIZE 40	/* maximum length of Epics strings */

/* alignment point structure definition */
typedef struct oPoint {
    int iX, iY; /* column nd row values for the alignment point of a surface*/
} oPoint;

typedef struct oRect {
    int lowX, lowY;  	/* column and row values inclusive for the lower left point in a rectangle */
    int hiX, hiY; 	/* column and row values exclusive for the upper right point of a rectangle*/
    int rows, cols;	/* the count of rows and columns equal to hi - low */
} oRect;


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

