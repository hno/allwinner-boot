/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include  "ui_main_i.h"
#include  "display_interface.h"
#include  "math.h"
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_draw_hollowrectangle(__s32 x1, __s32 y1, __s32 x2, __s32 y2)
{
	int   end_x, end_y;
	int   start_x, start_y;
	char *base1, *base2;
	int   x, y, tmp;
	int   line_offset;

	end_x   = x1;// * 800/ui_source.screen_width;
	end_y   = y1;// * 480/ui_source.screen_height;
	start_x = x2;// * 800/ui_source.screen_width;
	start_y = y2;// * 480/ui_source.screen_height;

	//确保起点比较小，方便计算
	if(start_y > end_y)
	{
		tmp 	= start_y;
		start_y = end_y;
		end_y   = tmp;
	}
	if(start_x > end_x)
	{
		tmp 	= start_x;
		start_x = end_x;
		end_x   = tmp;
	}
	//开始绘线
	base1  = ui_source.screen_buf + (ui_source.screen_width * start_y + start_x)* 4;
	base2  = base1 + (end_y - start_y) * ui_source.screen_width * 4;
	for(x=start_x;x<=end_x;x++)
	{
		*((int *)base1) = ui_source.color;
		*((int *)base2) = ui_source.color;
		base1 += 4;
		base2 += 4;
	}
	base1  = ui_source.screen_buf + (ui_source.screen_width * start_y + start_x)* 4;
	base2  = base1 + (end_x - start_x) * 4;
	line_offset = ui_source.screen_width * 4;
	for(y=start_y;y<=end_y;y++)
	{
		*((int *)base1) = ui_source.color;
		*((int *)base2) = ui_source.color;
		base1 += line_offset;
		base2 += line_offset;
	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_draw_solidrectangle(__s32 x1, __s32 y1, __s32 x2, __s32 y2)
{
	int   end_x, end_y;
	int   start_x, start_y;
	char *base1, *base2;
	char *base1_1, *base2_1;
	int   x, y, tmp;

	end_x   = x1;// * 800/ui_source.screen_width;
	end_y   = y1;// * 480/ui_source.screen_height;
	start_x = x2;// * 800/ui_source.screen_width;
	start_y = y2;// * 480/ui_source.screen_height;

	//确保起点比较小，方便计算
	if(start_y > end_y)
	{
		tmp 	= start_y;
		start_y = end_y;
		end_y   = tmp;
	}
	if(start_x > end_x)
	{
		tmp 	= start_x;
		start_x = end_x;
		end_x   = tmp;
	}
	//开始绘线
//	base1 = ui_source.screen_buf + (ui_source.screen_width * start_y + start_x) * 4;
//	base2 = base1 + ui_source.screen_width * 4;
//	for(y=end_y;y>=start_y;y--)
//	{
//		for(x=end_x;x>=start_x;x--)
//		{
//			*((int *)base1) = ui_source.color;
//			base1 += 4;
//		}
//		base1  = base2;
//		base2 += ui_source.screen_width * 4;
//	}

	base1 = ui_source.screen_buf + (ui_source.screen_width * start_y + start_x) * 4;
	base2 = base1 + ui_source.screen_width * (end_y - start_y) * 4;
	base1_1 = base1;
	base2_1 = base2;
	for(x=end_x;x>=start_x;x--)
	{
		for(y=end_y;y>=start_y;y-=2)
		{
			*((int *)base1_1) = ui_source.color;
			*((int *)base2_1) = ui_source.color;
			base1_1 += ui_source.screen_width * 4;
			base2_1 -= ui_source.screen_width * 4;
		}
		base1 += 4;
		base2 += 4;
		base1_1 = base1;
		base2_1 = base2;
	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_draw_line(__s32 x1, __s32 y1, __s32 x2, __s32 y2)
{
	int   end_x, end_y;
	int   start_x, start_y;
	char *base;
	int   x, y, tmp;
	int   x_tmp, y_tmp;
	int   x_offset, y_offset;
	float a, b;

	start_x = x1;// * 800/ui_source.screen_width;
	start_y = y1;// * 480/ui_source.screen_height;
	end_x 	= x2;// * 800/ui_source.screen_width;
	end_y 	= y2;// * 480/ui_source.screen_height;

	base   = ui_source.screen_buf;
	//开始绘制线条
	if(end_x == start_x)
	{
		base += (ui_source.screen_width * start_y + start_x)* 4;
		if(start_y > end_y)
		{
			tmp 	= start_y;
			start_y = end_y;
			end_y   = tmp;
		}
		for(y=start_y;y<=end_y;y++)
		{
			*((int *)base) = ui_source.color;
			base += ui_source.screen_width * 4;
		}
	}
	else if(end_y == start_y)
	{
		base += (ui_source.screen_width * start_y + start_x)* 4;
		if(start_x > end_x)
		{
			tmp 	= start_x;
			start_x = end_x;
			end_x   = tmp;
		}
		for(x=start_x;x<=end_x;x++)
		{
			*((int *)base) = ui_source.color;
			base += 4;
		}
	}
	else
	{
		a = (start_y - end_y)*1.0/(start_x - end_x);
		b = (start_x * end_y - end_x * start_y)*1.0/(start_x - end_x);

		if(abs(end_x - start_x) > abs(end_y - start_y))
		{
			if(end_x < start_x)
			{
				x_tmp   = end_x;
				end_x   = start_x;
				start_x = x_tmp;

				y_tmp   = end_y;
				end_y   = start_y;
				start_y = y_tmp;
			}
			if(end_y < start_y)
			{
				y_offset = -ui_source.screen_width * 4;
			}
			else
			{
				y_offset =  ui_source.screen_width * 4;
			}
			y_tmp = (int)(a * start_x + b);
			base += (ui_source.screen_width * start_y + start_x) * 4;
			*((int *)base) = ui_source.color;
			for(x=start_x+1;x<=end_x;x++)
			{
				y = (int)(a * x + b);

				if(y != y_tmp)
				{
					y_tmp = y;
					base += y_offset;
				}
				base += 4;
				*((int *)base) = ui_source.color;
			}
		}
		else
		{
			if(end_y < start_y)
			{
				y_tmp   = end_y;
				end_y   = start_y;
				start_y = y_tmp;

				x_tmp   = end_x;
				end_x   = start_x;
				start_x = x_tmp;
			}
			if(end_x < start_x)
			{
				x_offset = -4;
			}
			else
			{
				x_offset = 4;
			}
			y_tmp = (int)((start_y - b)/a);
			tmp = ui_source.screen_width * 4;
			base += (ui_source.screen_width * start_y + start_x) * 4;
			*((int *)base) = ui_source.color;
			for(y=start_y+1;y<=end_y;y++)
			{
				x = (int)((y - b)/a);

				if(x != x_tmp)
				{
					x_tmp = x;
					base += x_offset;
				}
				base += tmp;
				*((int *)base) = ui_source.color;
			}
		}
	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_draw_lineto(__s32 px, __s32 py)
{
	int   end_x, end_y;

	end_x   = px;// * 800/ui_source.screen_width;
	end_y   = py;// * 480/ui_source.screen_height;
	boot_ui_draw_line(ui_source.this_x, ui_source.this_y, end_x, end_y);
	ui_source.this_x = end_x;
	ui_source.this_y = end_y;

	return 0;
}
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_draw_point(__s32 x, __s32 y)
{
	int   scr_x, scr_y;
	char *base;

	scr_x  = x;// * 800/ui_source.screen_width;
	scr_y  = y;// * 480/ui_source.screen_height;

	base  = ui_source.screen_buf + (ui_source.screen_width * scr_y + scr_x)* 4;
	*((int *)base) = ui_source.color;

	ui_source.this_x = scr_x;
	ui_source.this_y = scr_y;

	return 0;
}
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_set_color(__s32 color)
{
	ui_source.color = color;

	return 0;
}
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_get_color(void)
{
	return ui_source.color;
}
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_moveto(int x, int y)
{
	int   scr_x, scr_y;

	scr_x  = x;// * 800/ui_source.screen_width;
	scr_y  = y;// * 480/ui_source.screen_height;

	ui_source.this_x = scr_x;
	ui_source.this_y = scr_y;

	return 0;
}
/*
************************************************************************************************************
*
*                                             ui_show_data
*
*    函数名称：ui_show_data
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：在屏幕的某个位置显示传进的数据
*
*
************************************************************************************************************
*/
__s32 boot_ui_clear(void)
{
	memset(ui_source.screen_buf, 0, ui_source.screen_size);

	return 0;
}

