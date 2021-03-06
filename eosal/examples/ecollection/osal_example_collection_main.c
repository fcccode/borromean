/**

  @file    examples/examplecollection/osal_example_collection_main.c
  @brief   Example code about threads.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  Main function for set of example. Multiple examples have been packed in this project to
  avoid creating many builds for multiple operating systems.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eosal/code/defs/osal_code.h"
#include "eosal/examples/examplecollection/osal_threads_example.h"
#include "eosal/examples/examplecollection/osal_threads_example_2.h"


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
	// return osal_threads_example_main(argc, argv);
	return osal_threads_example_2_main(argc, argv);
}
