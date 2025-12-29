#include "vxWorks.h"
 
#if (CPU==MC68040)
#  define SYS_WDOG      ((volatile unsigned long *) 0xfff4004c)
#  define SYS_RSET      ((volatile unsigned long *) 0xfff40060)
#  define SYS_WDOG_BITS (0x00000c00)
#  define SYS_RSET_BITS (0x00250000)
#else
#error Undefined CPU type for system reset address
#endif /* (CPU==MC68040) */
