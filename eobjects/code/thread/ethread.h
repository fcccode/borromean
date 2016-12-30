/**

  @file    ethread.h
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
#ifndef ETHREAD_INCLUDED
#define ETHREAD_INCLUDED

/**
****************************************************************************************************

  @brief Object base class.

  The eObject is base class for all eobject library objects. It defines basic functionality for
  networked objects.


****************************************************************************************************
*/
class eThread : public eObject
{
	/** 
	************************************************************************************************

	  @name Constructor, destructor, etc.

	  X...

	************************************************************************************************
	*/
	/*@{*/
public:
    /** Constructor.
	 */
	eThread(
		eObject *parent = OS_NULL,
		e_oid oid = EOID_ITEM,
		os_int flags = EOBJ_DEFAULT);

	/* Virtual destructor.
 	 */
	virtual ~eThread();

    /* Casting eObject pointer to eThread pointer.
        */
	inline static eThread *cast(
		eObject *o) 
	{ 
		return (eThread*)o;
	}

    /* Get class identifier.
     */
    virtual os_int classid() {return ECLASSID_THREAD;}

    /* Static constructor function for generating instance by class list.
     */
    static eThread *newobj(
        eObject *parent,
        e_oid oid = EOID_ITEM,
		os_int flags = EOBJ_DEFAULT)
    {
        return new eThread(parent, oid, flags);
    }

    /*@}*/

	/** 
	************************************************************************************************

	  @name Thread message buffer

	  X... 

	************************************************************************************************
	*/
	/*@{*/

    /* Create operating system thread and start running 
     */
    void start(
        eThreadHandle *thandle = OS_NULL,
        eContainer *params = OS_NULL);

    virtual void initialize(
        eContainer *params = OS_NULL) {};

    virtual void run();

    /* Check if thread exit is requested.
     */
    os_boolean exitnow();

    /* Get next message to thread to process.
     */
    void queue(
        eEnvelope *envelope);

    /* Get next message to thread to process.
     */
    void process_messages(
        os_int timeout_ms = OSAL_EVENT_INFINITE);


    /*@}*/

private:
    /* Thread triggger. 
     */
    osalEvent m_trigger;

    /* Message queue for incoming messages.
     */
    eContainer *m_message_queue;
};

#endif
