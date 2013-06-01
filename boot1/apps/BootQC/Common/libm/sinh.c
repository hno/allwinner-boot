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

static const double one = 1.0, shuge = 1.0e307;

double sinh(double x)
{
	double t, w, h;
	s32_t ix, jx;
	u32_t lx;

	GET_HIGH_WORD(jx,x);
	ix = jx & 0x7fffffff;

	if (ix >= 0x7ff00000)
		return x + x;

	h = 0.5;
	if (jx < 0)
		h = -h;

	if (ix < 0x40360000)
	{
		if (ix < 0x3e300000)
			if (shuge + x > one)
				return x;
		t = expm1(fabs(x));
		if (ix < 0x3ff00000)
			return h * (2.0 * t - t * t / (t + one));
		return h * (t + t / (t + one));
	}

	if (ix < 0x40862E42)
		return h * exp(fabs(x));

	GET_LOW_WORD(lx,x);
	if (ix < 0x408633CE || ((ix == 0x408633ce)
			&& (lx <= (u32_t) 0x8fb9f87d)))
	{
		w = exp(0.5 * fabs(x));
		t = h * w;
		return t * w;
	}

	return x * shuge;
}

