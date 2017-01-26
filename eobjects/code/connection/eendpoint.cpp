/**

  @file    eendpoint.cpp
  @brief   End point class.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    17.5.2016

  The eEndPoint is socket end point listening to specific TCP port for new connections.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eobjects/eobjects.h"

/* End point property names.
 */
os_char
    eendpp_ipaddr[] = "ipaddr",
    eendpp_isopen[] = "isopen";


/**
****************************************************************************************************

  @brief Constructor.

  X...

  @return  None.

****************************************************************************************************
*/
eEndPoint::eEndPoint(
	eObject *parent,
	e_oid oid,
	os_int flags)
    : eThread(parent, oid, flags)
{
    /** Listening stream (OSAL socket) handle.
     */
    m_stream = OS_NULL;
    m_initialized = OS_FALSE;

    m_ipaddr = new eVariable(this);

}


/**
****************************************************************************************************

  @brief Virtual destructor.

  X...

  @return  None.

****************************************************************************************************
*/
eEndPoint::~eEndPoint()
{
    close();
}


/**
****************************************************************************************************

  @brief Add eVariable to class list and class'es properties to it's property set.

  The eVariable::setupclass function adds eVariable to class list and class'es properties to
  it's property set. The class list enables creating new objects dynamically by class identifier, 
  which is used for serialization reader functions. The property stet stores static list of
  class'es properties and metadata for those.

****************************************************************************************************
*/
void eEndPoint::setupclass()
{
    const os_int cls = ECLASSID_ENDPOINT;
    eVariable *p;

    /* Synchwonize, add the class to class list and properties to property set.
     */
    osal_mutex_system_lock();
    eclasslist_add(cls, (eNewObjFunc)newobj);
    addproperty(cls, EENDPP_IPADDR, eendpp_ipaddr, EPRO_PERSISTENT|EPRO_SIMPLE, "IP");
    p = addpropertyl(cls, EENDPP_ISOPEN, eendpp_isopen, EPRO_NOONPRCH, "is open", OS_FALSE);
    p->setpropertys(EVARP_ATTR, "rdonly;chkbox");
    osal_mutex_system_unlock();
}


/**
****************************************************************************************************

  @brief Called to inform the class about property value change (override).

  The onpropertychange() function is called when class'es property changes, unless the
  property is flagged with EPRO_NOONPRCH. 
  If property is flagged as EPRO_SIMPLE, this function shuold save the property value
  in class members and and return it when simpleproperty() is called.

  Notice for change logging: Previous value is still valid when this function is called.
  You can get the old value by calling property() function inside onpropertychange()
  function.

  @param   propertynr Property number of changed property.
  @param   x Variable containing the new value.
  @param   flags
  @return  None.

****************************************************************************************************
*/
void eEndPoint::onpropertychange(
    os_int propertynr, 
    eVariable *x, 
    os_int flags)
{
    switch (propertynr)
    {
        case EENDPP_IPADDR:
            if (x->compare(m_ipaddr))
            {
                m_ipaddr->setv(x);
                close();
                open();
            }
            break;

        default:
            eThread::onpropertychange(propertynr, x, flags);
            break;
    }
}


/**
****************************************************************************************************

  @brief Get value of simple property (override).

  The simpleproperty() function stores current value of simple property into variable x.

  @param   propertynr Property number to get.
  @param   x Variable info which to store the property value.
  @return  If fproperty with property number was stored in x, the function returns 
           ESTATUS_SUCCESS (0). Nonzero return values indicate that property with
           given number was not among simple properties.

****************************************************************************************************
*/
eStatus eEndPoint::simpleproperty(
    os_int propertynr, 
    eVariable *x)
{
    switch (propertynr)
    {
        case EENDPP_IPADDR:
            x->setv(m_ipaddr);
            break;
   
        default:
            return eThread::simpleproperty(propertynr, x);
    }
    return ESTATUS_SUCCESS;
}


void eEndPoint::initialize(
    eContainer *params)
{
    osal_console_write("initializing worker\n");

    m_initialized = OS_TRUE;
    open();
}

void eEndPoint::run()
{
    eStatus s;
    osalSelectData selectdata;
    eStream *newstream;

    while (!exitnow())
    {
        /* If we have listening socket, wait for socket or thread event. 
           Call alive() to process thread events.
         */
        if (m_stream)
        {
            s = m_stream->select(&m_stream, 1, trigger(), &selectdata, OSAL_STREAM_DEFAULT);
            /* status = osal_stream_select(&m_stream, 1, OS_NULL, 
                &selectdata, OSAL_STREAM_DEFAULT); */

            alive(EALIVE_RETURN_IMMEDIATELY);

            if (s) 
            {
	            osal_console_write("osal_stream_select failed\n");
            }

            else if (selectdata.eventflags & OSAL_STREAM_ACCEPT_EVENT)
            {
                osal_console_write("accept event\n");

                newstream = 0; // new by class if4
            
            	s = m_stream->accept(newstream, OSAL_STREAM_DEFAULT);

                delete newstream;

                /* m_stream->accept();
                newstream = osal_stream_accept(m_stream, 
                    &status, OSAL_STREAM_DEFAULT); */


                if (s) 
                {
	                osal_console_write("osal_stream_accept failed\n");
                }
            }
        }

        /* Otherwise wait for thread events and process them.
         */
        else
        {
            alive(EALIVE_WAIT_FOR_EVENT);
        }

        osal_console_write("worker running\n");
    }
}


/**
****************************************************************************************************

  @brief Open the end point.

  The open starts listening end point.

  @return  None.

****************************************************************************************************
*/
void eEndPoint::open()
{
    eStatus s;

    if (m_stream || !m_initialized || m_ipaddr->isempty()) return;

    m_stream = 0; // new by class if4

    s = m_stream->open(m_ipaddr->gets(), OSAL_STREAM_LISTEN);
    if (s)
    {
	    osal_console_write("osal_stream_open failed\n");
        delete m_stream;
        m_stream = OS_NULL;
    }
    else
    {
        setpropertyl(EENDPP_ISOPEN, OS_TRUE);
    }
}


/**
****************************************************************************************************

  @brief Close the end point.

  The close function closes listening end point.

  @return  None.

****************************************************************************************************
*/
void eEndPoint::close()
{
    if (m_stream == OS_NULL) return;

    setpropertyl(EENDPP_ISOPEN, OS_FALSE);

//    m_stream->close();
    delete m_stream;
    m_stream = OS_NULL;
}
