/******************************
*
*	ipOctal.h - include file for IP-Octal driver
*
*	Copyright 1994-1995 - Target Technologies, Inc.
*
*	11/23/94	mwm	created from mdg 
*
***********************************************/


/* ioctl call definitions */  
#define FIOPARITY		100 /* set parity */
#define NO_PARITY		  0
#define ODD_PARITY		  1
#define EVEN_PARITY		  2

#define FIOSTOPBITS		101 /* set number of stop bits */
#define ONE_STOP_BIT		  0
#define TWO_STOP_BITS		  1

#define FIOCHARSIZE		102 /* set character size */
#define CHAR_SIZE_7		  0
#define CHAR_SIZE_8		  1

#define FIOSETRTS		        105 /* set RTS (request to send) */
#define FIOHANDSHAKE	        106 /* Enable HW handshake  */
#define RTS_ACTIVE		  0
#define RTS_INACTIVE		  1
  
#define FIONINTHBIT	       107 /* Enable use of ninth bit protocol */
#define NINTH_ACTIVE		  0
#define NINTH_INACTIVE		  1
  




