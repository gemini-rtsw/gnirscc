#
# This is the ucode command file for the gnAII_1024x1_RR ucode.
#
group {

   float $(top)pucMinInt.VAL = .243;
   float $(top)ucMinInt.VAL = .243;

   float $(top)pucMinRead.VAL = .244;
   float $(top)ucMinRead.VAL = .244;

   float $(top)pucMinDly.VAL = .0020;
   float $(top)ucMinDly.VAL = .0020;

   float $(top)ucFirstDly.VAL = .00000650;

   float $(top)maxPhotonTime.VAL = .490;
   float $(top)maxSpeed.VAL = 2.0;

# RDD code type reset read read pattern
   long $(top)pucuCodeType.VAL = 2;
   long $(top)uCodeType.VAL = 2;

   float $(top)pucDAvgsDly.VAL = .00000090;
   float $(top)ucDAvgsDly.VAL = .00000090;

   long $(top)pucFrmsPCycle.VAL = 2;
   long $(top)ucFrmsPCycle.VAL = 2;

}


