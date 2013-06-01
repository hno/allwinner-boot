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

double hypot(double x, double y)
{
	double a = x, b = y, t1, t2, y1, y2, w;
	s32_t j, k, ha, hb;

	GET_HIGH_WORD(ha, x);
	ha &= 0x7fffffff;
	GET_HIGH_WORD(hb, y);
	hb &= 0x7fffffff;
	if (hb > ha)
	{
		a = y;
		b = x;
		j = ha;
		ha = hb;
		hb = j;
	}
	else
	{
		a = x;
		b = y;
	}
	SET_HIGH_WORD(a, ha);
	SET_HIGH_WORD(b, hb);
	if ((ha - hb) > 0x3c00000)
	{
		return a + b;
	}
	k = 0;
	if (ha > 0x5f300000)
	{
		if (ha >= 0x7ff00000)
		{
			u32_t low;
			w = a + b;
			GET_LOW_WORD(low, a);
			if (((ha & 0xfffff) | low) == 0)
				w = a;
			GET_LOW_WORD(low, b);
			if (((hb ^ 0x7ff00000) | low) == 0)
				w = b;
			return w;
		}
		ha -= 0x25800000;
		hb -= 0x25800000;
		k += 600;
		SET_HIGH_WORD(a, ha);
		SET_HIGH_WORD(b, hb);
	}
	if (hb < 0x20b00000)
	{
		if (hb <= 0x000fffff)
		{
			u32_t low;
			GET_LOW_WORD(low, b);
			if ((hb | low) == 0)
				return a;
			t1 = 0;
			SET_HIGH_WORD(t1, 0x7fd00000);
			b *= t1;
			a *= t1;
			k -= 1022;
		}
		else
		{
			ha += 0x25800000;
			hb += 0x25800000;
			k -= 600;
			SET_HIGH_WORD(a, ha);
			SET_HIGH_WORD(b, hb);
		}
	}
	w = a - b;
	if (w > b)
	{
		t1 = 0;
		SET_HIGH_WORD(t1, ha);
		t2 = a - t1;
		w = sqrt(t1 * t1 - (b * (-b) - t2 * (a + t1)));
	}
	else
	{
		a = a + a;
		y1 = 0;
		SET_HIGH_WORD(y1, hb);
		y2 = b - y1;
		t1 = 0;
		SET_HIGH_WORD(t1, ha+0x00100000);
		t2 = a - t1;
		w = sqrt(t1 * y1 - (w * (-w) - (t1 * y2 + t2 * b)));
	}
	if (k != 0)
	{
		u32_t high;
		t1 = 1.0;
		GET_HIGH_WORD(high, t1);
		SET_HIGH_WORD(t1, high+(k<<20));
		return t1 * w;
	}
	else
		return w;
}

