static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: vxldnet.c,v 1.2 2009/05/27 19:33:37 fkraemer Exp $"
};
/************************************************************************/
/*									*/
/*	File:	ld-net.c						*/
/*									*/
/*	Transputer program downloader for use with a network of		*/
/*	Transputers connected to a INMOS Transputer system.  This	*/
/*	utility takes a script file description of a network topology	*/
/*	and downloads the associated programs.  The network must	*/
/*	contain a subgraph which forms a tree rooted at	the Transputer	*/
/*	which is connected to the host system (with the reset and	*/
/*	subsystem connections configured in the INMOS fashion).  This	*/
/*	tree must also be a strict subset of the overall link		*/
/*	connection topology for proper operation.			*/
/*									*/
/*	This program must be linked with a version of the INMOS		*/
/*	"link.c" I/O interface to customize it to the associated	*/
/*	hardware!							*/
/*									*/
/*		Written by Kirk Bailey of Logical Systems.		*/
/*									*/
/*		Copyright (c) 1988-1989 by Logical Systems.		*/
/*									*/
/*				12/21/89				*/
/*									*/
/************************************************************************/

#include	<stdio.h>	/* Standard include file */
#include	<ctype.h>	/* Character classification stuff */
#include	<string.h>	/* String functions */
#if defined(vxWorks)
#include	<stdlib.h>	/* Assembler/Linker definitions */
#endif
#include	"taldef.h"	/* Assembler/Linker definitions */
#include	"tload.h"	/* Transputer downloading info */
#include	"tplink.h"      /* Wad link.h conflect with EPICS */
#include <cicsLib.h>



int size;
static int debug;
int fd;				/* lock file desc */

int reset = 1;

/************************************************************************/
/*									*/
/*			Global Data Definitions				*/
/*									*/
/************************************************************************/

/*
 *	Configuration stuff.  Timeout values are in milliseconds.
 */
#define	NO_SERVER
#define vxw
#define	DEFAULT_SERVER	"scio"		/* Default host server */
#define IODRIVER	"SCIO"

#define	DEF_DEC_TOUT	1000L		/* 1 sec load decode timeout default */
#define	MAX_DEC_TOUT	20000L		/* 20 secs maximum decode timeout */
#define	MIN_DEC_TOUT	500L		/* .5 secs minimum decode timeout */

#define	DEF_LVL_TOUT	500L		/* .5 sec/level timeout default */
#define	MAX_LVL_TOUT	1000L		/* 1 secs/level maximum timeout */
#define	MIN_LVL_TOUT	25L		/* .025 secs/level minimum timeout */

#define	LINKIN16_ADDR	0x8008		/* Base addr of input links on T4/T8 */
#define	LINKIN32_ADDR	0x80000010	/* Base addr of input links on T4/T8 */
#define	LINKOUT16_ADDR	0x8000		/* Base addr of output links on T4/T8 */
#define	LINKOUT32_ADDR	0x80000000	/* Base addr of output links on T4/T8 */
#define	MAX_LINKS	4		/* Max # of links/processor */
#define	MAX_NODES	1000		/* Maximum user node # */

#define	MAX_PACKET	255		/* Longest packet with this driver */

/*
 *	I/O definitions.
 */
#ifdef vxw
extern int chan_end();
extern struct b011_map *map;
int size;
int fd;				/* lock file desc */
int Error = 0;
#endif

char	inbuf[FNSIZE + 1] = {0};	/* Input load filename buffer */
STREAM	in_fp = NULL;			/* Input load file pointer */

char	infobuf[FNSIZE + 1] = {0};	/* Network info filename buffer */
STREAM	info_fp = NULL;			/* Network info file pointer */

/*
 *	General definitions.
 */

#define	FROM_SYSTEM_RESET    0x01 /* Boot is from system chain */
#define	FROM_SUBSYS_RESET    0x02 /* Boot is from sub-system chain */

struct link_info
{
    int other_node;		/* The node which this connects to */
    int link_num;		/* Link # within other node if known */
};

struct program_info
{
    struct program_info *next;	/* Next program on the list */
    int     prog_num;		/* Program # in list */
    UCHAR   cpu_type;		/* Cpu type for program */
    SLONG   entry;		/* Program entrypoint */
    char    name[FNSIZE + 1];	/* Name of program being loaded */
    SLONG   stack;		/* Initial program stack pointer */
};

struct node_info
{
    UCHAR    boot_status;	/* Boot system/subsystem status */
    int      parent_num;	/* Who boots me via "boot_status" */
    int      system_output;	/* System output node connection */
    int      subsys_output;	/* Sub-system output node connection */
    int      next_boot;		/* Next user node to boot */
    int      boot_id;		/* Node id during booting */
    struct program_info *prog;	/* Info about prog to load on node */
    SLONG    timeout;		/* Timeout value for this node */
    struct link_info links[MAX_LINKS]; /* Links-to info */
};


UINT    buf_size = MAX_PACKET;		    /* Maximum buffer length */
SLONG   decode_timeout = (DEF_DEC_TOUT * 1000L) / MICRO_LOW_TICK;
char    host_server[FNSIZE + 1] = DEFAULT_SERVER;
SLONG   host_timeout = 0;		    /* Host communication timeout */
SLONG   level_timeout = (DEF_LVL_TOUT * 1000L) / MICRO_LOW_TICK;
UINT    line = 0;			    /* Current input line # */
int     LinkId = 0;			    /* LinkId from "OpenLink" */
struct node_info nodes[MAX_NODES + 1] = {0};/* Node information table */
STRING  open_str = NULL;		    /* Name to use for "OpenLink" call */
char    parse_buf[MAXLNSIZE + 1] = {0};	    /* Net info parsing buffer */
STRING  parse_ptr = NULL;		    /* Parsing buffer pointer */
struct program_info *programs = NULL;       /* List of programs to load */
int     root_node = -1;			    /* Root node */
char tmp[80];

/************************************************************************/
/*									*/
/*			Function Type Declarations			*/
/*									*/
/************************************************************************/
long    chan_read();
struct  program_info *add_program();
long addProgram();

VOID	banner();
int	boot_in_help();
long	boot_order();
int	boot_pre_help();
long	bootstrap();
long	close_input();
long	copyOut();
VOID	do_response();
long	doResponse();
long	download();
long	error_scan();
VOID	exit();
VOID	exit_close();
VOID	exitClose();
SLONG	eval();
long	Eval();
VOID	fatal();
BOOL	get_fname();
SLONG	hexbin();
void init_ldnet();
void init_vxw_link ();
BOOL	ishex();
BOOL	isoct();
long	localize_boot();
VOID	local_store();
long	main();
long	makefn();
BOOL	match();
BOOL	match_first();
BOOL	not_fname();
long	open_input();
long	parse();
VOID	parse_fatal();
VOID	parseFatal();
VOID	parse_node();
long	parseNode();
VOID	parse_warning();
long	pick_ichan();
VOID	skip_space();
SLONG	tin_4b();
VOID	tout_str();
long	toutStr();
VOID	tout_1b();
VOID	tout_4b();
long	tout1b();
long	tout4b();
VOID	usagerr();
VOID	warning();
long	in1b();
long	in2b();
long	in4b();
/*
 *	The following functions may be called only for side effects.
 */
/*lint +fvr */

int	in_1b();
int	in_2b();
SLONG	in_4b();

/*lint -fvr */
 
/************************************************************************/
/*									*/
/*	Routine:	main						*/
/*									*/
/*	Opens the network information file and controls the rest of the	*/
/*	operation.  Terminates by "exec"ing the desired I/O driver	*/
/*	process.							*/
/*									*/
/*	Parameters:							*/
/*		argc -	An integer count of the command line arguments.	*/
/*		argv -	A pointer to an array of pointers which point	*/
/*			to individual command line arguments.		*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

#if defined(vxWorks)
#include "call_main.h"
call_main( ldnet, xldnet )
long xldnet(argc, argv)
#else
long
main(argc, argv)
#endif
int	argc;
char	*argv[];
{
    extern STRING getenv();
    
    int     node;

#if 0
    int noio = 0;
    extern FILE *outfp;
    
    if (strcmp(options, "-q") == 0)
	noio++;
    else 
#endif
 
    init_vxw_link();
    init_ldnet();

    open_str = NULL;  
    programs = NULL;     
    root_node = -1; 

    banner();			/* Tell user who we are */
    if (argc < 2)
    {
	usagerr();		/* Must have at least a info file */
	return ERROR;
    }
    if (argc >= 3)
	reset = 0;

    if (strcmp(argv[0], "xldnet") == 0)
	debug = 4;

    /*
     *	Open network information file.
     */
    if (makefn(argv[1], infobuf, LD_INFO_EXT)== ERROR)	/* Make complete filename */
        return ERROR;
    printf("file = %s\n",infobuf);
    if ((info_fp = (STREAM) fopen(infobuf, "r")) == NULL)
    {
	sprintf(tmp,"Unable to open network information file: %s\n", infobuf);
	cicsLogMessage(0,tmp);
	return ERROR;
    }
    /*
     *	Initialize node information.
     */
    for (node = 0; node <= MAX_NODES; node++)
    {
	nodes[node].prog = NULL;	/* Unused initially */
	nodes[node].system_output = -1;	/* Filled in later */
	nodes[node].subsys_output = -1;	/* Filled in later */
	nodes[node].boot_id = -1; 	/* Node id during bootstrap */
    }
    /*
     *	Parse the network information file, figure out the boot order and
     *	load the network!
     */
    if (parse() == ERROR)			/* Parse network info file */
        return ERROR;
    if(error_scan()== ERROR)		/* Error check and polish node info */
        return ERROR;
    if (boot_order() == ERROR)	/* Compute bootstrap ordering */
        return ERROR;
    /*
     *	Reset the transputer and download the bootstrap program.
     */
    open_str = getenv(LINK_NAME);	/* Get desired link name if present */
    LinkId = OpenLink(open_str);	/* Open specified link channel */
    if (open_str == NULL)
	open_str = "(default)";
    if (LinkId < 0)/* use semaphores and use already open link pr????????*/
    {
        exitClose(ERROR);
	sprintf(tmp,"Unable to open link: %s \n", open_str);
	cicsLogMessage(0,tmp);
	return ERROR;
    }
    if (reset && ResetLink(LinkId) < 0)
    {
        sprintf (tmp,"Unable to reset link: %s\n", open_str);
	cicsLogMessage(0,tmp);
    }
    if (bootstrap() == ERROR)		/* Reset/bootstrap the network */
    {
        exitClose(ERROR);
        return ERROR;
    }
    if (download() == ERROR)	/* Download the programs */
    {
         exitClose(ERROR);
        return ERROR;
    }
#ifdef notdef
   
    if (CloseLink(LinkId) < 0)
    {
	LinkId = 0;		/* So "fatal" doesn't try again */
	sprintf(tmp,"Unable to close link: %s \n ", open_str);
	cicsLogMessage(0,tmp);
	return ERROR;
    }
#endif
    /*
     *	Close the network information file and hand control over to whatever
     *	I/O driver program has been specified (if any).
     *
     *	Note that a dummy slot is put in place in 'argv[0]' since the MSC
     *	'execvp' library routine trashes it anyway!  The handler must get the
     *	desired 'argv[0]' value from the 'argv[1]' slot, etc.
     */
    if (fclose(info_fp) == EOF)
    {
	sprintf(tmp,"Unable to close network information file: %s \n", infobuf);
	cicsLogMessage(0,tmp);
	return ERROR;
    }

