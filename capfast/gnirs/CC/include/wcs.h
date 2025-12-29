#define NPOINTS 100

#define MATRIXSIZE 6
#define NUM_CHIPS 3


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
