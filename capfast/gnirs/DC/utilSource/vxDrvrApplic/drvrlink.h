/*******************************************************************************
 * Program:     naacdrvr
 * File:	drvrlink.h
 * Purpose:	Defines structures and routine prototypes for VXWORKS naac
 *               link protocol 
 * Author:	Diana Kennedy
 * History:
 *	12-Jan-1996 - created file - djk
 *
 ******************************************************************************/

#ifndef NAACLINK
#define NAACLINK

#ifdef SERVER
#define IODRIVER	"Aura Inc. NAAC System driver Version 1.0"
#else
#define IODRIVER	"Aura Inc. NAAC System User Interface Version 1.0"
#endif

#define DEFAULT_LINK	"B014"
#define LINK_NAME	"B014_LINK"
#define DEFAULT_SERVER  "CIO"

#define SERVERPORT      7000		/* port number */

/* command bytes */
#define RESET_LINK	1
#define ANALYZE_LINK	2
#define TEST_ERROR      3
#define READ_STAT       4
#define WRITE_STAT      5
#define CLOSE_SOCKS     6

#define CMNDLEN		1

#ifndef LINKVARS
#define LINKVARS

extern int comm_sckt;				/* communications socket */
extern int cntl_sckt;				/* control socket */
#endif

#endif

extern int ResetLink(/**/);
extern int AnalzeLink(/**/);
extern UINT8 TestError(/**/);
extern UINT8 ReadStat(/**/);
extern UINT8 WriteStat(/**/);


