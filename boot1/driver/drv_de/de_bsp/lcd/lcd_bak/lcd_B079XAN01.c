
#include "lcd_B079XAN01.h"

static void spi_24bit_3wire(__u32 tx)
{
	__u8 i;

	spi_csx_set(0);

	for(i=0;i<24;i++)
	{
		LCD_delay_us(1);
		spi_sck_set(0);
		LCD_delay_us(1);
		if(tx & 0x800000)
			spi_sdi_set(1);
		else
			spi_sdi_set(0);
		LCD_delay_us(1);
		spi_sck_set(1);
		LCD_delay_us(1);
		tx <<= 1;
	}
	spi_sdi_set(1);
	LCD_delay_us(1);
	spi_csx_set(1);
	LCD_delay_us(3);
}

void lp079x01_init(__panel_para_t * info)
{
	__u32 pll_config = 0;

	if(info->lcd_xtal_freq == 12)
	{
		/* 12M xtal freq */
		pll_config = 0xc02d;
	} else if(info->lcd_xtal_freq == 27)
	{
		pll_config = 0xc014;
	} else if(info->lcd_xtal_freq == 24)
	{
		pll_config = 0xc22d;
	} else
	{
		/* default 12Mhz */
		pll_config = 0xc02d;
	}

	spi_24bit_3wire(0x7000B7); //enter LP mode
	spi_24bit_3wire(0x720340);
	ssd2828_rst(0);
	panel_rst(0);
	ssd2828_rst(1);
	panel_rst(1);
	LCD_delay_ms(10);

	spi_24bit_3wire(0x7000B1);  //VSA=50, HAS=64
	spi_24bit_3wire(0x723240);

	spi_24bit_3wire(0x7000B2); //VBP=30+50, HBP=56+64
	spi_24bit_3wire(0x725078);

	spi_24bit_3wire(0x7000B3); //VFP=36, HFP=60
	spi_24bit_3wire(0x72243C);

	spi_24bit_3wire(0x7000B4); //HACT=768
	spi_24bit_3wire(0x720300);

	spi_24bit_3wire(0x7000B5); //VACT=1240
	spi_24bit_3wire(0x720400);

	spi_24bit_3wire(0x7000B6);
	if(info->lcd_ext_dsi_colordepth == 1) {
		spi_24bit_3wire(0x720009); //0x720009:burst mode, 18bpp packed
	} else {
		spi_24bit_3wire(0x72000B); //0x72000B:burst mode, 24bpp
	}
	//0x72000A:burst mode, 18bpp loosely packed

	spi_24bit_3wire(0x7000DE); //no of lane=4
	spi_24bit_3wire(0x720003);

	spi_24bit_3wire(0x7000D6); //RGB order and packet number in blanking period
	spi_24bit_3wire(0x720005);

	spi_24bit_3wire(0x7000B9); //disable PLL
	spi_24bit_3wire(0x720000);

	spi_24bit_3wire(0x7000BA); //lane speed=560
	spi_24bit_3wire(0x720000 | pll_config); //may modify according to requirement, 500Mbps to  560Mbps, clk_in / (bit12-8) * (bit7-0)

	spi_24bit_3wire(0x7000BB); //LP clock
	spi_24bit_3wire(0x720008);

	spi_24bit_3wire(0x7000B9); //enable PPL
	spi_24bit_3wire(0x720001);

	spi_24bit_3wire(0x7000c4); //enable BTA
	spi_24bit_3wire(0x720001);

	spi_24bit_3wire(0x7000B7); //enter LP mode
	spi_24bit_3wire(0x720342);

	spi_24bit_3wire(0x7000B8); //VC
	spi_24bit_3wire(0x720000);

	spi_24bit_3wire(0x7000BC); //set packet size
	spi_24bit_3wire(0x720000);

	spi_24bit_3wire(0x700011); //sleep out cmd

	LCD_delay_ms(100);

	spi_24bit_3wire(0x700029); //display on

	LCD_delay_ms(200);
	spi_24bit_3wire(0x7000B7); //video mode on
	spi_24bit_3wire(0x72030b);
}


void lp079x01_exit(void)
{
	spi_24bit_3wire(0x7000B7); //enter LP mode
	spi_24bit_3wire(0x720342);

	LCD_delay_ms(50);
	spi_24bit_3wire(0x700028); //display off
	LCD_delay_ms(10);
	spi_24bit_3wire(0x700010); //sleep in cmd
	LCD_delay_ms(20);
	ssd2828_rst(0);
	panel_rst(0);
}


