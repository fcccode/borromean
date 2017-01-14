/**

  @file    serialize/common/osal_intser.c
  @brief   Serialize integers.
  @author  Pekka Lehtikoski
  @version 1.0
  @date    9.11.2011

  Pack and unpack integers for serialization.

  Copyright 2012 Pekka Lehtikoski. This file is part of the eobjects project and shall only be used,
  modified, and distributed under the terms of the project licensing. By continuing to use, modify,
  or distribute this file you indicate that you have read the license and understand and accept
  it fully.

****************************************************************************************************
*/
#include "eosal/eosalx.h"


/**
****************************************************************************************************

  @brief Convert integer to packed serial format.
  @anchor osal_intser_writer

  The osal_intser_writer() function pack integer x to serialization format. Serialization 
  format is packed by integer value size. Packed format is type independent, number of 
  generated bytes depends on value, not data type.

  First byte NNNSxxxx:
  - NNN number of follower bytes.
  - S sign
  - xxxx Least significant 4 data bits.

  @param  buf Bufffer where to store serialized integer, minimum OSAL_INTSER_BUF_SZ (10) 
          recommended.
  @param  x Integer to pack.
  @return Number of bytes stored in buf. 

****************************************************************************************************
*/
os_int osal_intser_writer(
	os_char *buf,
	os_long x)
{
	os_int
		n;

	os_char 
		*p;

	if (x < 0x10)
	{
		if (x >= 0)
		{
			*buf = (os_char)x;
			return 1;
		}

		x = -x;
		if (x < 0x10)
		{
			*buf = (os_char)x | 0x10;
			return 1;
		}

		*buf = (os_char)(x & 0xF) | 0x10;
	}
	else
	{
		*buf = (os_char)x & 0xF;
	}

	p = buf;
	x >>= 4;

	do
	{
		*(++p) = (os_char)x;
		x >>= 8;
	}
	while (x > 0);

	n = (os_int)(p - buf);
	*buf |= ((os_char)n << 5);

	return n+1;
}


/**
****************************************************************************************************

  @brief Get integer from packed serial format.
  @anchor osal_intser_reader

  The osal_intser_reader() function converts serialzed integer to C integer x.

  @param  buf Bufffer from which to parse integer.
  @param  x Pointer where store the C integer.
  @return Number of bytes parsed from buf. 

****************************************************************************************************
*/
os_int osal_intser_reader(
	os_char *buf,
	os_long *x)
{
	os_int 
		shift;

	os_char
		c,
		count,
		*p;

	os_long
		y;

	c = *buf;

	if ((c & 0xE0) == 0)
	{
		*x = (c & 0x10) ? -(c & 0xF) : c;
		return 1;
	}

	y = c & 0xF;
	count = c >> 5;
	shift = 4;
	p = buf;
	while (count--)
	{
		y |= ((os_long)*(++p)) << shift;
		shift += 8;
	}

	if (c & 0x10) *x = -y;
	else *x = y;

	return (os_int)(p - buf + 1);
}
