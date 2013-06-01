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

static const double xxx[] = {
	3.33333333333334091986e-01,
	1.33333333333201242699e-01,
	5.39682539762260521377e-02,
	2.18694882948595424599e-02,
	8.86323982359930005737e-03,
	3.59207910759131235356e-03,
	1.45620945432529025516e-03,
	5.88041240820264096874e-04,
	2.46463134818469906812e-04,
	7.81794442939557092300e-05,
	7.14072491382608190305e-05,
	-1.85586374855275456654e-05,
	2.59073051863633712884e-05,
	1.00000000000000000000e+00,
	7.85398163397448278999e-01,
	3.06161699786838301793e-17
};

#define	one		xxx[13]
#define	pio4	xxx[14]
#define	pio4lo	xxx[15]
#define	T		xxx

double __kernel_tan(double x, double y, int iy)
{
	double z, r, v, w, s;
	s32_t ix, hx;

	GET_HIGH_WORD(hx,x);
	ix = hx & 0x7fffffff;
	if (ix >= 0x3FE59428)
	{
		if (hx < 0)
		{
			x = -x;
			y = -y;
		}
		z = pio4 - x;
		w = pio4lo - y;
		x = z + w;
		y = 0.0;
	}
	z = x * x;
	w = z * z;

	r = T[1] + w * (T[3] + w * (T[5] + w * (T[7] + w * (T[9] + w * T[11]))));
	v = z * (T[2] + w * (T[4] + w * (T[6] + w
			* (T[8] + w * (T[10] + w * T[12])))));
	s = z * x;
	r = y + z * (s * (r + v) + y);
	r += T[0] * s;
	w = x + r;
	if (ix >= 0x3FE59428)
	{
		v = (double) iy;
		return (double) (1 - ((hx >> 30) & 2)) * (v - 2.0 * (x - (w * w / (w
				+ v) - r)));
	}
	if (iy == 1)
		return w;
	else
	{
		double a, t;
		z = w;
		SET_LOW_WORD(z,0);
		v = r - (z - x);
		t = a = -1.0 / w;
		SET_LOW_WORD(t,0);
		s = 1.0 + t * z;
		return t + a * (s + t * v);
	}
}
