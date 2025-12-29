# $Id: Makefile,v 1.2 2009/05/27 19:35:04 fkraemer Exp $ 

GS=gs

FMSRC=\
	niri_hty_001.fm \
	niri_hty_002.fm \
	niri_hty_003.fm \
	niri_hty_004.fm \
	niri_hty_005.fm \
	niri_hty_006.fm \
	niri_hty_007.fm \
	niri_hty_008.fm \
	niri_hty_009.fm \
	niri_hty_010.fm \
	niri_hty_011.fm \
	niri_hty_012.fm \
	niri_hty_015.fm \
	niri_hty_016.fm

BOOKSRC=niri_hty.book oiwfs_hty.book

PS=${FMSRC:.fm=.ps} ${BOOKSRC:.book=.ps} ${BOOKSRC:.book=_TOC.ps}
PDF=${PS:.ps=.pdf}

%.pdf: %.ps
	${GS} -q -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile=$*.pdf $*.ps

all: ${PDF}
