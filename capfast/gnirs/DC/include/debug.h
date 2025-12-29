/* #define DEBUG */
#ifndef _DEBUG_H
#define _DEBUG_H
#ifdef DEBUG
#define DPRINT(a,b) if(a) fputs (b,stderr)
#else
#define DPRINT(a,b)
#endif

#endif
