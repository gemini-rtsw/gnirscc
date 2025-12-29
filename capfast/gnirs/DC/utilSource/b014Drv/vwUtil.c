static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: vwUtil.c,v 1.2 2009/05/27 19:33:32 fkraemer Exp $"
};
/* Some usefull vxWorks routines. */

#include	<vxWorks.h>
#include	<stdio.h>
#include	<types.h>
#include	<stdlib.h>
#include        <sysSymTbl.h>
#include        <a_out.h>
#include	<sysLib.h>
#include	<taskLib.h>
#include	<string.h>


/*
 * Change a routine name to a VxWorks task name.  Follows the convention
 * of prepending 't' and capitalizing the first letter.
 *
 * Also truncate the name at an imbedded '_'.  This makes the i display
 * look better.
 *
 * Also truncate the name at 10 characters for the same reason.
 */
#include	<ctype.h>
    char *
vwTaskName( routine )
    char	*routine ;
{
    static	char	name[100] ;
    char	c = *routine ;
    char	*cp ;
    sprintf( name, "t%s", routine ) ;
    if( islower(c) )
	name[1] = toupper(c) ;
    if( ( strlen(name) > 2 ) && ( ( cp = index( &name[2], '_' ) ) != NULL ) )
	*cp = '\0' ;
    if( strlen(name) > 10 )
	name[10] = '\0' ;
    return( name ) ;
}


/*
 * Find a symbol in the system symbol table.
 */
    char *
vwSymFind( sym )
char        *sym ;
{
    char        name[MAX_SYS_SYM_LEN + 1] ;
    SYM_TYPE    type ;
    char	*value ;
    /* Prepend '_'. */
    sprintf( name, "_%s", sym ) ;
    if( symFindByNameAndType( sysSymTbl, name, &value, &type,
	N_TEXT, N_TYPE ) != OK ) {
#if	0
	    fprintf( stderr, "Symbol \"%s\" not in symbol table.\n",
	    sym ) ;
#endif
	value = NULL ;
    }
    return( value ) ;
}
