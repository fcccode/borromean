/**

  @file    main/linux/osal_main.c
  @brief   Linux process entry point function.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  This OSAL main process entry point header file. Generally the operating system calls entry
  point function to start the process. Unfortunately name, arguments and character encoding
  and other detail for this entry point function varies.

  To be start a process in generic way we write osal_main() function in our application
  and then link with osal_main, etc. library which contains appropriate operating system
  dependent entry point, converts the arguments to UTF8 and passes these on to osal_main()

  Notice that using osal_main() function to enter the process is optional, you can start the
  process in any way you like.

  Linux notes:
  - On linux terminating '\r' on command line is removed. This is typically caused by
    editing scripts on windows.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eosal/eosalx.h"


/**
****************************************************************************************************

  @brief Linux process entry point.
  @anchor main

  The main() function is used generally as entry point. This function just removes extra '\r'
  character, if one is found.

  @param   argc Number of command line arguments. First argument is name of the executable.
  @param   argv Array of string pointers, one for each command line argument plus first
           one for the executable name, UTF16 encoded.

  @return  Integer return value to caller.

****************************************************************************************************
*/
int main(
    int argc,
    char **argv)
{
    os_char **myargv, *p, *q;
    os_memsz sz;
    os_int i;
    int rval;

    /* Initialize operating system abstraction layer.
     */
    osal_initialize(OSAL_INIT_DEFAULT);

    /* Set normal thread priority.
     */
    osal_thread_set_priority(OSAL_THREAD_PRIORITY_NORMAL);

    /* Copy pointers to command line arguments. If command line argument contains '\r'
       character, make copy and terminate ar '\r'. These memory allocations will be
       cleaned up when osal_shutdown runs at exit, and all memory is released.
     */
    myargv = (os_char**)os_malloc(argc*sizeof(os_char*), OS_NULL);
    for (i = 0; i < argc; i++)
    {
        myargv[i] = argv[i];
        p = myargv[i];
        if (os_strchr(p, '\r'))
        {
            sz = os_strlen(p);
            q = os_malloc(sz, OS_NULL);
            os_memcpy(q, p, sz);
            myargv[argc-1] = q;
            p = os_strchr(q, '\r');
            *p = '\0';
        }
    }

    /* Call OS independent process entry point.
     */
    rval = osal_main(argc, myargv);

    /* Shut down operating system abstraction layer.
     */
    osal_shutdown();
    
    return rval;
}
