#
# This is the ucode command file for the gnAII_256xFFT_RR ucode.
# This file contains the wave form definitions for the gnaac aladdin array
#	controller. This ucode creates a 256x256 output file which reads a 
#	single pixel set 2048 times. the upper level software must be set to 
#	capture a 256x256 image in SEP mode.
# The number of digital averages selects a sampling frequency
#	1 = 500 kHz, 2 = 400 kHz, 4 = 250 kHz, 8 = 200 kHz, 16 = 100 kHz
# The Int_time seconds gives the number of rows to skip 1-512
# The FInt time seconds gives the number of 16 column groups to skip 1-32
# The code delays exactly 500ms between the two reads of the image
#

group {

   float $(top)pucMinInt.VAL = .090;
   float $(top)ucMinInt.VAL = .090;

   float $(top)pucMinRead.VAL = .091;
   float $(top)ucMinRead.VAL = .091;

   float $(top)pucMinDly.VAL = .0020;
   float $(top)ucMinDly.VAL = .0020;

   float $(top)ucFirstDly.VAL = .00000525;

   float $(top)maxPhotonTime.VAL = .185;
   float $(top)maxSpeed.VAL = 4.8;

# RDD code type reset read read pattern
   long $(top)pucuCodeType.VAL = 2;
   long $(top)uCodeType.VAL = 2;

   float $(top)pucDAvgsDly.VAL = .00000075;
   float $(top)ucDAvgsDly.VAL = .00000075;

   long $(top)pucFrmsPCycle.VAL = 2;
   long $(top)ucFrmsPCycle.VAL = 2;

}


