/**

  @file    examples/examplecollection/osal_threads_example.c
  @brief   Example code about threads.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  This example demonstrates how to create threads.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eosal/code/defs/osal_code.h"
#include "eosal/examples/examplecollection/osal_threads_example.h"

/** Parameter structure for creating thread.
 */
typedef struct
{
	/** A parameter for new thread.
	 */
    os_int some_parameter;
}
MyThreadParameters;


/* Forward referred static functions.
 */
static void my_thread_1_func(
    void *prm,
	volatile os_boolean *exit_requested,
	osalEvent done);


/**
****************************************************************************************************

  @brief Process entry point.

  The osal_main() function is OS independent entry point.

  @param   argc Number of command line arguments.
  @param   argv Array of string pointers, one for each command line argument. UTF8 encoded.

  @return  None.

****************************************************************************************************
*/
os_int osal_threads_example_main(
    os_int argc,
    os_char *argv[])
{
    MyThreadParameters myprm;

    os_memclear(&myprm, sizeof(myprm));

    osal_thread_create(my_thread_1_func, &myprm, OSAL_THREAD_DETACHED, 0, "mythread1");

    return 0;
}


/**
****************************************************************************************************

  @brief Thread 1 entry point function.

  The my_thread_1_func() function is called to start the thread.

  @param   prm Pointer to parameters for new thread. This pointer must can be used only
           before "done" event is set. This can be OS_NULL if no parameters are needed.
  @param   done Event to set when parameters have been copied to entry point 
           functions own memory.

  @return  None.

****************************************************************************************************
*/
static void my_thread_1_func(
    void *prm,
	volatile os_boolean *exit_requested,
	osalEvent done)
{
    MyThreadParameters
        myprm;

    /* Copy parameters to local stack
     */
    os_memcpy(&myprm, prm, sizeof(MyThreadParameters));

    /* Let thread which created this one proceed
     */
    osal_event_set(done);
}

