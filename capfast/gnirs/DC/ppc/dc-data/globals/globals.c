static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: globals.c,v 1.2 2009/05/27 19:32:40 fkraemer Exp $"
};
#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wdLib.h>
#include "gnDCADefs.h"
#define MAIN
#include "gnDCAVars.h"
#include "saver.h"
/* common globals*/
/* int coAdSim = 0; */
char *dbTop = NULL ;
char *dbSadTop  = NULL;

char buf[128];

long dhsConnected = 0;
/* saver globals*/
saverParams svrP;



/*
 *+
 * FUNCTION NAME:
 * strndup
 *
 * INVOCATION:
 * char *str;
 * char *s1
 * int num;
 * str = strndup(s1, num);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > s1 (const char *) - string to be copied
 * > num (int) - number of characters in returned string
 *
 * FUNCTION VALUE:
 * char * - new string containing s1
 *
 * PURPOSE:
 * Return a duplicate string N characters long.
 *
 * DESCRIPTION:
 * This routine mallocs memory for a string of the specified length and
 * copies the given string into it, returning a pointer to the new string.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 * 14-Aug-1997 Original version - Tad Morgan
 *-
 */
char *strndup(const char *s1, int num)
{
    char *tmp;
    printf( "strndup");
    tmp = malloc(num * sizeof(char));
    printf( " %x\n",(unsigned int )tmp);
    if (tmp != NULL)
    {
        strncpy(tmp, s1, num);
	printf("%s %s\n",tmp,s1);
    }

    return tmp;
}

void sleep (int a, int b)
{
	struct timespec to;
	struct timespec rm;
	to.tv_sec = a;
	to.tv_nsec = b;
	nanosleep(&to,&rm);
}
