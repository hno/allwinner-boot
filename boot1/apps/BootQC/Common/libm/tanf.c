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
	t1pio2 = 1 * M_PI_2,
	t2pio2 = 2 * M_PI_2,
	t3pio2 = 3 * M_PI_2,
	t4pio2 = 4 * M_PI_2;

float tanf(float x)
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
		return __kernel_tandf(x, 1);
	}
	if (ix <= 0x407b53d1)
	{
		if (ix <= 0x4016cbe3)
			return __kernel_tandf(x + (hx > 0 ? -t1pio2 : t1pio2), -1);
		else
			return __kernel_tandf(x + (hx > 0 ? -t2pio2 : t2pio2), 1);
	}
	if (ix <= 0x40e231d5)
	{
		if (ix <= 0x40afeddf)
			return __kernel_tandf(x + (hx > 0 ? -t3pio2 : t3pio2), -1);
		else
			return __kernel_tandf(x + (hx > 0 ? -t4pio2 : t4pio2), 1);
	}
	else if (ix >= 0x7f800000)
		return x - x;
	else
	{
		n = __ieee754_rem_pio2f(x, y);
		return __kernel_tandf((double) y[0] + y[1], 1 - ((n & 1) << 1));
	}
}

