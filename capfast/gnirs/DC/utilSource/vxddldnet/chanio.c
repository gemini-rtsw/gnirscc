static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: chanio.c,v 1.2 2009/05/27 19:33:37 fkraemer Exp $"
};
/*
 * chanio.c
 *
 * perform simulated channel i/o
 * via stream sockets to reader/writer
 * processes.
 *
 */

#include <stdio.h>
#if defined(vxWorks)
#include <time.h>
#include <selectLib.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/mman.h>
#endif
#include <signal.h>

#include "tplink.h"  /* Was link.h changed name dut to conflect in EPICS */


int ReadLink (LINK LinkId, char *Buffer, unsigned int Count,int Timeout);
int WriteLink (LINK LinkId, char *Buffer, unsigned int Count,int Timeout);
    
    
   

extern LINK LinkId;


extern int	fd;		/* lock file descriptor */
extern int	debug;
typedef unsigned char uchar;


int	chan_out;
int	chan_in;
int	readpid;
int	writepid;
FILE	*infp;
FILE	*outfp;
FILE    *dof;


int
chan_end()
{
    return OK;
}


int
chan_read(ptr, nbytes)
char	*ptr;
int	nbytes;
{
	return ReadLink(LinkId, ptr, nbytes, 10);
}


int
chan_write(ptr, nbytes)
char	*ptr;
int	nbytes;
{
	return WriteLink(LinkId, ptr, nbytes, 10);
}	

int
chan_flush()
{
    return OK;
}


int
selsleep(time)
int	time;
{
    struct timeval tv;
    
    tv.tv_sec = 0;
    tv.tv_usec = time;
    if (select(0, 0, 0, 0, &tv) < 0)
	perror("select");
    return OK;

}


