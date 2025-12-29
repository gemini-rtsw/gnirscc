static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: b014Drv.c,v 1.2 2009/05/27 19:33:32 fkraemer Exp $"
};
/* B014 device driver for VxWorks, 21Mar97, jan@noao.edu. */

#include "vxWorks.h"
#include "stdio.h"
#include "logLib.h"
#include "vme.h"
#include "iosLib.h"
#include "semLib.h"
#include "wdLib.h"
#include "types.h"
#include "ioctl.h"
#include "intLib.h"
#include "iv.h"
#include "sysLib.h"
#include "vxLib.h"
#include <cicsLib.h>
/* #include "../config/mv167/mv167.h" */
#include "b014regs.h"
#include "b014cmds.h"
#include "call_main.h"
#include	<semLib.h>

#ifdef	DEBUG
#define	STATIC
#else	/* DEBUG */
#define	STATIC	static
#endif	/* DEBUG */
char tmp[80];
STATIC	struct	b014_stat	b014_stat ;

extern	STATUS	b014Attach ();
extern	int	b014Open ();
extern	STATUS	b014Close ();
extern	STATUS	b014Ioctl ();
extern	int	b014Read ();
extern	int	b014Write ();
extern	VOID	bxivIntr ();
static	void	l_b014( char *, struct b014_stat * ) ;
SEM_ID tpSem;

call_main( b014mod, xb014mod ) 

    int
xb014mod( int argc, char **argv )
{
    char *usage = "[-r[eset]] [-dO[pen]] [-dI[ntr]] [-dS[trat]] [-dP[hysio]]\
 [-dR[dWr]] [-dC[tl]]";

    /* Process command line arguments. */
    if( argc <= 1 )
	    goto BAD ;
    while (argc > 1 && *argv[1] == '-') {
	    switch (argv[1][1]) {
	    case 'r':			      /* -reset */
		b014_stat.b014Dbg = 0 ;
		break;
	    case 'd':			      /* -dxxx */
	    case 'D':
		switch (argv[1][2]) {
		    case 'o':
		    case 'O':
			b014_stat.b014Dbg |= DbgOpen ;
			break;
		    case 'i':
		    case 'I':
			b014_stat.b014Dbg |= DbgIntr ;
			break;
		    case 's':
		    case 'S':
			b014_stat.b014Dbg |= DbgStrat ;
			break;
		    case 'p':
		    case 'P':
			b014_stat.b014Dbg |= DbgPhysio ;
			break;
		    case 'r':
		    case 'R':
			b014_stat.b014Dbg |= DbgRdWr ;
			break;
		    case 'c':
		    case 'C':
			b014_stat.b014Dbg |= DbgIoctl ;
			break;
		    default:
			break;
		}
		break;
	    default:
		sprintf(tmp,"%s: Don't know %s.\n",
		   "b014mod", argv[1]);
		cicsLogMessage(0,tmp);
BAD:
		sprintf(tmp,"Usage: %s %s\n", "modify", usage);
		cicsLogMessage(0,tmp);
		return(ERROR);
		break;
	    }
	    argc--, argv++;
    }
    return OK ;
}

/*******************************************************************************
*
* b014Drv - install B014 driver and add device /b014.
*
* RETURNS: OK or ERROR if board not present
*/

STATUS b014Drv ( int flag )
{
	/* FALSE means this routine has not been called. */
	static int result = FALSE ;
	int test ;		/* for board probe */
	struct B014Reg *r;
	int	b014Num ;

	tpSem = semBCreate( SEM_Q_PRIORITY, SEM_FULL ) ; 

	/* flag < 0 allows a re-call of b014Drv(). */
	if( flag < 0 ) {
	    flag = -flag ;
	    result = FALSE ;
	}
	b014_stat.b014Dbg = flag ;
	if (result != FALSE) {
		/* called before - return previous value */
		return( result ) ;
	}
	/* Determine local address of board.  We know the VME address and the
	 * VME address modifier.
	 */
	if( (result = sysBusToLocalAdrs( B014_AM, B014_BASE,
	    &b014_stat.b014_address ) ) != OK ) {
		cicsLogMessage(0,"b014Drv: Cannot convert VME address to local.\n");
		return( result ) ;
	}
	printf("b014Addr %08x\n", (int) b014_stat.b014_address);

	if( b014_stat.b014Dbg )
	    printf( "Local address is 0x%x.\n", (int)b014_stat.b014_address ) ;
	/* Probe for hardware;
	 * if it's not there, return ERROR.
	 */
	r = (struct B014Reg *)b014_stat.b014_address ;
	if( ( result = vxMemProbe( (char *)&r->Isr, VX_READ, 1,
	    (char *)&test) ) != OK ) 
	  {
	    sprintf(tmp,"b014Drv: vxMemProbe( 0x%x ) fails.\n",
		    (unsigned int)b014_stat.b014_address );
	    cicsLogMessage(0,tmp);
	    return (result);
	  }
	if( b014_stat.b014Dbg )
	    printf( "r->Isr is 0x%x.\n", (int)r->Isr ) ;
	/* Install */
	b014Num = iosDrvInstall ( (FUNCPTR)NULL, (FUNCPTR)NULL,
		b014Open, b014Close,
		b014Read, b014Write, b014Ioctl);
	/* Create the device. */
	if( ( result = iosDevAdd ((DEV_HDR *)&b014_stat, B014_DEV_NAME,
	    b014Num) ) != OK ) {
		cicsLogMessage(0,"b014Drv: can't iosDevAdd.\n");
		return (result);
	}
	/* Enable interrupt. */
	if( ( result = sysIntEnable( B014_INTERRUPT_LEVEL ) ) != OK ) {
		cicsLogMessage(0,"b014Drv: can't sysIntEnable.\n");
		return (result);
	}
	/* Connect interrupt service routine. */
	if( ( result = intConnect( INUM_TO_IVEC( B014_INTERRUPT_NUM ),
	    bxivIntr, (int)&b014_stat ) ) != OK ) {
		cicsLogMessage(0, "b014Drv: can't intConnect.\n");
		return (result);
	}
	printf ("a1: 0x%x 0x%x\n", (int)INUM_TO_IVEC( B014_INTERRUPT_NUM ), (int)bxivIntr);
	/* Initialize B014 registers.
	 * We call b014Attach() in b014.c.
	 * See also b014_init() in /repos2/NAACsrc/vxDrvrApplic/sys/naacsrvr.c
	 */
	result = b014Attach( &b014_stat ) ;
	if( b014_stat.b014Dbg )
		l_b014( "b014Drv", &b014_stat ) ;
	return (result);
}
    void
lb014( char *comm )	{
	l_b014( comm==NULL ? "Shell" : comm, &b014_stat ) ;
}
	static void
l_b014( cp, pb014Stat )
	char	*cp ;
	struct b014_stat *pb014Stat ;
{
	printf( "l_b014( %s, 0x%x )\n", cp, (int)pb014Stat ) ;
	printf( "B014 drvNum:   %d\n", pb014Stat->b014_hdr.drvNum ) ;
	printf( "B014 name:     %s\n", pb014Stat->b014_hdr.name ) ;
	printf( "B014 address:  0x%x\n", (int)pb014Stat->b014_address  ) ;
	printf( "B014 debug:    0x%x\n", (int)pb014Stat->b014Dbg  ) ;
	printf( "B014 nRdIntrs: %d\n", pb014Stat->nRdIntrs  ) ;
	printf( "B014 nWrIntrs: %d\n", pb014Stat->nWrIntrs  ) ;
}
