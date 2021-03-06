/**

  @file    socket/common/osal_socket.h
  @brief   OSAL Sockets API.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  This header file contains function prototypes and definitions for OSAL sockets API. 
  Osal sockets api is wrapper for operating system sockets.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#ifndef OSAL_SOCKET_INCLUDED
#define OSAL_SOCKET_INCLUDED

/** Stream interface structure for sockets.
 */
#if OSAL_FUNCTION_POINTER_SUPPORT
extern osalStreamInterface osal_socket_iface;
#endif

/** Define to get socket interface pointer. The define is used so that this can 
    be converted to function call.
 */
#define OSAL_SOCKET_IFACE &osal_socket_iface

/* Default socket port number if not specified
 */
#define OSAL_DEFAULT_SOCKET_PORT 21981
#define OSAL_DEFAULT_SOCKET_PORT_STR "21981"

/* Maximum number of socket streams to pass as an argument to osal_socket_select().
 */
#define OSAL_SOCKET_SELECT_MAX 8


/** 
****************************************************************************************************

  @name OSAL Socket Functions.

  These functions implement sockets as OSAL stream. These functions can either be called 
  directly or through stream interface.

****************************************************************************************************
 */
/*@{*/

/* Open socket.
 */
osalStream osal_socket_open(
	os_char *parameters,
	void *option,
	osalStatus *status,
	os_int flags);

/* Close socket.
 */
void osal_socket_close(
	osalStream stream);

/* Accept connection from listening socket.
 */
osalStream osal_socket_accept(
	osalStream stream,
	osalStatus *status,
	os_int flags);


/* Flush written data to socket.
 */
osalStatus osal_socket_flush(
	osalStream stream,
	os_int flags);

/* Write data to socket.
 */
osalStatus osal_socket_write(
	osalStream stream,
	const os_uchar *buf,
	os_memsz n,
	os_memsz *n_written,
	os_int flags);

/* Read data from socket.
 */
osalStatus osal_socket_read(
	osalStream stream,
	os_uchar *buf,
	os_memsz n,
	os_memsz *n_read,
	os_int flags);

/* Get socket parameter.
 */
os_long osal_socket_get_parameter(
	osalStream stream,
	osalStreamParameterIx parameter_ix);

/* Set socket parameter.
 */
void osal_socket_set_parameter(
	osalStream stream,
	osalStreamParameterIx parameter_ix,
	os_long value);

/* Wait for new data to read, time to write or operating system event, etc.
 */
osalStatus osal_socket_select(
	osalStream *streams,
    os_int nstreams,
	osalEvent evnt,
	osalSelectData *selectdata,
	os_int flags);

/* Initialize OSAL sockets library.
 */
void osal_socket_initialize(
	void);

/* Shut down OSAL sockets library.
 */
void osal_socket_shutdown(
	void);

/* Get host and port from network address string.
 */
os_char *osal_socket_get_host_name_and_port(
	os_char *parameters,
	os_int  *port,
	os_memsz *buf_sz,
    os_boolean *is_ipv6);

/*@}*/

#endif
