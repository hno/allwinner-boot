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

static const float huge = 1.0e30;

float floorf(float x)
{
	s32_t i0, j0;
	u32_t i;

	GET_FLOAT_WORD(i0,x);
	j0 = ((i0 >> 23) & 0xff) - 0x7f;

	if (j0 < 23)
	{
		if (j0 < 0)
		{
			if (huge + x > (float) 0.0)
			{
				if (i0 >= 0)
				{
					i0 = 0;
				}
				else if ((i0 & 0x7fffffff) != 0)
				{
					i0 = 0xbf800000;
				}
			}
		}
		else
		{
			i = (0x007fffff) >> j0;
			if ((i0 & i) == 0)
				return x;
			if (huge + x > (float) 0.0)
			{
				if (i0 < 0)
					i0 += (0x00800000) >> j0;
				i0 &= (~i);
			}
		}
	}
	else
	{
		if (j0 == 0x80)
			return x + x;
		else
			return x;
	}

	SET_FLOAT_WORD(x,i0);
	return x;
}
