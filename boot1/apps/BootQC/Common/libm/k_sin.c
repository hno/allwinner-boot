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
	half = 5.00000000000000000000e-01,
	S1 = -1.66666666666666324348e-01,
	S2 = 8.33333333332248946124e-03,
	S3 = -1.98412698298579493134e-04,
	S4 = 2.75573137070700676789e-06,
	S5 = -2.50507602534068634195e-08,
	S6 = 1.58969099521155010221e-10;

double __kernel_sin(double x, double y, int iy)
{
	double z, r, v;

	z = x * x;
	v = z * x;
	r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
	if (iy == 0)
		return x + v * (S1 + z * r);
	else
		return x - ((z * (half * y - v * r) - y) - v * S1);
}
