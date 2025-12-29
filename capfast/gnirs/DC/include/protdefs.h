/******************************************************************************
 * Program:  TProtocol
 * Purpose:  Handle multiplexing of multiple channels over a single transputer
 *		link.
 * File:     prot.h
 * Author:   Nick C. Buchholz
 * History:  
 *	21-Aug-1990 - created file - ncb
 *
 *****************************************************************************/

#ifndef TPPROT
#define TPPROT

#define ADR_LEN		4	/* bytes in an address */

typedef unsigned int uint;

struct sadr
{
    uchar unit;			/* unit number 0-B011, 1-SQIID, 2-DSP 3-...  */
    uchar tpnum;		/* Xputer number determined at compile time */
    uchar unused[2];	
};

union addr
{
    uchar caddr[4];		/* four bytes as characters */
    struct sadr saddr;		/* four byte structure */
    ulong  iaddr;		/* four byte integer */
};

typedef union addr *Addr;	/* pointer to an address object */

struct message
{
    union addr adr;			/* storage for the address */
    uint len;			/* length of message in bytes */
    char *msg;			/* pointer to first byte of message */
};
#define UNIT		0xFF000000 /* mask for unit number part of address */
#define TPUTER		0xFF0000   /* mask for Xputer part of address */
#define PNUM		0xFF00     /* mask for process number part of address */
#define CNUM		0xFF       /* mask for channel number part of address */

/* defines which identify units */
#define SUN		0x00	/* unit number for Sun processes */
#define B0BD		0x01	/* Unit number for B011 main T800 */
#define INST		0x10	/* Unit number for Sqiid instrument */
#define DSP		0x20	/* Unit number for the DSP Box */

/* integer equivalent of the unit identiies in addresses */
#define SUN_I		0x00000000
#define B0BD_I		0x01000000
#define INST_I		0x10000000
#define DSP_I		0x20000000

/* defines to identify transputers within units */
#define B0BD_MAIN	0x01
#define DSP_CNTRL	0x10
#define DSP_1		0x11
#define DSP_2		0x12
#define DSP_3		0x13
#define DSP_4		0x14
#define DSP_5		0x15
#define DSP_6		0x16
#define DSP_7		0x17
#define DSP_8		0x18
#define INST_CNTRL	0x20
#define SEQ_EV_TOP	0x21
#define SEQ_EV_BOT	0x22
#define SEQ_OD_TOP	0x23
#define SEQ_OD_BOT	0x24

/* integer equivalent of id numbers as they appear in addresses */
#define B0BD_MAIN_I	0x00010000
#define DSP_CNTRL_I	0x00100000
#define DSP_1_I		0x00110000
#define DSP_2_I		0x00120000
#define DSP_3_I		0x00130000
#define DSP_4_I		0x00140000
#define DSP_5_I		0x00150000
#define DSP_6_I		0x00160000
#define DSP_7_I		0x00170000
#define DSP_8_I		0x00180000
#define SQD_CNTRL_I	0x00200000
#define SQD_J_I		0x00210000
#define SQD_H_I		0x00220000
#define SQD_K_I		0x00230000
#define SQD_L_I		0x00240000

/* Process ID numbers for addresses */
/* in B011_MAIN */
#define B_B0BD_CC		0x01
#define B_HK_DATA_CAPT	0x02
#define B_CAPT_PIC_1_2	0x03
#define B_CAPT_PIC_3_4	0x04
#define B_RELOAD_DSP	0x05

/* in DSP_CNTRL */
#define D_CCC		0x01
#define D_RELOAD	0x02
#define D_RELAY	0x03

/* in DSP_WORKER */
#define D_WCC		0x01
#define D_PDATA_CAPT	0x02
#define D_PIC_XMIT	0x03
#define D_QUAL_ASS	0x04
#define D_PROC_ALG	0x05
#define D_RELOAD_LTP	0x06

/* in SQD_CNTRL */
#define S_SCC		0x01
#define S_HK_DATA_XMIT	0x02
#define S_INST_VOLT_C	0x03
#define S_SEQ_DOWN_LD	0x04
#define S_HK_DATA_CAPT	0x05

/* in SQD_[JHKL] */
#define S_SEQ_CC	0x01
#define S_WF_GENERATOR	0x02
#define S_EVENT_HNDL	0x03
#define S_SEQ_DOWN_LD	0x04

/* Common Communications paths in integer form */
#define TOSQD_CC	0x10200100 /* from sun to sqiid command control */
#define TOHK_DXMT	0x10200200 /* from sun to hk data xmit */



#endif /* TPPROT */



