#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "tests.h"

#if !defined(BAUD)
#define BAUD 9600
#endif

#define PASTE(a,b) a##b
#define BAUD_FLAG(baud) PASTE(B,baud)

#if !defined(DEBUG)
#define DEBUG 0
#endif

int
main()
{
	char *cp;
	int fd = open("/dev/term/a", O_RDWR);
	struct termios term;
	char buf[1024];
	int i;

	tcgetattr(fd, &term);

	term.c_iflag = IGNBRK | INPCK; 
	term.c_oflag = 0;
	term.c_lflag = 0;

	term.c_cflag = BAUD_FLAG(BAUD) | CLOCAL | CS7 | CREAD | CSTOPB | PARENB
		| PARODD;

	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;

	tcsetattr(fd, TCSANOW, &term);

	for (;;) {
#if DEBUG > 1
		printf("Reading\n");
#endif
		cp = buf;
		for (;;) {
			if (read(fd, cp, 1) < 0) {
				perror("read");
			} else if (*cp == '\n') {
				cp++;
				break;
			} else {
				cp++;
			}
		}
		*cp++ = '\0';

		for (i = 0; i < NTESTS; i++) {
			if (strcmp(buf, tests[i].msg) == 0)
				break;
		}

		if (i == NTESTS) {
			fprintf(stderr, "Unrecognized message: \"%s\"\n", buf);
		} else {
#if DEBUG > 1
			printf("Read: \"%s\"\n", buf);
			printf("Write: \"%s\"\n", tests[i].rpy);
#endif
			write(fd, tests[i].rpy, strlen(tests[i].rpy));
		}
	}

	return 0;
}
