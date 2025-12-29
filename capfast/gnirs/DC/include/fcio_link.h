/*******************************************************************************
 * Program:     firedrvr
 * File:	fcio_link.h
 * Purpose:	Defines structures and routine prototypes for SUN fire link 
 *		protocol 
 * Author:	Nick C. Buchholz
 * History:
 *	14-Dec-1990 - created file - ncb
 *
 *$Id: fcio_link.h,v 1.2 2009/05/27 19:32:27 fkraemer Exp $
 *
 *
 *$Log: fcio_link.h,v $
 *Revision 1.2  2009/05/27 19:32:27  fkraemer
 *fkraemer - copied my complete working dir over trunk
 *
 *Revision 1.1.1.1  1998/12/15 16:18:20  buchholz
 *Imported gnaacSrc into CVS
 *
 * Revision 1.0  1994/05/17  18:02:41  quenten
 * Initial revision
 *
 *****************************************************************************/

#ifndef FIRELINK
#define FIRELINK

/* Support for link.c  */

#define IODRIVER	"FIRE Standalone Link Server/Driver Version 1.0"

#define INFINITE_TIMEOUT	1

#define DEFAULT_LINK	"B011"
#define LINK_NAME	"B011_LINK"

#endif /* FIRELINK */

