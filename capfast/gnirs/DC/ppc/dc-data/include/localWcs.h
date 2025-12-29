#define NPOINTS 100

#define MATRIXSIZE 6
#define NUM_CHIPS 1

typedef struct 
{
    char ctype1[MAX_STRING];
    double crpix1;
    double crval1;
    char ctype2[MAX_STRING];
    double crpix2;
    double crval2;
    char radecsys[MAX_STRING];
    double equinox;
    double mjdobs;
    double cd1_1;
    double cd1_2;
    double cd2_1;
    double cd2_2;

}wcsHeader;

typedef struct
{
   double fpxy[NPOINTS][2];
    double pixij[NPOINTS][2];
    double detij[NPOINTS][2];
    double cij[MATRIXSIZE];
    double pixis;
    double pixjs;
    double perp;
    double orient;
    int numPoints;

}gmosChip;

long getWcsParams();
STATUS wcsCalculate(int);
int sdsuInitWcs(int numChips, char *file1, char *file2, char *file3);

STATUS getWCS(wcsHeader *wcs,int roiNum);
