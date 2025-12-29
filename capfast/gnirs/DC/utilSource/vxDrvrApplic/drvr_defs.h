/*******************************************************************************
 * Program:     naacdrvr
 * File:	drvr_defs.h
 * Purpose:	Defines structures and routine prototypes for vxWorks naac C 
 *              i/o communications server 
 * Author:	Diana Kennedy
 * History:
 *	11-Jan-1996 - created file - djk
 *
 ******************************************************************************/
#include <sys/types.h>

#ifndef NAACCIO
#define NAACCIO

#define LOG(str)	{ FILE *fp; fp=fopen("/home/fire/logfile","aw"); fprintf(fp,"%s\n",str); fclose(fp);}

#define COPYRIGHT	"Copyright Jan 1996 by Aura Inc."

#define MAXFILES	20		/* Maximum number of open files */

#define PROGNUM		0x42303134      /* program number "B014" ascii as int */
#define VERSNUM		0x4e414143      /* version number "NAAC" ascii as text */

#define SERVERPORT      7000		/* port number */
#define	MAX_MSG_DATA	25000		/* Max length of msg data contents */
#define	MAX_MSG_OVER	50		/* Max length of msg overhead */
#define SERVER_WORK_PRIORITY 80          /* server priority */
#define SERVER_WORK_PRIORITY_R 200       /* and another */
#define SERVER_STACK_SIZE 20000         /* server stack size */
#define INTERRUPT_LEVEL  0x04           /* server interrupt level */
#define INTERRUPT_NUM  0xCA             /* server interrupt number */
#define INTERRUPT_MASK 0x07             /* server interrupt mask */
#endif				/* NAACCIO */







