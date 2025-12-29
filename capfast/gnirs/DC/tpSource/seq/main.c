static char rcid[]="$Id: main.c,v 1.2 2009/05/27 19:33:28 fkraemer Exp $";
/******************************************************************************
 * Program:	dspw
 * File:	main.c
 * Purpose:	to allocate everything, setup vars, and start processes
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		19Nov91	created						dak
 *		23Dec91	all finished					dak
 *
 *****************************************************************************/
#include <conc.h>
#include "common.h"
#include "seq_defs.h"
#define MAIN
#include "seq_vars.h"
#include "seqhdw.h"

extern void *_heapend;

main()
{
	_heapend = (int *) 0x80000000;
	addfree((void *) (0x80014000), 64 * 1024);

	mem_init();				/* initialize all memory */

	Reader_to_Writer = ChanAlloc();
	Control_to_Reader = ChanAlloc();
	Writer_to_Control = ChanAlloc();

	chan_list[0] = Control_to_Reader;	/* must copy these in by */
	chan_list[1] = (Channel *) &Seq_to_Reader;
	chan_list[5] = Reader_to_Writer;

	Seq_to_Reader = NOPROCESS;
	Control_to_Seq = NOPROCESS;

	Preader = ProcAlloc(reader, 4096, 0);
	Pwriter = ProcAlloc(writer, 4096, 0);

	ProcRun(Preader);
	ProcRun(Pwriter);

	control();				/* turn this process into the
						   controller */
}
