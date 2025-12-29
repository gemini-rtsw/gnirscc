/* $Id: xy490.c,v 1.2 2009/05/27 19:34:47 fkraemer Exp $ */

/*
 * Author:  Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 * This module provides the software to use the XYCOM XVME-490 Serial
 * I/O module under VxWorks.  The XVME-490 provides 4 RS-232 serial I/O
 * channels.  Specifically, it provides two Zilog Z8530 SCC chips,
 * each of which provides two serial RS-232 channels.  For more
 * details, on this hardware, see the XVME-400/401/490/491 user's manual
 * (XYCOM, 750 North Maple Road, Saline, Michigan, 48176, (303)429-4971)
 * and the SCC User's manual -- Z8030, Z80C30, Z80230, Z8530, Z85C30,
 * Z85230, and Z85233 (Zilog).
 *
 * This module really should be rewritten as a tyLib module for 
 * general purpose use.  The functions provided here are not 
 * sufficient for all applications but are sufficient for the needs 
 * of the temperature controllers and sensors of the Gemini NIRI
 * instrument.
 */

#include "xy490.h"

#include <vme.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include <intLib.h>
#include <logLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <iv.h>
#include <string.h>
#include <math.h>
#include <devLib.h>
extern long locationProbe(int, void *);

#define DEBUG 1 

#if !defined(DEBUG)
#	define DEBUG 0
#endif

/*#define MUTEXDEBUG */

#define IO_PRIORITY (1) /* 0-255, 0 is the highest */

/*
 * How many seconds (rounded to integer number of clock ticks) to wait
 * before assuming that the write interrupt was lost?
 *
 * This should not be necessary, but it seems like the write interrupt
 * gets lost with apalling regularity.  Maybe there is an error in
 * the way that I'm handling the semaphore.
 */

#define WRITE_TIMEOUT_SEC (0.1)

/*
 * How long before a read times out (seconds, rounded to integer number
 * of clock ticks).
 */

#define READ_TIMEOUT_SEC (1.0) 

/*
 * How often to check for a lost read interrupt (seconds, rounded to
 * integer number of clock ticks)?
 */

#define READ_LOST_INT_SEC (10.0)

/*
 * If the write register is busy, how long to wait before retrying
 * the write operation (seconds, rounded to nanoseconds)?
 */

#define WRITE_RETRY_SEC (0.1)

#define SCC1  (8) /* SCC #1 base address (relative to card base address) */
#define SCC2  (0) /* SCC #1 base address (relative to card base address) */
#define B     (0) /* SCC B base address (relative to SCC base address) */
#define A     (4) /* SCC A base address (relative to SCC base address) */

#define CONTROL (1) /* Offset of control register */
#define DATA    (3) /* Offset of data register */

#define TIMER_CONST_19200 (4)   /* time constant for 19200 baud */
#define TIMER_CONST_9600  (10)  /* time constant for 9600 baud */
#define TIMER_CONST_2400  (46)  /* time constant for 2400 baud */
#define TIMER_CONST_1200  (94)  /* time constant for 1200 baud */
#define TIMER_CONST_300   (382) /* time constant for 300 baud */

#define TIMER_CONST_DEFAULT (TIMER_CONST_9600)

#define MAX_BOARDS     (2) /* Maximum number of boards */
#define SCC_PER_BOARD  (2) /* SCC's per board */
#define CHAN_PER_SCC   (2) /* Channels per SCC */
#define CHAN_PER_BOARD (CHAN_PER_SCC * SCC_PER_BOARD) /* Channels per board */

#define READ_BUF_SIZE (128)

typedef struct channel_ {
	volatile u_char *pRegs;
	volatile u_char *pScc;

	struct {
		SEM_ID semMutex; /* Protect struct from simultaneous access */
		SEM_ID semRead; /* Wait for an available character */
		SEM_ID semBlock; /* Wait for an interrupt */
		volatile u_char buf[READ_BUF_SIZE];
		volatile u_char *pStart;
		volatile u_char *pEnd;
		volatile int fifoOverflow;
	} read;

	struct {
		SEM_ID semMutex; /* Protect struct from simultaneous access */
		SEM_ID semBlock; /* Wait until a character can be written */
	} write;

	const char *error;
} Channel;

/*
 * Channels are:
 *
 * 0: Board 0, SCC 1, channel A
 * 1: Board 0, SCC 1, channel B
 * 2: Board 0, SCC 2, channel A
 * 3: Board 0, SCC 2, channel B
 * 4: Board 1, SCC 1, channel A
 * 5: Board 1, SCC 1, channel B
 * 6: Board 1, SCC 2, channel A
 * 7: Board 1, SCC 2, channel B
 */

