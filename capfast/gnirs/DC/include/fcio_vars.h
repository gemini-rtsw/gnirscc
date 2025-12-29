/*******************************************************************************
 * Program:     firedrvr
 * File:	fcio_vars.h
 * Purpose:	Defines structures and routine prototypes for SUN firecio 
 * Author:	Nick C. Buchholz
 * History:
 *	14-Dec-1990 - created file - ncb
 *
 *$Id: fcio_vars.h,v 1.2 2009/05/27 19:32:27 fkraemer Exp $
 *
 *
 *$Log: fcio_vars.h,v $
 *Revision 1.2  2009/05/27 19:32:27  fkraemer
 *fkraemer - copied my complete working dir over trunk
 *
 *Revision 1.1.1.1  1998/12/15 16:18:21  buchholz
 *Imported gnaacSrc into CVS
 *
 * Revision 1.0  1994/05/17  18:03:47  quenten
 * Initial revision
 *
 *****************************************************************************/


/*	Function prototypes	*/
/*  	File firedrvr.c 	*/
void    banner(void);
void    err_dump(void);
void    fatal(bool dump, string str1, string str2);
void    warning(string str1, string str2);
void	handle_args(int argc, char **argv);

/* File firesrvr.c */
int	cio_serve(int cmskt);
void    close_strm(uchar **sptr);
string  dec_str(uchar **sptr);
stream  dec_strm(uchar **sptr);
int     dec_1b(uchar **sptr);
int     dec_2b(uchar **sptr);
slong   dec_4b(uchar **sptr);
void    enc_str(uchar **sptr, register string s);
void    enc_time(uchar **sptr, struct tm *tmptr);
void    enc_2b(uchar **sptr, slong value);
void    enc_4b(uchar **sptr, slong value);
int     open_stream(stream fp);
void    reply_2beof(int value);
void    reply_nlstr(uchar *buf, int length);
void    reply_str(uchar *buf, int length);
void    reply_zeof(int value);
void    reply_1b(int value);
void    reply_2b(int value);
void    reply_4b(slong value);

/* File b0init.c	*/
int 	b011_map(ulong *addr, int *size);
int	b011_analyse(void);
int	b011_reset(void);

/* File link.c */
int	OpenLink(string Name);
int	CloseLink(int LinkId);
int	ReadLink(int LinkId, string Buffer, int Count, int Timeout);
int	WriteLink(int LinkId, string Buffer, int Count, int Timeout);
int	ResetLink(int LinkId);
int	AnalyseLink(int LinkId);
int	TestError(int LinkId);
int	TestRead(int LinkId);
int	TestWrite(int LinkId);



#ifndef FIRECIOV
#define FIRECIOV

/*	Global variable declarations 	*/
int	bytes_word = 4;		        /* # of bytes in target system word */
uchar	cmd_buf[MAX_MSG_DATA + MAX_MSG_OVER] = {0};/* Buf from T */
int	LinkId;				/* LinkId from "OpenLink" */
int	no_link;			/* flag indicates no link name given */
string	open_str = NULL;		/* Link name for "OpenLink"*/
uchar	rpy_buf[MAX_MSG_DATA + MAX_MSG_OVER] = {0}; /* Buf to T */
stream	strm_tbl[MAXFILES] = {NULL};	/* Stream translation table */
char	line[MAXLINE];			/* character buffer for temp strings */
string	tlinkname = NULL;		/* name of link to use */
int 	wait_socket;			/* socket handle */
int 	comm_socket;			/* communication socket */
struct b011_map *map;
int debug;
int fd;
int size;

#else
/*	global variable definitions	*/
extern int	bytes_word;		/* # of bytes in target system word */
extern uchar	cmd_buf[];		/* Buf from T */
extern int	LinkId;			/* LinkId from "OpenLink" */
extern string	open_str;		/* Link name for "OpenLink"*/
extern uchar	rpy_buf[];		/* Buf to T */
extern stream	strm_tbl[];		/* Stream translation table */
extern char	line[];			/* temp buffer */
extern string	tlinkname;		/* name of link to use */
extern int 	wait_socket;		/* socket handle */
extern int 	comm_socket;		/* communication socket */
extern struct b011_map *map;
extern int debug, size, fd;


#endif /* FIRECIOV */
