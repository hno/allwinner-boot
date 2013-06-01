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

static const double
	s1pio2 = 1 * M_PI_2,
	s2pio2 = 2 * M_PI_2,
	s3pio2 = 3 * M_PI_2,
	s4pio2 = 4 * M_PI_2;

float sinf(float x)
{
	float y[2];
	s32_t n, hx, ix;

	GET_FLOAT_WORD(hx,x);
	ix = hx & 0x7fffffff;

	if (ix <= 0x3f490fda)
	{
		if (ix < 0x39800000)
			if (((int) x) == 0)
				return x;
		return __kernel_sindf(x);
	}

	if (ix <= 0x407b53d1)
	{
		if (ix <= 0x4016cbe3)
		{
			if (hx > 0)
				return __kernel_cosdf(x - s1pio2);
			else
				return -__kernel_cosdf(x + s1pio2);
		}
		else
			return __kernel_sindf((hx > 0 ? s2pio2 : -s2pio2) - x);
	}

	if (ix <= 0x40e231d5)
	{
		if (ix <= 0x40afeddf)
		{
			if (hx > 0)
				return -__kernel_cosdf(x - s3pio2);
			else
				return __kernel_cosdf(x + s3pio2);
		}
		else
			return __kernel_sindf(x + (hx > 0 ? -s4pio2 : s4pio2));
	}
	else if (ix >= 0x7f800000)
		return x - x;
	else
	{
		n = __ieee754_rem_pio2f(x, y);
		switch (n & 3)
		{
		case 0:
			return __kernel_sindf((double) y[0] + y[1]);
		case 1:
			return __kernel_cosdf((double) y[0] + y[1]);
		case 2:
			return __kernel_sindf(-(double) y[0] - y[1]);
		default:
			return -__kernel_cosdf((double) y[0] + y[1]);
		}
	}
}

