#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#if !defined(DEBUG)
#define DEBUG 0
#endif

#if !defined(TS) && !defined(TC)
#define TC
#endif

int
main()
{
	char *cp;
	int fd = open("/dev/term/a", O_RDWR);
	struct termios term;
	char buf[1024];
#if defined(TS)
	const char *const str = "WS\r\n";
#elif defined(TC)
	const char *const str = "CDAT?\r\n";
#else
#error TS or TC must be defined
#endif
	const int ssz = strlen(str);

	tcgetattr(fd, &term);

	term.c_iflag = IGNBRK | INPCK; 
	term.c_oflag = 0;
	term.c_lflag = 0;

#if defined(TS)
	term.c_cflag = B300 | CLOCAL | CS7 | CREAD | CSTOPB | PARENB
		| PARODD;
#elif defined(TC)
	term.c_cflag = B1200 | CLOCAL | CS7 | CREAD | CSTOPB | PARENB | PARODD;
#else
#error TS or TC must be defined
#endif

	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;

	tcsetattr(fd, TCSANOW, &term);

	for (;;) {
		write(fd, str, ssz);

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

#if DEBUG > 1
		printf("Read: \"%s\"\n", buf);
		printf("Write: \"%s\"\n", str);
#endif
	}

	return 0;
}
