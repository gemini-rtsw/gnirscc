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
extern float  HKc0_15[16],    HKc16_31[16],   HKc32_47[16],   HKc48_63[16],
              HKc64_79[16],   HKc80_95[16],   HKc96_111[16],  HKc112_127[16],
              HKc128_143[16], HKc144_159[16], HKc160_175[16], HKc176_191[16],
              HKc192_207[16], HKc208_223[16], HKc224_239[16], HKc240_255[16];
#endif  

