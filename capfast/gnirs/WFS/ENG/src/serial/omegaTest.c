#include <stdio.h>
#include <time.h>

#include "xy490.h"

#if !defined(TC) && !defined(TS)
#define TC
#endif

#if !defined(FD)
#define FD (0)
#endif

int
omegaTest()
{
	char buf[1024];
	int ret = 0;
	struct timespec ts;

#if defined(TS) 
	ret = xy490SetBaud(FD, 300);
	printf("SetBaud: %d\n", ret);

	if (ret != -1) {
		ret = xy490Write(FD, "WS\r\n", 4);
		printf("Write: %d\n", ret);
	}
#elif defined(TC)
	ret = xy490SetBaud(FD, 1200);
	printf("SetBaud: %d\n", ret);

	if (ret != -1) {
		ret = xy490Write(FD, "SETP 30.0\r\n", 11);
		printf("Write: %d\n", ret);
	}

	if (ret != -1) {
		ts.tv_sec = 1;
		ts.tv_nsec = 0;
		nanosleep(&ts, NULL);

		ret = xy490Write(FD, "CDAT?\r\n", 7);
		printf("Write: %d\n", ret);
	}
#else
#	error TC or TS must be defined.
#endif

	if (ret != -1) {
		ret = xy490Read(FD, buf, sizeof(buf));
		printf("Read: %d\n", ret);
	}

	if (ret != -1)
		printf("%s\n", buf);

	return 0;
}
