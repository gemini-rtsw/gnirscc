/*******************************************************************************
 * Program:     naacdrvr
 * File:	drvr_vars.h
 * Purpose:	Defines structures and routine prototypes for vxworks naaccio
 * Author:	Diana Kennedy
 * History:
 *	11-Jan-1996 - created file - djk
 *
 ******************************************************************************/

/*	Function prototypes	*/
/*  	File naacdrvr.c 	*/
void	handle_args(/*int argc, char **argv*/);

/* File naacSRVR.c */
VOID	naacSRVR(/*int cmmskt, int cmdskt, struct ockaddr_in *clientAddr*/);

/* File naacSRVR.c	*/
int 	b014_init(/*int intrup_level, int intrup_num*/);
int	b014_analyse(/*void*/);
int	b014_reset(/*void*/);

/* File link.c */
int	OpenLink(/*string Name, string mname*/);
int	CloseLink(/*int LinkId*/);
int	ReadLink(/*int LinkId, char *Buffer, int Count, int Timeout*/);
int	WriteLink(/*int LinkId, char *Buffer, int Count, int Timeout*/);
int	ResetLink(/**/);
int	AnalyseLink(/**/);
UINT8	TestError(/**/);
UINT8   ReadStat(/**/);
UINT8	WriteStat(/**/);

#define IVEC_TO_INUM(intVec) ((int) (intVec) >> 2)
#define INUM_TO_IVEC(intNum) ((VOIDFUNCPTR *)((intNum) << 2))
#define TRAPNUM_TO_IVEC(trapNum) INUM_TO_IVEC (32 + trapNum)

#ifndef NAACCIOV
#define NAACCIOV

/*	Global variable declarations 	*/
int	bytes_word = 4;		        /* # of bytes in target system word */
uchar	cmd_buf[MAX_MSG_DATA + MAX_MSG_OVER] = {0};/* Buf from T */
int	LinkId;				/* LinkId from "OpenLink" */
int	no_link = FALSE;		/* flag indicates no link name given */
string	open_str = NULL;		/* Link name for "OpenLink"*/
uchar	rpy_buf[MAX_MSG_DATA + MAX_MSG_OVER] = {0}; /* Buf to T */
char	line[MAXLINE];			/* character buffer for temp strings */
string	tlinkname = NULL;		/* name of link to use */
int 	wait_socket;			/* socket handle */
int 	comm_socket;			/* communication socket */
int     csocket;			/* command listen socket */
int     cmnd_socket;			/* command socket */
B014_MAP b014;
int debug = 1;


#else
/*	global variable definitions	*/
extern int	bytes_word;		/* # of bytes in target system word */
extern uchar	cmd_buf[];		/* Buf from T */
extern int	LinkId;			/* LinkId from "OpenLink" */
extern int      no_Link;
extern string	open_str;		/* Link name for "OpenLink"*/
extern uchar	rpy_buf[];		/* Buf to T */
extern char	line[];			/* temp buffer */
extern string	tlinkname;		/* name of link to use */
extern int 	wait_socket;		/* socket handle */
extern int 	comm_socket;		/* communication socket */
extern B014_MAP b014;
extern int debug;

#endif /* NAACCIOV */
