static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: control.c,v 1.2 2009/05/27 19:33:36 fkraemer Exp $"
};
/*
 * control process for b014 driver
 *
 * read from socket perform b014 control functions.
 *  send OK or ERROR over socket as acknowledge
 * (output socket).
 *
 */
#include <vxWorks.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>
#include <signal.h>
#include <logLib.h>
#include	<unistd.h>
#include	"irstd.h"
extern int errno;

#include	"drvr_defs.h"
#include	"drvr_b014.h"
#define		NAACCIOV
#include	"drvr_vars.h"
#define         SERVER	1
#include	"drvrlink.h"
#include 	"fchanio.h"

extern B014_MAP b014;
extern int closeFLAG;
extern int dofr;
extern void b014_save();
extern void b014_restore();
int AnalyzeLink();
int ResetLink();
UINT8 TestError();
UINT8 WriteStat();
UINT8 ReadStat();

char	buf[CMNDLEN];

int
control(socket)
int	socket;
{

	int	n = CMNDLEN;
        char   writebuf[CMNDLEN];
	int     i=1;

	    errno = 0;
	    if ((n = read(socket, buf, CMNDLEN)) < 0)
	    {
		logMsg("read failed in control \n",0,0,0,0,0,0);
		return (ERROR);
	    }

            if (n == 0){
               return (OK);
	     };
            printf(writebuf,"0x%02x  ",(uchar)buf[0]);
            write(dofr,writebuf,2);
            ++i;
            if (i == 12)
	      {
                i = 1;
                write(dofr,"\n",1);
              };

	    switch (buf[0])
	    {
	      case RESET_LINK:
                b014_save();
		buf[0] = ResetLink();
                b014_restore();
		break;

	      case ANALYZE_LINK:
                b014_save();
		buf[0] = AnalyzeLink();
                b014_restore();
		break;

	      case TEST_ERROR:
		TestError();
		buf[0] = TestError();
		break;

	      case READ_STAT:
		buf[0] = ReadStat();
		logMsg("read stat in control %d \n",buf[0],0,0,0,0,0);
		break;

	      case WRITE_STAT:
		WriteStat();
		buf[0] = WriteStat();
		logMsg("write stat in control %d \n",buf[0],0,0,0,0,0);
		break;

	      case CLOSE_SOCKS:
		closeFLAG = TRUE;
		logMsg("tasks tread and twrite will disappear \n",0,0,0,0,0,0);
                buf[0] = 1;
		break;

              default:
                return ERROR;
	    }
        write(socket, buf, 1);
        return OK;
}














