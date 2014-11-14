
#include "lcd_panel_cfg.h"
#include "lcd_bak/lcd_edp_anx9804.h"
#include "lcd_bak/lcd_edp_anx6345.h"
#include "lcd_bak/lcd_B079XAN01.h"
#include "lcd_bak/lcd_vvx07h005a00.h"

static void LCD_panel_init(__u32 sel);
static void LCD_panel_exit(__u32 sel);

static void LCD_cfg_panel_info(__panel_extend_para_t * info)
{
    __u32 i = 0, j=0;
    __u32 items;
    __u8 lcd_gamma_tbl[][2] = 
    {
    //{input value, corrected value}
        {0, 0},
        {15, 15},
        {30, 30},
        {45, 45},
        {60, 60},
        {75, 75},
        {90, 90},
        {105, 105},
        {120, 120},
        {135, 135},
        {150, 150},
        {165, 165},
        {180, 180},
        {195, 195},
        {210, 210},
        {225, 225},
        {240, 240},
        {255, 255},
    };

    __u8 lcd_bright_curve_tbl[][2] = 
    {
        //{input value, corrected value}
        {0    ,0  },//0
        {15   ,3  },//0
        {30   ,6  },//0
        {45   ,9  },// 1
        {60   ,12  },// 2
        {75   ,16  },// 5
        {90   ,22  },//9
        {105   ,28 }, //15
        {120  ,36 },//23
        {135  ,44 },//33
        {150  ,54 },
        {165  ,67 },
        {180  ,84 },
        {195  ,108}, 
        {210  ,137}, 
        {225 ,171}, 
        {240 ,210}, 
        {255 ,255},
    };

    __u32 lcd_cmap_tbl[2][3][4] = {
    		{
                {LCD_CMAP_G0,LCD_CMAP_B1,LCD_CMAP_G2,LCD_CMAP_B3},
        		{LCD_CMAP_B0,LCD_CMAP_R1,LCD_CMAP_B2,LCD_CMAP_R3},
        		{LCD_CMAP_R0,LCD_CMAP_G1,LCD_CMAP_R2,LCD_CMAP_G3},
            },
    		{
                {LCD_CMAP_B3,LCD_CMAP_G2,LCD_CMAP_B1,LCD_CMAP_G0},
        		{LCD_CMAP_R3,LCD_CMAP_B2,LCD_CMAP_R1,LCD_CMAP_B0},
        		{LCD_CMAP_G3,LCD_CMAP_R2,LCD_CMAP_G1,LCD_CMAP_R0},
            },
    };

    memset(info,0,sizeof(__panel_extend_para_t));

    items = sizeof(lcd_gamma_tbl)/2;   
    for(i=0; i<items-1; i++)
    {
        __u32 num = lcd_gamma_tbl[i+1][0] - lcd_gamma_tbl[i][0];

        for(j=0; j<num; j++)
        {
            __u32 value = 0;

            value = lcd_gamma_tbl[i][1] + ((lcd_gamma_tbl[i+1][1] - lcd_gamma_tbl[i][1]) * j)/num;
            info->lcd_gamma_tbl[lcd_gamma_tbl[i][0] + j] = (value<<16) + (value<<8) + value;
        }
    }
    info->lcd_gamma_tbl[255] = (lcd_gamma_tbl[items-1][1]<<16) + (lcd_gamma_tbl[items-1][1]<<8) + lcd_gamma_tbl[items-1][1];

    items = sizeof(lcd_bright_curve_tbl)/2;   
    for(i=0; i<items-1; i++)
    {
        __u32 num = lcd_bright_curve_tbl[i+1][0] - lcd_bright_curve_tbl[i][0];

        for(j=0; j<num; j++)
        {
            __u32 value = 0;

            value = lcd_bright_curve_tbl[i][1] + ((lcd_bright_curve_tbl[i+1][1] - lcd_bright_curve_tbl[i][1]) * j)/num;
            info->lcd_bright_curve_tbl[lcd_bright_curve_tbl[i][0] + j] = value;
        }
    }
    info->lcd_bright_curve_tbl[255] = lcd_bright_curve_tbl[items-1][1];

    memcpy(info->lcd_cmap_tbl, lcd_cmap_tbl, sizeof(lcd_cmap_tbl));
}
static __s32 LCD_open_flow(__u32 sel)
{
	LCD_OPEN_FUNC(sel, LCD_power_on, 0);   //open lcd power, and delay 50ms
	LCD_OPEN_FUNC(sel, LCD_panel_init, 10);   //open lcd power, than delay 200ms
	LCD_OPEN_FUNC(sel, TCON_open, 50);     //open lcd controller, and delay 100ms
	LCD_OPEN_FUNC(sel, LCD_bl_open, 0);     //open lcd backlight, and delay 0ms

	return 0;
}

