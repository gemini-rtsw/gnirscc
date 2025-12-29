/* ===================================================================== */
/* INDENT OFF */
/*+
 *
 * FILENAME
 * -------- 
 * nirsLutLib.h
 * 
 * PURPOSE
 * -------
 * Header file for nirsLutLib library
 * 
 * NOTE
 * ----
 * The LUT arrays must be unit offset. I.E. They start at index 1 NOT 0
 *
 * AUTHORS
 * -------
 * Nigel Dipper, Dept. of Physics, University of Durham  (N.A.Dipper@durham.ac.uk)
 * Steven Beard, UK Astronomy Technology Centre          (smb@roe.ac.uk)
 * 
 * HISTORY
 * -------
 * $Log: nirsLutLib.h,v $
 * Revision 1.2  2009/05/27 19:33:44  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.3  2000/06/02 11:17:10  nirs
 * Cyclic lookup table handling added
 *
 * Revision 1.2  2000/05/15 16:49:05  nirs
 * Lookup table code tidied up and now assumes arrays start from index
 *
 * Revision 1.1  2000/05/11 12:55:56  nirs
 * gratingLib renamed nirsLutLib
 *
 * Revision 1.2  2000/05/10 08:15:50  nirs
 * Code padded out and made more readable. Lookup table checking function and error message function added.
 *
 * Revision 1.1  1999/12/10 14:33:46  nirs
 * Contents of assembly modules merged into nirscc
 *
 * Revision 1.2  1999/11/02 15:22:06  pbt
 * Added routine gratingSci2Tilt
 *
 * Revision 1.1  1999/09/22 14:56:27  nirs
 * New grating tilt function received from Nigel Dipper and converted to Gemini/VxWorks library
 *
 *-
 */
/* INDENT ON */
/* ===================================================================== */

#ifndef NIRSLUTLIB_INC
#define NIRSLUTLIB_INC


#define DECKER_FILE "deckerIS.lut"
#define FOCUS_FILE "focusIS.lut"
#define CAMERA_FILE "cameraIS.lut"
#define COVER_FILE "coverIS.lut"
#define XDISP_FILE "xdispIS.lut"
#define ACQ_FILE "acqIS.lut"

/* Public constants */

#define MAX_NIRS_LUT_ENTRIES 100             /* Max number of entries in lookup table */

/* Error status codes */

#define NIRSLUT_S_OK               0         /* Status return: OK                   */
#define NIRSLUT_E_TOOSMALL        -1000      /* Insufficient LUT entries            */
#define NIRSLUT_E_OUTRANGE        -1001      /* Value outside range of lookup table */
#define NIRSLUT_E_INVLUT          -1002      /* Invalid lookup table entry          */
#define NIRSLUT_E_LUTEQUAL        -1003      /* Two or more LUT entries equal       */
#define NIRSLUT_E_LUTNOTMONO      -1004      /* Lookup table entries not monotonic  */
#define NIRSLUT_E_NOSOLUTION      -1005      /* No solution found                   */
#define NIRSLUT_E_INTERPOLATION   -1006      /* Error during interpolation          */

/* Lookup table structure definition */

typedef struct
{
  int numberOfEntries;                       /* Number of entries in the table */
  double input[MAX_NIRS_LUT_ENTRIES];        /* Input parameter value          */
  double measured[MAX_NIRS_LUT_ENTRIES];     /* Measured response              */
} nirsLookupTable;

/* Public function prototypes */

int  nirsLutCheck( nirsLookupTable *lut );
int  nirsLutApply( const double given, nirsLookupTable *lut, double *measured );
int  nirsLutCycleApply( const double given, nirsLookupTable *lut, double *measured );
int  gratingSci2Tilt( const double wavelength, const int order,
                      const double r, double *tiltAngle );
void nirsLutMessage( const int code, const int size, char *message );

#endif
