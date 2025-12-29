#ifndef NISFOCUSPROC_H
#define  NISFOCUSPROC_H

#define FOCUS_BLANK     "blank"
#define FOCUS_NOGRATING "no-grating"
#define FOCUS_G10SB	"10/mmSB"
#define FOCUS_G10LB	"10/mmLB"
#define FOCUS_G10LR	"10/mmLR"
#define FOCUS_G10LXD	"10/mmLBLX"
#define FOCUS_G10SXD	"10/mmLBSX"

#define FOCUS_G32SB	"32/mmSB"
#define FOCUS_G32SR	"32/mmSR"
#define FOCUS_G32LB	"32/mmLB"
#define FOCUS_G32LR	"32/mmLR"

#define FOCUS_G111SB 	"111/mmSB"
#define FOCUS_G111SR 	"111/mmSR"
#define FOCUS_G111LB 	"111/mmLB"
#define FOCUS_G111LR 	"111/mmLR"

#define BEST_FOCUS_DIRECTIVE "best focus"

#define FOCUS_SB  "ShortBlue"
#define FOCUS_SR  "ShortRed"
#define FOCUS_LB  "LongBlue"
#define FOCUS_LR  "LongRed"

#define PRISM_MIR "MIR"
#define PRISM_SXD "SXD"
#define PRISM_LXD "LXD"
#define PRISM_WOLL "WOLL"

#define ACQ_POS_IN "In"
#define ACQ_POS_OUT "Out"

/* The following enumeration translates into the focus lookup table columns */
typedef enum {
  UNKNOWN_PRISM=-1,
  G10MIR=0,
  G32MIR,
  G111MIR,
  G10LXD,
  G32LXD,
  G111LXD,
  G10SXD,
  G32SXD,  
  G111SXD,  
  G10WOLL,
  G32WOLL,  
  G111WOLL  
} GnirsPrismGratingPair ;

typedef enum {
  UNKNOWN_CAMERA=-1,
  SHORT_BLUE=0,
  SHORT_RED,
  LONG_BLUE,
  LONG_RED  
} GnirsCamera ;

typedef enum {
SPECTRAL=0,
SPATIAL,
IMAGING
} GnirsFocusMode;

#ifndef NISFOCUSPROC_EXTERNAL

#include "NIRSfocusCommon.h"

#endif   

#endif
