/* $Id: xy490.h,v 1.2 2009/05/27 19:34:46 fkraemer Exp $ */

#if !defined(XY490_H)
#define XY490_SERIAL_H

#include <sys/types.h>

#define XY490_FIFO_SIZE (3) /* Size of the hardware FIFO */

extern int xy490Init(int, unsigned, unsigned);
extern int xy490WriteString(int, const char *);
extern int xy490Read(int, char *const, size_t);
extern int xy490SetBaud(int, int);
extern int xy490FlushInput(int);
extern int xy490InputClearError(int);
extern void xy490PrintfKludge(void);
extern void xy490Reinit(int);
extern const char *xy490GetError(int);
extern void xy490SetError(int, const char *);
extern int xy490DebugPushChar(int, char);

#endif
