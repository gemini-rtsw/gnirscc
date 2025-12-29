/* $Id: recCool.h,v 1.2 2009/05/27 19:34:45 fkraemer Exp $ */

#if !defined(REC_COOL_H)
#define REC_COOL_H

#include <ifaErrors.h>

#define COOL_MONITOR_RATE  (0x0001)
#define COOL_MONITOR_BUSY  (0x0002)
#define COOL_MONITOR_INC   (0x0004)

/*
 * Note: this must match the rates that are defined in choiceRec.ascii
 */

#define COOL_RATE_OFF      (0)
#define COOL_RATE_LOW      (1)
#define COOL_RATE_HIGH     (2)

/*
 * Error messages
 */

#define S_ifa_cool_Busy (M_ifa_cool | IFA_ERR | 1)

#endif /* REC_COOL_H */
