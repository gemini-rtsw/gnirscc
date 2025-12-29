/******************************************************************************
 * Program:	control
 * File:	util.h
 * Purpose:	declare the routines in util.c
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		20Jul92	created						dak
 *
 *****************************************************************************/

typedef struct p_arr {  /* parrallel array */
        char *name;
        int val;
} p_arr;

char *parse(char *s);
char *to_varname(int num);
int to_varnum(char *name);
char *to_lower(char *s);
char *from_int(char *s, char *buf);
char *from_float(char *s, char *buf);
char *from_str(char *s, char *buf);
char *to_int(char *s, int *i);
char *to_float(char *s, float *f);
char *to_str(char *s, char **str);
int search_s(p_arr *a, char *s);
char *search_i(p_arr *a, int i);
char *search_i_delta(p_arr *a, int i, int delta);
int command_to_int(char *s);
int node_to_int(char *s);
void byte_swap(char *s, int len);

#ifndef HKC
extern double  *HKc[16];
#endif  

