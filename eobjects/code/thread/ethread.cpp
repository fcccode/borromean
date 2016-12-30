/**

  @file    ethread.cpp
  @brief   Thread class.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    28.12.2016

  The thread object is the root of thread's object tree.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eobjects/eobjects.h"

/** Parameter structure for creating thread.
 */
typedef struct
{
    /* Pointer to the thread object
     */
    eThread *thread;

	/** Parameters for new thread.
	 */
    eContainer *params;
	// os_int some_parameter;
}
eThreadParameters;


/* Forward referred static functions.
 */
static void ethread_func(
    void *prm,
	volatile os_boolean *exit_requested,
	osalEvent done);



/**
****************************************************************************************************

  @brief Constructor.

  X...

  @return  None.

****************************************************************************************************
*/
eThread::eThread(
	eObject *parent,
	e_oid oid,
	os_int flags)
    : eObject(parent, oid, flags)
{
    /* Create thread triggger. 
     */
    m_trigger = osal_event_create();

    /* Create message queue for incoming messages.
     */
    m_message_queue = new eContainer(this,  EOID_INTERNAL, 
        EOBJ_IS_ATTACHMENT|EOBJ_NOT_CLONABLE|EOBJ_NOT_SERIALIZABLE);
}


/**
****************************************************************************************************

  @brief Virtual destructor.

  X...

  @return  None.

****************************************************************************************************
*/
eThread::~eThread()
{

    /* Release thread triggger. 
     */
    osal_event_delete(m_trigger);
}


/**
****************************************************************************************************

  @brief Start thread.

  After calling this funcion, eThread pointer thiso cannot be used from calling thread.

  @return  None.

****************************************************************************************************
*/
void eThread::start(
    eThreadHandle *thandle,
    eContainer *params)
{
    eThreadParameters 
		prmstruct;

	osalThreadHandle
		*handle;

    os_memclear(&prmstruct, sizeof(prmstruct));
    prmstruct.thread = this;
    prmstruct.params = params; // CHECK WHICH THREAD OWNS && MAKE COPY??????????????????????????????????????????????????????????????????????

    handle = osal_thread_create(ethread_func, &prmstruct, OSAL_THREAD_ATTACHED, 0, "threadnamehere");
    thandle->set_osal_handle(handle);
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
static void ethread_func(
    void *prm,
	volatile os_boolean *exit_requested,
	osalEvent done)
{
    eThreadParameters
        prmstruct;

    /* Copy parameters to local stack
     */
    os_memcpy(&prmstruct, prm, sizeof(eThreadParameters));

    /* Initialize the thread.
     */
    prmstruct.thread->initialize(prmstruct.params);

    /* Let thread which created this one proceed.
     */
    osal_event_set(done);

    /* Run the thread.
     */
    prmstruct.thread->run();
}


/* void eThread::initialize(
    eContainer *params)
{
}
*/

void eThread::run()
{
    while (!exitnow())
    {
        process_messages();
    }
} 


/**
****************************************************************************************************

  @brief Check if thread exit is requested.

  Check if thread termination is requested.

  @return  None.

****************************************************************************************************
*/
os_boolean eThread::exitnow()
{
//	while (!osal_thread_exit_requested(exit_requested))
return OS_FALSE;
}


/**
****************************************************************************************************

  @brief Place an envelope to thread's message queue

  The eThread::queue function...

  Process mutex must be locked when calling this function!!!

  @param  envelope Pointer to envelope. Envelope will be adopted by this function.
  @return None.

****************************************************************************************************
*/
void eThread::queue(
    eEnvelope *envelope)
{
    m_message_queue->adopt(envelope);
}


/**
****************************************************************************************************

  @brief Process messages.

  The process_messages function processed messages incoming to thread. It takes a message
  item at a time and and forwards those.

  @return None.

****************************************************************************************************
*/
void eThread::process_messages(
    os_int timeout_ms)
{
    eEnvelope
        *envelope;

    /* Wait for thread to be trigged.
     */
    osal_event_wait(m_trigger, OSAL_EVENT_INFINITE);

    while (OS_TRUE)
    {
        /* Synchronize and get message from queue.
         */
	    osal_mutex_system_lock();
        envelope = eEnvelope::cast(m_message_queue->first());
        if (envelope) adopt(envelope);
	    osal_mutex_system_unlock();

        /* If no message, do nothing more.
         */
        if (envelope == OS_NULL) return;

        /* Call message processing.
         */
        onmessage(envelope);
    }           
} 
