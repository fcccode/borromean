/**

  @file    evariable.cpp
  @brief   Dynamically typed variables.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  Dynamically typed variables...

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used, 
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept 
  it fully.

****************************************************************************************************
*/
#include "eobjects/eobjects.h"


/* Variable property names.
 */
os_char
    evarp_value[] = "x",
    evarp_digs[] = "x.digs",
    evarp_text[] = "x.text",
    evarp_unit[] = "x.unit",
    evarp_min[] = "x.min",
    evarp_max[] = "x.max",
    evarp_type[] = "x.type",
    evarp_attr[] = "x.attr",
    evarp_default[] = "x.default",
    evarp_gain[] = "x.gain",
    evarp_offset[] = "x.offset",
    evarp_quality[] = "x.quality",
    evarp_timestamp[] = "x.timestamp",
    evarp_conf[] = "conf";


/**
****************************************************************************************************

  @brief Constructor.

  X...

  @return  None.

****************************************************************************************************
*/
eVariable::eVariable(
	eObject *parent,
	e_oid oid,
	os_int flags)
	: eObject(parent, oid, flags)
{
	m_vflags = OS_UNDEFINED_TYPE;
    m_value.valbuf.tmpstr = OS_NULL;
}


/**
****************************************************************************************************

  @brief Virtual destructor.

  X...

  @return  None.

****************************************************************************************************
*/
eVariable::~eVariable()
{
    /* Release any allocated memory.
     */
    clear();
}


/**
****************************************************************************************************

  @brief Add properties to class'es property set.

  The eVariable::setupclass X...

  @return  None.

****************************************************************************************************
*/
void eVariable::setupclass()
{
    const os_int cls = ECLASSID_VARIABLE;

    /* Order of these addproperty() calls is important, since eVariable itself is used to 
       describe the properties in property set. The property to set must be added to 
       property set before setting value for it. This effects only eVariable class.
     */
    addproperty (cls, EVARP_DEFAULT, evarp_default, EPRO_METADATA|EPRO_NOONPRCH, "default");
    addpropertys(cls, EVARP_TEXT, evarp_text, EPRO_METADATA|EPRO_NOONPRCH, "text");
    addpropertyl(cls, EVARP_TYPE, evarp_type, EPRO_METADATA|EPRO_NOONPRCH, "type", OS_UNDEFINED_TYPE);
    addproperty (cls, EVARP_VALUE, evarp_value, EPRO_PERSISTENT|EPRO_SIMPLE, "value");
    addpropertyl(cls, EVARP_DIGS, evarp_digs, EPRO_METADATA|EPRO_NOONPRCH|EPRO_SIMPLE, "digs", 2);
    addpropertys(cls, EVARP_UNIT, evarp_unit, EPRO_METADATA|EPRO_NOONPRCH, "unit");
    addpropertyd(cls, EVARP_MIN, evarp_min, EPRO_METADATA|EPRO_NOONPRCH, "min");
    addpropertyd(cls, EVARP_MAX, evarp_max, EPRO_METADATA|EPRO_NOONPRCH, "max");
    addpropertyl(cls, EVARP_ATTR, evarp_attr, EPRO_METADATA|EPRO_NOONPRCH, "digs", 2);
    addpropertyd(cls, EVARP_GAIN, evarp_gain, EPRO_METADATA|EPRO_NOONPRCH, "gain");
    addpropertyd(cls, EVARP_OFFSET, evarp_offset, EPRO_METADATA|EPRO_NOONPRCH, "offset");
    addproperty(cls, EVARP_QUALITY, evarp_quality, EPRO_METADATA|EPRO_NOONPRCH, "quality");
    addproperty(cls, EVARP_TIMESTAMP, evarp_timestamp, EPRO_METADATA|EPRO_NOONPRCH, "timestamp");
    addproperty(cls, EVARP_CONF, evarp_conf, EPRO_METADATA|EPRO_NOONPRCH, "conf");
}


/**
****************************************************************************************************

  @brief Get next child variable identified by oid.

  The eVariable::nextv() function returns pointer to the next child object of this object.

  @param   oid Object idenfifier. Default value EOID_CHILD specifies to count a child objects, 
		   which are not flagged as an attachment. Value EOID_ALL specifies to get count all 
           child objects, regardless wether these are attachment or not. Other values
		   specify object identifier, only children with that specified object identifier 
           are searched for. 

  @return  Pointer to the first child variable, or OS_NULL if none found.

****************************************************************************************************
*/
eVariable *eVariable::nextv(
	e_oid oid)
{
	if (mm_handle == OS_NULL) return OS_NULL;
	eHandle *h = mm_handle->next(oid);
    while (h)
    {
        if (h->object()->classid() == ECLASSID_VARIABLE) 
            return eVariable::cast(h->object());

        h = h->next(oid);
    }
    return OS_NULL;
}


