/**

  @file    osal_main.h
  @brief   Calling generic entry point.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  This OSAL main process entry point header file. Generally the operating system calls entry
  point function to start the process. Unfortunately name, arguments and character encoding
  for this entry point function varies. For example Windows uses main, wmain, WinMain and
  wWinMain. Besides many tool libraries define their own entry point functions.

  To be start a process in generic way we write osal_main() function in our application
  and then link with osal_main, etc. library which contains appropriate operating system
  dependent entry point, converts the arguments to UTF8 and passes these on to osal_main()

  Notice that using osal_main() function to enter the process is optional, you can start the
  process in any way you like.

  Windows notes :
  - An application using /SUBSYSTEM:CONSOLE calls wmain, set wmainCRTStartup linker option
    and link with osal_maind.lib (debug) or osal_main.lib (release).
  - An application using /SUBSYSTEM:WINDOWS; calls wWinMain, which must be defined with
    __stdcall, set wWinMainCRTStartup as entry point. Link with ?

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#ifndef OSAL_MAIN_INCLUDED
#define OSAL_MAIN_INCLUDED

/* If C++ compilation, all functions, etc. from this point on in this header file are
   plain C and must be left undecorated.
 */
OSAL_C_HEADER_BEGINS

/* Prototype for application's entry point function.
 */
os_int osal_main(
    os_int argc,
    os_char *argv[]);

/* If C++ compilation, end the undecorated code.
 */
OSAL_C_HEADER_ENDS

#endif