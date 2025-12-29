This UAE application directory contains all the library, utility files and
capfast schematicswhich are shared by the whole CICS system. It contains: 

in the "capfast" subdirectory:
   comp*.sch   - Schematics for various kinds of components
   plus various utility schematics
   plus any Capfast symbols missing from the general EPICS release

in the "include" subdirectory:
   cicsConst.h - CICS constants
   cicsLib.h   - CICS library function definitions

in the "src" subdirectory:
   cicsLib     - CICS utility library
   sysCad      - General library for handling sequence commands
   compCad     - Library of CAD functions common to all components
   compcCad    - Library of CAD functions common to discrete components
   compcCad    - Library of CAD functions common to continuous components
   cicsCcSt    - Master sequences for Components Controllers
   compmSt - Sequences for one axis components
