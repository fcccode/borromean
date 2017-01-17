/**

  @file    examples/console/osal_console_example.c
  @brief   Socket client example.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eosal/eosalx.h"


/**
****************************************************************************************************

  @brief Process entry point.

  The osal_main() function is OS independent entry point.

  @param   argc Number of command line arguments.
  @param   argv Array of string pointers, one for each command line argument. UTF8 encoded.

  @return  None.

****************************************************************************************************
*/
os_int osal_main(
    os_int argc,
    os_char *argv[])
{
    osalStream handle;
    osalStatus *status;

    handle = osal_stream_open(OSAL_SOCKET_IFACE, "127.0.0.1:" OSAL_DEFAULT_SOCKET_PORT_STR,
        OS_NULL, OS_NULL, &status, OSAL_STREAM_CONNECT);

    osal_stream_close(handle);
}
