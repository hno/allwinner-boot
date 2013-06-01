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
	two54 = 1.80143985094819840000e+16,
	ivln10 = 4.34294481903251816668e-01,
	log10_2hi = 3.01029995663611771306e-01,
	log10_2lo = 3.69423907715893078616e-13;

static const double zero = 0.0;

double log10(double x)
{
	double y, z;
	s32_t i, k, hx;
	u32_t lx;

	EXTRACT_WORDS(hx,lx,x);

	k = 0;
	if (hx < 0x00100000)
	{
		if (((hx & 0x7fffffff) | lx) == 0)
			return -two54 / zero;
		if (hx < 0)
			return (x - x) / zero;
		k -= 54;
		x *= two54;
		GET_HIGH_WORD(hx,x);
	}
	if (hx >= 0x7ff00000)
		return x + x;
	k += (hx >> 20) - 1023;
	i = ((u32_t) k & 0x80000000) >> 31;
	hx = (hx & 0x000fffff) | ((0x3ff - i) << 20);
	y = (double) (k + i);
	SET_HIGH_WORD(x,hx);
	z = y * log10_2lo + ivln10 * log(x);
	return z + y * log10_2hi;
}

