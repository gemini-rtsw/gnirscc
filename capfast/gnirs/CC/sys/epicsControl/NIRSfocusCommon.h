#ifndef NISFOCUSCOMMON_H
#define NISFOCUSCOMMON_H

#define LINE_LEN 		256
#define PRISM_GRATING_ID_MAX 	12
#define MAX_FILTERS_PER_GRISM  	32

/* Relative to the base*/
#define DAT_FILE_DIR "../IS/IS/data/"
/*#define DAT_FILE_DIR "./data/"*/

#define NIRS_FOCUS_ERROR -1
#define SUCCESS 0

typedef struct NISfocusConfDef {
   char* filter1;
   char* filter2;
   int focus[PRISM_GRATING_ID_MAX];
} NISfocusConfDef;

int updateFocusConfArray(const char *name);
int updateAllSpectralFocusConfArrays();
int updateAllSpatialFocusConfArrays();
int updateAllImagingFocusConfArrays();

extern NISfocusConfDef *focusSBspectral;
extern NISfocusConfDef *focusSRspectral;
extern NISfocusConfDef *focusLBspectral;
extern NISfocusConfDef *focusLRspectral;

extern NISfocusConfDef *focusSBspatial;
extern NISfocusConfDef *focusSRspatial;
extern NISfocusConfDef *focusLBspatial;
extern NISfocusConfDef *focusLRspatial;

extern NISfocusConfDef *focusSBimaging;
extern NISfocusConfDef *focusSRimaging;
extern NISfocusConfDef *focusLBimaging;
extern NISfocusConfDef *focusLRimaging;

/* Uncomment when testing as a standalone on solaris - need a main 
#define NIS_FOC_TESTING
#define DAT_FILE_DIR "./"
*/

#endif
