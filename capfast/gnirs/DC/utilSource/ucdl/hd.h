
#include <string.h>
#include <ioLib.h>

#ifdef SUN
#include <fcntl.h>  
#endif

#include <stdio.h> 

#ifdef SUN
#include "ansi.h" 
#endif

#include "b016.h" 

#ifdef SUN
#include "tcl.h" 
#endif

#include "util.h" 

#define	ONLY_REL_RECS
#include "taldef.h" 
#include "protocol.h" 
#define LINK int
#define BUF_SIZE	128

extern void init_vxw_link ();
void sleep (int a, int b);
long ldmc (char *mcpath,char *mcname, char *controlpath );
long ldseq(char *path, char *name, char * cpath);

static void byte(unsigned char c);
int CloseLink (LINK LinkId);
int sendTP(char *arg1, char *arg2, char *arg3);
int receive (char *arg0,char *arg1,char *arg2);
static void send_block();

extern LINK link_id;                       /* fd for the link */
static int ct = 0;
static int w = 0;
static unsigned char *cp = (unsigned char *) &w;
static int buf[BUF_SIZE];
static int bInd = 0;
static int pInd = 0;
static char *dest;
