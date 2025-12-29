/* b016reg --- header file for B016 device driver           06/06/1990 */
/* Copyright (c) 1990 John Honniball, INMOS Limited                    */

/* Modification:
   06/06/90 BJ  Initial coding
*/


struct RegPair {              /* VIC registers are always odd bytes */
   unsigned char junk;           /* Even bytes are unused          */
   unsigned char reg;            /* Odd bytes are actual registers */
};

struct SetClr {               /* Switch registers have two bytes each */
   unsigned char set;            /* Set the switch   */
   unsigned char clr;            /* Clear the switch */
};

/* Struct that will overlay VIC registers in VME 16-bit address space */
struct B016Reg {
   struct RegPair vic[5];        /* Five general purpose registers     */
   unsigned char noReg5;
   unsigned char vicIDReg;       /* VIC ID register = 0xf1 (read-only) */
   unsigned char noReg6;
   unsigned char vicStatusReg;   /* VIC status register (read-only)    */
   unsigned char noReg7;
   unsigned char vicSemaReg;     /* VIC semaphore registers            */
   struct SetClr ICGS[4];        /* Four Global Switch registers       */
   unsigned char noReg8[8];
   struct SetClr ICMS[4];        /* Four Module Switch registers       */
   unsigned char noReg9[8];
};


/* Commands recognised by B016 ROM program */
#define NO_CMD             0
#define TEST_ERROR_CMD     1
#define READ_LINK_CMD      2
#define WRITE_LINK_CMD     3
#define RESET_CMD          4
#define ANALYSE_CMD        5


/* Error codes returned from ROM program */
#define E_NOERR            1
#define E_TIMEOUT          2
#define E_SETERROR         3
#define E_CLRERROR         4
#define E_STUCK            0xfe
#define E_NULL             0xff

#define NO_LINK            0xff
