/******************************************************************************
 * File:	shared.h
 * Purpose:	describe shared memory and provide methods of accessing it.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		15Jan92	created						dak
 *
 *$Id: shared.h,v 1.2 2009/05/27 19:32:28 fkraemer Exp $
 *
 *
 *$Log: shared.h,v $
 *Revision 1.2  2009/05/27 19:32:28  fkraemer
 *fkraemer - copied my complete working dir over trunk
 *
 *Revision 1.1.1.1  1998/12/15 16:18:21  buchholz
 *Imported gnaacSrc into CVS
 *
 * Revision 1.0  1994/05/17  17:49:58  quenten
 * Initial revision
 *
 *****************************************************************************/

/* NOTE: All data in the shared memory will be in transputer format. */


#define MAXHK 148
#define NOT_SET -2





#define SUN_MESSAGE	0
#define B011_MESSAGE	4
#define SUN_ARG		8
#define B011_ARG	12
#define CUR_PIC		16
#define CUR_DISP	20
#define CHAN_SELECT	24
#define TEST_DATA	28
#define TEST_DATA2	32
#define HK_DATA_PTR	36
#define HK_DATA_READY	40
#define HK_SCREEN	44
#define HK_SAVE		48
#define IM_LIST		52

#define NO_MESSAGE	0
#define GET_PICTURE	1
#define DEL_PICTURE	2
#define SAVE_PICTURE	3
#define SAVE_DISPLAY	4
#define SAVE_PICTURE_A	5
#define SAVE_DISPLAY_A	6


