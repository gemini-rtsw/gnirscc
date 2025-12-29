#include <intrpt.h>

#ifndef DEFUIUC
typedef unsigned char uc;
typedef unsigned int  ui;
#define DEFUIUC
#endif

void interrupt sio();
myrp(ui, ui);
myprintf(char *);

/* Space 0x7D through 0x7F reserved for protocol */

static uc CMDAV @ 0x7F;
static uc RBYTE @ 0x7E;
static uc BCNT  @ 0x7D;

#asm
CMDAV EQU $7F
RBYTE EQU $7E
BCNT  EQU $7D
#endasm

#ifdef SIOSRC
uc *bbuf, xxcmd;
ui xxd1, xxd2, xxd3;
int errno;
#else
extern uc *bbuf, xxcmd;
extern ui  xxd1, xxd2, xxd3;
extern int errno;
#endif
