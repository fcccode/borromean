/**

  @file    stringx/common/osal_stringx.h
  @brief   String manipulation, extended functions.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  This header file contains functions prototypes for more string manipulation.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#ifndef OSAL_STRINGX_INCLUDED
#define OSAL_STRINGX_INCLUDED


/** 
****************************************************************************************************

  @name Extended string manipulation and conversions functions

  The ...

****************************************************************************************************
 */
/*@{*/

/* Find a value of specified list item.
 */
os_char *osal_string_get_item_value(
    os_char *list_str,
	const os_char *item_name,
	os_memsz *n_chars,
	os_short flags);

/* Find beginning of next line in list string.
 */
os_char *osal_string_get_next_line(
    os_char *list_str);

/* Get integer value of an item in list string.
 */
os_long osal_string_get_item_int(
    os_char *list_str,
	const os_char *item_name,
	os_long default_value,
	os_short flags);

/* Convert string to integer.
 */
os_long osal_string_to_int(
    const os_char *str,
	os_memsz *count);

/* Check if string matches to pattern.
 */
os_boolean osal_pattern_match(
    const os_char *str,
    const os_char *pattern,
    os_int flags);

/*@}*/

#endif
