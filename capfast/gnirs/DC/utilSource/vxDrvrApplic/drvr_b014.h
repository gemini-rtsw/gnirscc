/*******************************************************************************
 * Program:	naacdrvr
 * File:	drvr_b014.h
 * Purpose:	describes b014 registers and memory mapping
 * Author:	Diana Kennedy
 * History:
 *	11-Jan-1996 created file - djk
 *
 ******************************************************************************/
#ifndef B014MAP
#define B014MAP

#include "vxWorks.h"

#define PAGE_SIZE	getpagesize();

#define DEFAULT_BASE	0x9900
#define ADDRMAP		0x80000000	/* addr 0 maps to transputer addr ? */

#define CHIPMEM		4096		/* for a t414 */

/* bytes displacement from the base of registers */

#define IDR_OFF 0x01;
#define ODR_OFF 0x03;
#define ISR_OFF 0x05;
#define OSR_OFF 0x07;
#define RESET_ERROR_OFF 0x09;
#define ANALYSE_OFF 0x0B;
#define INTRUPT_ENABLE_OFF 0x0D;
#define INTRUPT_LEVEL_OFF 0x0F;
#define INTRUPT_STATID_OFF 0x11;
#define TRAM_ERRORS_OFF 0x13;

/* b014 register bit definitions */
#define R_INTERRUPT_ENABLE 0x08
#define W_INTERRUPT_ENABLE 0x04
#define E_INTERRUPT_ENABLE 0x02

#define R_INTERRUPT_DISABLE 0xF7
#define W_INTERRUPT_DISABLE 0xFB
#define E_INTERRUPT_DISABLE 0xFD

#define STATUS_BIT_0 0x01
#define STATUS_BIT_1 0x02
#define STATUS_NOT_BIT_0 0xFE
#define STATUS_NOT_BIT_1 0xFD



/* addresses of registers to talk to the b014 */


typedef struct {
  char * baseAddr;        /* local address of b014 */
  BOOL  initialized;      /* B014_MAP  initialized indication */
  UINT8 idr;              /* current state of input data */
  UINT8 * pidr;
  UINT8 odr;              /* current state of output data */
  UINT8 * podr;
  UINT8 isr;              /* current state of input status */
  UINT8 * pisr;
  UINT8 osr;              /* current state of output status */
  UINT8 * posr;
  UINT8 reset_error;      /* current state of reset_error */
  UINT8 * preset_error;
  UINT8 analyse;          /* current state of analyse */
  UINT8 * panalyse;
  UINT8 intrupt_enable;   /* current state of interrupt enable */
  UINT8 * pintrupt_enable;
  UINT8 intrupt_level;    /* current state of interrupt level */
  UINT8 * pintrupt_level;
  UINT8 intrupt_statid;   /* current state of interrupt statid */ 
  UINT8 * pintrupt_statid;
  UINT8 tram_errs;        /* current state of tram errors */
  UINT8 * ptram_errs;
}B014_MAP;

#endif /*B014MAP*/