static Channel channels[MAX_BOARDS * CHAN_PER_BOARD];

/*
 * Define for return test on locationProbe()
 *
 * Taken from the motor record OMS stepper motor driver
 */

#define PROBE_SUCCESS(STATUS) ((STATUS)==S_dev_addressOverlap)

static void chan_init(int, int, volatile u_char *);
static void scc_init(int, volatile u_char *, u_int);
static void handler(int);
static int char_available(int);
static int push_char(int, char);

#if defined(MUTEXDEBUG)
	static STATUS unlock(SEM_ID, const int, const int);
	static STATUS lock(SEM_ID, int, const int, const int);

	#define LOCK(sid,timeout) (lock((sid), (timeout), channel, __LINE__))
	#define UNLOCK(sid) (unlock((sid), channel, __LINE__))
#else
	#define LOCK(sid,timeout) semTake((sid),(timeout))
	#define UNLOCK(sid) semGive((sid))
#endif

/*
 * Author:
 *
 *     Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 * Purpose: 
 *
 *     This routine initializes the XYCOM XVME-490 Quad RS-232 I/O
 *     module, initializes registers, and installs interrupt handlers.
 *
 * Arguments:
 *
 *     (>) sbase  (int)  Base address (in the VME short address space) of the
 *                       card.
 *
 *     (>) vector (int)  Which interrupt vector to use.  'Vector' will be
 *                       used for SCC1 (both channels) and 'Vector + 1'
 *                       will be used for SCC2 (both channels).
 *
 *     (>) baud   (int)  Baud rate (300, 1200, 2400, 9600, or 19200)
 *
 * Return value:
 *    
 *     Returns a OK if successful, and ERROR if there is
 *     an error.
 *
 * Limitations:
 *
 *     An invalid base address, an illegal vector, or a vector which
 *     is already in use will cause unpredictable behavior of the system.
 *
 *     The results are unpredictable if this is called twice with the
 *     same board number.
 */

int
xy490Init(int board, unsigned sbase, unsigned vector)
{
	volatile u_char *pBoard;
	int status = OK;
	long probeStatus;

	if (status != ERROR && board >= MAX_BOARDS) {
		fprintf(stderr, __FILE__ "(%d): xy490Init: Illegal board number\n",
			__LINE__);
		status = ERROR;
	}

	if (status != ERROR) {
		probeStatus = locationProbe(atVMEA16, (char *)sbase);
		if (!PROBE_SUCCESS(probeStatus)) {
			fprintf(stderr,
				__FILE__ "(%d): xy490Init: Illegal board address (%u)\n",
				__LINE__, sbase);
			status = ERROR;
		}
	}

	if (status != ERROR) {
		status = sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char *)sbase, 
			(char **)&pBoard);
		if (status != OK) {
			fprintf(stderr,
				__FILE__ "(%d): xy490Init: Failed to map XY490 adr (%.4X)\n",
				__LINE__, sbase);
			status = ERROR;
		}
	}

	if (status != ERROR) {
		scc_init(board * CHAN_PER_BOARD, pBoard + SCC1, vector);

		scc_init(board * CHAN_PER_BOARD + CHAN_PER_SCC, pBoard + SCC2, 
			vector + 1);
	}

	return status;
}

/*
 * Output a null-terminated string.
 *
 * The xy490 is set up in an interrupt-driven write mode.  As soon
 * as a single character is written to a data register, the board
 * will start generating interrupts.  It generates an interrupt
 * each time that is has finished sending the current character,
 * and is ready to send the next, until the interrupts are suspended
 * (not disabled!).
 *
 * Prerequsites:
 *
 *     The board must previously have been initialized by xy490Init(). 
 *
 * Limitations:
 *
 *     Attempts to call this function twice, simultaneously, will
 *     have unpredictable results (probably one call will hang).
 */

