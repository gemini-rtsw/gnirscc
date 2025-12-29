static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: util.c,v 1.2 2009/05/27 19:33:35 fkraemer Exp $"
};
/******************************************************************************
 * File:	util.c
 * Purpose:	Provide a tcl interface to the INMOS link library. Actually
 *		this file just provides lower level utility routines for the
 *		tcl commands.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		 1Jun92	created						dak
 *		20Jul92	split from message.c				dak
 *              22-May-1995 Changed name fo commands to ircommande name 
 *              change needed use tcl-tk and tcl-dp commands is used 
 *              globaly by tkWindows  SLP 
 *
 *		5 Mar 97  modified to run on vxWorks
 *		     1.	removed any references to tcl routines
 *		     2. changed header file includes to be consistent with 
 *			 vxWorks (Peter Ruckle)
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
/* #include <strings.h> */
#include <string.h>
/* #include <sys/time.h> */
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
/* #include "ansi.h" */
/* #include "tcl.h" */

#define HKC
#include "util.h"

#define ABS(x)		((x) < 0 ? -(x) : (x))
extern p_arr ircommands[], variables[], nodes[];

extern double  *HKc[16];
extern double  HKc0[], HKc1[], HKc2[], HKc3[], HKc4[], HKc5[], HKc6[], HKc7[],
		HKc8[], HKc9[], HKc10[], HKc11[], HKc12[], HKc13[], HKc14[],
		HKc15[];

#define LINK int
/* LINK link_id = -1; */			/* fd for the link */

void byte_swap(char *s, int len);


/******************************************************************************
 * Routine: parse
 * Purpose: parse a string into pieces (space separated or "")
 * Inputs:  s -- string to parse
 * Returns: char * -- next piece
 * 
 *****************************************************************************/
char *parse(char *s)
{
    static char *str = NULL;
    static char *pos = NULL;
    static char buf[1024], *dest = buf;

    if (s != str) 
    {
	str = s;
	pos = s;
    }

    if (s == NULL)
	return NULL;

    dest = buf;

    while (*pos != '\0' && isspace(*pos))	/* skip any leading space */
	pos ++;

    if (*pos == '\0')
	return NULL;
    
    if (*pos == '"') {				/* match with other '"' */
	pos++;
	while (*pos != '"')
	    *(dest++) = *(pos++);
	pos ++;
    } else {
	while (!isspace(*pos) && *pos != '\0')
	    *(dest++) = *(pos++);
    }

    *dest = '\0';
    return buf;
}

/******************************************************************************
 * Routine: to_varname
 * Purpose: convert a number into a variable name
 * Inputs:  int -- var_num
 * Returns: char * -- name
 * 
 *****************************************************************************/

char *to_varname(int num)
{
    byte_swap((char *) &num, 4);
    return search_i(variables, num);
}


/******************************************************************************
 * Routine: to_varnum
 * Purpose: convert a variable name into a number
 * Inputs:  char * -- name
 * Returns: int -- num
 * 
 *****************************************************************************/

int to_varnum(char *name)
{
    int num, ind = 0;
    char nameCp[80];
    char *paren;

    if ((paren = strchr(name, '(')) != NULL) {
	strncpy(nameCp, name, paren - name);
	nameCp[paren - name] = '\0';
	ind = atoi(paren + 1);
    } else {
	strcpy(nameCp, name);
    }
    
    num = search_s(variables, to_lower(nameCp));
    num |= (ind << 16);
    byte_swap((char *) &num, 4);
    return num;
}

int command_to_int(char *s)
{

    return search_s (ircommands, to_lower(s));
}

int node_to_int(char *s)
{
    return search_s(nodes, to_lower(s));
}

/******************************************************************************
 * Routine: to_lower
 * Purpose: convert a string into all lower case
 * Inputs:  char * -- string to convert
 * Returns: char * -- converted string
 * 
 ******************************************************************************/

char *to_lower(char *s)
{
    char *p;

    p = s;

    while (*p != '\0') {

	if (isupper(*p))
	    *p = tolower(*p);
	
	p++;

    }

    return s;
}

/******************************************************************************
 * Routine: buff_to_floats
 * Purpose: convert a buffer into the a buffer of floats
 * Inputs:  buffer of length 
 * Returns: new buffer
 * 
 ******************************************************************************/

/* float *buff_to_floats(Tcl_Interp *interp,char *s,int length, float *fbuff) */
float *buff_to_floats(char *s,int length, float *fbuff)

