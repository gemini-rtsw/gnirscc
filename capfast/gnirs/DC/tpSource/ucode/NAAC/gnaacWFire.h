/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	gnaacWFire.h
 *
 * DESCRIPTION
 * This file contains the hardware dependency definitions for the gnaac
 *	aladdin array controller. It is used by the SDC waveform compiler in
 *	conjunction with gnaacBits.h and a ucode .c file to create a ucode .tld
 *	file capable of running	an aladdin I or II array. 

 * DEPENDENCIES
 *  This file requires sdc v2.3 or later. and an appropriate gnaacBits.h file
 *
 * Author:
 *	Nick C. Buchholz
 *
 * History:
 *	29-May-1997 - Created file from previous naac ucode for prototype
 *		controller - ncb
 *	
 *INDENT-OFF*
 * $Log: gnaacWFire.h,v $
 * Revision 1.2  2009/05/27 19:33:31  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:17:31  buchholz
 * Imported gnaacSrc into CVS
 *
 *INDENT-ON* 
 *****************************************************************************/

/* LastHdwVar is number of last global variable defined for all hardware in
   seqHdw.h */

#define lnr			gINT(LastHdwVar + 1)
#define coadds			gINT(LastHdwVar + 2)
#define inst_state		gINT(LastHdwVar + 3)
#define quadrant		gINT(LastHdwVar + 4)
#define ndavgs			gINT(LastHdwVar + 5)
#define colcnt			gINT(LastHdwVar + 6)

#define RESETING	0
#define READ1		1
#define READ2		2
#define PEDESTAL	3






