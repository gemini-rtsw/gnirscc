/*
 *  Priorities (inspired by syslog)
 */
#define	GEMLOG_EMERG	0	/* system is unusable */
#define	GEMLOG_ALERT	1	/* action must be taken immediately */
#define	GEMLOG_CRIT		2	/* critical conditions */
#define	GEMLOG_ERR		3	/* error conditions */
#define	GEMLOG_WARNING	4	/* warning conditions */
#define	GEMLOG_NOTICE	5	/* normal but signification condition */
#define	GEMLOG_INFO		6	/* informational */
#define	GEMLOG_DEBUG	7	/* debug-level messages */

int gemLogMsg(int level, const char *pFormat, ...);
void gemSetLogLevel(int level);
void gemSetUseiocLog(int state);

