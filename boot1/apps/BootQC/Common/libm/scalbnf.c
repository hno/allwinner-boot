/*
* (C) Copyright 2007-2013
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Martin zheng <zhengjiewen@allwinnertech.com>
*
* See file CREDITS for list of people who contributed to this
* project.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
*/

#include <math.h>

static const float
	two25 = 3.355443200e+07,
	twom25 = 2.9802322388e-08,
	huge = 1.0e+30, tiny = 1.0e-30;

float scalbnf(float x, int n)
{
	s32_t k, ix;

	GET_FLOAT_WORD(ix,x);
	k = (ix & 0x7f800000) >> 23;
	if (k == 0)
	{
		if ((ix & 0x7fffffff) == 0)
			return x;
		x *= two25;
		GET_FLOAT_WORD(ix,x);
		k = ((ix & 0x7f800000) >> 23) - 25;
		if (n < -50000)
			return tiny * x;
	}
	if (k == 0xff)
		return x + x;
	k = k + n;
	if (k > 0xfe)
		return huge * copysignf(huge, x);
	if (k > 0)
	{
		SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));
		return x;
	}
	if (k <= -25)
	{
		if (n > 50000)
			return huge * copysignf(huge, x);
		else
			return tiny * copysignf(tiny, x);
	}
	k += 25;
	SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));

	return x * twom25;
}

float ldexpf(float x, int n)
{
	return scalbnf(x, n);
}
