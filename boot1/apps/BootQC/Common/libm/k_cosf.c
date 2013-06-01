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
	one = 1.0,
	C0 = -0x1ffffffd0c5e81.0p-54,
	C1 = 0x155553e1053a42.0p-57,
	C2 = -0x16c087e80f1e27.0p-62,
	C3 = 0x199342e0ee5069.0p-68;

float __kernel_cosdf(double x)
{
	double r, w, z;

	z = x * x;
	w = z * z;
	r = C2 + z * C3;
	return ((one + z * C0) + w * C1) + (w * z) * r;
}
