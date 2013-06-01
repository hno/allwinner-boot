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
#include  "ui_main_i.h"
#include  "ui_main.h"
#include  "ui_progressbar_i.h"
#include  "display_interface.h"

progressbar_t   progress;
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
__hdle boot_ui_progressbar_create(int x1, int y1, int x2, int y2)
{
	progressbar_t *progress = NULL;
	int tmp;

	progress = wBoot_malloc(sizeof(progressbar_t));
	if(!progress)
	{
		__inf("boot ui: create progressbar failed\n");

		return NULL;
	}
	if(x1 > x2)
	{
		tmp = x1;
		x1 	= x2;
		x2  = tmp;
	}
	if(y1 > y2)
	{
		tmp = y1;
		y1 	= y2;
		y2  = tmp;
	}
	progress->x1 = x1;
	progress->x2 = x2;
	progress->y1 = y1;
	progress->y2 = y2;
	progress->width  = x2 - x1;
	progress->height = y2 - y1;
	progress->st_x	 = progress->pr_x = x1 + 1;
	progress->st_y	 = progress->pr_y = y1 + 1;
	progress->frame_color 	 = UI_BOOT_GUI_WHITE;
	progress->progress_color = UI_BOOT_GUI_GREEN;
	progress->progress_ratio = 0;
	progress->thick          = 1;
	progress->frame_color 	 = UI_BOOT_GUI_GREEN;
	progress->progress_color = UI_BOOT_GUI_RED;

	return (__hdle)progress;
}

__s32 boot_ui_progressbar_config(__hdle p, int frame_color, int progress_color, int thickness)
{
	progressbar_t *progress = (progressbar_t *)p;

	if(!p)
	{
		return -1;
	}
	progress->frame_color 	 = frame_color;
	progress->progress_color = progress_color;
	progress->progress_ratio = 0;
	progress->thick          = thickness;
	progress->st_x	 		 = progress->pr_x = progress->x1 + thickness;
	progress->st_y	 		 = progress->pr_y = progress->y1 + thickness;

	return 0;
}

__s32 boot_ui_progressbar_active(__hdle p)
{
	int base_color;
	int i;
	progressbar_t *progress = (progressbar_t *)p;

	if(!p)
	{
		return -1;
	}
	base_color = boot_ui_get_color();
	boot_ui_set_color(progress->frame_color);
	for(i=0;i<progress->thick;i++)
	{
		boot_ui_draw_hollowrectangle(progress->x1+i, progress->y1+i, progress->x2-i, progress->y2-i);
	}
	boot_ui_set_color(base_color);

	return 0;
}

__s32 boot_ui_progressbar_destroy(__hdle p)
{
	progressbar_t *progress = (progressbar_t *)p;
	int base_color;

	if(!p)
	{
		return -1;
	}
	base_color = boot_ui_get_color();
	boot_ui_set_color(UI_BOOT_GUI_BLACK);
	boot_ui_draw_solidrectangle(progress->x1, progress->y1, progress->x2, progress->y2);

	boot_ui_set_color(base_color);

	wBoot_free(progress);

	return 0;
}

__s32 boot_ui_progressbar_upgrate(__hdle p, int rate)
{
	progressbar_t *progress = (progressbar_t *)p;
	int base_color, progresscolor;
	int pixel;
	int x1, y1;
	int x2, y2;

	if((rate < 0) || (rate > 100))
	{
		__inf("boot ui progressbar: invalid progressbar rate\n");
		return -1;
	}
	if(!p)
	{
		__inf("boot ui progressbar: invalid progressbar pointer\n");
		return -1;
	}
	pixel = (rate * (progress->width - progress->thick*2)/100);
	if(rate == progress->progress_ratio)
	{
		return 0;
	}
	else
	{
		x1 = progress->pr_x;
		x2 = progress->st_x + pixel;
		progresscolor = (rate > progress->progress_ratio)?(progress->progress_color):(UI_BOOT_GUI_BLACK);
		progress->pr_x  = x2;
		progress->progress_ratio = rate;

	}
	base_color = boot_ui_get_color();
	boot_ui_set_color(progresscolor);
	y1 = progress->y1 + progress->thick;
	y2 = progress->y2 - progress->thick;

	boot_ui_draw_solidrectangle(x1, y1, x2, y2);

	boot_ui_set_color(base_color);

	return 0;
}


