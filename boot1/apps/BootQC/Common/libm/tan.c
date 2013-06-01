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

double tan(double x)
{
	double y[2], z = 0.0;
	s32_t n, ix;

	GET_HIGH_WORD(ix,x);

	ix &= 0x7fffffff;
	if (ix <= 0x3fe921fb)
	{
		if (ix < 0x3e300000)
			if ((int) x == 0)
				return x;
		return __kernel_tan(x, z, 1);
	}
	else if (ix >= 0x7ff00000)
		return x - x;
	else
	{
		n = __ieee754_rem_pio2(x, y);
		return __kernel_tan(y[0], y[1], 1 - ((n & 1) << 1));
	}
}