/**
****************************************************************************************************

  @brief Empty the variable value.

  The clear() function clears variable value and frees ay memory allocated for the variable.

  @return  None.

****************************************************************************************************
*/
void eVariable::clear()
{
    switch (type())
    {
        /* If this is string, check if long string has been allocated in separate
           buffer.
         */
        case OS_STRING:
            /* If separate string buffer has been allocated.
             */
            if (m_vflags & EVAR_STRBUF_ALLOCATED)
            {
                osal_memory_free(m_value.strptr.ptr, m_value.strptr.allocated);
            }
            break;

        /* If variable contains object, delete it.
         */
        case OS_OBJECT:
            delete m_value.valbuf.v.o;

            /* Continues...
             */

        /* Other data types: Delete temporary string, if any.
         */
        default:
            if (m_value.valbuf.tmpstr)
            {
                osal_memory_free(m_value.valbuf.tmpstr, 
                    m_value.valbuf.tmpstr_sz);

            }
            break;
    }

    /* Mark that variable contains no data. Mark that string buffer has not 
       been allocated.
     */
	settype(OS_UNDEFINED_TYPE);
    m_value.valbuf.tmpstr = OS_NULL;
    m_vflags &= ~EVAR_STRBUF_ALLOCATED;
}


/**
****************************************************************************************************

  @brief Set integer value to variable.

  The setl() function sets an integer value to variable. Integer values are stored as os_long type.

  @param   x Integer value to set.
  @return  None.

****************************************************************************************************
*/
void eVariable::setl(
	const os_long x)
{
    /* Release any allocated memory.
     */
    clear();

    /* Save value.
     */
    m_value.valbuf.v.l = x;

    /* Set data type.
     */
	settype(OS_LONG);
}


/**
****************************************************************************************************

  @brief Set double value to variable.

  The setd() function sets a double precision floating point value to variable. 

  @param   x Value to set.
  @return  None.

****************************************************************************************************
*/
void eVariable::setd(
	const os_double x)
{
    /* Release any allocated memory.
     */
    clear();

    /* Save value.
     */
    m_value.valbuf.v.d = x;

    /* Set data type.
     */
	settype(OS_DOUBLE);
}


/**
****************************************************************************************************

  @brief Set string value to variable.

  The sets() function sets a string value to variable. 

  @param   x Value to set.
  @param   max_chars Maximum number of characters to store excluding terminating NULL character.
           -1 for unlimited.
  @return  None.

****************************************************************************************************
*/
void eVariable::sets(
	const os_char *x,
    os_memsz max_chars)
{
    os_memsz
        n,
        tmpstr_sz;

    os_char
        *tmpstr;

    /* Save temporary buffer if any, in case it is used as argument.
     */
    tmpstr = OS_NULL;
    if (type() != OS_STRING)
    {
        tmpstr = m_value.valbuf.tmpstr;
        tmpstr_sz = m_value.valbuf.tmpstr_sz;
        m_value.valbuf.tmpstr = OS_NULL;
    }

    /* Release any allocated memory.
     */
    clear();

    /* Get string length. Check if string length is limited.
     */
    n = os_strlen(x);
    if (max_chars >= 0)
    {
        if (n > max_chars+1) n = max_chars+1;
        else max_chars = -1;
    }

    /* If string fits into small buffer, copy it and save used size.
     */
    if (n <= EVARIABLE_STRBUF_SZ)
    {
        os_memcpy(m_value.strbuf.buf, x, n);
        if (max_chars >= 0) m_value.strbuf.buf[max_chars] = '\0';
        m_value.strbuf.used = (os_uchar)n;
    }

    /* Otherwise we need to allocate buffer for long string. Allocate buffer, copy data in,
       save allocated size and used size. Set EVAR_STRBUF_ALLOCATED flag to indicate that
       buffer was allocated.
     */
    else
    {
        m_value.strptr.ptr = (os_char*)osal_memory_allocate(n, &m_value.strptr.allocated);
        os_memcpy(m_value.strptr.ptr, x, n);
        if (max_chars >= 0) m_value.strptr.ptr[max_chars] = '\0';
        m_value.strptr.used = n;
        m_vflags |= EVAR_STRBUF_ALLOCATED;
    }

    /* Set data type.
     */
	settype(OS_STRING);

    /* Release temporary string buffer.
     */
    if (tmpstr) 
    {
        osal_memory_free(tmpstr, tmpstr_sz);
    }
}