static __s32 LCD_open_flow1(__u32 sel)
{
	LCD_OPEN_FUNC(sel, LCD_power_on, 0);   //open lcd power, and delay 50ms
	LCD_OPEN_FUNC(sel, LCD_panel_init, 10);   //open lcd power, than delay 200ms
	LCD_OPEN_FUNC(sel, TCON_open, 50);     //open lcd controller, and delay 500ms
	LCD_OPEN_FUNC(sel, LCD_bl_open, 0);     //open lcd backlight, and delay 0ms

	return 0;
}

static __s32 LCD_close_flow(__u32 sel)
{
	LCD_CLOSE_FUNC(sel, LCD_bl_close, 0);       //close lcd backlight, and delay 0ms
	LCD_CLOSE_FUNC(sel, TCON_close, 0);         //close lcd controller, and delay 0ms
	LCD_CLOSE_FUNC(sel, LCD_panel_exit,	200);   //open lcd power, than delay 200ms
	LCD_CLOSE_FUNC(sel, LCD_power_off, 500);   //close lcd power, and delay 500ms

	return 0;
}

static void LCD_power_on(__u32 sel)
{
    LCD_POWER_EN(sel, 1);//config lcd_power pin to open lcd power
}

static void LCD_power_off(__u32 sel)
{
    LCD_POWER_EN(sel, 0);//config lcd_power pin to close lcd power
}

static void LCD_bl_open(__u32 sel)
{
    LCD_PWM_EN(sel, 1);//open pwm module
    LCD_BL_EN(sel, 1);//config lcd_bl_en pin to open lcd backlight
}

static void LCD_bl_close(__u32 sel)
{
    LCD_BL_EN(sel, 0);//config lcd_bl_en pin to close lcd backlight
    LCD_PWM_EN(sel, 0);//close pwm module
}

static void LCD_panel_init(__u32 sel)
{
    __panel_para_t *info = OSAL_malloc(sizeof(__panel_para_t));

    lcd_get_panel_para(sel, info);
    if((info->lcd_if == LCD_IF_EDP) && (info->lcd_edp_tx_ic == 0))
    {
        //__inf("edp select anx9804 ic\n");
        anx9804_init(info);
    }
    else if((info->lcd_if == LCD_IF_EDP) && (info->lcd_edp_tx_ic == 1))
    {
        //__inf("edp select anx6345 ic\n");
        anx6345_init(info);
    }else if((info->lcd_if == LCD_IF_EXT_DSI) && (info->lcd_ext_dsi_if == 0))
    {
        lp079x01_init(info);
    }else if((info->lcd_if == LCD_IF_EXT_DSI) && (info->lcd_ext_dsi_if == 1))
  	{
  	    vvx07h005a00_init(info);
  	}

    OSAL_free(info);

    return;
}

static void LCD_panel_exit(__u32 sel)
{
    __panel_para_t *info = OSAL_malloc(sizeof(__panel_para_t));
    
    lcd_get_panel_para(sel, info);
	if((info->lcd_if == LCD_IF_EXT_DSI) && (info->lcd_ext_dsi_if == 0))
	{
		lp079x01_exit();
	}else if((info->lcd_if == LCD_IF_EXT_DSI) && (info->lcd_ext_dsi_if == 1))
	{
		vvx07h005a00_exit();
	}else if((info->lcd_if == LCD_IF_EDP) && (info->lcd_edp_tx_ic == 1))
    {
        anx6345_exit(info);
    }

	OSAL_free(info);

	return ;
}

//sel: 0:lcd0; 1:lcd1
static __s32 LCD_user_defined_func(__u32 sel, __u32 para1, __u32 para2, __u32 para3)
{
    return 0;
}

void LCD_get_panel_funs_0(__lcd_panel_fun_t * fun)
{
    fun->cfg_panel_info = LCD_cfg_panel_info;//delete this line if you want to use the lcd para define in sys_config1.fex
    fun->cfg_open_flow = LCD_open_flow;
    fun->cfg_close_flow = LCD_close_flow;
    fun->lcd_user_defined_func = LCD_user_defined_func;
}

