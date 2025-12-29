/*****************************************************************************
 * standard header file
 *
 * Copyright Nick C. Buchholz
 *
 ****************************************************************************/

#define YES     1
#define NO      0

#define TRUE    1
#define FALSE   0

#if !defined(ERROR)
#define ERROR   -1
#endif

#define OK	0

#define	NULL		0
#define	CNULL		(char *) 0
#define	LNULL		(List *) 0

#define MAXLINE 	1024
#define SCREENLINE	80
#define FNAMESZ		1024
#define EXACT   	-1

#define LESS    -1
#define GREATER  1
#define EQUAL    0

/* defines for open system call modes */
#define READ		0
#define WRITE		1
#define READ_WRITE	2

/* defines to facilitate debuging */
extern int debug;

#ifdef DEBUG
#define dprintf(x, y)	if (debug > x) { fprintf(stderr, y); } 
#define prtdebug(x, y)  if (debug & x) { fprintf(stderr, y); }
#else
#define dprintf(x, y)
#define prtdebug(x, y)
#endif

typedef	char 		bool;
typedef	FILE		*stream;
typedef	char		*string;
typedef	unsigned char	uchar;
typedef	long		slong;
typedef	unsigned long	ulong;


#define	or		else if
#define	when		break;case
#define	otherwise	break;default
#define	forever		for(;;)
#define	until(expr)	while(!(expr))

#if !defined(max)
#define	max(a, b)	((a) > (b) ? (a) : (b))
#endif

#if !defined(min)
#define	min(a, b)	((a) < (b) ? (a) : (b))
#endif

#define streq(s1, s2)		(strcmp(s1, s2) == 0)
#define strdiff(s1, s2)		(strcmp(s1, s2) != 0)
#define strneq(s1, s2, n)	(strncmp(s1, s2, n) == 0)
#define strndiff(s1, s2, n)	(strncmp(s1, s2, n) != 0)

#define await_event(x)		{ while(!(x)) ; }

#define UP_LINK_IN		LINK0IN
#define UP_LINK_OUT		LINK0OUT
#define DOWN_LINK_IN		LINK1IN
#define DOWN_LINK_OUT		LINK1OUT





