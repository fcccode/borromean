/**

  @file    eclasslist.cpp
  @brief   Class list.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    17.5.2016

  Maintain list of classes which can be created dynamically by class ID.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eobjects/eobjects.h"

static eClassList eclasslist_buf;

eClassList *eclasslist = &eclasslist_buf;


/**
****************************************************************************************************

  @brief Add a class to class list.

  Class list contains pointers to static constructor for the class by class ID index.
  It is used to constructr objects so that class can be a parameter.

  @return  None.

****************************************************************************************************
*/
void eclasslist_add(
    os_int classid, 
    eNewObjFunc func)
{
//    eclasslist->func[0] = eVariable::newobj;

    if (classid > 0 && classid <= ECLASSID_MAX)
    {
        eclasslist->func[classid] = func;
    }
}


/**
****************************************************************************************************

  @brief Add a eobjects base classes to class list.

  The eclasslist_add_eobjects function...

  @return  None.

****************************************************************************************************
*/
void eclasslist_add_eobjects()
{
    if (eclasslist == OS_NULL)
    {
        eclasslist = &eclasslist_buf;

//    eclasslist_add(ECLASSID_ECONTAINER, ?
        eclasslist_add(ECLASSID_VARIABLE, (eNewObjFunc)eVariable::newobj);
    }
}


/**
****************************************************************************************************

  @brief Get static constuctor function pointer by class ID.

  The eclasslist_get_func function...

  @param   classid Class ifentifier to look for.
  @return  Pointer to static constructor function, or OS_NULL if none found.

****************************************************************************************************
*/
eNewObjFunc eclasslist_get_func(
    os_int classid)
{
    if (eclasslist == OS_NULL) 
        eclasslist_add_eobjects();

    if (classid > 0 && classid <= ECLASSID_MAX) 
        return eclasslist->func[classid];

    return OS_NULL;
}

