/**

  @file    console/common/osal_console.h
  @brief   Application console IO.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  This header file contains functions prototypes for application's console IO. 

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#ifndef OSAL_CONSOLE_INCLUDED
#define OSAL_CONSOLE_INCLUDED


#define OSAL_CONSOLE_BUFFER_CHARS 8

/** 
****************************************************************************************************

  @name Console State Structure

  X...

****************************************************************************************************
*/
typedef struct
{
	/** Console input buffer.
	 */
	os_uint buf[OSAL_CONSOLE_BUFFER_CHARS];
}
osalConsoleState;


/** 
****************************************************************************************************

  @name Application's Console Functions

  The osal_console_write() function writes a string to console or reads a character from
  console.

****************************************************************************************************
 */
/*@{*/

#if OSAL_CONSOLE

    /* Write text trough console IO.
     */
    #define osal_console_write(t) osal_sysconsole_write(t)

    /* Read an UTF32 character from console. 0 = no char.
     */
    #define osal_console_read(t) osal_sysconsole_read(t)

#else

    #define osal_console_write(t)
    #define osal_console_read(t)

#endif


#endif
