
#include "lcd_vvx07h005a00.h"

#define delayms LCD_delay_ms

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

void vvx07h005a00_init(__panel_para_t * info)
{
	
	
	spi_24bit_3wire(0x7000B7); //enter LP mode
	spi_24bit_3wire(0x720340);
	
	ssd2828_shutdown(1);
	
	ssd2828_rst(0);
	LCD_delay_ms(20);
	ssd2828_rst(1);	
	
	
	spi_24bit_3wire(0x7000B1);
    spi_24bit_3wire(0x720110);

    spi_24bit_3wire(0x7000B2);
    spi_24bit_3wire(0x720330);

    spi_24bit_3wire(0x7000B3);
    spi_24bit_3wire(0x720510);

    spi_24bit_3wire(0x7000B4);
    spi_24bit_3wire(0x720320);

    spi_24bit_3wire(0x7000B5);
    spi_24bit_3wire(0x720500);

    spi_24bit_3wire(0x7000B6);
    //spi_24bit_3wire(0x720007);
    spi_24bit_3wire(0x72000A); //Burst mode

    //////////////////////////
    spi_24bit_3wire(0x7000C9);
    spi_24bit_3wire(0x721e04);

    spi_24bit_3wire(0x7000CA);
    spi_24bit_3wire(0x722f04);

    spi_24bit_3wire(0x7000CB);
    spi_24bit_3wire(0x720228);

    spi_24bit_3wire(0x7000CC);
    spi_24bit_3wire(0x720f0f);

    delayms(10);
    ////////////////////////////

    spi_24bit_3wire(0x7000DE);
    spi_24bit_3wire(0x720003);

    spi_24bit_3wire(0x7000D6);
    spi_24bit_3wire(0x720005);

    spi_24bit_3wire(0x7000B9);
    spi_24bit_3wire(0x720000);

    delayms(10);
    ////////////////////////////
    spi_24bit_3wire(0x7000BA);
    spi_24bit_3wire(0x728012);//27M OSC 500/27M=18=dex(12)

    spi_24bit_3wire(0x7000BB);
    spi_24bit_3wire(0x72000a);


    spi_24bit_3wire(0x7000B9);
    spi_24bit_3wire(0x720001);

    delayms(200);
    ////////////////////////////

    spi_24bit_3wire(0x7000B8);
    spi_24bit_3wire(0x720000);

    spi_24bit_3wire(0x7000B7);
    spi_24bit_3wire(0x720342);

    delayms(10);

	delayms(10);
	//while(1)
	{
    ////////////////////////////
    spi_24bit_3wire(0x7000Bc);
    spi_24bit_3wire(0x720000);
    spi_24bit_3wire(0x700011);
    delayms(10);
	}

    spi_24bit_3wire(0x7000Bc);
    spi_24bit_3wire(0x720002);


    spi_24bit_3wire(0x7000BF);
    spi_24bit_3wire(0x720BAE);
    delayms(10);

    spi_24bit_3wire(0x7000BF);
    spi_24bit_3wire(0x7202BE);
    delayms(10);

    spi_24bit_3wire(0x7000BF);
    spi_24bit_3wire(0x7290B5);
    delayms(10);

    spi_24bit_3wire(0x7000BF);
    spi_24bit_3wire(0x7209B6);
    delayms(10);


    ////////////////////////////
    spi_24bit_3wire(0x7000Bc);
    spi_24bit_3wire(0x720000);
    spi_24bit_3wire(0x700010);
    delayms(10);


	delayms(50);
    spi_24bit_3wire(0x7000B7);
    spi_24bit_3wire(0x72024B);//0x72024B

    delayms(50);

//    spi_24bit_3wire(0x7000c0);
//    spi_24bit_3wire(0x720100);
/*
    spi_24bit_3wire(0x7000Bc);
    spi_24bit_3wire(0x720000);
    spi_24bit_3wire(0x700010);
    delayms(10);
*/
/*
    spi_24bit_3wire(0x7000Bc);
    spi_24bit_3wire(0x720000);
    spi_24bit_3wire(0x700029);
    delayms(10);
*/
    ////////////////////////////

}


void vvx07h005a00_exit(void)
{
	//spi_24bit_3wire(0x7000B7); //enter LP mode
	//spi_24bit_3wire(0x720342);
	
	//LCD_delay_ms(50);
	//spi_24bit_3wire(0x700028); //display off
	//LCD_delay_ms(10);	
	//spi_24bit_3wire(0x700010); //sleep in cmd
	LCD_delay_ms(20);
	ssd2828_rst(0);
	//ssd2828_shutdown(0);
}


