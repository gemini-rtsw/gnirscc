/* $Id: ifaErrors.h,v 1.2 2009/05/27 19:34:44 fkraemer Exp $ */

#if !defined(IFA_ERRORS_H)
#define IFA_ERRORS_H

/* 
 * Error codes
 *
 * 0xAAAABCDD
 *
 * AAAA = VxWorks module number (assigned by gemini)
 * B    = Sub-module (currently always 1)
 * C    = Severity 
 *        9 = Programming error (Should never see these)
 *        6 = Error
 *        3 = Warning
 * DD   = Error number
 */

/* This number is assigned by Gemini */
#define M_ifa                  (603 << 16)     /* IFA library */

/* Submodules */
/* Module 0 is global */
#define M_ifa_hs     (M_ifa | (1 << 12)) /* Hallstep record */
#define M_ifa_shs    (M_ifa | (2 << 12)) /* Mechanism simulator */
#define M_ifa_ocyc   (M_ifa | (3 << 12)) /* Temperature controller */
#define M_ifa_ocyd   (M_ifa | (4 << 12)) /* Temperature monitor */
#define M_ifa_tsim   (M_ifa | (5 << 12)) /* Temperature simulator */
#define M_ifa_wfs    (M_ifa | (6 << 12)) /* OIWFS controller */
#define M_ifa_cool   (M_ifa | (7 << 12)) /* Cool record */

#define IFA_ESEVERE (9 << 8)
#define IFA_EWARN   (6 << 8)
#define IFA_ERR     (3 << 8)

#define S_ifa_NotImplemented   (M_ifa | IFA_ESEVERE | 1)
#define S_ifa_Undefined        (M_ifa | IFA_ESEVERE | 2)
#define S_ifa_Impossible       (M_ifa | IFA_ESEVERE | 3)

#endif /* IFA_ERRORS_H */
