/* @(#) Module: ims_bcmd.h, revision 1.1 of 8/23/90 */

#ifndef _IMS_BCMD_H_

#define _IMS_BCMD_H_

#if	defined(vxWorks)
#include "ioctl.h"
#else
#include <sys/ioccom.h>
#endif
/*
 * I/O controls
 */
struct IMS_SETF {
   unsigned int    op:16;
   unsigned int    val:16;
};

struct IMS_READF {
   unsigned int    reserved:28;
   unsigned int    read_f:1;
   unsigned int    write_f:1;
   unsigned int    timeout_f:1; /* not used */
   unsigned int    error_f:1;
};

union IMS_IO {
   struct IMS_SETF set;
   struct IMS_READF status;
};

#define RESET                   (1)
#define ANALYSE                 (2)
#define SETTIMEOUT              (3)
#define SETERRORSIGNAL          (4)
#define RESETERRORSIGNAL        (5)
#define SETREADDMA              (6)     /* use DMA for reads */
#define SETWRITEDMA             (7)     /* use DMA for writes */
#define RESETDMA                (8)     /* turn off DMA */
/*
 * _IOR and _IOW encode the read/write instructions to the kernel within the
 * ioctl command code.
 */

#if	defined(vxWorks)
#define READFLAGS       _IOR('k', 0, union IMS_IO)
#define SETFLAGS        _IOW('k', 1, union IMS_IO)
#else
#define READFLAGS       _IOR(k, 0, union IMS_IO)
#define SETFLAGS        _IOW(k, 1, union IMS_IO)
#endif

/* Some early inmos device drivers used different names */

/* S308A needs these */
#define B008_SETF  IMS_SETF
#define B008_READF IMS_READF
#define B008_IO    IMS_IO

/* S514A and S514B need these */
#define B014_SETF  IMS_SETF
#define B014_READF IMS_READF
#define B014_IO    IMS_IO

#endif /* _IMS_BCMD_H_ */


