/**

  @file    time/common/osal_time.h
  @brief   Get and set system time.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  Get or set system time (GMT) as long integer.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#ifndef OSAL_CLOCK_INCLUDED
#define OSAL_CLOCK_INCLUDED

/** 
****************************************************************************************************

  @name Time functions

  The os_time() function gets the current time as 64 bit integer, microseconds since epoc 
  1970 and the os_settime() function sets the system clock. 

****************************************************************************************************
 */
/*@{*/

/* Get system time (GMT).
 */
void os_time(
    os_int64 *t);

/* Set system time (GMT).
 */
osalStatus os_settime(
    const os_int64 *t);

/*@}*/

#endif
