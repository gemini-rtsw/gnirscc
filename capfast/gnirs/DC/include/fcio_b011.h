/*
 * b011.h
 *
 * description of b011 registers and memory mapping
 *
 *$Id: fcio_b011.h,v 1.2 2009/05/27 19:32:27 fkraemer Exp $
 *
 *
 *$Log: fcio_b011.h,v $
 *Revision 1.2  2009/05/27 19:32:27  fkraemer
 *fkraemer - copied my complete working dir over trunk
 *
 *Revision 1.1.1.1  1998/12/15 16:18:20  buchholz
 *Imported gnaacSrc into CVS
 *
 * Revision 1.0  1994/05/17  17:59:54  quenten
 * Initial revision
 *
 */

#define DEVICE_FILE	"/dev/ptvme32d32"
#if BIG_B016
#define MEM_ON_BOARD	(16 * 1024 * 1024)	/* 16M of memory (in bytes) */
#else
#define MEM_ON_BOARD	(4 * 1024 * 1024)	/* 4M of memory (in bytes) */
#endif

#define PAGE_SIZE	getpagesize();

#define ADDR_SIZE	MEM_ON_BOARD
#if 1
#define DEFAULT_BASE	0x8000000
#else
#define DEFAULT_BASE	0x400000
#endif

struct b011_map {
	unsigned char	tadpole_mem[MEM_ON_BOARD];
};