/**
****************************************************************************************************

  @brief Copy or move variable value from another variable.

  The setv() function sets a double precision floating point value to variable. 

  @param  x Value to set.
  @param  move_value If OS_FALSE, the value of x is copied to this variable. If OS_TRUE and
          x contains allocated string or object, the value is moved from x to this variable
		  and X will be empty after this call.
  @return None.

****************************************************************************************************
*/
void eVariable::setv(
	eVariable *x,
    os_boolean move_value)
{
    osalTypeId
        srctype;

    os_memsz
        n;

    /* Release any allocated memory.
     */
    clear();

    if (x == OS_NULL) return;

    srctype = x->type();
    switch (srctype)
    {
        /* If already empty.
         */
        case OS_UNDEFINED_TYPE:
            return;

        /* If this is string, check if long string has been allocated in separate
           buffer.
         */
        case OS_STRING:
            /* If separate string buffer has been allocated.
             */
            if (x->m_vflags & EVAR_STRBUF_ALLOCATED)
            {
                if (move_value)
                {
                    /* Move string value.
                     */
                    m_value.strptr.ptr = x->m_value.strptr.ptr;
                    m_value.strptr.allocated = x->m_value.strptr.allocated;
                    m_value.strptr.used = x->m_value.strptr.used;

                    /* Source variable has no value.
                     */
	                x->settype(OS_UNDEFINED_TYPE);
                    x->m_vflags &= ~EVAR_STRBUF_ALLOCATED;
                }
                else
                {
                    n = x->m_value.strptr.used;
                    m_value.strptr.ptr = (os_char*)osal_memory_allocate(n, 
                        &m_value.strptr.allocated);
                    os_memcpy(m_value.strptr.ptr, x->m_value.strptr.ptr, n);
                    m_value.strptr.used = n;
                    m_vflags |= EVAR_STRBUF_ALLOCATED;
                }
            }
            else
            {
                n = x->m_value.strbuf.used;
                os_memcpy(m_value.strbuf.buf, x->m_value.strbuf.buf, n);
                m_value.strbuf.used = (os_uchar)n;

                sets(x->m_value.strbuf.buf);
            }
            break;

        /* If variable contains object, delete it.
         */
        case OS_OBJECT:
            if (move_value)
            {

            }
            else
            {

            }
            break;

        /* Other data types, just copy the value.
         */
        default:
            m_value.valbuf.v = x->m_value.valbuf.v;
            break;
    }

    /* Set data type.
     */
	settype(srctype);
}


/**
****************************************************************************************************

  @brief Set object as variable value.

  The seto() function sets an object as variable value. 

  @param  x Value to set.
  @param  adopt_x If OS_FALSE, the object x is copied to be value of this variable. If OS_TRUE,
		  the object x is adopted as value of the variable.

  @return None.

****************************************************************************************************
*/
void eVariable::seto(
	eObject *x,
    os_boolean adopt_x)
{
    /* Release any allocated memory.
     */
    clear();

    if (x == OS_NULL) return;

	if (adopt_x)
	{
		m_value.valbuf.v.o = x;
		adopt(x, 0 /* Id value */);
	}
	else
	{
		m_value.valbuf.v.o = x->clone(this, 0 /* Id value */);
	}
}

  
/**
****************************************************************************************************

  @brief Set pointer as a value to variable.

  The setd() function sets apointer value to variable. 

  @param   x Value to set.
  @return  None.

****************************************************************************************************
*/
void eVariable::setp(
    os_pointer x)
{
    /* Release any allocated memory.
     */
    clear();

    /* Save value.
     */
    m_value.valbuf.v.p = x;

    /* Set data type.
     */
	settype(OS_POINTER);
}


/**
****************************************************************************************************

  @brief Check if variable is empty.

  The isempty() function checks if variable is empty. Empty string is considered empty.

  @return OS_TRUE if variable is empty, OS_FALSE if not.

****************************************************************************************************
*/
os_boolean eVariable::isempty()
{
	os_char
		c;

	switch (type())
	{
		case OS_UNDEFINED_TYPE: 
			return OS_TRUE;

		case OS_STRING: 
            if (m_vflags & EVAR_STRBUF_ALLOCATED)
			{
				c = *m_value.strptr.ptr;
			}
			else
			{
				c = m_value.strbuf.buf[0];
			}
			return (c == '\0') ? OS_TRUE : OS_FALSE;

		default:
			return OS_FALSE;
	}
}


