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

static const double one = 1.0, Zero[] = {0.0, -0.0,};

double fmod(double x, double y)
{
	s32_t n, hx, hy, hz, ix, iy, sx, i;
	u32_t lx, ly, lz;

	EXTRACT_WORDS(hx,lx,x);
	EXTRACT_WORDS(hy,ly,y);
	sx = hx & 0x80000000;
	hx ^= sx;
	hy &= 0x7fffffff;

	if ((hy | ly) == 0 || (hx >= 0x7ff00000) ||
		((hy | ((ly | -ly) >> 31)) > 0x7ff00000))
		return (x * y) / (x * y);
	if (hx <= hy)
	{
		if ((hx < hy) || (lx < ly))
			return x;
		if (lx == ly)
			return Zero[(u32_t) sx >> 31];
	}

	if (hx < 0x00100000)
	{
		if (hx == 0)
		{
			for (ix = -1043, i = lx; i > 0; i <<= 1)
				ix -= 1;
		}
		else
		{
			for (ix = -1022, i = (hx << 11); i > 0; i <<= 1)
				ix -= 1;
		}
	}
	else
		ix = (hx >> 20) - 1023;

	if (hy < 0x00100000)
	{
		if (hy == 0)
		{
			for (iy = -1043, i = ly; i > 0; i <<= 1)
				iy -= 1;
		}
		else
		{
			for (iy = -1022, i = (hy << 11); i > 0; i <<= 1)
				iy -= 1;
		}
	}
	else
		iy = (hy >> 20) - 1023;

	if (ix >= -1022)
		hx = 0x00100000 | (0x000fffff & hx);
	else
	{
		n = -1022 - ix;
		if (n <= 31)
		{
			hx = (hx << n) | (lx >> (32 - n));
			lx <<= n;
		}
		else
		{
			hx = lx << (n - 32);
			lx = 0;
		}
	}
	if (iy >= -1022)
		hy = 0x00100000 | (0x000fffff & hy);
	else
	{
		n = -1022 - iy;
		if (n <= 31)
		{
			hy = (hy << n) | (ly >> (32 - n));
			ly <<= n;
		}
		else
		{
			hy = ly << (n - 32);
			ly = 0;
		}
	}

	n = ix - iy;
	while (n--)
	{
		hz = hx - hy;
		lz = lx - ly;
		if (lx < ly)
			hz -= 1;
		if (hz < 0)
		{
			hx = hx + hx + (lx >> 31);
			lx = lx + lx;
		}
		else
		{
			if ((hz | lz) == 0)
				return Zero[(u32_t) sx >> 31];
			hx = hz + hz + (lz >> 31);
			lx = lz + lz;
		}
	}
	hz = hx - hy;
	lz = lx - ly;
	if (lx < ly)
		hz -= 1;
	if (hz >= 0)
	{
		hx = hz;
		lx = lz;
	}

	if ((hx | lx) == 0)
		return Zero[(u32_t) sx >> 31];
	while (hx < 0x00100000)
	{
		hx = hx + hx + (lx >> 31);
		lx = lx + lx;
		iy -= 1;
	}
	if (iy >= -1022)
	{
		hx = ((hx - 0x00100000) | ((iy + 1023) << 20));
		INSERT_WORDS(x,hx|sx,lx);
	}
	else
	{
		n = -1022 - iy;
		if (n <= 20)
		{
			lx = (lx >> n) | ((u32_t) hx << (32 - n));
			hx >>= n;
		}
		else if (n <= 31)
		{
			lx = (hx << (32 - n)) | (lx >> n);
			hx = sx;
		}
		else
		{
			lx = hx >> (n - 32);
			hx = sx;
		}
		INSERT_WORDS(x,hx|sx,lx);
		x *= one;
	}
	return x;
}
