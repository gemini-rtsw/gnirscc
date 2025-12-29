/* $Id: choiceHmotor.h,v 1.2 2009/05/27 19:33:58 fkraemer Exp $ */

/*
 * This record is derived from the motor record.
 */

/*
 *      Author:
 *      Date:
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 1991, the Regents of the University of California,
 *      and the University of Chicago Board of Governors.
 *
 *      This software was produced under  U.S. Government contracts:
 *      (W-7405-ENG-36) at the Los Alamos National Laboratory,
 *      and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *      Initial development by:
 *              The Controls and Automation Group (AT-8)
 *              Ground Test Accelerator
 *              Accelerator Technology Division
 *              Los Alamos National Laboratory
 *
 *      Co-developed with
 *              The Controls and Computing Group
 *              Accelerator Systems Division
 *              Advanced Photon Source
 *              Argonne National Laboratory
 *
 * Modification Log:
 * -----------------
 * .01  10-19-92        jbk     Initial Definition
 * .02  01-26-93        tmm     Initial Definition continued
 */

#ifndef INCchoiceHmotorh
#define INCchoiceHmotorh 1
#define REC_HMOTOR_DIR	0
#define REC_HMOTOR_SET	1
#define REC_HMOTOR_MODE	2
#define REC_HMOTOR_YN	3
#define REC_HMOTOR_SPMG	4
#define REC_HMOTOR_FOFF	5
#define REC_HMOTOR_POWER	6
/* for record-support use */
#define HMOTOR_SPMG_STOP	0
#define HMOTOR_SPMG_PAUSE	1
#define HMOTOR_SPMG_MOVE	2
#define HMOTOR_SPMG_GO		3
#define HMOTOR_MODE_POSITION	0
#define HMOTOR_MODE_VELOCITY	1
#define HMOTOR_DIR_POS		0
#define HMOTOR_DIR_NEG		1
#define HMOTOR_CHOICE_YES	1
#define HMOTOR_PWR_AUTO    0
#define HMOTOR_PWR_ON      1
#define HMOTOR_PWR_OFF     2
#endif