/**
****************************************************************************************************

  @brief Get variable value as long integer.

  The getl() function returns variable value as long integer os_long.

  @return Variable value as long integer. If variable has no value, or value cannot be converted 
		  to long integer, the function returns 0.

****************************************************************************************************
*/
os_long eVariable::getl()
{
	os_long 
		x;

    /* Convert value to long integer.
     */
    switch (type())
    {
        case OS_LONG:
            x = m_value.valbuf.v.l;
            break;

        case OS_DOUBLE:
            if (m_value.valbuf.v.d >= 0)
            {
                x = (os_long)(m_value.valbuf.v.d + 0.5);
            }
            else
            {
                x = -(os_long)(-m_value.valbuf.v.d + 0.5);
            }
            break;

        case OS_STRING:
			x = osal_string_to_int(
				(m_vflags & EVAR_STRBUF_ALLOCATED) 
				? m_value.strptr.ptr 
				: m_value.strbuf.buf, 
				OS_NULL);
            break;

        default:
            x = 0;
			break;
    }

    /* Return the value.
     */
    return x;
}

/**
****************************************************************************************************

  @brief Get variable value as double.

  The getd() function returns variable value as double precision floating point number.

  @return  Variable value. If variable has no value, or value cannot be converted to double, the
		   function returns 0.0.

****************************************************************************************************
*/
os_double eVariable::getd()
{
	os_double
		x;

    /* Convert value to double.
     */
    switch (type())
    {
        case OS_LONG:
            x = (os_double)m_value.valbuf.v.l;
            break;

        case OS_DOUBLE:
            x = m_value.valbuf.v.d;
            break;

        case OS_STRING:
            if (m_vflags & EVAR_STRBUF_ALLOCATED)
			{
				osal_strcnv_string_to_float(&x, m_value.strptr.ptr);
			}
			else
			{
				osal_strcnv_string_to_float(&x, m_value.strbuf.buf);
			}
            break;

        default:
            x = 0;
			break;
    }

    /* Return the value.
     */
    return x;
}


/**
****************************************************************************************************

  @brief Get variable value as string.

  The gets() function gets variable value as string. Integers, and floating point numbers
  are converted to string stored within temporary buffer allocated within variable.
  Temporary buffer will exists as long as variale object exit, and variable value is 
  not modified.

  @param   sz Pointer where to store number of bytes in string (including terminating null 
           character), OS_NULL if not needed.

  @return  Pointer to value as string.

****************************************************************************************************
*/
os_char *eVariable::gets(
    os_memsz *sz) 
{
    os_char
        *str,
        buf[64];

    os_memsz
        vsz;

    switch (type())
    {
        /* If empty or object.
         */
        case OS_UNDEFINED_TYPE:
            str = (os_char*)"";
            vsz = 1;
            goto getout;
        
        /* If this is string, just return pointer to the string.
         */
        case OS_STRING:
            /* If separate string buffer has been allocated.
             */
            if (m_vflags & EVAR_STRBUF_ALLOCATED)
            {
                str = m_value.strptr.ptr;
                vsz = m_value.strptr.used;
            }
            else
            {
                str = m_value.strbuf.buf;
                vsz = m_value.strbuf.used;
            }
            goto getout;

        /* We need to convert to string.
         */
        default:
            break;
    }

    /* If we already have converted value to string, no need to do anything.
     */
    if (m_value.valbuf.tmpstr)
    {
        str = m_value.valbuf.tmpstr;
        vsz = m_value.valbuf.tmpstr_sz;
        goto getout;
    }

    /* Convert to string.
     */
    switch (type())
    {
        case OS_LONG:
            vsz = osal_int_to_string(buf, sizeof(buf), m_value.valbuf.v.l);
            break;

        case OS_DOUBLE:
            vsz = osal_strcnv_float_to_string(buf, sizeof(buf), 
				m_value.valbuf.v.d, digs(), OSAL_FLOAT_DEFAULT);
            break;

        case OS_OBJECT:
            os_strncpy(buf, "?", sizeof(buf));
            vsz = os_strlen(buf);
            break;

        default:
            str = (os_char*)"";
            vsz = 1;
            goto getout;
    }

    /* Allocate buffer for temporary string and save it.
     */
    str = (os_char*)osal_memory_allocate(vsz, OS_NULL);
    os_memcpy(str, buf, vsz);
    m_value.valbuf.tmpstr = str;
    m_value.valbuf.tmpstr_sz = vsz;

getout:
    if (sz) *sz = vsz;
    return str;
}