int
xy490WriteString(int channel, const char *pString)
{
	Channel *const pC = &channels[channel];
	int status = OK;
	int old_priority;
	int size;
	const int timeout = (int)ceil(sysClkRateGet() * WRITE_TIMEOUT_SEC);

#if DEBUG > 1
	{
		const char *cp;
		fprintf(stderr, __FILE__ "(%d): xy490WriteString(%d,\"",
			__LINE__, channel);
		for (cp = pString; *cp != '\0'; cp++) {
			if (*cp == '\r')
				fputs("\\r", stderr);
			else if (*cp == '\n')
				fputs("\\n", stderr);
			if (isascii(*cp) && isprint(*cp))
				putc(*cp, stderr);
			else
				fprintf(stderr, "\\%03o", *cp);
		}
		fprintf(stderr, "\")\n");
	}
#endif

	/*
	 * Lock necessary data structures.  There is no need to queue 
	 * multiple write statements, so we just fail immediately, if
	 * there is a problem.
	 */

	if (semTake(pC->write.semMutex, timeout) == ERROR) {
		pC->error = "Write in progress";
		fprintf(stderr, __FILE__ "(%d): Write in progress\n", __LINE__);
		xy490PrintfKludge();
		status = ERROR;
	} else {
		if (status != ERROR) {
			taskPriorityGet(taskIdSelf(), &old_priority);
			taskPrioritySet(taskIdSelf(), IO_PRIORITY);
		}

		size = (pString == NULL) ? 0 : strlen(pString);
		if (status == OK && size > 0) {
			/*
			 * The previous write command may not have completed.  If the
			 * write register is busy, wait long enough for the last
			 * character to be transmitted.
			 */
			
			if ((pC->pRegs[CONTROL] & 0x4) == 0) { 
				struct timespec ts;

				ts.tv_sec = WRITE_RETRY_SEC;
				ts.tv_nsec = (WRITE_RETRY_SEC - (double)ts.tv_sec) * 1.0E9;
				nanosleep(&ts, NULL);
			}

			if ((pC->pRegs[CONTROL] & 0x4) == 0) { 
				/* Write register is busy:  should never happen */
#if DEBUG > 0
				fprintf(stderr, __FILE__ "(%d):  Write register is busy\n",
					__LINE__);
#endif

				pC->error = "Write register is busy";
				status = ERROR;
			} else { /* Ready to write */
#if DEBUG > 1
				fprintf(stderr, __FILE__ "(%d): Start writing\n", __LINE__);
#endif

				pC->pRegs[DATA] = *pString++; /* Start the write operation */
				size--;

				while (status != ERROR && size > 0) {
					/*
					 * Wait for the interrupt that indicates that the
					 * output register is empty and can be written to.
					 */

					if (semTake(pC->write.semBlock, timeout) == ERROR
							&& (pC->pRegs[CONTROL] & 0x4) == 0) {
						pC->error = "Write blocked";
						status = ERROR;
					}

					/*
					 * Write the next character.
					 */

					if (status != ERROR) {
						pC->pRegs[DATA] = *pString++;
						size--;
					}

					/*
					 * Clear the interrupt so that further interrupt processing
					 * can take place.
					 */

					pC->pRegs[CONTROL] = 0;
					pC->pRegs[CONTROL] = 0x38;
				}

				/*
				 * Wait for the interrupt that shows that the character
				 * was successfully written.
				 */

				if (semTake(pC->write.semBlock, timeout) == ERROR
						&& (pC->pRegs[CONTROL] & 0x4) == 0) {
					pC->error = "Final write blocked";
					status = ERROR;
				}

				/*
				 * We're done.  Stop generating interrupts.
				 */

				pC->pRegs[CONTROL] = 0;
				pC->pRegs[CONTROL] = 0x28;

				/*
				 * Clear the interrupt so that further interrupt processing
				 * can take place.
				 */

				pC->pRegs[CONTROL] = 0;
				pC->pRegs[CONTROL] = 0x38;
			}
		}

		taskPrioritySet(taskIdSelf(), old_priority);

		if (semGive(pC->write.semMutex) == ERROR)
			fprintf(stderr, __FILE__ "(%d): Could not give mutex\n", __LINE__);
	}

#if DEBUG > 2
	fprintf(stderr, __FILE__ "(%d): ocycSet() return %ld\n", __LINE__, status);
#endif

	return status;
}

/*
 * TODO:  This routine, when called from the Ocyc driver, seems to
 * do something strange, that causes printf to stop working.
 */

