#
# This is an example of a ucode command file.
# It is not in any way a realist example, but
# serves as a test of the parsing routine in
# the init command which will parse a file
# of this format and store pertinant values
# in EPICS records.
#
group {

   float $(top)dtxyXScale.VAL = 100.0;
   float $(top)dtxyXOffset.VAL = 10.0;

}

#

group {
   float $(top)pucMinInt.VAL = 100.0;
   float $(top)pucMinRead.VAL = 10.0;
   float $(top)pucMinDly.VAL = 150.0;
}


group {
   long $(top)pucuCodeType.VAL = 4;
   float $(top)dtxyXOffset.VAL = 10.0;
}

   float $(top)pucdAvgDly.VAL = 20.0;

   float $(top)dtxyXScale.VAL = 100.0;
   long $(top)pucFrmsPCycle.VAL = 8;
   float $(top)dtxyXOffset.VAL = 10.0;

#
# 
# 
# comments
# 
#
# 
#
group {

   long $(top)dtxyXC.IVAL = 1;
   long $(top)dtxyYC.IVAL = 1;
   long $(top)dtxyC.IVAL = 1;

}
