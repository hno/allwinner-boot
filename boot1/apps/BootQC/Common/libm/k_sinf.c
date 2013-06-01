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
	S1 = -0x15555554cbac77.0p-55,
	S2 = 0x111110896efbb2.0p-59,
	S3 = -0x1a00f9e2cae774.0p-65,
	S4 = 0x16cd878c3b46a7.0p-71;

float __kernel_sindf(double x)
{
	double r, s, w, z;

	z = x * x;
	w = z * z;
	r = S3 + z * S4;
	s = z * x;
	return (x + s * (S1 + z * S2)) + s * w * r;
}
