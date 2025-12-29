#if !defined(TESTS_H)
#define TESTS_H

typedef struct test_ {
	const char *msg;
	const char *rpy;
} Test;

Test tests[] = {
	{ "test\r\n", "foobarbazbletch\r\n" },
	{ "alttest\r\n", "somethingsomething\r\n" },
	{ "test3\r\n", "response 3\r\n" },
};

#define NTESTS (sizeof(tests) / sizeof(*tests))

#endif /* TESTS_H */
