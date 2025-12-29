static char rcid[]="$Id: var_sr.c,v 1.2 2009/05/27 19:33:24 fkraemer Exp $";
/******************************************************************************
 * File:	var_sr.c (variable set/read)
 * Purpose:	to provide an easy method for adding the variable set and
 *		read to future programs
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		18Mar91	created						dak
 *		26Mar91	added support for array indices			dak
 *		26Dec91 fixed to work under wildfire			dak
 *		10Aug92	cleaned up					dak
 *
 ******************************************************************************/

#include <string.h>
#include <conc.h>
#include <stdlib.h>
#include "protocol.h"
#include "common.h"
#include <stdio.h>

extern int _node_number;

extern Channel *Control_to_Reader;

/******************************************************************************
 * Routine: set_var
 * Purpose: to set a variable pointed to by the var_ptr array
 * Inputs:  buf -- pointer to an integer buffer with the set var message
 *          header -- integer header
 * Returns: nothing
 * 
 ******************************************************************************/

void set_var(int header, int *buf)
{
    int i;
     
    int *ptr = var_ptr[VAR_NUMVAL(buf)] + ARR_INDEX(buf);

    for (i = 0; i < LENGTH(header) - 1; i++)
    {
	ptr[i] = buf[VAR_VAL + i];
    }
    if (LENGTH(header) > 2)
      send_debug ("Variables 1 - %d set",LENGTH(header) - 1);
    else if (LENGTH(header) == 2)
      send_debug("Variable # %d on Node %d set to %d", (int)(VAR_NUMVAL(buf)), 
                                           (int)_node_number, (int)(buf[VAR_VAL])); 
    else
      send_debug("No variables to set");
}

/******************************************************************************
 * Routine: read_var
 * Purpose: to read the value of a variable and send an appropriate message
 *          to whoever requested the information
 * Inputs:  buf -- pointer to an integer buffer that contained the
 *                 request for info
 *          header -- integer header
 * Returns: nothing (but it does call send())
 * 
 ******************************************************************************/

void read_var(int header, int *buf)
{
    int i,len;
    int *ptr = var_ptr[VAR_NUMVAL(buf)] + ARR_INDEX(buf);
    int *my_buf;

    if (LENGTH(header) == 3)		/* length supplied */
        if (buf[LENGTH_TO_SEND] == 0)	/* use strlen */
            len = 2 + (strlen((char *) ptr) + 4) / 4;
        else
            len = 2 + buf[LENGTH_TO_SEND];
    else
	len = 3;			/* varnum + from + value */

    header = TO_NODE(buf[REPLY_TO]) | MESSAGE(VAR_READ) | OF_LENGTH(len);
    my_buf = mem_alloc((int)(sizeof(int) * len));

    for (i = 0; i < len - 2; i++)
        my_buf[RETURN_VAL+i] = ptr[i];

    my_buf[VAR_NUM] = buf[VAR_NUM];
    my_buf[FROM] = _node_number;/* send message with value of */

    SEND(header, (char *) my_buf, Control_to_Reader);
					/* var[VAR_NUM] to REPLY_TO */
}

