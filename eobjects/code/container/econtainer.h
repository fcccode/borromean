/**

  @file    econtainer.h
  @brief   Simple object container.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  The container object is like a box holding a set of child objects.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#ifndef ECONTAINER_INCLUDED
#define ECONTAINER_INCLUDED



/**
****************************************************************************************************

  @brief Object base class.

  The eObject is base class for all eobject library objects. It defines basic functionality for
  networked objects.


****************************************************************************************************
*/
class eContainer : public eObject
{
	/** 
	************************************************************************************************

	  @name Constructors and destructor

	  X...

	************************************************************************************************
	*/
	/*@{*/
public:
        /** Constructor.
		 */
		eContainer(
			eObject *parent,
			e_oid oid = EOID_ITEM,
			os_int flags = EOBJ_DEFAULT);

		/** Public constructor.
		 */
//		inline eContainer() : eContainer(OS_NULL) {}

		/* Virtual destructor.
		 */
		virtual ~eContainer();

        /* Casting eObject pointer to eContainer pointer.
         */
		inline static eContainer *cast(
			eObject *o) 
		{ 
			return (eContainer*)o;
		}

        /* Get class identifier.
         */
        virtual os_int getclassid() {return ECLASSID_CONTAINER;}

        /* Static constructor function for generating instance by class list.
         */
        static eContainer *newobj(
            eObject *parent,
            e_oid oid = EOID_ITEM,
			os_int flags = EOBJ_DEFAULT)
        {
            return new eContainer(parent, oid, flags);
        }

    /*@}*/

	/** 
	************************************************************************************************

	  @name eObject virtual function implementations

	  X... 

	************************************************************************************************
	*/
	/*@{*/
        /* Write container content to stream.
         */
        virtual eStatus writer(
            eStream *stream, 
            os_int flags);

        /* Read container content from stream.
         */
        virtual eStatus reader(
            eStream *stream, 
            os_int flags);

    /*@}*/

};

#endif