{

    int i;
    double *HKcP[] = { HKc0, HKc1, HKc2, HKc3, HKc4, HKc5, HKc6, HKc7,
		       HKc8, HKc9, HKc10, HKc11, HKc12, HKc13, HKc14, HKc15 };
    double *HKch;
    float newfloat;
    /* char tclfloat[136]; */

   if (length > 136)
       length = 136;

    for (i = 0; i < length; i++)
    {
         HKch = HKcP[i / 16];
         s = to_float(s, &newfloat);
         fbuff[i] = newfloat;
         HKch[i % 16] = (double) newfloat;
      /*   sprintf(tclfloat,"set buffloats(%d) %f",i,newfloat); */
      /*    Tcl_Eval(interp,tclfloat); */
	 
    };

    return fbuff;
}
void byte_swap(char *s, int len)
{    /* change the order of the bytes from 1234 to 4321 for every four bytes*/
    /*  "abcdefghijkl" would be "dcbahgfelkji"*/


   char *d = s + 3;
    char t;

    for ( ; len > 0; len -= 4) {
	t = *s;
	*s = *d;
	*d = t;

	s++; d--;

	t = *s;
	*s = *d;
	*d = t;

	s += 3;
	d += 5;
    }
}


/******************************************************************************
 * Routine: from_int
 * Purpose: convert from an int into message buffer format
 * Inputs:  char * -- int to convert
 *	    char * -- buffer
 * Returns: none
 * 
 ******************************************************************************/

char *from_int(char *s, char *buf)
{
    *((int *) buf) = atoi(s);

    byte_swap(buf, 4);

    return buf + 4;
}

/******************************************************************************
 * Routine: from_float
 * Purpose: convert from a float into a message buffer format
 * Inputs:  char * -- float to convert
 *	    char * -- buf to write into
 * Returns: none
 * 
 ******************************************************************************/

char *from_float(char *s, char *buf)
{
    *((float *) buf) = atof(s);

    byte_swap(buf, 4);

    return buf + 4;
}

/******************************************************************************
 * Routine: from_str
 * Purpose: convert a string into message buffer format
 * Inputs:  char * -- string to convert
 *	    char * -- buf to write into
 * Returns: none
 * 
 ******************************************************************************/

char *from_str(char *s, char *buf)
{
    while (*s != '\0') {
	*(buf++) = *(s++);
    }
    
    *(buf++) = '\0';

    buf = (char *) (((int) buf + 3) & ~0x3);

    return buf;
}

/******************************************************************************
 * Routine: to_int
 * Purpose: 
 * Inputs:
 * Returns: char * -- position after the last one converted
 * 
 ******************************************************************************/

char *to_int(char *s, int *i)
{
    byte_swap(s, 4);

    *i = *((int *) s);

    return s + 4;
}

/******************************************************************************
 * Routine: to_float
 * Purpose: 
 * Inputs:
 * Returns: char * -- position after the last one converted
 * 
 ******************************************************************************/

char *to_float(char *s, float *f)
{
    byte_swap(s, 4);

    *f = *((float *) s);

    return s + 4;
}

/******************************************************************************
 * Routine: to_str
 * Purpose: 
 * Inputs:
 * Returns: char * -- position after the last one converted
 * 
 ******************************************************************************/

void test_str (char *t1)
{	
    char *str;
    char *ss;
   
    ss = to_str (t1,&str);
    printf ("in = %s, out = %s\n, ss = %s",t1,str,ss);
}

char *to_str(char *s, char **str)
{
    static char buf[1024], *p;

    *str = buf;
    p = buf;

    while (*s != '\0') {
        *(p++) = *(s++);
    }

    *(p++) = '\0';

    s = (char *) (((int) s + 3) & ~0x3);

    return s;
}

/******************************************************************************
 * Routine: search_[si]
 * Purpose: search an array for a string and return its associated value.
 * Inputs:  a -- (p_arr *) array to search, "" terminated.
 *	    s -- (char *) string to search for
 * Returns: int associated with s, or -1 if not found.
 * 
 ******************************************************************************/

int search_s(p_arr *a, char *s)
{
    while (*a->name != '\0') {

	if (strcmp(a->name, s) == 0)
	    return a->val;
	
	a++;

    }

    return -1;
}

char *search_i(p_arr *a, int i)
{
    while (*a->name != '\0') {

	if (a->val == i)
	    return a->name;
	
	a++;
    
    }

    return a->name;
}

char *search_i_delta(p_arr *a, int i, int delta)
{
    while (*a->name != '\0') {

	if (ABS(a->val - i) <= delta)
	    return a->name;
	
	a++;
    
    }

    return a->name;
}