/**
****************************************************************************************************

  @brief Release memory allocated for temporary buffer by gets() function.

  The gets_free() function frees temporary string conversion buffer, if one is allocated. 
  If gets() function needs to convert for example integer or double value to string, it 
  allocates temprary buffer for the returned string. This buffer will be deleted when the 
  variable value is modified or variable is deleted. This function may be called to release
  the temporary buffer immediately, altough this should not normally be needed.

  @return  None.

****************************************************************************************************
*/
void eVariable::gets_free()
{
    if (type() != OS_STRING && m_value.valbuf.tmpstr)
    {
        osal_memory_free(m_value.valbuf.tmpstr, 
            m_value.valbuf.tmpstr_sz);

        m_value.valbuf.tmpstr = OS_NULL;
    }
}



/**
****************************************************************************************************

  @brief Get pointer to object contained by variable.

  The geto() function returns pointer to object, if variable value is object.

  @return  Pointer to object, or OS_NULL if variable value is not object.

****************************************************************************************************
*/
eObject *eVariable::geto() 
{
	if (type() == OS_OBJECT)
	{
		return m_value.valbuf.v.o;
	}

	return OS_NULL;
}

/**
****************************************************************************************************

  @brief Get pointer value of variable.

  The getp() function returns pointer which was stored as variable value, if value type 
  is OE_POINTER.

  @return  Pointer value, or OS_NULL if variable value is pointer or is NULL pointer.

****************************************************************************************************
*/
os_pointer eVariable::getp() 
{
	if (type() == OS_POINTER)
	{
		return m_value.valbuf.v.p;
	}

	return OS_NULL;
}

/**
****************************************************************************************************

  @brief Append string value to variable.

  The appends() function appends a string to variable. After this
  call the variable always contains a string.

  @param   x Value to append. OS_NULL value is treated as empty string.
  @return  None.

****************************************************************************************************
*/
void eVariable::appends(
	const os_char *x)
{
    if (x == OS_NULL) x = "";

    appends_internal(x, os_strlen(x)-1);
}


/**
****************************************************************************************************

  @brief Append string value to variable.

  The appendv() function appends value of another variable to variable as string. After this
  call the variable always contains a string.

  @param   x Value to append. OS_NULL value is treated as empty string.
  @return  None.

****************************************************************************************************
*/
void eVariable::appendv(
	eVariable *x)
{
    os_char
        *str;

    os_memsz 
        sz;

    os_boolean
        del_tmpstr;

    if (x == OS_NULL) 
    {
        appends(OS_NULL);
        return;
    }

    del_tmpstr = !x->tmpstrallocated();

    str = x->gets(&sz);
    appends_internal(str, sz-1);

    if (del_tmpstr) x->gets_free();
}


