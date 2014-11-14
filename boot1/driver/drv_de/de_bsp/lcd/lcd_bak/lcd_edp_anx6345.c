#include "lcd_edp_anx6345.h"

#define LCD_RESET(v)  (LCD_GPIO_write(0, 0, v))
/**********************************************************************
 *
 *  anx6345 function
 *
 **********************************************************************/
__u8 SP_TX_I2C_Write_BYTE(__u8 dev_addr, __u8 offset, __u8 d)
{
	lcd_iic_write(dev_addr, offset, d);
	return 0;
}

__u8 SP_TX_I2C_Read_BYTE(__u8 dev_addr, __u8 offset, __u8 *d)
{
	lcd_iic_read(dev_addr, offset, d);
	return 0;
}

__u8 SP_TX_Write_Reg(__u8 dev_addr, __u8 offset, __u8 d)
{
	__u8 cResult;

	//SP_TX_I2C_Write_BYTE( dev_addr,  offset,	d);

	cResult = SP_TX_I2C_Write_BYTE( dev_addr,  offset,	d);

	if(cResult !=0)
	{
		//debug_printf("write eror:result code=%.2x,dev addr= %.2x, offset=%.2x,d=%x\n",(WORD)cResult,(WORD)dev_addr, (WORD)offset,(WORD)d);
	}

	  return 1;

}

__u8 SP_TX_Read_Reg(__u8 dev_addr, __u8 offset, __u8 *d)
{
	__u8 cResult;

	//SP_TX_I2C_Read_BYTE( dev_addr,  offset,  d);

		
	cResult = SP_TX_I2C_Read_BYTE( dev_addr,  offset,  d);

	if ( cResult!= 0 ) 
	{
		//debug_printf("read eror:result code = %.2x,dev addr= %.2x, offset=%.2x,d=%.2x\n",(WORD)cResult,(WORD)dev_addr, (WORD)offset,(WORD)(*d));
	
	}
	
	   return 1;
}


//---------------------------------------------------------
/*

#define lcd_reset_set_output()	LCD_GPIO_set_attr(0, 2, 1)
#define lcd_reset_set_value_0() LCD_GPIO_write(0, 2, 0)
#define lcd_reset_set_value_1() LCD_GPIO_write(0, 2, 1)
*/


__u32 SP_TX_Wait_AUX_Finished(void)
{
	u8 c;
	u8 cCnt;
	cCnt = 0;

	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_STATUS, &c);
	while(c & 0x10){//aux busy
		cCnt++;
		SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_STATUS, &c);

		if(cCnt>100){
            return 0; //aux fail
        }
	}

	return 1;//aux ok
}
__u32 SP_TX_AUX_DPCDRead_Bytes(u8 addrh, u8 addrm, u8 addrl,u8 cCount,u8 *pBuf)
{
	u8 c,i;

	//clr buffer
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_BUF_DATA_COUNT_REG, 0x80);

	//set read cmd and count
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG, ((cCount-1) <<4)|0x09);

	//set aux address15:0
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_7_0_REG, addrl);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_15_8_REG, addrm);

	//set address19:16 and enable aux
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_19_16_REG, &c);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_19_16_REG, (c & 0xf0) | addrh);

	//Enable Aux
    SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
    SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c|0x01);

	mdelay(5);
	if(!SP_TX_Wait_AUX_Finished())
		return 0;

	for(i =0;i<cCount;i++)
	{
		SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_BUF_DATA_0_REG+i, &c);

		*(pBuf+i) = c;

		if(i >= MAX_BUF_CNT)
			break;
	}

	return 1;//aux ok
}


