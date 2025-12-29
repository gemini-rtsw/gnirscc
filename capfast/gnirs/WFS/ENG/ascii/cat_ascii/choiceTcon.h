/* $Id: choiceTcon.h,v 1.2 2009/05/27 19:33:58 fkraemer Exp $ */

/*
 * Author: Hubert Yamada
 * Date:   1997-04-09
 *
 * Needed for the tcon Record
 */

#if !defined(INCchoiceTcon)
#define INCchoiceTcon

#define REC_TCON_RANG 0
#define REC_TCON_TUNE 1
#define REC_TCON_UNITS 2

/********************************************************************
 * The following constants must match values defined in 
 * choiceRec.ascii.
 ********************************************************************/

/*
 * Temperature controller range.
 *
 * Warning: These values do _not_ match the numeric constants for the
 * Omega CYC RANG command.
 */

#define TCON_RANG_OFF   (0) /* default */
#define TCON_RANG_LOW   (1)
#define TCON_RANG_HIGH  (2)

/* 
 * Autotuning status
 *
 * Warning: These values do _not_ match the numeric constants for the
 * Omega CYC RANG command.
 */

#define TCON_TUNE_MANUAL (0) /* default */
#define TCON_TUNE_P      (1)
#define TCON_TUNE_PI     (2)
#define TCON_TUNE_PID    (3)

/*
 * Units
 */

#define TCON_UNITS_C (0) /* default */
#define TCON_UNITS_K (1)
#define TCON_UNITS_F (2)

#endif