int
xy490Read(int channel, char *const pBuf, size_t bufSz)
{
	Channel *const pC = &channels[channel];
	char *pDest = pBuf;
	int destSz = bufSz;
	char ch;
	int status = OK;
	const int timeout = (int)ceil(sysClkRateGet() * READ_TIMEOUT_SEC);
	int charsRead = 0;
	int mustWait;

	/*
	 * Sanity check arguments.
	 */

	if (destSz < 1) {
		pC->error = "Invalid buffer size";
		status = ERROR;
	} else {
		destSz--; /* Allocate space for terminating \0 */
	}

	/*
	 * If the fifo overflowed, the data is useless, so don't try to 
	 * process it.
	 */

	if (status != ERROR) {
		if (LOCK(pC->read.semMutex, timeout) == ERROR) {
			pC->error = "Could not take mutex";
			status = ERROR;
		} else {
			if (pC->read.fifoOverflow) {
				pC->error = "FIFO overflow";
				status = ERROR;
			}

			if (UNLOCK(pC->read.semMutex) == ERROR) {
				fprintf(stderr, __FILE__ "(%d): Could not give mutex\n",
					__LINE__);
			}
		}
	}

	while (status != ERROR) {
		/* See if the buffer is empty */

		if (LOCK(pC->read.semMutex, timeout) == ERROR) {
			pC->error = "Could not take mutex";
			status = ERROR;
		} else {
			if (pC->read.pStart == pC->read.pEnd)
				mustWait = 1;
			else
				mustWait = 0;
			if (UNLOCK(pC->read.semMutex) == ERROR) {
				fprintf(stderr, __FILE__ "(%d): Could not give mutex\n",
					__LINE__);
			}
		}

		/*
		 * Block until a character is available
		 */

		if (!status && mustWait) {
			status = semTake(pC->read.semRead, timeout);
			if (status)
				pC->error = "Character not available";
		}

		/*
		 * 
		 * Copy one character
		 *
		 * It should be impossible to get here if the buffer, is
		 * empty, but it sometimes happens.  It seems to be harmless,
		 * so just ignore it.
		 */

		if (status != ERROR) {
			if (LOCK(pC->read.semMutex, timeout) == ERROR) {
				status = ERROR;
				pC->error = "Mutex not available";
			} else {
				int fifoOverflow;

				if (pC->read.pStart != pC->read.pEnd) {
					ch = *pC->read.pStart++;
					if (pC->read.pStart == pC->read.buf + READ_BUF_SIZE)
						pC->read.pStart = pC->read.buf;

					if (destSz > 0)
						*pDest++ = ch;

					destSz--;
					charsRead++;
				}

				fifoOverflow = pC->read.fifoOverflow;

				if (UNLOCK(pC->read.semMutex) == ERROR) {
					fprintf(stderr, __FILE__ "(%d): Could not give mutex\n",
						__LINE__);
				}

				if (fifoOverflow) {
					*pBuf = '\0';
					pC->error = "FIFO overflow";
					status = ERROR;
					break;
				} else if (ch == '\n') {
					break;
				}
			}
		}
	}

	if (status != ERROR && destSz < 0) {
		status = ERROR;
		pC->error = "Buffer overflow";
	}

	if (status != ERROR)
		*pDest++ = '\0';
	else if (bufSz > 0)
		*pBuf = '\0';

	return (status == ERROR) ? -1 : charsRead;
}

static void
scc_init(int first_channel, volatile u_char *pScc, u_int basevec)
{
	/* Add an interrupt handler */

	intConnect((VOIDFUNCPTR *)INUM_TO_IVEC(basevec),
		(VOIDFUNCPTR)handler, first_channel);

	/*
	 * Reset the hardware and set the interrupt vector.
	 */

	(pScc + A)[CONTROL] = 9;    /* Set WR9 */
	(pScc + A)[CONTROL] = 0x40; /* Reset channel A */

	(pScc + A)[CONTROL] = 9;    /* Set WR9 */
	(pScc + A)[CONTROL] = 0x80; /* Reset channel B */

	(pScc + A)[CONTROL] = 9;    /* Set WR9: status low, MIE, VIS set */
	(pScc + A)[CONTROL] = 0x08; /* DCL=0, IACK vector variable */

    (pScc + A)[CONTROL] = 2;    /* Set WR2 */
	(pScc + A)[CONTROL] = basevec;  /* IACK vector */

	chan_init(first_channel, 0, pScc);
	chan_init(first_channel, 1, pScc);
}

