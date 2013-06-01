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

static const float one = 1.0, two = 2.0, tiny = 1.0e-30, huge = 1.0e30;

float tanhf(float x)
{
	float t, z;
	s32_t jx, ix;

	GET_FLOAT_WORD(jx,x);
	ix = jx & 0x7fffffff;

	if (ix >= 0x7f800000)
	{
		if (jx >= 0)
			return one / x + one;
		else
			return one / x - one;
	}

	if (ix < 0x41100000)
	{
		if (ix < 0x39800000)
		{
			if (huge + x > one)
				return x;
		}
		if (ix >= 0x3f800000)
		{
			t = expm1f(two * fabsf(x));
			z = one - two / (t + two);
		}
		else
		{
			t = expm1f(-two * fabsf(x));
			z = -t / (t + two);
		}
	}
	else
	{
		z = one - tiny;
	}
	return (jx >= 0) ? z : -z;
}