/**
****************************************************************************************************

  @brief Compare value of this variable to another variable.

  If eVariable::compare function...

  @param   x Variable to compare to.

  @return  -1:thiso < x, 0:thiso==x,1:thiso>x.

****************************************************************************************************
*/
os_int eVariable::compare(
	eVariable *x,
	os_int flags)
{
    eVariable
        *y,
        *tmp;

    os_int
        rval = 0,
        reverse;

    os_long
        lx,
        ly;

    os_double
        dx,
        dy;

    os_char
        nbuf[32];
    
    /* Arrange by type id enum, so that type number of x is smaller than y's.
     */
    y = this;
    reverse = -1;
    if (x->type() > y->type())
    {
        tmp = x;
        x = y;
        y = tmp;
        reverse = 1;
    }

    switch (x->type())
    {
        case OS_LONG:
            switch (y->type())
            {
                case OS_LONG:
                    lx = x->m_value.valbuf.v.l;
                    ly = y->m_value.valbuf.v.l;
                    if (ly > lx) rval = 1;
                    if (ly < lx) rval = -1;
                    break;

                case OS_DOUBLE:
                    dx = (os_double)x->m_value.valbuf.v.l;
                    dy = y->m_value.valbuf.v.d;
                    if (dy > dx) rval = 1;
                    if (dy < dx) rval = -1;
                    break;

                case OS_STRING:
                    /* If string can be converted to number, compare as numbers.
                     */
                    if (y->autotype(OS_FALSE))
                    {
                        tmp = new eVariable(this);
                        tmp->setv(y);
                        tmp->autotype(OS_TRUE);
                        dx = (os_double)x->m_value.valbuf.v.l;
                        dy = tmp->getd();
                        if (dy > dx) rval = 1;
                        if (dy < dx) rval = -1;
                        delete(tmp);
                    }
                    else
                    {
                        osal_int_to_string(nbuf, sizeof(nbuf), x->m_value.valbuf.v.l);
                        rval = os_strcmp(nbuf, y->gets());
                    }
                    break;

                case OS_OBJECT:
                    rval = -1;
                    break;

                default:
                    osal_debug_error("eVariable::compare error 1");
                    break;
            }
            break;

        case OS_DOUBLE:
            switch (y->type())
            {
                case OS_DOUBLE:
                    dx = x->m_value.valbuf.v.d;
                    dy = y->m_value.valbuf.v.d;
                    if (dy > dx) rval = 1;
                    if (dy < dx) rval = -1;
                    break;

                case OS_STRING:
                    /* If string can be converted to number, compare as numbers.
                     */
                    if (y->autotype(OS_FALSE))
                    {
                        tmp = new eVariable(this);
                        tmp->setv(y);
                        tmp->autotype(OS_TRUE);
                        dx = x->m_value.valbuf.v.d;
                        dy = tmp->getd();
                        if (dy > dx) rval = 1;
                        if (dy < dx) rval = -1;
                        delete(tmp);
                    }
                    else
                    {
osal_int_to_string(nbuf, sizeof(nbuf), x->m_value.valbuf.v.d); // ???????????????????????????????
                        rval = os_strcmp(nbuf, y->gets());
                    }
                    break;

                case OS_OBJECT:
                    rval = -1;
                    break;

                default:
                    osal_debug_error("eVariable::compare error 2");
                    break;
            }
            break;

        case OS_STRING:
            switch (y->type())
            {
                case OS_STRING:
                    rval = os_strcmp(x->gets(), y->gets());
                    break;
    
                case OS_OBJECT:
                    rval = -1;
                    break;

                default:
                    osal_debug_error("eVariable::compare error 3");
                    break;
            }
            break;

        case OS_OBJECT:
            break;

        case OS_POINTER:
            if (y->type() == OS_POINTER)
            {
                if (y->m_value.valbuf.v.p > x->m_value.valbuf.v.p) rval = 1;
                if (y->m_value.valbuf.v.p < x->m_value.valbuf.v.p) rval = -1;
            }
            break;
    }
    return reverse * rval;
}


/**
****************************************************************************************************

  @brief Automatically type variable value.

  If variable contains a string, the eVariable::tostring() function checks if string is well
  defined integer or floating point number. If so, the autotype function converts variable
  to one.

  @param   modify_value value is set by default. If modify_value is set to OS_TRUE, this function 
		   will only check if variable can be automatically typed, but doesn't modify it's value.

  @return  The function returns OS_TRUE is variable value was or can be automatically typed.

****************************************************************************************************
*/
os_boolean eVariable::autotype(
	os_boolean modify_value)
{
	os_char
		*p,
		*q,
		c;

	os_double
		d;

	os_long
		l;

    os_memsz
        count;

	os_boolean
		digit_found,
		dot_found;

    /* If this variable isn't a string, do nothing.
     */
    if (type() != OS_STRING) return OS_FALSE;

    /* Pointer to string content.
     */
	p = (m_vflags & EVAR_STRBUF_ALLOCATED) ? m_value.strptr.ptr : m_value.strbuf.buf;

	/* Skip spaces in beginning
	 */
	while (osal_char_isspace(*p)) p++;

	/* If optional + or minus sign, save value start pointer, skip sign and following empty space.
	 */
	q = p;
	if (*p == '+' || *p == '-') 
	{
		p++;
		while (osal_char_isspace(*p)) p++;
	}

	/* Check the number part.
	 */
	digit_found = dot_found = OS_FALSE;
	while (OS_TRUE)
	{
		c = *p;

		/* If this is decimal digit ?
		 */
		if (osal_char_isdigit(c))
		{
			digit_found = OS_TRUE;
		}

		/* If this is dot ?
		 */
		else if (c == '.')
		{
			/* If second dot, like in date, then do not convert.
			 */
			if (dot_found) return OS_FALSE;
			dot_found = OS_TRUE;
		}

		/* If this 'e' or 'E' for exponent ?
		 */
		/* else if (c == 'e' || c == 'E')
		{
		} */

		/* Otherwise end of data. This may contain only blanks
		 */
		else
		{
			while (*p)
			{
				if (!osal_char_isspace(*p)) return OS_FALSE;
				p++;
			}
			break;
		}
	}

	if (!digit_found) return OS_FALSE;
	if (dot_found) 
	{
		if (osal_strcnv_string_to_float(&d, q))
		{
			if (modify_value) setd(d);
			return OS_TRUE;
		}
	}
	else
	{
        l = osal_string_to_int(q, &count);
		if (count)
		{
			if (modify_value) setl(l);
			return OS_TRUE;
		}
	}
	return OS_FALSE;
}


