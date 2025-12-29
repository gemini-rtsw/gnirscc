
/*Motor names*/
/* #define FILT1 */
/* #define FILT2 */
/* #define CAM */
/* #define GRAT */
/* #define XDISP */
/* #define DECK */
/* #define FOCUS */
/* #define SLIT */
/* #define COVER */
/* #define ACQMIRROR */
/* #define SPARE1 */
/* #define SPARE2 */
#define DISABLED "disabled"
#define DBG_QUIET 0x0000
#define DBG_NONE  0x0001
#define DBG_MIN   0x0002
#define DBG_FULL  0x0004
#define DBG_MAX   0x0008
#define LUT_TAG_SZ	MAX_STRING_SIZE
#define FILTPERWHEEL    10  
#define MAXMENU         16      /* Max number of strings allowed in mbbi menu           */
#define STRING_BUF_SZ   256     /* Size of large string buffers.                        */
#define HALF_BUF_SZ     128     /* Half size of large string buffers.                   */


/*
 * Structure for data from $(mech).lut data file describing the filters
 * installed on a particular filter wheel.
 */
#define POS_LEN 10

typedef struct MECHLUT
{
    NODE node;                     /* Next node in linked list.          */
    long barcode;                  /* Barcode ID of .                     */
    char name[LUT_TAG_SZ];     /* Position name .                    */
    char pos[POS_LEN];                      /*  position  installed.*/
    double tilt;                   /* Current grating tilt angle (degrees). */
} MECHLUT;

/* linked list for storing all filters gratings etc. that might be used for GNIRS
   loaded from $(mech)IS.lut
*/
typedef struct NIRSLUT
{
    NODE node;                     /* Next node in linked list.         */
    char tag[LUT_TAG_SZ];          /* mechanism name.                   */
    long barcodeId;                /* Barcode ID of                     */
    double focusOffset;            /* Focus offset  (microns)           */
    double effWavelength;          /* Effective wavelength  (nm)        */ 
	long linesPerMm;               /* Ruling density (lines per mm).    */
    long blazeDir;                 /* Blaze direction at this orientation. */
} NIRSLUT;
