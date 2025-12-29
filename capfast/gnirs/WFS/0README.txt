This UAE application directory contains the files for the Gemini Near
Infared Imager (NIRI) software, together with an adapted version of the
Core Instrument Control System (CICS).

The simulator for the Gemini NOAO Advanced Array Controller (GNAAC)
is included here, but is not compatible with the current release.

Authors: Hubert Yamada, Institute for Astronomy, Honolulu, Hawaii.
         Steven Beard, Royal Observatory, Edinburgh, Scotland.

The top level directory contains the NIRI Instrument Sequencer plus the
following subdirectories, which are themselves UAE application trees: 

CC      - NIRI Components Controller, adapted from the CICS
WFS     - NIRI On-instrument wavefront sensor, adapted from the CICS
ENG     - NIRI Motor Control parts of the Component Controller
IS      - NIRI Instrument Sequencer
DC      - Interface to the GNAAC Detector Controller
LIB     - Files shared in common between all the other directories

Important scripts are:

niriLogin  - Define essential environment variables.
niriSetup  - Set up the NIRI UAE application directories.

Warning: DO NOT run 'make clean' in this directory tree, because that
will destroy some important files.  Use 'make reset' instead.

Release notes for this system are available in the docs directory, as
a FrameMaker file:

	docs/niri_hty_006.fm

The NIRI documents (including the release notes) are available in the
docs directory as PostScript file:

	docs/niri_hty.ps