void anx6345_init(__panel_para_t * info)
{
    __u8 c;
    __u32 count = 0;
    __u32 lanes;
    __u32 data_rate;
    __u8 c1;
    __u32 colordepth;
    __u8 count1 =0;

    lanes = info->lcd_edp_tx_lane;
    data_rate = 0x06;
    if(info->lcd_edp_tx_rate== 1)
    {
        data_rate = 0x06;//1.62G
    }
    else if(info->lcd_edp_tx_rate == 2)
    {
        data_rate = 0x0a;//2.7G
    }

    colordepth = (info->lcd_edp_colordepth == 1)? 0x00:0x10;//0x00: 6bit;  0x10:8bit

        OSAL_PRINTF("GPIO RESET\n\n\n");
        LCD_RESET(1);
        LCD_delay_ms(10);

		SP_TX_Write_Reg (0x72, 0x05, 0x00);


		 SP_TX_Read_Reg(0x72, 0x01, &c);

		 if(c==0xaa)
		 	{
		 		OSAL_PRINTF("ANX6345 Chip found\n\n\n");
		 	}
		 else
		 	{
		 		OSAL_PRINTF("ANX6345 Chip not found\n\n\n");
		 	}

		 //if aux read fail, do h/w reset,
		while((!SP_TX_AUX_DPCDRead_Bytes(0x00,0x00,0x00,1,&c1))&&(count1<200))
		{
			//read fail, h/w reset
			SP_TX_Write_Reg (0x72, 0x06, 0x01);
			SP_TX_Write_Reg (0x72, 0x06, 0x00);

			SP_TX_Write_Reg (0x72, 0x05, 0x00);
			LCD_delay_ms(10);
			count1++;
		}

	
		 //software reset
		 SP_TX_Read_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL_REG, &c);
		 SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL_REG, c | SP_TX_RST_SW_RST);
		 SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL_REG, c & ~SP_TX_RST_SW_RST);
	
		 SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_EXTRA_ADDR_REG, 0x50);//EDID address for AUX access
		 SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_HDCP_CTRL, 0x00);	//disable HDCP polling mode.
		 //SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_HDCP_CTRL, 0x02);	//Enable HDCP polling mode.
		 SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_LINK_DEBUG_REG, 0x30);//enable M value read out
	
		 //SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_DEBUG_REG1, &c);
		 SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_DEBUG_REG1, 0x00);//disable polling HPD
	
			 SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_HDCP_CONTROL_0_REG, &c);
		 SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_HDCP_CONTROL_0_REG, c | 0x03);//set KSV valid
	
		 SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
		 SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c|0x08);//set double AUX output
	
		 SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_COMMON_INT_MASK1, 0xbf);//unmask pll change int
		 SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_COMMON_INT_MASK2, 0xff);//mask all int
		 SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_COMMON_INT_MASK3, 0xff);//mask all int
		 SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_COMMON_INT_MASK4, 0xff);//mask all int
	
	
		//reset AUX
		SP_TX_Read_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL2_REG, &c);
		SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL2_REG, c |SP_TX_AUX_RST);
		SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL2_REG, c & (~SP_TX_AUX_RST));
	
		 //Chip initialization
	
		 SP_TX_Write_Reg(0x70, SP_TX_SYS_CTRL1_REG, 0x00);
		 LCD_delay_ms(10);
#if 0
		 for(i=0;i<50;i++)
		{
			SP_TX_Read_Reg(0x70, SP_TX_SYS_CTRL1_REG, &c);
		//	SP_TX_Write_Reg(0x70, SP_TX_SYS_CTRL1_REG, c);
		//	SP_TX_Read_Reg(0x70, SP_TX_SYS_CTRL1_REG, &c);
			if((c&SP_TX_SYS_CTRL1_DET_STA)==0x04)
			{
				//debug_puts("clock is detected.\n");
				break;
			}
	
			LCD_delay_ms(10);
		}
