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

static const double huge = 1.0e300;

double ceil(double x)
{
	s32_t i0, i1, j0;
	u32_t i, j;

	EXTRACT_WORDS(i0,i1,x);
	j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;

	if (j0 < 20)
	{
		if (j0 < 0)
		{
			if (huge + x > 0.0)
			{
				if (i0 < 0)
				{
					i0 = 0x80000000;
					i1 = 0;
				}
				else if ((i0 | i1) != 0)
				{
					i0 = 0x3ff00000;
					i1 = 0;
				}
			}
		}
		else
		{
			i = (0x000fffff) >> j0;
			if (((i0 & i) | i1) == 0)
				return x;
			if (huge + x > 0.0)
			{
				if (i0 > 0)
					i0 += (0x00100000) >> j0;
				i0 &= (~i);
				i1 = 0;
			}
		}
	}
	else if (j0 > 51)
	{
		if (j0 == 0x400)
			return x + x;
		else
			return x;
	}
	else
	{
		i = ((u32_t)(0xffffffff)) >> (j0 - 20);
		if ((i1 & i) == 0)
			return x;
		if (huge + x > 0.0)
		{
			if (i0 > 0)
			{
				if (j0 == 20)
					i0 += 1;
				else
				{
					j = i1 + (1 << (52 - j0));
					if (j < i1)
						i0 += 1;
					i1 = j;
				}
			}
			i1 &= (~i);
		}
	}

	INSERT_WORDS(x,i0,i1);
	return x;
}
