#include <intLib.h>
#include <stdio.h>
#include <iv.h>

int
showInts()
{
	FUNCPTR *f;
	int i;
	
	for (f = 0, i = 0; i < 256; i++, f++) {
		printf("0x%.2X %p\n", i, intVecGet((FUNCPTR *)INUM_TO_IVEC(i)));
	}

	return 0;
}
