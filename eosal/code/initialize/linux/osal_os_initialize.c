/**

  @file    initialize/linux/osal_os_initialize.c
  @brief   Operating system specific OSAL initialization.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  Operating system specific OSAL initialization and shut down.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eosal/eosal.h"
#include <signal.h>


/**
****************************************************************************************************

  @brief Operating system specific OSAL library initialization.
  @anchor osal_init_os_specific

  The osal_init_os_specific() function does operating system specific initialization
  OSAL library for use.

  @param  flags Bit fields. OSAL_INIT_DEFAULT (0) for normal initalization.
          OSAL_INIT_NO_LINUX_SIGNAL_INIT not to initialize linux signals.

  @return  None.

****************************************************************************************************
*/
void osal_init_os_specific(
    os_int flags)
{
    if ((flags & OSAL_INIT_NO_LINUX_SIGNAL_INIT) == 0)
    {
        /* Do not terminate program if socket breaks.
         */
        /* signal (SIGPIPE, SIG_IGN); */
    }
}


/**
****************************************************************************************************

  @brief Shut down operating system specific part of the OSAL library.
  @anchor osal_shutdown

  The osal_shutdown_os_specific() function...

  @return  None.

****************************************************************************************************
*/
void osal_shutdown_os_specific(
    void)
{
}
