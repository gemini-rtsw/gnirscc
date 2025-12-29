/* call_main.c - Call a main program under vxWorks */

#include "vxWorks.h"
#include "stdioLib.h"
#include "sysSymTbl.h"
#include "strLib.h"
#include "ctype.h"
#include <cicsLib.h>
char tmp[80];
#define	BUFFER_SIZE	1024
#define	MAX_ARGC	40

/*
 * Set up argc and argv as in UNIX.  See below for example.
 */

#ifdef	CM_DEBUG
static	char	*save ;
#endif

/* Call the entry point xaddr which expects arguments in the argc, argv
 * fashion.  I.E.
 * int xaddr( int argc, char **argv )
 */
    int
_call_main( char *xaddr, ... )
{
	va_list	ap ;
	FUNCPTR	routine ;
	char	sym_name[MAX_SYS_SYM_LEN + 1];
	char	*cp ;
	int	address ;
	SYM_TYPE	type ;
	char	*buffer, *sbuf, **argv ;
	int	unused, argc ;
	int	n ;
	char	*malloc() ;
	void	free() ;
	va_start( ap, xaddr ) ;
#ifdef	CM_DEBUG
	while( n = va_arg( ap, int ) ) {
		printf( " 0x%x", n ) ;
	}
	printf( "\n" ) ;
	va_start( ap, xaddr ) ;
#endif
	/* First is the routine to execute. */
	routine = (FUNCPTR)xaddr ;
	if( symFindByValue( sysSymTbl,
	    (UINT)routine, sym_name, &address, &type ) != OK ) 
	  {
		sprintf( tmp, "Could not FindByValue 0x%x.\n",
		    (int)routine ) ;
		cicsLogMessage(0,tmp);
		return( ERROR ) ;
	  }
#ifdef	CM_DEBUG
	printf( "Name of 0x%x is \"%s\", 0x%x, type=%d.\n",
		(int)routine, sym_name, address, type ) ;
#endif
	/* Get memory to store character strings and for argv. */
	buffer = (char *)malloc( BUFFER_SIZE + (MAX_ARGC+1)*sizeof( char *) ) ;
#ifdef	CM_DEBUG
	save = buffer ;
	printf( "malloc( %d ) = 0x%x.\n",
		BUFFER_SIZE + (MAX_ARGC+1)*sizeof( char *),
		(int)buffer ) ;
#endif
	sbuf = buffer ;
	unused = BUFFER_SIZE ;
	argv = (char **)&buffer[BUFFER_SIZE] ;
	argc = 0 ;
	argv[ argc++ ] = sym_name ;
	while( cp = va_arg( ap, char * ) ) {
		char	c ;
		/* It is an error if the buffer gets full or if there are too
		 * many arguments.
		 */
		n = strlen( cp ) + 1 ;
		if( unused-n <= 0 ) {
			cicsLogMessage(0,"Arguments too long in call_main.\n" ) ;
			return( ERROR ) ;
		}
		strcpy( sbuf, cp ) ;
		/* Take care of " -a -b -c"; we want argv to point to "-a",
		 * "-b" etc.
		 */
		cp = sbuf ;
		while( c = *cp++ ) {
			/* skip leading blanks */
			if( isspace( c ) )
				continue ;
			if(  argc+1 >= MAX_ARGC ) {
				cicsLogMessage(0,
				    "Too many arguments in call_main.\n" ) ;
				return( ERROR ) ;
			}
			argv[ argc++ ] = cp - 1 ;
			/* replace trailing blank with '\0' */
			while( c = *cp ) {
				cp++ ;
				if( isspace( c ) ) {
					*(cp - 1) = '\0' ;
					break ;
				}
			}
		}
		sbuf += n ;
		unused -= n ;
	}
	va_end( ap ) ;
	argv[ argc ] = (char *)0 ;
#ifdef	CM_DEBUG
	{	char	**cpp = argv ;
	while( *cpp )
		printf( "%s ", *cpp++ ) ;
	printf( "\n" ) ;
	}
#endif
	n = routine( argc, argv ) ;
#ifdef	CM_DEBUG
	printf( "buffer=0x%x, save=0x%x.\n", (int)buffer, (int)save ) ;
/*
	if( save == buffer )
		free( buffer ) ;
*/
#else
	free( buffer ) ;
#endif
	return( n ) ;
}

#ifdef	EXAMPLE
#include	"call_main.h"

/* Macro call_main( ep, routine ) defines the entry point ep which
 * expects any number of char * arguments.  The arguments are parsed and
 * arranged in the classic argc, argv fashion and passed to
 * int routine( int argc, char **argv ).
 */

call_main( z, xx )

/* Some test routines below. */

    int
x( a1, a2, a3, a4, a5, a6, a7, a8, a9, aa )
	int a1, a2, a3, a4, a5, a6, a7, a8, a9, aa ;
{
	extern	int xx() ;
	return( _call_main( (char *)xx,
		a1, a2, a3, a4, a5, a6, a7, a8, a9, aa ) ) ;
}
    int
y()
{
	int	xx() ;
	char	*argv[10] ;
	argv[0] = "y" ;
	argv[1] = "aaaaaaaaa" ;
	argv[2] = "bbb cccc" ;
	argv[3] = (char *)0 ;
	xx( 5, argv ) ;
	return OK ;
}
	int
xx( argc, argv )
	int	argc ;
	char	**argv ;
{
	printf( "argc=%d.\n", argc ) ;
	while( *argv )
		printf( "%s ", *argv++ ) ;
	printf( "\n" ) ;
	return( argc ) ;
}
#endif