#endif
		   //check whether clock is stable
	
	/*	for(i=0;i<50;i++)
		{
	
			SP_TX_Read_Reg(0x70, SP_TX_SYS_CTRL2_REG, &c);
			if((c&SP_TX_SYS_CTRL2_CHA_STA)==0x0)
			{
				debug_puts("clock is stable.\n");
				break;
			}
			LCD_delay_ms(10);
		}
	*/
	
			//VESA range, 8bits BPC, RGB
		SP_TX_Write_Reg(0x72, SP_TX_VID_CTRL2_REG, colordepth);
	
		//ANX6345 chip analog setting
		SP_TX_Write_Reg(0x70, SP_TX_PLL_CTRL_REG, 0x00);				  //UPDATE: FROM 0X07 TO 0X00
	
	
		//ANX chip analog setting
	//	SP_TX_Write_Reg(0x72, ANALOG_DEBUG_REG1, 0x70); 			  //UPDATE: FROM 0XF0 TO 0X70
		SP_TX_Write_Reg(0x70, SP_TX_LINK_DEBUG_REG, 0x30);
	
		//force HPD
		SP_TX_Write_Reg(0x70, SP_TX_SYS_CTRL3_REG, 0x30);

  /* enable ssc function */
  SP_TX_Write_Reg(0x70, 0xA7, 0x00); 		          // disable SSC first
  SP_TX_Write_Reg(0x70, 0xD0, 0x5f);              // ssc d  0.4%, f0/4 mode
  SP_TX_Write_Reg(0x70, 0xD1, 0x00); 
  SP_TX_Write_Reg(0x70, 0xD2, 0x75);              // ctrl_th 
  SP_TX_Read_Reg(0x70, 0xA7, &c); 
  SP_TX_Write_Reg(0x70, 0xA7, c | 0x10);          // enable SSC
  SP_TX_Read_Reg(0x72, 0x07, &c);                 //reset SSC      
  SP_TX_Write_Reg(0x72, 0x07, c | 0x80);
  SP_TX_Write_Reg(0x72, 0x07, c & (~0x80)); 

		//Select 2.7G
		   SP_TX_Write_Reg(0x70, SP_TX_LINK_BW_SET_REG, data_rate);		//2.7g:0x0a;1.62g:0x06
		//Select 2 lanes
		SP_TX_Write_Reg(0x70, 0xa1, lanes);
	
		 SP_TX_Write_Reg(0x70, SP_TX_LINK_TRAINING_CTRL_REG, SP_TX_LINK_TRAINING_CTRL_EN);
		LCD_delay_ms(5);
		SP_TX_Read_Reg(0x70, SP_TX_LINK_TRAINING_CTRL_REG, &c);
		while((c&0x80)!=0)																				  //UPDATE: FROM 0X01 TO 0X80
		{
			//debug_puts("Waiting...\n");
			LCD_delay_ms(5);
            count ++;
            if(count > 100)
            {
                OSAL_PRINTF("ANX6345 Link training fail\n");
				break;				
			}
			SP_TX_Read_Reg(0x70, SP_TX_LINK_TRAINING_CTRL_REG, &c);
		}
	
	
	
		SP_TX_Write_Reg(0x72, 0x12, 0x2c);
		SP_TX_Write_Reg(0x72, 0x13, 0x06);
		SP_TX_Write_Reg(0x72, 0x14, 0x00);
		SP_TX_Write_Reg(0x72, 0x15, 0x06);
		SP_TX_Write_Reg(0x72, 0x16, 0x02);
		SP_TX_Write_Reg(0x72, 0x17, 0x04);
		SP_TX_Write_Reg(0x72, 0x18, 0x26);
		SP_TX_Write_Reg(0x72, 0x19, 0x50);
		SP_TX_Write_Reg(0x72, 0x1a, 0x04);
		SP_TX_Write_Reg(0x72, 0x1b, 0x00);
		SP_TX_Write_Reg(0x72, 0x1c, 0x04);
		SP_TX_Write_Reg(0x72, 0x1d, 0x18);
		SP_TX_Write_Reg(0x72, 0x1e, 0x00);
		SP_TX_Write_Reg(0x72, 0x1f, 0x10);
		SP_TX_Write_Reg(0x72, 0x20, 0x00);
		SP_TX_Write_Reg(0x72, 0x21, 0x28);
	
		//SP_TX_Write_Reg(0x72, 0x11, 0x03);
	/*
		SP_TX_Write_Reg(0x72, 0x12, 0xee);
		SP_TX_Write_Reg(0x72, 0x13, 0x02);
		SP_TX_Write_Reg(0x72, 0x14, 0xd0);
		SP_TX_Write_Reg(0x72, 0x15, 0x02);
		SP_TX_Write_Reg(0x72, 0x16, 0x05);
		SP_TX_Write_Reg(0x72, 0x17, 0x05);
		SP_TX_Write_Reg(0x72, 0x18, 0x14);
		SP_TX_Write_Reg(0x72, 0x19, 0x39);
		SP_TX_Write_Reg(0x72, 0x1a, 0x03);
		SP_TX_Write_Reg(0x72, 0x1b, 0x80);
		SP_TX_Write_Reg(0x72, 0x1c, 0x02);
		SP_TX_Write_Reg(0x72, 0x1d, 0x37);
		SP_TX_Write_Reg(0x72, 0x1e, 0x00);
		SP_TX_Write_Reg(0x72, 0x1f, 0x14);
		SP_TX_Write_Reg(0x72, 0x20, 0x00);
		SP_TX_Write_Reg(0x72, 0x21, 0x6e);
	*/
		SP_TX_Write_Reg(0x72, 0x11, 0x00);
	
		   //enable BIST. In normal mode, don't need to config this reg
		//SP_TX_Write_Reg(0x72, 0x0b, 0x09);//colorbar:08,graystep:09
	
		//enable video input, set DDR mode, the input DCLK should be 102.5MHz;
		//In normal mode, set this reg to 0x81, SDR mode, the input DCLK should be 205MHz
		SP_TX_Write_Reg(0x72, 0x08, 0x83);		//SDR:0x81;DDR:0x8f
	
		   //force HPD and stream valid
		SP_TX_Write_Reg(0x70, 0x82, 0x33);


}

void anx6345_exit(__panel_para_t * info)
{
	LCD_RESET(0);
}