/**
****************************************************************************************************

  @brief Convert variable value to string and get string pointer.

  The eVariable::tostring() function converts variable value to string (modifies variable value, 
  if not already string) and returns pointer to string value. 

  @return  Pointer to string variable value. Never OS_NULL.

****************************************************************************************************
*/
os_char *eVariable::tostring()
{
    /* If this variable isn't a string, convert to one.
     */
    if (type() != OS_STRING)
    {
        sets(gets());
    }

    /* Return pointer to string content.
     */
    return (m_vflags & EVAR_STRBUF_ALLOCATED) ? m_value.strptr.ptr : m_value.strbuf.buf;
}


/**
****************************************************************************************************

  @brief Write variable to stream.

  The eVariable::writer() function serialized variable to stream. This writes only variable
  specific content, use eObject::write() to save also class information, attachements, etc.
  
  @param  stream The stream to write to.
  @param  flags Serialization flags.

  @return If successfull the function returns ESTATUS_SUCCESS (0). If writing object to stream
          fails, value ESTATUS_WRITING_OBJ_FAILED is returned. Assume that all nonzero values
          indicate an error.

****************************************************************************************************
*/
eStatus eVariable::writer(
    eStream *stream, 
    os_int flags) 
{
    /* Version number. Increment if new serialized items are to the object,
       and check for new version's items in read() function.
     */
    const os_int 
        version = 1;

	/* Begin the object and write version number.
     */
    if (stream->write_begin_block(version)) goto failed;

    /* Write type and number of decimal digits in flags.
     */
    if (*stream << (m_vflags & EVAR_SERIALIZATION_MASK)) goto failed;

    /* Write the value, if any.
     */
    switch (type())
    {
        case OS_LONG:
            if (*stream << m_value.valbuf.v.l) goto failed;
            break;

        case OS_DOUBLE:
            if (*stream << m_value.valbuf.v.d) goto failed;
            break;

        case OS_STRING:
            if (m_vflags & EVAR_STRBUF_ALLOCATED)
			{
                if (*stream << m_value.strptr.used - 1) goto failed;
                if (stream->write(m_value.strptr.ptr, m_value.strptr.used - 1)) goto failed;
			}
			else
			{
                if (*stream << m_value.strptr.used - 1) goto failed;
                if (stream->write(m_value.strbuf.buf, m_value.strbuf.used - 1)) goto failed;
			}
            break;

        case OS_OBJECT:
            if (m_value.valbuf.v.o->write(stream, flags)) goto failed;
            break;

        default:
			break;
    }

	/* End the object.
     */
    if (stream->write_end_block()) goto failed;

    /* Object succesfully written.
     */
    return ESTATUS_SUCCESS;

    /* Writing object failed.
     */
failed:
    return ESTATUS_WRITING_OBJ_FAILED;
}


