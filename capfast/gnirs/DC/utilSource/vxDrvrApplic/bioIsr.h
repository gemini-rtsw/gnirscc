/*************************************************************************
 *  bioIsr.h
 *
 *  include header file for Interrupt Service Routine
 *
 *
 *  created 17-Jan-1996 Diana Kennedy
 *
 *************************************************************************
*/

#ifdef BIOISR

extern int OBcount;
extern int OBin;
extern int OBout;
extern int IBcount;
extern int IBin;
extern int IBout;
extern int OBflag;
extern int OBloop;
extern UINT8 OutBuff[8192];
extern UINT8 InBuff[8192];
extern int Buf_MAX;
extern int closeFLAG;
#else
int Buf_MAX = 8192;
#define BIOISR
int OBcount;
int OBin;
int OBout;
int IBcount;
int IBin;
int IBout;
int OBflag;
int OBloop;
int closeFLAG;
UINT8 OutBuff[8192];
UINT8 InBuff[8192];
#endif





