#
# This is the ucode command file for the gnAII_1024x1_RR ucode.
#
group {

   float $(top)pucMinInt.VAL = .176;
   float $(top)ucMinInt.VAL = .176;

   float $(top)pucMinRead.VAL = .177;
   float $(top)ucMinRead.VAL = .177;

   float $(top)pucMinDly.VAL = .0020;
   float $(top)ucMinDly.VAL = .0020;

   float $(top)ucFirstDly.VAL = .00000525;

   float $(top)maxPhotonTime.VAL = .352;
   float $(top)maxSpeed.VAL = 2.8;

# RDD code type reset read read pattern
   long $(top)pucuCodeType.VAL = 2;
   long $(top)uCodeType.VAL = 2;

   float $(top)pucDAvgsDly.VAL = .00000075;
   float $(top)ucDAvgsDly.VAL = .00000075;

   long $(top)pucFrmsPCycle.VAL = 2;
   long $(top)ucFrmsPCycle.VAL = 2;

}


