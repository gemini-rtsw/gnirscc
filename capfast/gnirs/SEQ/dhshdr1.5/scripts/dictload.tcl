#+
# dictload
#
# Loads the dhs service dictionary.
#
# D Terrett  7 May 2002
# M Bec     17 Jun 2001
#
# Copyright CCLRC
#+

proc dhshdr::dictload {} {
   hsender -dict add instrument
   hsender -dict add OBJECT
   hsender -dict add GEMPRGID
   hsender -dict add obsid
   hsender -dict add DATALAB
   hsender -dict add OBSERVER
   hsender -dict add OBSTYPE
   hsender -dict add SSA
   hsender -dict add AIRMASS -type DOUBLE
   hsender -dict add AMEND -type DOUBLE
   hsender -dict add AMSTART -type DOUBLE
   hsender -dict add HA
   hsender -dict add LT
   hsender -dict add TRKFRAME
   hsender -dict add DECTRACK -type DOUBLE
   hsender -dict add TRKEPOCH -type DOUBLE
   hsender -dict add RATRACK -type DOUBLE
   hsender -dict add FRAME
   hsender -dict add PMDEC -type DOUBLE
   hsender -dict add PMRA -type DOUBLE
   hsender -dict add WAVELENG -type DOUBLE
   hsender -dict add P1ARA -type DOUBLE
   hsender -dict add P1ARV -type DOUBLE
   hsender -dict add P1AWAVEL -type DOUBLE
   hsender -dict add P1ADEC -type DOUBLE
   hsender -dict add P1AEPOCH -type DOUBLE
   hsender -dict add P1AEQUIN -type DOUBLE
   hsender -dict add P1AFRAME
   hsender -dict add P1AOBJEC
   hsender -dict add P1APMDEC -type DOUBLE
   hsender -dict add P1APMRA -type DOUBLE
   hsender -dict add P1APARAL -type DOUBLE
   hsender -dict add P2ARA -type DOUBLE
   hsender -dict add P2ARV -type DOUBLE
   hsender -dict add P2AWAVEL -type DOUBLE
   hsender -dict add P2ADEC -type DOUBLE
   hsender -dict add P2AEPOCH -type DOUBLE
   hsender -dict add P2AEQUIN -type DOUBLE
   hsender -dict add P2AFRAME
   hsender -dict add P2AOBJEC
   hsender -dict add P2APMDEC -type DOUBLE
   hsender -dict add P2APMRA -type DOUBLE
   hsender -dict add P2APARAL -type DOUBLE
   hsender -dict add OIARA -type DOUBLE
   hsender -dict add OIARV -type DOUBLE
   hsender -dict add OIAWAVEL -type DOUBLE
   hsender -dict add OIADEC -type DOUBLE
   hsender -dict add OIAEPOCH -type DOUBLE
   hsender -dict add OIAEQUIN -type DOUBLE
   hsender -dict add OIAFRAME
   hsender -dict add OIAOBJEC
   hsender -dict add OIAPMDEC -type DOUBLE
   hsender -dict add OIAPMRA -type DOUBLE
   hsender -dict add OIAPARAL -type DOUBLE
   hsender -dict add RAWIQ
   hsender -dict add RAWCC
   hsender -dict add RAWWV
   hsender -dict add RAWBG
   hsender -dict add RAWPIREQ
   hsender -dict add RAWGEMQA
   hsender -dict add CGUIDMOD

   hsender -dict add OBSERVAT
   hsender -dict add TELESCOP
   hsender -dict add RA -type DOUBLE
   hsender -dict add DEC -type DOUBLE
   hsender -dict add ELEVATIO -type DOUBLE
   hsender -dict add AZIMUTH -type DOUBLE
   hsender -dict add CRPA -type DOUBLE

   hsender -dict add PARALLAX -type DOUBLE
   hsender -dict add RADVEL -type DOUBLE
   hsender -dict add EPOCH -type DOUBLE
   hsender -dict add EQUINOX -type DOUBLE
   hsender -dict add TRKEQUIN -type DOUBLE
   
   hsender -dict add UT 
   hsender -dict add DATE
   hsender -dict add M2BAFFLE
   hsender -dict add M2CENBAF
   hsender -dict add ST

   hsender -dict add XOFFSET -type DOUBLE
   hsender -dict add YOFFSET -type DOUBLE
   hsender -dict add RAOFFSET -type DOUBLE
   hsender -dict add DECOFFSE -type DOUBLE  
#   hsender -dict add PARANGLE -type DOUBLE 
   hsender -dict add PA -type DOUBLE
   
   hsender -dict add GCALLAMP
   hsender -dict add GCALFILT
   hsender -dict add GCALDIFF
   hsender -dict add GCALSHUT
   hsender -dict add SFRT2 -type DOUBLE
   hsender -dict add SFTILT -type DOUBLE
   hsender -dict add SFLINEAR -type DOUBLE


   hsender -dict add GPOLPOS                    
   hsender -dict add GPOLPL1                  
   hsender -dict add GPOLPL2
   hsender -dict add GPOLPL3
   hsender -dict add GPOLANG -type DOUBLE
   hsender -dict add GPOLCON

}
