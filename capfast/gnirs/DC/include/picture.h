/******************************************************************************
 * Program:	b011.tld/saver
 * File:	picture.h
 * Purpose:	Describes the format of a picture to be saved.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		31Jan92	created						dak
 *
 *$Id: picture.h,v 1.2 2009/05/27 19:32:28 fkraemer Exp $
 *
 *
 *$Log: picture.h,v $
 *Revision 1.2  2009/05/27 19:32:28  fkraemer
 *fkraemer - copied my complete working dir over trunk
 *
 *Revision 1.1.1.1  1998/12/15 16:18:20  buchholz
 *Imported gnaacSrc into CVS
 *
 * Revision 1.0  1994/05/17  18:08:10  quenten
 * Initial revision
 *
 ******************************************************************************/

/* NOTE:
	The data is in transputer format, which may or may not be different
	than sun format.
*/

#define T_NONE		0
#define T_FLOAT		1		/* data in floating format */
#define T_INTEGER	2		/* data in integer format */
#define T_STRING	3		/* data in string format */
#define T_SHORT		4		/* data in short integer format */

typedef struct header {
	char *name, *comment;
	int format;
	void *data;
	struct header *next;
} header;

typedef struct image {
	int *pixels;			/* pointer to pixel data */
	int x,y;			/* size of the image, in pixels */
	int format;			/* type of data: int, float ... */
	char *pixel_dir;		/* pixels directory */
	char *header_dir;		/* header directory */
	char *im_list;			/* file to save image names in */
	char filename[512];		/* filename, numbers tacked on */
	header *head;			/* header data */
	header *hk_data;		/* housekeeping info */
} image;

typedef struct display {		/* params for the display program */
	float zs[2];			/* zscale */
} display;
