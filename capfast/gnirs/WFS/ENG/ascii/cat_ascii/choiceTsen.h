/* $Id: choiceTsen.h,v 1.2 2009/05/27 19:33:58 fkraemer Exp $ */

/*
 * Author: Hubert Yamada
 * Date:   1998-09-30
 *
 * Needed for the tsen Record
 */

#if !defined(INCchoiceTsen)
#define INCchoiceTsen

#define REC_TSEN_UNITS 0

/********************************************************************
 * The following constants must match values defined in 
 * choiceRec.ascii.
 ********************************************************************/

/*
 * Units
 */

#define TSEN_UNITS_C (0) /* default */
#define TSEN_UNITS_K (1)
#define TSEN_UNITS_F (2)

#endif
