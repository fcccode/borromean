/**

  @file    esocket.cpp
  @brief   TCP socket class.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    17.5.2016

  TCP socket class encodes and buffers data and calls OSAL's stream functions to read/write the
  socket.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eobjects/eobjects.h"


/**
****************************************************************************************************

  @brief Constructor.

  X...

  @return  None.

****************************************************************************************************
*/
eSocket::eSocket(
	eObject *parent,
	e_oid oid,
	os_int flags)
    : eStream(parent, oid, flags)
{
    /* Clear member variables.
     */
    m_in = m_out = OS_NULL;
    m_socket = OS_NULL;
}


/**
****************************************************************************************************

  @brief Virtual destructor.

  X...

  @return  None.

****************************************************************************************************
*/
eSocket::~eSocket()
{
}


/**
****************************************************************************************************

  @brief Add eSocket to class list and class'es properties to it's property set.

  The eSocket::setupclass function adds eSocket to class list and class'es properties to
  it's property set. The class list enables creating new objects dynamically by class identifier, 
  which is used for serialization reader functions. The property stet stores static list of
  class'es properties and metadata for those.

****************************************************************************************************
*/
void eSocket::setupclass()
{
    const os_int cls = ECLASSID_SOCKET;

    /* Add the class to class list.
     */
    osal_mutex_system_lock();
    eclasslist_add(cls, (eNewObjFunc)newobj);
    osal_mutex_system_unlock();
}


/**
****************************************************************************************************

  @brief Open a socket.

  The open() function opens a socket. The function can either connect a socket or listen to 
  specific TCP port.

  @param  parameters Socket parameters, a list string or direct value.
		  Address and port to connect to, or interface and port to listen for.
          Socket IP address and port can be specified either as value of "addr" item
          or directly in parameter sstring. For example "192.168.1.55:20" or "localhost:12345"
          specify IPv4 addressed. If only port number is specified, which is often 
          useful for listening socket, for example ":12345".
          IPv4 address is automatically recognized from numeric address like
          "2001:0db8:85a3:0000:0000:8a2e:0370:7334", but not when address is specified as string
          nor for empty IP specifying only port to listen. Use brackets around IP address
          to mark IPv6 address, for example "[localhost]:12345", or "[]:12345" for empty IP.

  @param  flags Flags for creating the socket. Bit fields, combination of:
          - OSAL_STREAM_CONNECT: Connect to specified socket port at specified IP address. 
          - OSAL_STREAM_LISTEN: Open a socket to listen for incoming connections. 
          - OSAL_STREAM_UDP_MULTICAST: Open a UDP multicast socket. 
          - OSAL_STREAM_TCP_NODELAY: Disable Nagle's algorithm on TCP socket.
          - OSAL_STREAM_NO_REUSEADDR: Disable reusability of the socket descriptor.

  @return  If successfull, the function returns ESTATUS_SUCCESS. Other return values
           indicate an error. 

****************************************************************************************************
*/
eStatus eSocket::open(
	os_char *parameters,
    os_int flags)
{
    osalStatus s;

    /* If we are listening, delete any queues. If connecting, create and open input and 
       output queues. This clears the queues if they were already open.
     */
    if (flags & OSAL_STREAM_LISTEN)
    {
        delete m_in;
        delete m_out;
        m_in = m_out = OS_NULL;
    }
    else
    {
        if (m_in == OS_NULL) m_in = new eQueue(this);
        if (m_out == OS_NULL) m_out = new eQueue(this);
        m_in->open(OS_NULL, OSAL_STREAM_DECODE_ON_READ);
        m_out->open(OS_NULL, OSAL_STREAM_ENCODE_ON_WRITE);
    }

    /* Open socket
     */
    m_socket = osal_socket_open(parameters, OS_NULL, &s, flags);
    
    return ESTATUS_SUCCESS;
}


eStatus eSocket::close() 
{
    return ESTATUS_SUCCESS;
}