#ifdef notdef
    if (host_server[0] != '\0')
    {
	if (execvp(host_server, argv) == -1)
	{
	    sprintf(tmp,"Unable to EXEC I/O driver: ", host_server);
	cicsLogMessage(0,tmp);
	    return ERROR;
	}
    }
    else
	warning("No I/O driver installed", "");
#endif
#if defined(DOS)		/* XXXXXXXXXXX */
    /* 
     * default server is built in
     */
    
    if ((host_server[0] != '\0') && (strcmp(host_server,DEFAULT_SERVER)!=0))
    {
	int i;
	
	chan_end();
	for (i=3; i<getdtablesize(); i++)
	    close(i);
	if (execvp(host_server, &argv[2]) == -1)
	{
	    sprintf(tmp,"Unable to EXEC I/O driver: ", host_server);
	cicsLogMessage(0,tmp);
	    return ERROR;
	}
    }
    else
    {
	if (!noio)
	{
	    cio_main(argc,argv);
	}
	else
	{
	    fclose(outfp);
	    sleep(1);
	    chan_end();
	}
    }
#else

  
    CloseLink(LinkId);
#ifdef NO_SERVER
    
#else
    if(host_server[0] != '\0')
    {
	if(execvp(host_server,argv) == -1)
	{
	    sprintf(tmp,"Unable to EXEC I/O driver: %s \n",host_server);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
    }
    else	warning("No I/O driver installed","");
#endif
#endif
     exitClose(NOERRORS); 
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	addProgram					*/
/*									*/
/*	See if a program to be loaded on the network is already on the	*/
/*	load list, if not add it.					*/
/*									*/
/*	Parameters:	"s" -	Points to the program name to add.	*/
/*									*/
/*	Returns:	A pointer to the "program_info" structure which	*/
/*			describes the program loading details.		*/
/*									*/
/************************************************************************/

long
addProgram(s,temp)
STRING  s;
struct program_info **temp;
{
    int val;
    register int i;
    register struct program_info **p;
  /*   register struct program_info *temp; */

    /*	See if already on list. */
    for (i = 0, p = (struct program_info **) &programs;
					*p != NULL; i++, p = &((*p)->next))
    {
	if (strcmp(s, (*p)->name) == 0)	/* Already on list? */
	{
	    *temp = *p;
	    return (OK);		/* Yep */
	}
    }

    /*	Nope, malloc up a fresh unit.  */
    if ((*temp = (struct program_info *)
	 			malloc(sizeof(struct program_info))) == NULL)
    {
	parseFatal("Insufficient memory for names of programs being loaded", "");
	return ERROR;
    }
    
    /*	Link it into the programs-to-be-loaded list. */
    *p = *temp;
    (*temp)->next = NULL;		/* Current end of list */
    (*temp)->prog_num = i;		/* Program # in list */
    strcpy((*temp)->name,s);	/* Program name */
    
    /*Open the file to determine the cpu type and other overhead information
     *	(and also to verify that the program exists).
     */
    if (open_input(s,&val) == ERROR)
    {
        exitClose(ERRORS);
	return ERROR;
    }
    switch (val)
    {
      case TYPE_UNKNOWN:
	(*temp)->cpu_type = BID_T4 | BID_T8;
	break;
	
      case TYPE_212:
	(*temp)->cpu_type = BID_T2;
	break;
	
      case TYPE_414:
	(*temp)->cpu_type = BID_T4;
	break;
	
      case TYPE_800:
	(*temp)->cpu_type = BID_T8;
	break;

      default:
	sprintf(tmp,"Internal error #1 \n");
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
     if (in1b(&val) == ERROR) 
         return ERROR; 
    if (val != T_LOAD)	/* Should be T_LOAD record */
    {
	sprintf(tmp,"Missing T_LOAD record in download input file: %s \n", inbuf);
	    cicsLogMessage(0,tmp);
	return ERROR;
    }

     if (in4b(&val) == ERROR) 	/* Toss load address */
         return ERROR; 
  
     if (in1b(&val) == ERROR) 
         return ERROR; 
     if (val != T_STACK)	/* Should be T_STACK record */
     {
       sprintf(tmp,"Missing T_STACK record in download input file: %s \n", inbuf);
      
	    cicsLogMessage(0,tmp);
	 return ERROR;
     }


      if (in4b(&((*temp)->stack)) == ERROR)  	/* Initial stack address */
          return ERROR;  
   /*  (*temp)->stack = in_4b(); */ 	/* Initial stack address */

     if (in1b(&val) == ERROR) 
         return ERROR; 
    if (val != T_ENTRY)	/* Should be T_ENTRY record */
    {
	sprintf(tmp,"Missing T_ENTRY record in download input file: %s \n", inbuf);
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    if (in4b(&((*temp)->entry)) == ERROR)  	/* Entrypoint */
        return ERROR;  
  /*   (*temp)->entry = in_4b(); */	/* Entrypoint */
  /*   close_input(); */
    if ( close_input() == ERROR)
      return ERROR; 
    
    return (OK);
}
/************************************************************************/
/*									*/
/*	Routine:	banner						*/
/*									*/
/*	Display sign-on message to user.				*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
banner()
{
    fprintf(stderr, "LD-NET (Network Loader), %s [Link I/O Driver: '%s']\n",
	    			VERSION, IODRIVER);
    fprintf(stderr, "%s\n\n", COPYRIGHT);
}

/************************************************************************/
/*									*/
/*	Routine:	boot_order					*/
/*									*/
/*	Given the network configuration information, perform a in-order	*/
/*	traversal of the sub-tree of the network which will be used	*/
/*	during the bootstrapping operation.  This establishes the node	*/
/*	numbers used during the bootstrapping process and sets the	*/
/*	node depths within the bootstrap tree.  Then we process the	*/
/*	node data to see if any nodes to be booted aren't connected to	*/
/*	the root and convert the level information into timeout values.	*/
/*	Assuming everything is OK, we do a pre-order traversal to	*/
/*	establish a bootstrapping order for the nodes and report the	*/
/*	desired order back (via the "next_boot" linked list through the	*/
/*	"nodes" data structure).					*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

/*
 *	Helper for "boot_order".
 *
 *	The in-order traverse routine to set the bootstrap node #'s and
 *	establish the node depths within the tree (and the maximum depth).
 */
int
bo_in_help(node, boot_num, level, max_level)
int     node;
int     boot_num;
int     level;
int     *max_level;
{
    if (node == -1)
	return (boot_num);
    
    nodes[node].timeout = ++level;
    if (level > *max_level)
	*max_level = level;
    boot_num = bo_in_help(nodes[node].system_output, boot_num, level,
			  max_level) + 1;
    nodes[node].boot_id = boot_num;
    return (bo_in_help(nodes[node].subsys_output, boot_num, level,
		       max_level));
}

/*
 *	Helper for "boot_order".
 *
 *	A pre-order traverse routine to establish the bootstrapping order.
 */
int
bo_pre_help(new, old)
int     new;
int     old;
{
    if (new == -1)
	return (old);
    
    nodes[old].next_boot = new;
    old = bo_pre_help(nodes[new].system_output, new);
    return (bo_pre_help(nodes[new].subsys_output, old));
}

long
boot_order()
{
    BOOL    err_printed;
    int     max_level;
    register int temp;
    /*
     *	First scan through and set the level # of each node and the bootstrap
     *	node numbers.
     */
    max_level = 0;
    (VOID) bo_in_help(root_node, D_MASK, 0, &max_level);
    /*
     *	Compute the host communication timeout value (tenths of seconds!).
     */
    host_timeout = (((SLONG) MICRO_LOW_TICK) *
		    (decode_timeout + (level_timeout *
				       ((SLONG) max_level + 1L)))) / 100000L;
    /*
     *	Now scan through and see if all nodes can be reached (also convert
     *	bootstrap tree node depths to node timeout values).
     */
    for (err_printed = FALSE, temp = 1; temp <= MAX_NODES; temp++)
    {
	if (nodes[temp].prog == NULL)	/* In network? */
	    continue;		/* Nope */
	if (nodes[temp].boot_id < 0)	/* Does it have a parent? */
	{			/* Nope */
	    if (! err_printed)
	    {
	    cicsLogMessage(0,"FATAL: The following node(s) are unreachable from the root node\n");
		err_printed = TRUE;
	    }
	   
	}
	else			/* In network and has a parent */
	{
	    nodes[temp].timeout = decode_timeout + (level_timeout *
						    ((SLONG) max_level - nodes[temp].timeout));
	    /*
	     *	Ensure that timeout value are non-negative.
	     */
	    if (nodes[temp].prog->cpu_type == BID_T2)
	    {
		if (nodes[temp].timeout & 0xFFFF8000)
		{
		    sprintf(tmp,"WARNING: Node %d timeout too long for T2 CPU, clipping at maximum\n",temp);
	    cicsLogMessage(0,tmp);
		    nodes[temp].timeout = 0x00007FFF;
		}
	    }
	    else
	    {
		if (nodes[temp].timeout & 0x80000000)
		{
		    sprintf(tmp, "WARNING: Node %d timeout too long for T4/T8 CPU, clipping at maximum\n",temp);
	    cicsLogMessage(0,tmp);
		    nodes[temp].timeout = 0x7FFFFFFF;
		}
	    }
	}
    }
    if (err_printed)
    {
	exitClose(ERRORS);
	return ERROR;
    }
    /*
     *	Now do the pre-order traversal to establish a load order.
     */
    temp = bo_pre_help(root_node, 0);
    nodes[temp].next_boot = -1;
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	bootstrap					*/
/*									*/
/*	Download the bootstrap programs to the Transputer network and	*/
/*	make sure everything goes OK.					*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

/*
 *	See the "netboot.tal" file for the source for this bootstrap code.
 */
unsigned char *bootcode;
/* some routines change the bootcode,  This backup is necessary to restore
*	the bootcode back to original*/
static const unsigned char bootcode_back[] =
{
    0xf4,0xd0,0x27,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0xf7,0x68,0x4e,0x21,0xfb,0x23,
    0xfc,0xb7,0xd1,0xd2,0x24,0xf2,0xd1,0x24,0xf2,0x71,0x21,0xfc,0x71,0x21,0xf8,
    0x71,0x71,0xe0,0x71,0x71,0x51,0x4a,0x23,0xf4,0x24,0xfa,0x40,0x9e,0x11,0x72,
    0x41,0xf7,0x25,0x44,0x21,0xfb,0x72,0x2f,0x4f,0xf7,0x2e,0x03,0x40,0xd2,0x11,
    0x76,0x44,0x23,0xf4,0xf4,0x44,0xfb,0x22,0xf0,0x71,0x33,0x21,0xa4,0x18,0x71,
    0xe5,0x71,0x33,0x28,0x20,0x80,0x71,0xe4,0x49,0x21,0xfb,0x71,0xe2,0x71,0x53,
    0x81,0x23,0xf9,0x22,0xf0,0x72,0x70,0x44,0x23,0xf4,0xf4,0x41,0xfb,0x72,0x31,
    0x70,0x44,0x23,0xf4,0xf4,0x72,0x30,0xfb,0x11,0x70,0x44,0xf7,0x21,0xf5,0x63,
    0x90,0x73,0x21,0xa5,0x22,0xf9,0x21,0x74,0x77,0x60,0xef,0x72,0x21,0x75,0x77,
    0x23,0xfc,0xf0,0xf6,0x00,0x00,0x00,0x00,0x00,0x00,0xd4,0x14,0x72,0x41,0xf7,
    0x75,0x72,0x74,0xf7,0x75,0x84,0x30,0xd1,0x71,0x43,0xf9,0x23,0xad,0x70,0x71,
    0xf9,0xa4,0x1e,0x66,0x9b,0x03,0x18,0x66,0x97,0x22,0xf2,0x21,0x76,0xf5,0xd6,
    0x10,0x81,0x23,0xf9,0x21,0xf5,0x7c,0xa3,0x18,0xd1,0x06,0x21,0x72,0x64,0xa4,
    0x1e,0xd1,0x22,0xf2,0x76,0xf4,0x40,0xf9,0x61,0xa7,0x71,0x33,0xd3,0x73,0x69,
    0x96,0x71,0x34,0x69,0x92,0x14,0x73,0x44,0xf7,0x74,0x6a,0x9b,0x60,0x07,0x1e,
    0x69,0x92,0x18,0x6a,0x9f,0x71,0xc0,0xa4,0x41,0xd3,0x63,0x01,0x75,0x30,0x21,
    0x77,0xf4,0xc0,0x64,0xa9,0x71,0xc1,0xac,0x75,0x8c,0x75,0x88,0x30,0x74,0x60,
    0x84,0x24,0xfa,0x65,0x0a,0x75,0x88,0x30,0xd1,0x75,0x8c,0x30,0xd3,0x73,0x66,
    0xaf,0x73,0x60,0x8f,0xd3,0x40,0x71,0x23,0xfb,0x71,0x81,0xd1,0x60,0x00,0x2a,
    0x4a,0x26,0x44,0x23,0xf4,0x40,0x21,0x27,0xfc,0xd6,0xd1,0x71,0xa2,0x71,0xd6,
    0x76,0x4a,0x22,0xfc,0x87,0xd1,0x44,0x71,0x44,0x24,0xf6,0xc0,0xab,0x42,0x71,
    0x42,0x24,0xf6,0xc0,0xa4,0x29,0xfc,0x48,0x40,0xd3,0x74,0x24,0xf6,0xa9,0x72,
    0x75,0xf4,0xae,0x72,0x22,0x20,0x80,0x04,0x76,0x24,0x20,0x80,0x61,0x2e,0x90,
    0x60,0x0e,0x22,0xf2,0x25,0xf4,0x64,0x49,0x21,0xfb,0xd5,0x41,0x40,0xe0,0x40,
    0x44,0xe0,0x95,0x40,0xe0,0x92,0x6f,0xa7,0x22,0xf2,0x29,0x4c,0xf5,0x22,0xfb,
    0x40,0x22,0xf0,

};

#if 0
unsigned char bootcode[] =
{
    0xF4,0xD0,0x27,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0xF7,0x68,0x4E,0x21,0xFB,0x23,
    0xFC,0xB7,0xD1,0xD2,0x24,0xF2,0xD1,0x71,0x21,0xFC,0x71,0x21,0xF8,0x71,0x71,
    0xE0,0x71,0x71,0x51,0x4A,0x23,0xF4,0x24,0xFA,0x40,0x9E,0x11,0x72,0x41,0xF7,
    0x25,0x46,0x21,0xFB,0x72,0x2F,0x4F,0xF7,0x2E,0x05,0x40,0xD2,0x11,0x76,0x44,
    0x23,0xF4,0xF4,0x44,0xFB,0x22,0xF0,0x71,0x33,0x21,0xA4,0x18,0x71,0xE5,0x71,
    0x33,0x28,0x20,0x80,0x71,0xE4,0x49,0x21,0xFB,0x71,0xE2,0x71,0x53,0x81,0x23,
    0xF9,0x22,0xF0,0x72,0x70,0x44,0x23,0xF4,0xF4,0x41,0xFB,0x72,0x31,0x70,0x44,
    0x23,0xF4,0xF4,0x72,0x30,0xFB,0x11,0x70,0x44,0xF7,0x21,0xF5,0x63,0x90,0x73,
    0x21,0xA5,0x22,0xF9,0x21,0x74,0x77,0x60,0xEF,0x72,0x21,0x75,0x77,0x23,0xFC,
    0xF0,0xF6,0x00,0x00,0x00,0x00,0x00,0x00,0xD4,0x14,0x72,0x41,0xF7,0x75,0x72,
    0x74,0xF7,0x75,0x84,0x30,0xD1,0x71,0x43,0xF9,0x23,0xAD,0x70,0x71,0xF9,0xA4,
    0x1E,0x66,0x9B,0x03,0x18,0x66,0x97,0x22,0xF2,0x21,0x76,0xF5,0xD6,0x10,0x81,
    0x23,0xF9,0x21,0xF5,0x7C,0xA3,0x18,0xD1,0x06,0x21,0x72,0x64,0xA4,0x1E,0xD1,
    0x22,0xF2,0x76,0xF4,0x40,0xF9,0x61,0xA7,0x71,0x33,0xD3,0x73,0x69,0x96,0x71,
    0x34,0x69,0x92,0x14,0x73,0x44,0xF7,0x74,0x6A,0x9B,0x60,0x07,0x1E,0x69,0x92,
    0x18,0x6A,0x9F,0x71,0xC0,0xA4,0x41,0xD3,0x63,0x01,0x75,0x30,0x21,0x77,0xF4,
    0xC0,0x64,0xA9,0x71,0xC1,0xAC,0x75,0x8C,0x75,0x88,0x30,0x74,0x60,0x84,0x24,
    0xFA,0x65,0x0A,0x75,0x88,0x30,0xD1,0x75,0x8C,0x30,0xD3,0x73,0x66,0xAF,0x73,
    0x60,0x8F,0xD3,0x40,0x71,0x23,0xFB,0x71,0x81,0xD1,0x60,0x00,0x00,0x00,0x2A,
    0x4A,0x26,0x44,0x23,0xF4,0x40,0x21,0x27,0xFC,0xD6,0xD1,0x71,0xA2,0x71,0xD6,
    0x76,0x4A,0x22,0xFC,0x87,0xD1,0x44,0x71,0x44,0x24,0xF6,0xC0,0xAB,0x42,0x71,
    0x42,0x24,0xF6,0xC0,0xA4,0x29,0xFC,0x48,0x40,0xD3,0x74,0x24,0xF6,0xA9,0x72,
    0x75,0xF4,0xAE,0x72,0x22,0x20,0x80,0x04,0x76,0x24,0x20,0x80,0x61,0x2D,0x9E,
    0x60,0x0E,0x25,0xF4,0x64,0x4B,0x21,0xFB,0xD5,0x41,0x40,0xE0,0x40,0x44,0xE0,
    0x95,0x40,0xE0,0x92,0x6F,0xA7,0x22,0xF2,0x29,0x4C,0xF5,0x22,0xFB,0x40,0x22,
    0xF0
	};

extern unsigned char *bootcode;
#endif

long
bootstrap()
{
    register int i;
    UINT    length;
    int     node;
    /*
     *	Handle initial error checking on the bootstrap code.
     */
#if 0
    length = sizeof(bootcode);	/* Bootstrap length */
#else
    extern int size_bootcode;
    length = size_bootcode;
#endif
    if (length > (MAX_PACKET * 2))
    {
	sprintf(tmp,"Transputer bootstrap code is too long\n");
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    if ((bootcode[0x103] != 0) || (bootcode[0x104] != 0) ||
	(bootcode[0x105] != 0) || (bootcode[0x106] != 0))
    {
	    cicsLogMessage(0,"Transputer bootstrap code needs a window from 0x103 through 0x106\n");
	return ERROR;
    }
    for (i = (((LOCAL_OFFSET + LOCALS_STACK) * 4) - 1);
	 i < (((LOCAL_OFFSET + LOCALS_STACK + PARAM_LENGTH) * 4) - 1);
	 i++)
    {
	if (bootcode[i] != 0)
	{
	sprintf(tmp,"bootcode[%d] = %d, bootcode = %x\n", i, bootcode[i], (unsigned int)bootcode);
	    cicsLogMessage(0,tmp);
	    cicsLogMessage(0,"Transputer bootstrap code local variables not initialized to zero\n");
	    return ERROR;
	}
    }
    /*
     *	First load root Transputer with blow-by-blow description (in addition
     *	to root timeout).
     */
    if (TestRead(LinkId))
    {
	    cicsLogMessage(0,"Transputer response available prior to bootstrap\n");
	return ERROR;
    }
    if (localize_boot(root_node) == ERROR)	/* Initialize node info */
        return ERROR;
    sprintf(tmp, "Loading first phase of bootstrap to root node %d\n",
	    root_node);
	    cicsLogMessage(0,tmp);
    if(tout1b((int) MAX_PACKET) == ERROR)
        return ERROR;
    if(toutStr((STRING) bootcode, MAX_PACKET) == ERROR)
        return ERROR;
	   fprintf(stderr,"Finished loading first phase, awaiting first acknowledge\n");
    if( doResponse(root_node) == ERROR)	/* Handle response */
        return ERROR;
    fprintf(stderr, "Loading second phase of bootstrap to root node %d\n",
	    root_node);
    if(tout1b((int) MAX_PACKET) == ERROR)
        return ERROR;
    if(toutStr((STRING) (bootcode + MAX_PACKET), MAX_PACKET) == ERROR)
        return ERROR;
    
    fprintf(stderr, "Bootstrap loaded, awaiting acknowledge\n");
    if (doResponse(root_node) == ERROR)	/* Handle response */
       return ERROR;
    fprintf(stderr, "Successfully bootstrapped root node %d\n\n", root_node);
    fprintf(stderr, "Bootstrapping the remainder of the network:\n");
    /*
     *	Bootstrap the rest of the network.
     */
    for (node = nodes[root_node].next_boot; node >= 0;
	 					node = nodes[node].next_boot)
    {
	if (TestRead(LinkId))
	{
	    sprintf(tmp,"Transputer response available prior to message\n");
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
	fprintf(stderr, "\tBootstrapping node %d\n", node);
	if (localize_boot(node) == ERROR)	/* Initialize node info */
	    return ERROR;
	if(tout1b((int) MAX_PACKET) == ERROR)
	    return ERROR;
	if(toutStr((STRING) bootcode, MAX_PACKET) == ERROR)
	    return ERROR;
	if (doResponse(node) == ERROR)	/* Handle response */
	    return ERROR;
	if(tout1b((int) MAX_PACKET) == ERROR)
	    return ERROR;
	if(toutStr((STRING) (bootcode + MAX_PACKET), MAX_PACKET) == ERROR)
	   return ERROR;
	if (doResponse(node) == ERROR)	/* Handle response */
	    return ERROR;
    }
    cicsLogMessage(0, "Network successfully bootstrapped\n\n");
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	close_input					*/
/*									*/
/*	Close the current download input file.				*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
close_input()
{
    if (fclose(in_fp) == EOF)
    {
	sprintf(tmp,"Unable to close download input file: %s \n", inbuf);
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    return OK;
}
#if 0
/************************************************************************/
/*									*/
/*	Routine:	copy_out					*/
/*									*/
/*	Copy a specified # of bytes from the input load file to the	*/
/*	root transputer.						*/
/*									*/
/*	Parameters:							*/
/*		"count"	The number of bytes to copy.			*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
copy_out(count)
UINT count;
{
    char    buf[MAX_PACKET];
    int     rval;
    
    if (fread(buf, 1, count, in_fp) != count)
    {
	if (ferror(in_fp) || feof(in_fp))
	{
	    fatal("Error reading download input file: ", inbuf);
	}
    }

    if (debug==4)
    {
	for(rval=0; rval<count;rval++)
	    fprintf(stdout,"0x%02x,",(buf[rval]&0xff));
	fprintf(stdout,"\n");
    }

    if ((rval = WriteLink(LinkId, buf, count, (int) host_timeout)) != count)
    {
	if (rval < 0)
	    fatal("Error writing to link: ", open_str);
	else
	    fatal("Timed out waiting to write to link: ", open_str);
    }
}
#endif
/************************************************************************/
/*									*/
/*	Routine:	copyOut					*/
/*									*/
/*	Copy a specified # of bytes from the input load file to the	*/
/*	root transputer.						*/
/*									*/
/*	Parameters:							*/
/*		"count"	The number of bytes to copy.			*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
copyOut(count)
UINT count;
{
    char    buf[MAX_PACKET];
    int     rval;
    
    if (fread(buf, 1, count, in_fp) != count)
    {
	if (ferror(in_fp) || feof(in_fp))
	{
	    sprintf(tmp,"Error reading download input file: %s \n", inbuf);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
    }

    if (debug==4)
    {
	for(rval=0; rval<count;rval++)
	    fprintf(stdout,"0x%02x,",(buf[rval]&0xff));
	fprintf(stdout,"\n");
    }

    if ((rval = WriteLink(LinkId, buf, count, (int) host_timeout)) != count)
    {
	if (rval < 0)
	{
	    sprintf(tmp,"Error writing to link: %s \n", open_str);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
	else
	{
	    sprintf(tmp,"Timed out waiting to write to link:%s \n ", open_str);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
    }
    return OK;
}
/************************************************************************/
/*									*/
/*	Routine:	doResponse					*/
/*									*/
/*	Verify that root Transputer gives correct acknowledgements, if	*/
/*	not do appropriate stuff to handle error diagnosis and		*/
/*	reporting.							*/
/*									*/
/*	Parameters:							*/
/*		node -	The node # for which the original message was	*/
/*			intended. 0 if the message is a	broadcast one.	*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
doResponse(node)
int     node;
{
    static STRING err1="Unable to continue error traceback any farther\n";
    int     link;
    int     old_node;
    int     result;
    STRING  str;
  
     result = tin_4b(); 
    if (result != 0)		/* If some form of error */
    {
	if (node != 0)
	{
	    sprintf(tmp, "Network error detected while bootstrapping node %d\n", node);
	    cicsLogMessage(0,tmp);
	}
	else
	{
	    sprintf(tmp, "Network error detected while downloading program(s)");
	    cicsLogMessage(0,tmp);
	}
	fprintf(stderr, "Beginning error traceback:\n");
	old_node = 0;
	node = root_node;
	FOREVER
	{
	    if (result & (ERR_BOOT | ERR_CPU | ERR_TIMEOUT))
		break;
	    if (nodes[node].prog->cpu_type == BID_T2)
		link = (result - (LINKIN16_ADDR & 0xFF)) / 2;
	    else
		link = (result - (LINKIN32_ADDR & 0xFF)) / 4;
	    if ((link < 0) || (link >= MAX_LINKS))
	    {
		fprintf(stderr, err1);
		exitClose(ERRORS);
		return ERROR;
	    }
	    fprintf(stderr, "Error received by node %d via channel %d\n",
		    node, link);
	    
	    if (nodes[node].links[link].other_node < 0)
	    {
		fprintf(stderr, err1);
		exitClose(ERRORS);
		return ERROR;
	    }
	    node = nodes[old_node = node].links[link].other_node;
	    result = tin_4b();
	    result = result & 0x7FFF;
	}
	if (result == 0)
	    fprintf(stderr, err1);
	else
	{
	    if (result & ERR_TIMEOUT)
	    {
		node = old_node; /* Timeout error */
	    }
	    fprintf(stderr, "Error originated at node %d and was caused by ", node);
	    
	    if (result & ERR_CPU)
	    {
		switch ((result & (~ERR_CPU)) / BID_MAX_MREVS)
		{
		  case (BID_T2XX / BID_MAX_MREVS):
		    str = "T212/T222";
		    break;
		  case (BID_T225 / BID_MAX_MREVS):
		    str = "T225";
		    break;
		  case (BID_T400 / BID_MAX_MREVS):
		    str = "T400";
		    break;
		  case (BID_T414 / BID_MAX_MREVS):
		    str = "T414";
		    break;
		  case (BID_T425 / BID_MAX_MREVS):
		    str = "T425";
		    break;
		  case (BID_T800 / BID_MAX_MREVS):
		    str = "T800";
		    break;
		  case (BID_T801 / BID_MAX_MREVS):
		    str = "T801";
		    break;
		  case (BID_T805 / BID_MAX_MREVS):
		    str = "T805";
		    break;
		  default:
		    str = "????";
		}
		fprintf(stderr, "the wrong cpu (was a %s)\n", str);
	    }
	    else
	    {
		if (nodes[node].prog->cpu_type == BID_T2)
		{
		    link = ((result & 0xFF) - (LINKIN16_ADDR & 0xFF)) / 2;
		}
		else
		{
		    link = ((result & 0xFF) - (LINKIN32_ADDR & 0xFF)) / 4;
		}
		if ((link < 0) || (link >= MAX_LINKS))
		    fprintf(stderr, "an unknown source\n");
		else if (result & ERR_TIMEOUT)
		{
		    fprintf(stderr, "a timeout of channel %d\n", link);
		}
		else	/* ERR_BOOT */
		{
		    fprintf(stderr, "the wrong boot channel (was %d)\n", link);
		}
	    }
	}
	exitClose(ERRORS);
	return ERROR;
    }
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	download					*/
/*									*/
/*	Download all necessary programs to the Transputer network.	*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
download()
{
    SLONG   addr;
    SLONG   big_temp;
    BOOL    finished_file;
    register struct program_info *pinfo;
    register UINT prog_num;
    UINT temp;
    int junk;
    /*
     *	Load all the programs into the network, one at a time.
     */
    for (prog_num = 0, pinfo = programs; pinfo != NULL;
	 prog_num++, pinfo = pinfo->next)
    {
	if ( open_input(pinfo->name,&junk) == ERROR)	/* Open file */
	    return ERROR;
	sprintf(tmp, "Downloading program: %s\n", pinfo->name);
	    cicsLogMessage(3,tmp);
	/*
	 *	Loop through all the records in the load file.  Write those that are
	 *	needed to the network, toss the rest.
	 */
	for (finished_file = FALSE; !finished_file; )
	{
	    if (in1b(&temp) == ERROR) 
	        return ERROR; 
	    switch (temp)
	    {
	      case T_DATA:	/* Normal data definition */
		if (in2b(&junk) == ERROR) 	/* Toss line # */
		    return ERROR; 
		if (in2b(&temp) == ERROR)     	/* Get length */ 
		    return ERROR; 
	
		while (temp > (buf_size - 12))
		{
		    temp -= (buf_size - 12);
		    if (tout1b((int) buf_size) == ERROR)
		        return ERROR;
		    if (tout4b((SLONG) prog_num) == ERROR)
		        return ERROR;
		    if (tout4b((SLONG) D_DATA) == ERROR)
		        return ERROR;
		    /*lint -e530 thinks addr hasn't been initialized */
		    if (tout4b((SLONG) addr) == ERROR)
		        return ERROR;
		    /*lint +e530 thinks addr hasn't been initialized */
		    if (copyOut((UINT) (buf_size - 12)) == ERROR)
		        return ERROR;
		    /*lint -e530 thinks addr hasn't been initialized */
		    addr += (buf_size - 12);
		    /*lint +e530 thinks addr hasn't been initialized */
		    if (doResponse(0) == ERROR)
		        return ERROR;
		}
		 if (tout1b((int) (temp + 12)) == ERROR)
		     return  ERROR;;
		 if (tout4b((SLONG) prog_num) == ERROR)
		     return ERROR;;
		 if (tout4b((SLONG) D_DATA) == ERROR)
		     return ERROR;
		/*lint -e530 thinks addr hasn't been initialized */
		 if (tout4b((SLONG) addr) == ERROR)
		     return ERROR;
		/*lint +e530 thinks addr hasn't been initialized */
		if (copyOut(temp) == ERROR)
		    return ERROR;
		/*lint -e530 thinks addr hasn't been initialized */
		addr += temp;
		/*lint +e530 thinks addr hasn't been initialized */
		if (doResponse(0) == ERROR)
		    return ERROR;
		break;
		
	      case T_STORAGE:	/* Reserve space pseudo-op */
		if (tout1b((int) 16) == ERROR)
		    return ERROR;
		if (tout4b((SLONG) prog_num) == ERROR)
		    return ERROR;
		if (tout4b((SLONG) D_STORAGE) == ERROR)
		    return ERROR;
		if (in2b(&junk) == ERROR)	/* Toss line # */
		    return ERROR; 
	
		/*lint -e530 thinks addr hasn't been initialized */
		if (tout4b((SLONG) addr) == ERROR)
		     return ERROR;
		/*lint +e530 thinks addr hasn't been initialized */
		if(in4b(&big_temp) == ERROR)
		    return ERROR;
		if (tout4b((SLONG) big_temp) == ERROR)
		     return ERROR;
		/*lint -e530 thinks addr hasn't been initialized */
		addr += big_temp;
		/*lint +e530 thinks addr hasn't been initialized */
		 if (doResponse(0) == ERROR)
		     return ERROR;
		break;
		

	      case T_DEBUG_DATA: /* Sym. debug stuff */
		  if (in2b(&junk) == ERROR)	/* Toss line # */
		      return ERROR; 
		  if (in4b(&junk) == ERROR)	/* Toss value */
		      return ERROR; 
		  if (in2b(&temp) == ERROR)     /* Get length */	
		      return ERROR; 
		  while (temp--)
		      if (in1b(&junk) == ERROR)     /* Toss debug data */
		          return ERROR; 
		  break;
		
	      case T_FILENAME:	/* Filename record */
		  if (in4b(&junk) == ERROR)	/* Toss old/new #'s */
		      return ERROR; 
		  if (in1b(&temp) == ERROR)     /* Get length */	
		      return ERROR;
		  while (temp--)
		      if (in1b(&junk) == ERROR)   /* Toss filename */ 
		          return ERROR; 
		  break;
		
	      case T_LOAD:	/* Set load address */
		  if (in4b(&addr) == ERROR)	
		      return ERROR; 
		  break;
		
	      case T_STACK:	/* Set stack pointer address */
		  if (in4b(&junk) == ERROR)	/* Ignore */
		      return ERROR;
		  break;
		
	      case T_ENTRY:	/* Set entry point address */
		  if (in4b(&junk) == ERROR)	/* Ignore */
		      return ERROR;
	
		break;
		
	      case T_EOF:	/* Found EOF record */
		  finished_file = TRUE;
		  break;
		
	      default:
	        {
		    sprintf(tmp,"Bad record format on download input file: %s \n", inbuf);
		    cicsLogMessage(0,tmp);
		    return ERROR;
		}
	    }
	}
	if (close_input() == ERROR)		/* Close input file */
	    return ERROR;
    }
    /*
     *	We've loaded all the programs, jumpstart the network!
     */
     if (tout1b((int) 8) == ERROR)
         return ERROR;
     if (tout4b((SLONG) 0) == ERROR)
         return ERROR;		/* Garbage */
     if (tout4b((SLONG) D_DONE) == ERROR)
         return ERROR;
    if (doResponse(0) == ERROR)
        return ERROR;
	    cicsLogMessage(3, "Program downloading completed\n\n");
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	error_scan					*/
/*									*/
/*	Scan through the node structure constructed by "parse" and	*/
/*	flesh out the critical information so that the "boot_order"	*/
/*	routine can determine a reasonable bootstrap load order.  Also	*/
/*	perform additional error detection.				*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
error_scan()
{
    register int child_node;
    register int child_unknown;
    char    errbuf[80];
    register int known;
    register int link;
    register int parent_node;
    register int parent_unknown;
    
    for (child_node = 1; child_node <= MAX_NODES; child_node++)
    {
	if ((nodes[child_node].prog != NULL) &&
	    ((parent_node = nodes[child_node].parent_num) != 0))
	{
	    /*
	     *	First, verify no more than one connection to each system/sub-system
	     *	reset output.
	     */
	    if (nodes[child_node].boot_status & FROM_SYSTEM_RESET)
	    {
		if (nodes[parent_node].system_output != -1)
		{
		    sprintf(errbuf, "%d", parent_node);
		    sprintf(tmp,"More than one node connected to the system output of node:%s \n ", errbuf);
	    cicsLogMessage(0,tmp);
		    
		    return ERROR;
		}
		nodes[parent_node].system_output = child_node;
	    }
	    else if (nodes[child_node].boot_status & FROM_SUBSYS_RESET)
	    {
		if (nodes[parent_node].subsys_output != -1)
		{
		    sprintf(errbuf, "%d", parent_node);
		    sprintf(tmp,"More than one node connected to the sub-system output of node: %s \n",  errbuf);
	    cicsLogMessage(0,tmp);
		    return ERROR;
		}
		nodes[parent_node].subsys_output = child_node;
	    }
	    else
	    {
	    cicsLogMessage(0,"Internal error #2\n");
		return ERROR;
	    }
	    /*
	     *	Second, verify that if more than one connection exists between this
	     *	child node and its parent that enough information is available to
	     *	determine which links at least one connection has on both ends.
	     */
	    
	    /*
	     *	First flesh out links which are fully defined at only one end.
	     */
	    for (link = 0; link < MAX_LINKS; link++)
	    {
		/*
		 *	Handle links defined on parent side.
		 */
		if ((nodes[parent_node].links[link].other_node == child_node) &&
		    ((known = nodes[parent_node].links[link].link_num) >= 0))
		{
		    child_unknown = nodes[child_node].links[known].other_node;
		    if ((child_unknown >= 0) && (child_unknown != parent_node))
		    {
			sprintf(errbuf, "%d link %d, and node %d", parent_node,
				link, child_node);
			sprintf(tmp,"Conflict in defined connection between node %s\n ",     errbuf);
	    cicsLogMessage(0,tmp);
			return ERROR;
		    }
		    nodes[child_node].links[known].other_node = parent_node;
		    child_unknown = nodes[child_node].links[known].link_num;
		    if ((child_unknown >= 0) && (child_unknown != link))
		    {
			sprintf(errbuf, "%d link %d, and node %d", parent_node,
				link, child_node);
			sprintf(tmp,"Conflict in defined connection between node %s\n",     errbuf);
	    cicsLogMessage(0,tmp);
			return ERROR;
		    }
		    nodes[child_node].links[known].link_num = link;
		}
		/*
		 *	Handle links defined on child side.
		 */
		if ((nodes[child_node].links[link].other_node == parent_node) &&
		    ((known = nodes[child_node].links[link].link_num) >= 0))
		{
		    parent_unknown = nodes[parent_node].links[known].other_node;
		    if ((parent_unknown >= 0) && (parent_unknown != child_node))
		    {
			sprintf(errbuf, "%d link %d, and node %d", child_node,
				link, parent_node);
			sprintf(tmp,"Conflict in defined connection between node %s \n",     errbuf);
	    cicsLogMessage(0,tmp);
			return ERROR;
		    }
		    nodes[parent_node].links[known].other_node = child_node;
		    parent_unknown = nodes[parent_node].links[known].link_num;
		    if ((parent_unknown >= 0) && (parent_unknown != link))
		    {
			sprintf(errbuf, "%d link %d, and node %d", child_node,
				link, parent_node);
			sprintf(tmp,"Conflict in defined connection between node%s \n ",     errbuf);
	    cicsLogMessage(0,tmp);
			return ERROR;
		    }
		    nodes[parent_node].links[known].link_num = link;
		}
	    }
	    /*
	     *	Now go through and figure out how many known and unknown links are
	     *	involved so we can make inferences if required.
	     */
	    known = child_unknown = parent_unknown = 0;
	    for (link = 0; link < MAX_LINKS; link++)
	    {
		if (nodes[parent_node].links[link].other_node == child_node)
		{
		    if (nodes[parent_node].links[link].link_num >= 0)
			known++;
		    else
			parent_unknown++;
		}
		if (nodes[child_node].links[link].other_node == parent_node)
		{
		    if (nodes[child_node].links[link].link_num < 0)
			child_unknown++;
		}
	    }
	    /*
	     *	Now go through and make sure we know enough about the connections
	     *	between the two nodes.
	     */
	    if ((known == 0) && (parent_unknown == 0))
	    {
		sprintf(errbuf, "%d to node %d", parent_node, child_node);
		sprintf(tmp,"Missing connection from node %s \n", errbuf);
	    cicsLogMessage(0,tmp);
		return ERROR;
	    }
	    if ((known == 0) && ((parent_unknown > 1) || (child_unknown > 1)))
	    {
		sprintf(errbuf, "%d to node %d", parent_node, child_node);
		sprintf(tmp,"Unresolved duplicate connections from node %s \n", errbuf);
	    cicsLogMessage(0,tmp);
		return ERROR;
	    }
	    /*
	     *	See if we have no known connections but only one unknown on each end,
	     *	if so assume they are connected together.
	     */
	    if (known == 0)
	    {
		for (link = 0; link < MAX_LINKS; link++)
		{
		    if (nodes[parent_node].links[link].other_node == child_node)
			parent_unknown = link;
		    if (nodes[child_node].links[link].other_node == parent_node)
			child_unknown = link;
		}
		nodes[parent_node].links[parent_unknown].link_num =
		    						child_unknown;
		nodes[child_node].links[child_unknown].link_num =
		    						parent_unknown;
	    }
	}
    }
    return OK;
}
/************************************************************************/
/*									*/
/*	Routine:	Eval						*/
/*									*/
/*	Evaluate "C" style decimal, hexadecimal and octal numbers.	*/
/*									*/
/*	Parameters:	optional -	TRUE if # is optional.		*/
/*									*/
/*	Returns:	The numeric value (-1 if non present and	*/
/*			optional.					*/
/*									*/
/************************************************************************/

long
Eval(optional,val)
BOOL    optional;
SLONG *val;
{
    register int c;
   
    
    skip_space();			/* Skip whitespace */
    if (isdigit(c = *parse_ptr++)) 	/* Is it a number? */
    {
	*val = 0L;
	/*
	 *	See if it is a octal or hex # or if it is a decimal #.
	 */
	if (c == '0')
	{
	    c = *parse_ptr++;
	    /* Its either octal or hex, figure out which. */
	    if ((c == 'x') || (c == 'X'))
	    {
		/* Its a hexadecimal #. */
		while (ishex(c = *parse_ptr++))
		{
		    if (*val & 0xF0000000) /* Detect overflow */
		    {
			parseFatal("Hex constant too large", "");
			return ERROR;
		    }
		    *val = (*val << 4) + hexbin(c);
		}
		parse_ptr--;
	    }
	    else		/* Its an octal #. */
	    {
		while (isoct(c))
		{
		    if (*val & 0xE0000000) /* Detect overflow */
		    {
			parseFatal("Octal constant too large", "");
			return ERROR;
		    }
		    *val = (*val << 3) + c - '0';
		    c = *parse_ptr++;
		}
		parse_ptr--;
	    }
	}
	else			/* Its a decimal #. */
	{
	    *val = (*val * 10) + c - '0';
	    while (isdigit(c = *parse_ptr++))
	    {
		*val = (*val * 10) + c - '0';
	    }
	    parse_ptr--;
	}
    }
    else			/* Wasn't any # we could recognize! */
    {
	parse_ptr--;
	*val = -1L;
	if (!optional)
	{
	    parseFatal("Illegal numeric constant", "");
	    return ERROR;
	}
    }
    /* Return the result. */
    return (OK);
}
#if 0
/************************************************************************/
/*									*/
/*	Routine:	exit_close					*/
/*									*/
/*	Calls "exit" and closes the link I/O driver interface if	*/
/*	needed.								*/
/*									*/
/*	Parameters:	status	-	Desired "exit" return value.	*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
exit_close(status)
int    status;
{
    if (LinkId > 0)
      {
   

	(VOID) CloseLink(LinkId);
      }
    exit(status);
}
#endif
/************************************************************************/
/*									*/
/*	Routine:	exitClose					*/
/*									*/
/*	Calls "exit" and closes the link I/O driver interface if	*/
/*	needed.								*/
/*									*/
/*	Parameters:	status	-	Desired "exit" return value.	*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
exitClose(status)
int    status;
{
    if (LinkId > 0)
      {
   
	(VOID) CloseLink(LinkId);
      }
   
}
#if 0
/************************************************************************/
/*									*/
/*	Routine:	fatal						*/
/*									*/
/*	Print out a "FATAL" error message and quit.			*/
/*									*/
/*	Parameters:							*/
/*		"sptr1"	A pointer to the first of the error message.	*/
/*		"sptr2" A pointer to the rest of the error message.	*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
fatal(sptr1, sptr2)
STRING    sptr1;
STRING    sptr2;
{
    fprintf(stderr, "FATAL: %s%s\n", sptr1, sptr2);	/* Print message */
    exit_close(ERRORS);		/* Bye... */
}
#endif
/************************************************************************/
/*									*/
/*	Routine:	get_fname					*/
/*									*/
/*	Get a file or pathname from the parse buffer.			*/
/*									*/
/*	Parameters:							*/
/*		"buf"	A pointer to where to put the result.		*/
/*									*/
/*	Returns:	TRUE if an error was detected.			*/
/*									*/
/************************************************************************/

BOOL
get_fname(buf)
char    buf[];
{
    register UINT i;
    char    tbuf[FNSIZE + 1];
    
    skip_space();		/* Skip whitespace */
    for (i = 0; i < FNSIZE; i++)
    {
	if (not_fname(*parse_ptr))
	    break;		/* End of name */
	tbuf[i] = *parse_ptr++;
    }
    if (i == FNSIZE)
	return (TRUE);
    tbuf[i] = '\0';
    strcpy(buf, tbuf);		/* Update with new value */
    return (FALSE);
}

/************************************************************************/
/*									*/
/*	Routine:	hexbin						*/
/*									*/
/*	Convert a hexadecimal digit to binary.				*/
/*									*/
/*	Parameters:							*/
/*		"c"	The digit to convert.				*/
/*									*/
/*	Returns:	The resulting binary value.			*/
/*									*/
/************************************************************************/

SLONG
hexbin(c)
int    c;
{
    if (isdigit(c))
	return ((SLONG) (c - '0'));
    else
	return ((SLONG) ((isupper(c) ? tolower(c) : c) - 'a' + 10));
}
#if 0
/************************************************************************/
/*									*/
/*	Routine:	in_1b,in_2b,in_4b				*/
/*									*/
/*	Read 1, 2 or 4 bytes from the current download input file.	*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	The 1, 2 or 4 byte value read.			*/
/*									*/
/************************************************************************/

int
in_1b()
{
    int    c;
    
    if ((c = fgetc(in_fp)) == EOF)
    {
	if (ferror(in_fp) || feof(in_fp))
	{
	    fatal("Error reading download input file: ", inbuf);
	    
	}
    }
    return (c & 0xFF);
}

int
in_2b()
{
    int    rval;
    
    rval = in_1b();
    rval |= (in_1b() << 8);
    return (rval & 0xFFFF);
}

SLONG
in_4b()
{
    SLONG    rval;
    
    rval = (SLONG) in_1b();
    rval |= (((SLONG) in_1b()) << 8);
    rval |= (((SLONG) in_1b()) << 16);
    rval |= (((SLONG) in_1b()) << 24);
    return (rval);
}
#endif
/************************************************************************/
/*									*/
/*	Routine:	in1b,in2b,in4b				        */
/*									*/
/*	Read 1, 2 or 4 bytes from the current download input file.	*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	The 1, 2 or 4 byte value read.			*/
/*									*/
/************************************************************************/

long
in1b(c)
int *c;
{
  
    
    if ((*c = fgetc(in_fp)) == EOF)
    {
	if (ferror(in_fp) || feof(in_fp))
	{
	    sprintf(tmp,"Error reading download input file: %s \n", inbuf);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	    
	}
    }
    return (*c & 0xFF);
}

long
in2b(val)
int *val;
{
    int    rval;
    if (in1b(val) == ERROR)
        return ERROR;
    if (in1b(&rval) == ERROR)
        return ERROR;
    *val |= (rval<<8);
    *val = *val & 0xffff;
    return OK;

 /*    rval = in_1b(); */
/*     rval |= (in_1b() << 8); */
/*     return (rval & 0xFFFF); */
}

long
in4b(val)
SLONG *val;
{
    int    rval;
    
    if (in1b(&rval) == ERROR)
        return ERROR;
    *val = (SLONG) rval;
      
    if (in1b(&rval) == ERROR)
        return ERROR;
    *val |= ((SLONG)rval <<8);

    if (in1b(&rval) == ERROR)
        return ERROR;
    *val |= ((SLONG)rval <<16);

    if (in1b(&rval) == ERROR)
        return ERROR;
    *val |= ((SLONG)rval <<24);

  /*   rval = (SLONG) in_1b(); */
/*     rval |= (((SLONG) in_1b()) << 8); */
/*     rval |= (((SLONG) in_1b()) << 16); */
/*     rval |= (((SLONG) in_1b()) << 24); */
/*     return (rval); */
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	ishex						*/
/*									*/
/*	Test a digit to see if it could be part of a hexadecimal #.	*/
/*									*/
/*	Parameters:							*/
/*		"c"	The digit to test.				*/
/*									*/
/*	Returns:	TRUE if so, FALSE if not.			*/
/*									*/
/************************************************************************/

BOOL
ishex(c)
int    c;
{
    return (isdigit(c) ||
	    ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')));
}

/************************************************************************/
/*									*/
/*	Routine:	isoct						*/
/*									*/
/*	Test a digit to see if it could be part of a octal #.		*/
/*									*/
/*	Parameters:							*/
/*		"c"	The digit to test.				*/
/*									*/
/*	Returns:	TRUE if so, FALSE if not.			*/
/*									*/
/************************************************************************/

BOOL
isoct(c)
int    c;
{
    return ((c >= '0') && (c <= '7'));
}

/************************************************************************/
/*									*/
/*	Routine:	localize_boot					*/
/*									*/
/*	Set the node specific features of the bootstrap.		*/
/*									*/
/*	Parameters:	"node"	The node # for which the bootstrap is	*/
/*			intended.					*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/*	WARNING:	ANY CHANGES TO THE LOCAL TEMPLATE IN "tload.h"	*/
/*			OR "netboot.tal" WILL PROBABLY REQUIRE		*/
/*			MATCHING CHANGES to BE MADE HERE!		*/
/*									*/
/************************************************************************/

long
localize_boot(node)
int    node;
{
    register int i;
    register int word_size;
    SLONG val;
    
    if (nodes[node].prog->cpu_type == BID_T2)
	word_size = 2;
    else
	word_size = 4;		/* Assumed to be T4/T8 */
    /*
     *	Set node address in both parts of the bootstrap and in the local data.
     *	Note that the node addresses in both halves of the bootstrap must be
     *	4 bytes long since they are interpreted by both 16 and 32 bit CPU's.
     */
    local_store(4, -LOCALS_STACK, (SLONG) (i = nodes[node].boot_id));
    bootcode[0x103] = i & 0xFF;
    bootcode[0x104] = (i >> 8) & 0xFF;
    bootcode[0x105] = 0;	/* Max. of 32K nodes */
    bootcode[0x106] = 0;
    local_store(word_size, OUR_ADDRESS, (SLONG) i);
    /*
     *	Put channel addresses in bootstrap.  Note that we use the third
     *	parameter to "pick_ichan" to determine which node (source or
     *	destination), to search first when hunting for a link between the
     *	current node and the associated parent or child nodes.  This ensures
     *	that if two or more links connect the same two nodes we will always
     *	pick the same link between the two.  The rule is that we always pick
     *	the first connected link by checking from link zero to the maximum
     *	number of links in the connection list for the child.
     */
    i = nodes[node].parent_num;

    /* XXXXXXXXXXXX */
    if (node == 1)
	local_store(word_size, IUP_IN, (SLONG) 0x80000010);
    else {
	/* we must change the bootcode for all other nodes. */
	/* replace a
		j @past
	   with a mint (the closest I could come to a nop */

	bootcode[142] = 0x24;
	bootcode[143] = 0xf2;
	if(pick_ichan(node, i, FALSE,&val) == ERROR)
	    return ERROR;
	local_store(word_size, IUP_IN, val);
    }

    if ((i = nodes[node].system_output) < 0)
	local_store(word_size, LEFT+C_ICHAN, (SLONG) 0);
    else
    {
        if (pick_ichan(node, i, TRUE,&val) == ERROR)
	    return ERROR;
	local_store(word_size, LEFT+C_ICHAN,val);
    }
    if ((i = nodes[node].subsys_output) < 0)
	local_store(word_size, RIGHT+C_ICHAN, (SLONG) 0);
    else
    {
         if (pick_ichan(node, i, TRUE,&val) == ERROR)
	     return ERROR;
	local_store(word_size, RIGHT+C_ICHAN, val);
    }
    /*
     *	Put remainder of node specific information in bootstrap.
     */
    local_store(word_size, USER_NODE, (SLONG) node);
    local_store(word_size, INIT_TIMEOUT, (SLONG) nodes[node].timeout);
    /*
     *	Find program specific information and put in bootstrap.
     */
    local_store(word_size, PROG_NUM, (SLONG) nodes[node].prog->prog_num);
    local_store(word_size, ICPU_TYPE, (SLONG) nodes[node].prog->cpu_type);
    local_store(word_size, STACK, (SLONG) nodes[node].prog->stack);
    local_store(word_size, ENTRY, (SLONG) nodes[node].prog->entry);
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	local_store					*/
/*									*/
/*	Set a specific word in the bootstrap to a specified value.	*/
/*									*/
/*	Parameters:							*/
/*		size -	The size of the word "offset" is measured in.	*/
/*		offset-	The relative word offset within bootstrap local	*/
/*			storage.					*/
/*		value -	The value to store.				*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/*	WARNING:	ANY CHANGES TO THE LOCAL TEMPLATE IN "tload.h"	*/
/*			OR "netboot.tal" WILL PROBABLY REQUIRE		*/
/*			MATCHING CHANGES BE MADE HERE!			*/
/*									*/
/************************************************************************/

VOID
local_store(size, offset, value)
int    size;
int    offset;
SLONG  value;
{
    register UCHAR *cptr;
    
    cptr = bootcode + (LOCAL_OFFSET * 4) + ((LOCALS_STACK + offset) * size);
    *cptr++ = value;
    *cptr++ = (value >>= 8);
    if (size == 4)
    {
	*cptr++ = (value >>= 8);
	*cptr = (value >> 8);
    }
}

/************************************************************************/
/*									*/
/*	Routine:	makefn						*/
/*									*/
/*	Expand a skeleton filename by adding a default extension if	*/
/*	none is supplied.						*/
/*									*/
/*	Parameters:							*/
/*		"sptr"	A pointer to the skeleton filename.		*/
/*		"bptr"	A pointer to the buffer the result goes in.	*/
/*		"ext"	A pointer to the default extension to use.	*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
makefn(sptr, bptr, ext)
STRING    sptr;
STRING    bptr;
STRING    ext;
{
    extern STRING strchr();
    extern STRING strrchr();
    
    STRING  s;
    
    if (strlen(sptr) > FNSIZE)
    {
	usagerr();
	return ERROR;
    }
    strcpy(bptr, sptr);
    if ((s = strrchr(bptr, '.')) != NULL)
    {
	if ((strchr(s, '/') == NULL) && (strchr(s, '\\') == NULL))
	    return OK;
    }
    if (strlen(bptr) > (FNSIZE - 4))
    {
	usagerr();
	return ERROR;
    }
    strcat(bptr, ext);
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	match						*/
/*									*/
/*	Used by the input parsing routines to check and see if the next	*/
/*	characters pointed at by "parse_ptr" match the string passed to	*/
/*	"match".  If so "parse_ptr" is updated to point beyond the	*/
/*	matched item.  Preceeding whitespace is skipped	("parse_ptr"),	*/
/*	prior to performing the match.  Note that the match is case	*/
/*	independent (the parameter to "match" should be upper case).	*/
/*									*/
/*	Parameters:							*/
/*		"str"	A pointer to the string being compared.		*/
/*									*/
/*	Returns:	TRUE if successfully matched.			*/
/*									*/
/************************************************************************/

BOOL
match(str)
STRING    str;
{
    register STRING temp;
    
    skip_space();
    temp = parse_ptr;
    while (*str)
    {
	if (*str++ != (islower(*temp) ? toupper(*temp) : *temp))
	    return (FALSE);
	else
	    temp++;
    }
    parse_ptr = temp;		/* Point past matched string */
    return (TRUE);
}

/************************************************************************/
/*									*/
/*	Routine:	match_first					*/
/*									*/
/*	Like "match", but also slurps any alphanumerics which		*/
/*	immediately follow the matched string.				*/
/*									*/
/*	Parameters:							*/
/*		"str"	A pointer to the string being compared.		*/
/*									*/
/*	Returns:	TRUE if successfully matched.			*/
/*									*/
/************************************************************************/

BOOL
match_first(str)
STRING    str;
{
    if (!match(str))
	return (FALSE);
    while (isalnum(*parse_ptr) || (*parse_ptr == '_'))
	parse_ptr++;
    return (TRUE);
}

/************************************************************************/
/*									*/
/*	Routine:	not_fname					*/
/*									*/
/*	Return TRUE if the character parameter could not be part of a	*/
/*	filename.							*/
/*									*/
/*	Parameters:							*/
/*		"c" -	The character being tested.			*/
/*									*/
/*	Returns:	TRUE if not part of a legal filename.		*/
/*									*/
/************************************************************************/

BOOL
not_fname(c)
int    c;
{
    if (isspace(c) || (c == ',') || (c == ';') || (c == '\n') || (c == '\0'))
	return (TRUE);
    return (FALSE);
}

/************************************************************************/
/*									*/
/*	Routine:	open_input					*/
/*									*/
/*	Open the specified input file, verify that its in load format	*/
/*	and return the intended CPU type.				*/
/*	the network information file.					*/
/*									*/
/*	Parameters:	"name" -	The filename to open.		*/
/*									*/
/*	Returns:	The intended processor type.			*/
/*									*/
/************************************************************************/

long
open_input(name,cpu)
STRING    name;
int *cpu;
{
   
   int val;

    strcpy(inbuf, name);	/* Copy of the name */
    /*
     *	Open file to be downloaded.
     */
#if	CRLF
    if ((in_fp = (STREAM) fopen(inbuf, "rb")) == NULL)
#else
    if ((in_fp = (STREAM) fopen(inbuf, "r")) == NULL)
#endif
    {
	sprintf(tmp,"Unable to open download input file: %s \n", inbuf);
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    /*
     *	Get file type and CPU type.
     */
    if (in1b(&val) == ERROR)
        return ERROR;
    if (val != T_LD_FILE)
    {
	sprintf(tmp,"Download input file is not in load format: %s \n", inbuf);
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    if (in1b(cpu) == ERROR)
        return ERROR;
   /*  *cpu = in_1b(); */		/* Get load CPU type */
    if ((*cpu != TYPE_UNKNOWN) && (*cpu != TYPE_212) && (*cpu != TYPE_414) &&
	(*cpu != TYPE_800))
    {
       sprintf(tmp,"Transputer CPU type in download file is not recognizable \n");
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    return (OK);
}

/************************************************************************/
/*									*/
/*	Routine:	parse						*/
/*									*/
/*	Figure out what the user wants done by reading commands from	*/
/*	the network information file.					*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
parse()
{
    static STRING err = "Expected ';'";
    BOOL    had_buf;
    BOOL    had_dtime;
    BOOL    had_ltime;
    BOOL    had_serv;
    
    had_buf = had_serv = had_dtime = had_ltime = FALSE;
    
    while (fgets(parse_buf, (MAXLNSIZE + 1), info_fp) != NULL)
    {
	++line;
	parse_ptr = parse_buf;
	if (match_first("BUFFER"))
	{
	    if (had_buf)
	    {
		parse_warning("Already had a BUFFER_SIZE command", "");
	    }
	    if(Eval(FALSE,&buf_size) == ERROR)
	        return ERROR;
	    if ((buf_size < 16) || (buf_size > MAX_PACKET))
	    {
		parseFatal("BUFFER_SIZE must be between 16 and 255 bytes", "");
		return ERROR;
	    }
	    if (!match(";"))
	    {
		parseFatal(err, "");
		return ERROR;
	    }
	    had_buf = TRUE;
	}
	else if (match_first("DECODE"))
	{
	    if (had_dtime)
	    {
		parse_warning("Already had a DECODE_TIMEOUT command", "");
	    }
	    if(Eval(FALSE,&decode_timeout) == ERROR)
	        return ERROR;
	  
	    if ((decode_timeout < MIN_DEC_TOUT) ||
					(decode_timeout > MAX_DEC_TOUT))
	    {
		parseFatal("DECODE_TIMEOUT too large or too small", "");
		return ERROR;
	    }
	    decode_timeout = (decode_timeout * 1000L) /
		MICRO_LOW_TICK;
	    if (!match(";"))
	    {
		parseFatal(err, "");
		return ERROR;		
	    }
	    
	    had_dtime = TRUE;
	}
	else if (match_first("HOST"))
	{
	    if (had_serv)
	    {
		parse_warning("Already had a HOST_SERVER command", "");
	    }
	    if (get_fname(host_server))
	    {
		parseFatal("Host server name too long", "");
		return ERROR;
	    }
	    if (!match(";"))
	    {
		parseFatal(err, "");
		return ERROR;
	    }
	    had_serv = TRUE;
	}
	else if (match_first("LEVEL"))
	{
	    if (had_ltime)
	    {
		parse_warning("Already had a LEVEL_TIMEOUT command", "");
	    }
	    if (Eval(FALSE,&level_timeout) == ERROR)
	        return ERROR;
	   
	    if ((level_timeout < MIN_LVL_TOUT) ||
					(level_timeout > MAX_LVL_TOUT))
	    {
		parseFatal("LEVEL_TIMEOUT too large or too small", "");
		return ERROR;
	    }
	    level_timeout = (level_timeout * 1000L) / MICRO_LOW_TICK;
	    if (!match(";"))
	    {
		parseFatal(err, "");
		return ERROR;
	    }
	    had_ltime = TRUE;
	}
	else if (isdigit(*parse_ptr))
	{
	    if (parseNode() == ERROR)
	        return ERROR;
	    if (!match(";"))
	    {
		parseFatal(err, "");
		return ERROR;
	    }
	}
	else if (isalpha(*parse_ptr))
	{
	    parseFatal("Unrecognized command", "");
	    return ERROR;
	}
    }
    if (ferror(info_fp))	/* If error */
    {
	sprintf(tmp,"Error reading network information file: %s \n", infobuf);
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    
    if (root_node == -1)
    {
	sprintf(tmp,"No root node specified: %s \n", infobuf);
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    return OK;
}
/************************************************************************/
/*									*/
/*	Routine:	parseFatal					*/
/*									*/
/*	Display a message to the user in response to a fatal error	*/
/*	detected while the parsing the network information file.	*/
/*									*/
/*	Parameters:							*/
/*		"s1" -	A pointer to the first string to display.	*/
/*		"s2" -	A pointer to the second string to display.	*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
parseFatal(s1, s2)
STRING  s1;
STRING  s2;
{
    STRING  tptr;
    
    fprintf(stderr, "<%s> @ %u: FATAL: %s%s\n", infobuf, line, s1, s2);
    fprintf(stderr, "%s", parse_buf);
    for (tptr = parse_buf; tptr != parse_ptr; tptr++)
    {
	if (*tptr == '\t')
	    putc('\t', stderr);
	else
	    putc(' ', stderr);
    }
    putc('^', stderr);		/* Show where error occurred */
  
}
/************************************************************************/
/*									*/
/*	Routine:	parseNode					*/
/*									*/
/*	Parse the information pertaining to a single node.		*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
parseNode()
{
    extern STRING strchr();
    extern STRING strrchr();
    
    static STRING err1 = "Expected ','";
    static STRING err2 = "Program name too long";
    register int link;
    char    name[FNSIZE + 1];
    int     node;
    int     parent_node;
    BOOL    root_link;
    register STRING s;
    int temp1;
    int temp2;
    /*
     *	Get node #.
     */
    if(Eval(FALSE,&node) == ERROR)
        return ERROR;
    if (node > MAX_NODES)
    {
	parseFatal("Node # too large\n");
	return ERROR;
    }
    if (node == 0)
    {
	parseFatal("Node 0 is reserved for the host interface\n");
	return ERROR;
    }
    if (nodes[node].prog != NULL)
    {
	parseFatal("Node # already used\n");
	return ERROR;
    }
    
    if (!match(","))
    {
	parseFatal("%s \n",err1);
	return ERROR;
    }
    /*
     *	Get name of program to load onto that node.
     */
    if (get_fname(name))	/* Get desired program */
    {
	parseFatal("%s \n",err2);
	return ERROR;
    }
    if (name[0] == '\0')
    {
	parseFatal("Program name required\n");
	return ERROR;
    }
    /*
     *	Add appropriate extension if none supplied.
     */
    if (!(((s = strrchr(name, '.')) != NULL) && (strchr(s, '/') == NULL) &&
	  (strchr(s, '\\') == NULL)))
    {
	if (strlen(name) > (FNSIZE - 4))
	{
	    parseFatal("%s\n",err2);
	    return ERROR;
	}
	else
	    strcat(name, TLNK_OUTPUT_EXT);
    }
     if ( addProgram(name,&nodes[node].prog) == ERROR) 
         return ERROR; 
  /*   nodes[node].prog =  add_program(name); */
    if (!match(","))
    {
	parseFatal("%s\n",err1);
	return ERROR;
    }
    /*
     *	Parse the reset information.
     */
    if (match("R"))
    {
	nodes[node].boot_status = FROM_SYSTEM_RESET;
	if (Eval(FALSE,&parent_node) == ERROR)
	    return ERROR;
    }
    else if (match("S"))
    {
	nodes[node].boot_status = FROM_SUBSYS_RESET;	
	if (Eval(FALSE,&parent_node) == ERROR)
	    return ERROR;
	if (parent_node == 0)
	{
	    parseFatal("Host interface doesn't have sub-system connection", "");
	    return ERROR;
	}
    }
    else
    {
	parseFatal("Expected 'R' or 'S'\n");
	return ERROR;
    }
    if (parent_node > MAX_NODES)
    {
	parseFatal("Node # too large\n");
	return ERROR;
    }
    nodes[node].parent_num = parent_node;
    if (parent_node == 0)
    {
	if (root_node != -1)
	{
	    parseFatal("Duplicate root node\n");
	    return ERROR;
	}
	root_node = node;
	root_link = FALSE;	/* No root<->host link found yet */
    }
    /*
     *	Parse the link information.
     */
    for (link = 0; link < MAX_LINKS; link++)
    {
	if (!match(","))
	{
	    parseFatal("%s\n",err1);
	    return ERROR;
	}
	if (Eval(TRUE,&temp1) == ERROR)
	    return ERROR;
   
	if (temp1 < 0)
	{
	    nodes[node].links[link].other_node = -1;
	    nodes[node].links[link].link_num = -1;
	}
	else if (temp1 > MAX_NODES)
	{
	    parseFatal("Node # too large\n");
	    return ERROR;
	}
	else
	{
	    nodes[node].links[link].other_node = temp1;
	    if (temp1 == 0)
	    {
		if (node != root_node)
		{
		    parseFatal("Host interface is only connected to root node\n");
		    return ERROR;
		}
		else
		{
		    if (root_link)
		    {
			parseFatal("Only one link allowed between host interface and root node\n");
			return ERROR;
		    }
		    root_link = TRUE;
		}
	    }
	}
	if (match("["))		/* Link # for other end of link? */
	{
	    if (Eval(TRUE,&temp2) == ERROR)
	        return ERROR;
	  
	    if (temp2 >= 0)
	    {
		if (temp2 >= MAX_LINKS)
		{
		    parseFatal("Link # too large\n");
		    return ERROR;
		}
		nodes[node].links[link].link_num = temp2;
	    }
	    else
		nodes[node].links[link].link_num = -1;
	    if (!match("]"))
	    {
		parseFatal("Expected ']'\n");
		return ERROR;
	    }
	}
	else
	    nodes[node].links[link].link_num = -1;
	/*
	 *	If the link is to the host we arbitrarily decree it to connect to link
	 *	zero on the host to facilitate later error checking.
	 */
	if (temp1 == 0)
	    nodes[node].links[link].link_num = 0;
    }
    /*
     *	Verify that one of the links is connected to the node which boots
     *	this node.
     */
    for (link = 0; link < MAX_LINKS; link++)
    {
	if (nodes[node].links[link].other_node == nodes[node].parent_num)
	    break;
    }
    if (link == MAX_LINKS)
    {
	parseFatal("A link MUST exist to the node which resets this node\n");
	return ERROR;
    }
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	parse_warning					*/
/*									*/
/*	Display a message to the user in response to a non-fatal error	*/
/*	detected while the parsing the network information file.	*/
/*									*/
/*	Parameters:							*/
/*		"s1" -	A pointer to the first string to display.	*/
/*		"s2" -	A pointer to the second string to display.	*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
parse_warning(s1, s2)
STRING  s1;
STRING  s2;
{
    fprintf(stderr, "<%s> @ %u: WARNING: %s%s\n", infobuf, line, s1, s2);
}

/************************************************************************/
/*									*/
/*	Routine:	pick_ichan					*/
/*									*/
/*	Given a source and destination node pick a fully defined input	*/
/*	channel to use for communication (input with respect to the	*/
/*	source).							*/
/*									*/
/*	Parameters:							*/
/*		src -	The source node #.				*/
/*		dst -	The destination node #.				*/
/*		order -	Allows for resolving which link to use when	*/
/*			more than one link connects two nodes.  Set to	*/
/*			TRUE to search "dst" links left to right, FALSE	*/
/*			to search "src" links left to right to choose	*/
/*			the link to use.				*/
/*									*/
/*	Returns:	An appropriate input channel.			*/
/*									*/
/************************************************************************/

long
pick_ichan(src, dst, order, val)
register int  src;
register int  dst;
int           order;
SLONG *val;
{
    register int link;
   

    for (link = 0; link < MAX_LINKS; link++)
    {
	if (order)
	{
	    if ((nodes[dst].links[link].other_node == src) &&
					(nodes[dst].links[link].link_num >= 0))
	    {
		link = nodes[dst].links[link].link_num;
		break;
	    }
	}
	else
	{
	    if ((nodes[src].links[link].other_node == dst) &&
					(nodes[src].links[link].link_num >= 0))
	    {
		break;
	    }
	}
    }
    if (link == MAX_LINKS)
    {
	sprintf(tmp,"Internal error #3 \n");
	    cicsLogMessage(0,tmp);
	return ERROR;
    }
    
    if (nodes[src].prog->cpu_type == BID_T2)
	*val = (LINKIN16_ADDR + (link * 2));
    else
	*val = (LINKIN32_ADDR + (link * 4));
    return OK;
}

/************************************************************************/
/*									*/
/*	Routine:	skip_space					*/
/*									*/
/*	Skip whitespace in the parse buffer.				*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
    skip_space()
{
    while (isspace(*parse_ptr))
	parse_ptr++;
}

#ifdef notdef
/************************************************************************/
/*									*/
/*	Routine:	tin_4b						*/
/*									*/
/*	Read 4 bytes from the root Transputer link.			*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	The value read.					*/
/*									*/
/************************************************************************/

SLONG
tin_4b()
{
    UCHAR   buf[4];
    SLONG   rval;
    int     temp;
    
    if ((temp = ReadLink(LinkId, buf, 4, (int) host_timeout)) <= 0)
    {
	if (temp < 0)
	    fatal("Error reading from link: ", open_str);
	else
	{
	    fatal("Timed out waiting to read from link: ", open_str);
	}
    }
    rval = (SLONG) buf[0];
    rval |= (((SLONG) buf[1]) << 8);
    rval |= (((SLONG) buf[2]) << 16);
    rval |= (((SLONG) buf[3]) << 24);
    return (rval);
}
#endif
#if 0
/************************************************************************/
/*									*/
/*	Routine:	tout_str					*/
/*									*/
/*	Copy a specified # of bytes from a buffer to the Transputer.	*/
/*									*/
/*	Parameters:							*/
/*		buf -	The buffer to copy from.			*/
/*		count -	The number of bytes to copy.			*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
tout_str(buf, count)
STRING  buf;
UINT    count;
{
    int    rval;
    int i;

    if (debug==4)
    {
	for(rval=0; rval<count;rval++)
	{
	    i = buf[rval];
	    i = i & 0x00FF;
	    fprintf(stdout,"0x%2.2x,",i );
	}
	fprintf(stdout,"\n");
    }

    if ((rval = WriteLink(LinkId, buf, count, (int) host_timeout)) != count)
    {
	if (rval < 0)
	    fatal("Error writing to link: ", open_str);
	else
	    fatal("Timed out waiting to write to link: ", open_str);
    }
}
#endif
/************************************************************************/
/*									*/
/*	Routine:	toutStr					*/
/*									*/
/*	Copy a specified # of bytes from a buffer to the Transputer.	*/
/*									*/
/*	Parameters:							*/
/*		buf -	The buffer to copy from.			*/
/*		count -	The number of bytes to copy.			*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
toutStr(buf, count)
STRING  buf;
UINT    count;
{
    int    rval;
    int i;

    if (debug==4)
    {
	for(rval=0; rval<count;rval++)
	{
	    i = buf[rval];
	    i = i & 0x00FF;
	    fprintf(stdout,"0x%2.2x,",i );
	}
	fprintf(stdout,"\n");
    }

    if ((rval = WriteLink(LinkId, buf, count, (int) host_timeout)) != count)
    {
	if (rval < 0)
	{
	    sprintf(tmp,"Error writing to link: %s ,\n", open_str);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
	else
	{
	    sprintf(tmp,"Timed out waiting to write to link: %s \n", open_str);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
    }
    return OK;
}
#if 0
/************************************************************************/
/*									*/
/*	Routine:	tout_1b,tout_4b					*/
/*									*/
/*	Send 1 or 4 bytes out the root Transputer link.			*/
/*									*/
/*	Parameters:							*/
/*		value -	The 1 or 4 byte value to write.			*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
tout_1b(value)
int    value;
{
    char    c;
    int     rval;
    static count=0;
    
    
    c = rval = value;
    rval = rval & 0x00FF;
    if (debug==4)
    {
	if ((count+=5) > 79)
        {       
	    fprintf(stdout,"0x%2.2x\n",rval);
	    count = 0;
	}
	else
	    fprintf(stdout,"0x%2.2x,",rval);
    }

    if ((rval = WriteLink(LinkId, &c, 1, (int) host_timeout)) <= 0)
    {
	if (rval < 0)
	    fatal("Error writing to link: ", open_str);
	else
	{
	    fatal("Timed out waiting to write to link: ", open_str);
	}
    }

}

VOID
tout_4b(value)
SLONG    value;
{
    tout_1b((int) value);
    tout_1b((int) (value >>= 8));
    tout_1b((int) (value >>= 8));
    tout_1b((int) (value >> 8));
}
#endif
/************************************************************************/
/*									*/
/*	Routine:	tout1b,tout4b					*/
/*									*/
/*	Send 1 or 4 bytes out the root Transputer link.			*/
/*									*/
/*	Parameters:							*/
/*		value -	The 1 or 4 byte value to write.			*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

long
tout1b(value)
int    value;
{
    char    c;
    int     rval;
    static count=0;
    
    
    c = rval = value;
    rval = rval & 0x00FF;
    if (debug==4)
    {
	if ((count+=5) > 79)
        {       
	    fprintf(stdout,"0x%2.2x\n",rval);
	    count = 0;
	}
	else
	    fprintf(stdout,"0x%2.2x,",rval);
    }

    if ((rval = WriteLink(LinkId, &c, 1, (int) host_timeout)) <= 0)
    {
	if (rval < 0)
	{
	    sprintf(tmp,"Error writing to link: %s \n", open_str);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
	else
	{
	    sprintf(tmp,"Timed out waiting to write to link:%s \n ", open_str);
	    cicsLogMessage(0,tmp);
	    return ERROR;
	}
    }

    return OK;
}

long
tout4b(value)
SLONG    value;
{
    if(tout1b((int) value) == ERROR)
        return ERROR;
    if(tout1b ((int) (value >>= 8)) == ERROR)
       return ERROR;
    if(tout1b((int) (value >>= 8)) == ERROR)
        return ERROR;
    if(tout1b((int) (value >> 8)) == ERROR)
        return ERROR;
    return OK;
	
}

/************************************************************************/
/*									*/
/*	Routine:	usagerr						*/
/*									*/
/*	Report incorrect loader invocation to user with a suggestion	*/
/*	of how it should be done.					*/
/*									*/
/*	Parameters:	None.						*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
usagerr()
{
   /*  fatal("Usage is 'ld-net \"network_info_file\" [-d] [-driver_parameters]", ""); */
   cicsLogMessage(0,"Usage is 'ld-net \"network_info_file\" [-d] [-driver_parameters]\n");
}

/************************************************************************/
/*									*/
/*	Routine:	warning						*/
/*									*/
/*	Print out a "WARNING" error message.				*/
/*									*/
/*	Parameters:							*/
/*		sptr1 -	A pointer to the first of the error message.	*/
/*		sptr2 -	A pointer to the rest of the error message.	*/
/*									*/
/*	Returns:	None.						*/
/*									*/
/************************************************************************/

VOID
warning(sptr1, sptr2)
STRING    sptr1;
STRING    sptr2;
{
    sprintf(tmp, "WARNING: %s%s\n", sptr1, sptr2); /* Print message */
    cicsLogMessage(0,tmp);
}

int
tin_1b()
{
#if defined(vxWorks)
    char ch;
    /*
     * code seems to be waiting for input  ready
     */
    if((chan_read(&ch,1)) == 1)
    {
	return(ch);
    }
    return(-1);
#else
    register int delay;
    
    delay = 50;
    while (!(inp(HOST_IN_RBF) & HOST_RBF_MASK))
    {
	if (--delay == 0)
	{
	    delay = 50;
	    (VOID) kbhit();	/* Respond to control-C */
	}
    }
    return (inp(HOST_IN) & 0xFF);
#endif
}

#if defined(vxWorks)
#define ULONG unsigned long
SLONG
tin_4b()
{
    ULONG   rval;

#if 0
    rval = tin_1b() & 0xff;
    rval |= ((0xff & tin_1b()) << 8);
    rval |= ((0xff & tin_1b()) << 16);
    rval |= ((0xff & tin_1b()) << 24);
#endif

    unsigned int i;

    chan_read(&i, 4);

    rval = ((i >> 24) & 0xff) | ((i >> 8) & 0xff00) | 
	((i << 8) & 0xff0000) | ((i << 24) & 0xff000000);

    return rval;
}
#else

SLONG
tin_4b()
{
    SLONG   rval;
    
    rval = (SLONG) tin_1b();
    rval |= (((SLONG) tin_1b()) << 8);
    rval |= (((SLONG) tin_1b()) << 16);
    rval |= (((SLONG) tin_1b()) << 24);
    return (rval);
}
#endif

void init_ldnet()
{
    int siz;
    in_fp = NULL;		  
    Error = 0;  
    buf_size = MAX_PACKET;    
    decode_timeout = (DEF_DEC_TOUT * 1000L) / MICRO_LOW_TICK;  

    host_timeout = 0;		  
    level_timeout = (DEF_LVL_TOUT * 1000L) / MICRO_LOW_TICK;  
    line = 0;			  
   /*  LinkIdLN = 0;  */
    LinkId = 0;
    in_fp = info_fp = NULL;
    siz = sizeof(bootcode_back);
    bootcode = malloc (siz);
    memcpy ((char *)bootcode,(char *)bootcode_back,siz);
}