static void
chan_init(int first_channel, int channel, volatile u_char *pScc)
{
	Channel *const pC = &channels[first_channel + channel];
	volatile u_char *const pRegs = pScc + ((channel == 0) ? A : B);
	char buf[1024];
	static int count = 0;

	pC->pRegs = pRegs;
	pC->pScc = pScc;
	pC->error = NULL;

	pC->read.semRead = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	pC->read.semBlock = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	pC->read.semMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
	pC->read.pStart = pC->read.buf;
	pC->read.pEnd = pC->read.buf;
	pC->read.fifoOverflow = FALSE;
	sprintf(buf, "xy490RdTsk%d", count++);
	taskSpawn(buf, IO_PRIORITY, VX_FP_TASK, 8000, (FUNCPTR)char_available,
		first_channel + channel, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	pC->write.semBlock = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	pC->write.semMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

	xy490Reinit(first_channel + channel);
}

void
xy490Reinit(int channel)
{
	volatile u_char *const pRegs = channels[channel].pRegs;

#define WR4_PARITY_ENABLE (0x01)
#define WR4_PARITY_EVEN   (0x02)
#define WR4_STOPBITS_1    (0x04)
#define WR4_CLOCK_X16     (0x40)

	/* Set WR4: X16 clock, 1 stop bits, Odd parity */

	pRegs[CONTROL] = 4;
	pRegs[CONTROL] = WR4_STOPBITS_1 | WR4_PARITY_ENABLE | WR4_CLOCK_X16;

#define WR3_RXBITS_8     (0xC0)
#define WR3_RXBITS_7     (0x40)
#define WR3_RX_ENABLE    (0x01)

#define WR3_MODE         (WR3_RXBITS_7)

	/* Set WR3: 7 RX bits, No auto enable, RX disabled */

	pRegs[CONTROL] = 3;           
	pRegs[CONTROL] = WR3_MODE; 

	/* Set WR3: 7 TX bits, DTR and RTS asserted, TX disabled */

#define WR5_TXBITS_7     (0x20)
#define WR5_TXBITS_8     (0x60)
#define WR5_DTR          (0x80)
#define WR5_RTS          (0x02)
#define WR5_TX_ENABLE    (0x08)

#define WR5_MODE        (WR5_TXBITS_7 | WR5_DTR | WR5_RTS)

	pRegs[CONTROL] = 5;
	pRegs[CONTROL] = WR5_MODE;

	pRegs[CONTROL] = 1;    /* Set WR1: DMA/WAIT pins */
	pRegs[CONTROL] = 0x40; /* Set RX, TX, ext. int. disabled */

	pRegs[CONTROL] = 10;   /* Set WR10 to NRZ */
	pRegs[CONTROL] = 0;

	pRegs[CONTROL] = 11;   /* Set WR11: no XTAL */
	pRegs[CONTROL] = 0x56; /* RX, TX clock=BRG
	                        * TRXC=BRG */

	pRegs[CONTROL] = 13;   /* Set WR13: High order time constant */	
	pRegs[CONTROL] = (TIMER_CONST_DEFAULT >> 8) & 0xFF;

	pRegs[CONTROL] = 12;   /* Set WR12: Low order time constant */
	pRegs[CONTROL] = TIMER_CONST_DEFAULT & 0xFF;

	pRegs[CONTROL] = 14;   /* Set WR14: */
	pRegs[CONTROL] = 2;    /* BRG source = PCLK */
	pRegs[CONTROL] = 14;
	pRegs[CONTROL] = 3;    /* Enable BRG */

	pRegs[CONTROL] = 15;   /* Set WR15: Disable all external interrupts */
	pRegs[CONTROL] = 0;

	pRegs[CONTROL] = 3;    /* Enable receiver */
	pRegs[CONTROL] = WR3_MODE | WR3_RX_ENABLE;

	pRegs[CONTROL] = 5;    /* Enable transmitter */
	pRegs[CONTROL] = WR5_MODE | WR5_TX_ENABLE;

	/*
	 *  Enable Interrupts.  This must be done after all other
	 *  initialization is complete.
	 */

	pRegs[CONTROL] = 1;    /* Set WR1: DMA/WAIT pins */
	pRegs[CONTROL] = 0x52; /* Set RX, TX enabled; ext. int. disabled */
}

/* 
 * Limitations:  Only certain baud rates are legal.
 */

int
xy490SetBaud(int channel, int baud)
{
	int status = OK;
	unsigned timer_const = TIMER_CONST_9600;
	Channel *const pC = &channels[channel];

	if (status == OK) {
		switch (baud) {
		case 300:
			timer_const = TIMER_CONST_300;
			break;

		case 1200:
			timer_const = TIMER_CONST_1200;
			break;

		case 2400:
			timer_const = TIMER_CONST_2400;
			break;

		case 9600:
			timer_const = TIMER_CONST_9600;
			break;

		case 19200:
			timer_const = TIMER_CONST_19200;
			break;

		default:
#if DEBUG > 0
			fprintf(stderr, __FILE__ "(%d):  Invalid baud rate (%d)\n",
				__LINE__, baud);
#endif
			pC->error = "Invalid baud rate";
			status = ERROR;
		}
	}

	if (status == OK) {
		pC->pRegs[CONTROL] = 13;   /* Set WR13: High order time constant */	
		pC->pRegs[CONTROL] = (timer_const >> 8) & 0xFF;

		pC->pRegs[CONTROL] = 12;   /* Set WR12: Low order time constant */
		pC->pRegs[CONTROL] = timer_const & 0xFF;
	}

	return status;
}

/*
 * Warning: This is an interrupt handler.  Use only functions that are 
 * safe for use within an interrupt handler.  (In particular, use
 * logMsg, not printf.)
 */

static void
handler(int first_channel)
{
	Channel *pC = &channels[first_channel];
	int channel = first_channel;
	int vec;
	int r1;
	int r3;

	(pC->pScc + B)[CONTROL] = 2;
	vec = (pC->pScc + B)[CONTROL];

	if (!(vec & 0x08)) { /* Which SCC channel? */
		channel++;
		pC = &channels[channel];
	}

	(pC->pScc + A)[CONTROL] = 1;
	r1 = (pC->pScc + A)[CONTROL];

	(pC->pScc + A)[CONTROL] = 3;
	r3 = (pC->pScc + A)[CONTROL];

	switch (vec & 0x06) {
	case 0: /* Transmit buffer empty */
#if DEBUG > 2
		logMsg(__FILE__ "(%d): handler(%d) vec=%p 0x%.2X 0x%.2X XmitInt(%d)\n",
			__LINE__, first_channel, vec, r1, r3, channel);
#endif
		if (semGive(pC->write.semBlock) == ERROR) { /* Wake the write routine */
			logMsg(__FILE__ "(%d): Could not give write semaphore\n",
				__LINE__, 0, 0, 0, 0, 0);
		}
		break;

	case 2: /* External status change (Should never occur) */
#if DEBUG > 2
		logMsg(__FILE__ "(%d): handler(%d) vec=%p 0x%.2X 0x%.2X StatInt(%d)\n",
			__LINE__, first_channel, vec, r1, r3, channel);
#endif
		break;
		
	case 4: /* Character arrived */
#if DEBUG > 2
		logMsg(__FILE__ "(%d): handler(%d) vec=%p 0x%.2X 0x%.2X RecvInt(%d)\n",
			__LINE__, first_channel, vec, r1, r3, channel);
#endif
		if (semGive(pC->read.semBlock) == ERROR) { /* Wake the helper routine */
			logMsg(__FILE__ "(%d): Could not give read semaphore\n",
				__LINE__, 0, 0, 0, 0, 0);
		}
		break;

	case 6: /* Special condition */
#if DEBUG > 2
		logMsg(__FILE__ "(%d): handler(%d) vec=%p 0x%.2X 0x%.2X SpecInt(%d)\n",
			__LINE__, first_channel, vec, r1, r3, channel);
#endif
		/*
		 * There is no need to process this, because the read routine
		 * checks for an error.   Clear the error and the interrupt so 
		 * that further interrupt processing can take place.
		 */

		pC->pRegs[CONTROL] = 0;
		pC->pRegs[CONTROL] = 0x30;

		pC->pRegs[CONTROL] = 0;
		pC->pRegs[CONTROL] = 0x38;

		break;
	}
}

/*
 * This is a helper routine, spawned as a separate thread, to help
 * the interrupt handler.  The main reason that the functionality
 * of this routine cannot be implemented directly within the 
 * interrupt handler is that an interrupt handler is limited
 * in what it can do with semaphores.
 *
 * Restrictions:
 *
 *     Be careful of stack usage.  This has only a small, private stack.
 *
 *     This should be spawned as a separate task, exactly once per channel.
 */

static int
char_available(int channel)
{
	Channel *const pC = &channels[channel];
	const int timeout = (int)ceil(sysClkRateGet() * READ_TIMEOUT_SEC);
	const int lostInt = (int)ceil(sysClkRateGet() * READ_LOST_INT_SEC);

	for (;;) {
		/*
		 * Wait for the interrupt handler to notify us that a character
		 * is ready to be read.  Interrupts appear to get lost once in
		 * a while, so we set a timeout, to make sure that the input
		 * stream doesn't get hung at this point.
		 */

		if (semTake(pC->read.semBlock, lostInt) == ERROR) {
			if ((pC->pRegs[CONTROL] & 0x1) == 0)
				continue;
#if DEBUG > 0
			fprintf(stderr, __FILE__ "(%d): Lost interrupt?\n",
				__LINE__);
#endif
		}

		/*
		 * There's not much that we can do if push_char fails,
		 * so we just let it print a warning message, and let
		 * the character drop.  It should only fail if it
		 * can't grab the mutex, which should never happen.
		 */

		while ((pC->pRegs[CONTROL] & 0x1) != 0)
			(void)push_char(channel, pC->pRegs[DATA] & 0x7F);

		/*
		 * Check for a hardware fifo overflow.  If there is a 
		 * hardware fifo overflow, lock the data structure,
		 * clear the buffer, and set the fifoOverflow flag.  If
		 * the mutex is not available in a reasonable time there
		 * is nothing that can be done, so just print a warning
		 * message and hope for the best. 
		 */

		pC->pRegs[CONTROL] = 1;
		if (pC->pRegs[CONTROL] & 0x20) {
			if (LOCK(pC->read.semMutex, timeout) == ERROR) {
				fprintf(stderr,
					__FILE__ "(%d): %d could not take mutex!\n",
					__LINE__, channel);
			} else {
				pC->read.fifoOverflow = TRUE;
				pC->read.pStart = pC->read.pEnd;
				fprintf(stderr, __FILE__ "(%d): xy490 FIFO overflow\n",
					__LINE__);
				if (UNLOCK(pC->read.semMutex) == ERROR) {
					fprintf(stderr, __FILE__ "(%d): Could not give mutex\n",
						__LINE__);
				}
			}
		}	

		/*
		 * Clear the interrupt so that further interrupt processing
		 * can take place.
		 */

		pC->pRegs[CONTROL] = 0;
		pC->pRegs[CONTROL] = 0x38;
	}

	return 0;
}

/*
 * This is normally called asynchronously, so doesn't set the error
 * field.
 */

static int
push_char(int channel, char ch)
{
	Channel *const pC = &channels[channel];
	const int timeout = (int)ceil(sysClkRateGet() * READ_TIMEOUT_SEC);
	int status = OK;

	if (LOCK(pC->read.semMutex, timeout) == ERROR) {
		fprintf(stderr,
			__FILE__ "(%d): %d Could not push character\n", __LINE__, channel);
		status = ERROR;
	} else {
		*pC->read.pEnd++ = ch;
#if DEBUG > 2
		if (isascii(pC->read.pEnd[-1]) && isprint(pC->read.pEnd[-1])) {
			fprintf(stderr, "char_available(%d) %c\n", 
				channel, pC->read.pEnd[-1]);
		} else {
			fprintf(stderr, "char_available(%d) %d\n", 
				channel, pC->read.pEnd[-1]);
		}
#endif
		if (pC->read.pEnd == pC->read.buf + READ_BUF_SIZE)
			pC->read.pEnd = pC->read.buf;

		/*
		 * The buffer is full (which looks exactly like an
		 * empty buffer).  We could just drop a few
		 * characters to make room for more incoming characters,
		 * but that makes it harder to resynch.  So we just
		 * flag it as an error, and let it be treated as an
		 * empty buffer.
		 */

		if (pC->read.pEnd == pC->read.pStart)
			pC->read.fifoOverflow = TRUE;

		if (semGive(pC->read.semRead) == ERROR) { /* Wake the read routine */
			fprintf(stderr, __FILE__ "(%d): Could not give read semaphore\n",
				__LINE__);
		}

		if (UNLOCK(pC->read.semMutex) == ERROR)
			fprintf(stderr, __FILE__ "(%d): Could not give mutex\n", __LINE__);
	}

	return status;
}

/*
 * For debugging purposes only.  Push a character onto the ring buffer,
 * as if it came from the serial hardware.
 */

int
xy490DebugPushChar(int channel, char ch)
{
	Channel *const pC = &channels[channel];
	static int status = OK;

	if (push_char(channel, ch) == ERROR) {
		pC->error = "Could not push character\n";
		status = ERROR;
	}

	if (pC->read.fifoOverflow)
		status = ERROR;

	return status;
}

int
xy490FlushInput(int channel)
{
	volatile int junk;
	int status = OK;
	Channel *const pC = &channels[channel];
	const int timeout = (int)ceil(sysClkRateGet() * READ_TIMEOUT_SEC);

	if (LOCK(pC->read.semMutex, timeout) == ERROR) {
		fprintf(stderr, __FILE__ "(%d): Cannot flush input\n", __LINE__);
		pC->error = "Cannot lock mutex!\n";
		status = ERROR;
	} else {
		while ((pC->pRegs[CONTROL] & 0x1) != 0)
			junk = pC->pRegs[DATA];

		pC->read.pStart = pC->read.pEnd;
		pC->read.fifoOverflow = FALSE;

		while (semTake(pC->read.semRead, timeout) == OK)
			;

		if (UNLOCK(pC->read.semMutex) == ERROR)
			fprintf(stderr, __FILE__ "(%d): Could not give mutex\n", __LINE__);
	}

	return status;
}

int
xy490InputClearError(int channel)
{
	Channel *const pC = &channels[channel];
	const int timeout = (int)ceil(sysClkRateGet() * READ_TIMEOUT_SEC);
	int status = OK;

	if (LOCK(pC->read.semMutex, timeout) == ERROR) {
		pC->error = "Could not take mutex";
		status = ERROR;
	} else {
		pC->read.fifoOverflow = FALSE;
		if (UNLOCK(pC->read.semMutex) == ERROR)
			fprintf(stderr, __FILE__ "(%d): Could not give mutex\n", __LINE__);
	}

	return status;
}

const char *
xy490GetError(int channel)
{
	Channel *const pC = &channels[channel];

	return pC->error;
}

/* 
 * Warning: err must be a static or dynamic variable, not an auto
 * variable.
 */

void
xy490SetError(int channel, const char *err)
{
	Channel *const pC = &channels[channel];

	pC->error = err;
}

/*
 * This is EVIL!  Sometimes, after devOcycd.c calls xy490Read(), the next
 * few printf/logMsg calls don't work, or are only partially displayed.
 * This may indicate that memory is getting corrupted.  Calling this
 * statement immediately after any printf statements seems to supress
 * the problem.  Obviously, if there is memory corruption, I really need
 * to SOLVE the problem, but I have been unable to isolate it.  I don't
 * understand why this solves the problem.
 */

void
xy490PrintfKludge(void)
{
	struct timespec ts;

	fflush(stdout);
	fflush(stderr);

	ts.tv_sec = 0;
	ts.tv_nsec = 100000000;
	nanosleep(&ts, NULL);
}

#if defined(MUTEXDEBUG)

static int zero = 0;
static int count[MAX_BOARDS * CHAN_PER_BOARD] = {0};

static STATUS
lock(SEM_ID sid, int timeout, int channel, const int line)
{
	STATUS status = OK;

	if (zero == 0)
		zero = time(NULL);

	xy490PrintfKludge();
	if (channel != 1) {
		fprintf(stderr, "%4d: TAKE   channel=%d, %4ld, %d\n",
			line, channel, time(NULL) - zero, count[channel]);
	}
	fflush(stderr);
	xy490PrintfKludge();

	status = semTake(sid, timeout);

	if (status == ERROR) {
		xy490PrintfKludge();
		if (channel!=1) {
			fprintf(stderr, "%4d: *FAIL* channel=%d, %4ld, %d\n",
				line, channel, time(NULL) - zero, count[channel]);
		}
		fflush(stderr);
		xy490PrintfKludge();
	} else {
		count[channel]++;
		
		xy490PrintfKludge();
		if (channel!=1) {
			fprintf(stderr, "%4d: GOT    channel=%d, %4ld, %d\n",
				line, channel, time(NULL) - zero, count[channel]);
		}
		fflush(stderr);
		xy490PrintfKludge();
	}

	return status;
}

static STATUS
unlock(SEM_ID sid, int channel, const int line)
{
	STATUS status = OK;

	if (zero == 0)
		zero = time(NULL);

	status = semGive(sid);

	if (status == ERROR) {
		xy490PrintfKludge();
		if (channel!=1) {
			fprintf(stderr,
				"%4d: *KEPT* channel=%d, %4ld, %d\n",
				line, channel, time(NULL) - zero, count[channel]);
		}
		fflush(stderr);
	} else {
		count[channel]--;
		xy490PrintfKludge();
		if (channel!=1) {
			fprintf(stderr,
				"%4d: GIVE   channel=%d, %4ld, %d\n",
				line, channel, time(NULL) - zero, count[channel]);
		}
		fflush(stderr);
	}

	return status;
}

#endif
