/**

  @file    filesys/common/osal_filefstat.h
  @brief   Get file information.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#ifndef OSAL_FILESTAT_INCLUDED
#define OSAL_FILESTAT_INCLUDED

/** File status information.
 */
typedef struct osalFileStat
{
    /** File size in bytes.
     */
    os_long sz;

    /** File modification time.
     */
    os_int64 tstamp;

    /** OS_TRUE if item is directory, OS_FALSE if file.
     */
    os_boolean isdir;
}
osalFileStat;


/** 
****************************************************************************************************

  @name Functions.

  X...

****************************************************************************************************
 */
/*@{*/

/* List directory. Allocates directory list list.
 */
osalStatus osal_filestat(
    const os_char *path,
    osalFileStat *filestat);

/*@}*/

#endif