/**
****************************************************************************************************

  @brief Read variable from stream.

  The eVariable::reader() function reads serialized variable from stream.
  This function reads only object content. To read whole object including attachments, names,
  etc, use eObject::read().
  
  @param  stream The stream to read from.
  @param  flags Serialization flags.

  @return If successfull the function returns ESTATUS_SUCCESS (0). If writing object to stream
          fails, value ESTATUS_READING_OBJ_FAILED is returned. Assume that all nonzero values
          indicate an error.

****************************************************************************************************
*/
eStatus eVariable::reader(
    eStream *stream, 
    os_int flags) 
{
    /* Version number. Used to check which versions item's are in serialized data.
     */
    os_int 
        version;

    os_short 
        vflags;

    os_long 
        sz;

    /* Release any allocated memory.
     */
    clear();

	/* Begin the object and write version number.
     */
    if (stream->read_begin_block(version)) goto failed;

    /* Write type and number of decimal digits in flags.
     */
    if (*stream >> vflags) goto failed;

    /* Read the value, if any.
     */
    switch (vflags & EVAR_TYPE_MASK)
    {
        case OS_LONG:
            if (*stream >> m_value.valbuf.v.l) goto failed;
            break;

        case OS_DOUBLE:
            if (*stream >> m_value.valbuf.v.d) goto failed;
            break;

        case OS_STRING:
            if (*stream >> sz) goto failed;

            /* If string fits into small buffer, copy it and save used size.
               Leave space for '\0' character (sz doesn not include '\0').
             */
            if (sz < EVARIABLE_STRBUF_SZ)
            {
                if (stream->read(m_value.strbuf.buf, sz)) goto failed;
                m_value.strbuf.buf[sz] = '\0';
                m_value.strbuf.used = (os_uchar)sz;
            }

            /* Otherwise we need to allocate buffer for long string. Allocate buffer, copy data in,
               save allocated size and used size. Set EVAR_STRBUF_ALLOCATED flag to indicate that
               buffer was allocated.
             */
            else
            {
                m_value.strptr.ptr = (os_char*)osal_memory_allocate(sz+1, &m_value.strptr.allocated);
                if (stream->read(m_value.strptr.ptr, sz)) goto failed;
                m_value.strptr.ptr[sz] = '\0';
                m_value.strptr.used = sz+1;
                m_vflags |= EVAR_STRBUF_ALLOCATED;
            }
            break;

        case OS_OBJECT:
            m_value.valbuf.v.o = read(stream, flags);
            if (m_value.valbuf.v.o == OS_NULL) goto failed;
            break;

        default:
			break;
    }

    /* Store data type and decimal digits.
     */
    m_vflags &= ~EVAR_SERIALIZATION_MASK;
    m_vflags |= (vflags & EVAR_SERIALIZATION_MASK);

	/* End the object.
     */
    if (stream->read_end_block()) goto failed;

    /* Object succesfully read.
     */
    return ESTATUS_SUCCESS;

    /* Reading object failed.
     */
failed:
    return ESTATUS_READING_OBJ_FAILED;
}


void eVariable::onmessage()
{
}


/**
****************************************************************************************************

  @brief Append string to variable value, internal.

  The appends_internal() function appends characters from string to variable. After this
  call the variable always contains a string.

  @param   str Pointer to the string to append.
  @param   nchard Number of characters to append. This should not include terminating null
		   character.
  @return  None.

****************************************************************************************************
*/
void eVariable::appends_internal(
    const os_char *str,
    os_memsz nchars)
{
    os_char
        *val,
        *newval;

    os_memsz 
        used,
        n,
        allocated;

    /* If this variable isn't a string, convert to one.
     */
    if (type() != OS_STRING)
    {
        sets(gets());
    }

    /* If no separate string buffer has allocated.
     */
    if ((m_vflags & EVAR_STRBUF_ALLOCATED) == 0)
    {
        val = m_value.strbuf.buf;
        used = m_value.strbuf.used;
        n = used + nchars;

        /* If all can fit into contained buffer, append and return.
         */
        if (n <= EVARIABLE_STRBUF_SZ) 
        {
            os_memcpy(val + (used - 1), str, nchars);
            val[n-1] = '\0';
            m_value.strbuf.used = (os_uchar)n;
            return;
        }
    }
    else
    {
        val = m_value.strptr.ptr;
        used = m_value.strptr.used;
        n = used + nchars;

        /* If all can fit into already allocated buffer, append and return.
         */
        if (n <= m_value.strptr.allocated)
        {
            os_memcpy(val + (used - 1), str, nchars);
            val[n-1] = '\0';
            m_value.strptr.used = n;
            return;
        }
    }

    /* Allocate and combine within new buffer.
     */
    newval = (os_char*)osal_memory_allocate(n, &allocated);
    os_memcpy(newval, val, used - 1);
    os_memcpy(newval + (used - 1), str, nchars);
    newval[n-1] = '\0';

    /* If we need to delete old buffer.
     */
    if (m_vflags & EVAR_STRBUF_ALLOCATED)
    {
        osal_memory_free(m_value.strptr.ptr, m_value.strptr.allocated);
    }

    /* Set pointer to buffer, buffer use and allocated bytes.
     */
    m_value.strptr.ptr = newval;
    m_value.strptr.used = n;
    m_value.strptr.allocated = allocated;

    /* Flag that we have allocated buffer.
     */
    m_vflags |= EVAR_STRBUF_ALLOCATED;
}
