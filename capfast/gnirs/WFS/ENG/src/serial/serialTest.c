#include <vxWorks.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <logLib.h>

#include "tests.h"
#include "xy490.h"

#if !defined(BAUD)
#define BAUD 9600
#endif

#if !defined(FD)
#define FD (0)
#endif

static int test(int);

int
serialTest()
{
	xy490SetBaud(FD, BAUD);
	return test(FD);
}

static int
test(int channel)
{
	char dest[64];
	int tries = 0;
	int successes = 0;
	int errors = 0;
	struct timespec start;
	int err;
	struct timespec ts;
	int i;

	clock_gettime(CLOCK_REALTIME, &start);

	for (i = 0;; i = (i + 1) % NTESTS) {
#if DEBUG > 1
		printf("Write \"%s\"\n", tests[i].msg);
#endif
		xy490Write(channel, tests[i].msg, strlen(tests[i].msg));

		memset(dest, '\0', sizeof(dest));

		err = xy490Read(channel, dest, sizeof(dest));
#if DEBUG > 1
		printf("Read: \"%s\"\n", dest);
#endif

		tries++;
		if (err < 0) {
			printf("Read error or timeout\n");
			errors++;

			ts.tv_sec = 1;
			ts.tv_nsec = 0;
			nanosleep(&ts, NULL);
			xy490FlushInput(channel);
		} else if (strcmp(tests[i].rpy, (char *)dest) == 0) {
#if DEBUG > 1
			fprintf(stderr, "Succeeded: \"%s\"\n", dest);
#endif
			successes++;
		} else {
			printf("Read failed (got \"%s\", expected \"%s\")\n", 
				dest, tests[i].rpy);

			ts.tv_sec = 1;
			ts.tv_nsec = 0;
			nanosleep(&ts, NULL);
			fflush(stdout);
			xy490FlushInput(channel);
		}
			
		if (tries % 20 == 0) {
			struct timespec now;
			double elapsed;

			clock_gettime(CLOCK_REALTIME, &now);

			elapsed = now.tv_sec - start.tv_sec
				+ (now.tv_nsec - start.tv_nsec) * 1.0E-9;

			printf("%d successes %d errors in %d tries (%.2f seconds)\n",
				successes, errors, tries, elapsed);

			clock_gettime(CLOCK_REALTIME, &start);
		}
	}

	return 0;
}
