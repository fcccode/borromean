/**

  @file    strcnv/common/osal_strcnv.h
  @brief   Conversions between numbers and strings.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  This osal_strcnv module header file. This module includes conversions between floating 
  point numbers, 64 bit integers and strings.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#ifndef OSAL_STRCNV_INCLUDED
#define OSAL_STRCNV_INCLUDED

/* If C++ compilation, all functions, etc. from this point on in this header file are
   plain C and must be left undecorated.
 */
OSAL_C_HEADER_BEGINS

/** Flags for osal_double_to_string() function.
 */
#define OSAL_FLOAT_DEFAULT 0
#define OSAL_FLOAT_E_FORMAT 1

/* Convert floating point number to string.
 */
os_memsz osal_double_to_string(
    os_char *buf, 
    os_memsz buf_sz,
    os_double x, 
    os_int ddigs,
	os_int flags);

/* Convert string to floating point number.
 */
os_double osal_string_to_double(
    const os_char *str,
    os_memsz *count);

/* #define osal_int64_to_string(b,s,x) osal_int_to_string((b),(s),*(x)) */

/* Convert a string to 64 bit integer.
 */
os_memsz osal_string_to_int64(
    os_int64 *x, 
    os_char *str);

/* Convert 64 bit integer to string. If operating system supports 64 bit integers as 
   os_long type, we just use macro to map this to osal_int_to_string() function. 
   If operating system has no 64 bit support, the function implementation is used.
 */
#if OSAL_LONG_IS_64_BITS
    #define osal_int64_to_string(b,s,x) osal_int_to_string((b),(s),*(x))
#else
    os_memsz osal_int64_to_string(
		os_char *buf, 
        os_memsz buf_sz,
        os_int64 *x);
#endif

/* If C++ compilation, end the undecorated code.
 */
OSAL_C_HEADER_ENDS

#endif
