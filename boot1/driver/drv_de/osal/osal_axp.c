#include "osal_axp.h"

static inline int abs(int x)
{
	return x>0?x:(-x);
}

static inline int axp_i2c_read(unsigned char chip, unsigned int addr, unsigned char *buffer)
{
	return p2wi_read(&addr, buffer, 1);
}

static inline int axp_i2c_write(unsigned char chip, unsigned int addr, unsigned char data)
{
	return p2wi_write((u8*)&addr, &data, 1);
}

int axp221_set_dc1sw(int on_off)
{
    u8   reg_value;


	if(axp_i2c_read(NULL, BOOT_POWER22_OUTPUT_CTL2, &reg_value))
    {
		wlibc_uprintf("%d\n", __LINE__);
        return -1;
    }
    if(on_off)
    {
		reg_value |= (1 << 7);
	}
	else
	{
		reg_value &= ~(1 << 7);
	}
	if(axp_i2c_write(NULL, BOOT_POWER22_OUTPUT_CTL2, reg_value))
	{
		wlibc_uprintf("sunxi pmu error : unable to set dcdc1\n");

		return -1;
	}

	return 0;
}
//voltage: 0.1v,   1 voltage = 0.1v
int axp221_set_dldo1(int on_off, int voltage)
{
    u8   reg_value;
    u8   vol_reg;

	voltage = (voltage < 7)? 7 :voltage;
	voltage = (voltage > 33)?33:voltage;
	vol_reg = voltage - 7;
	if(axp_i2c_write(NULL, BOOT_POWER22_OUTPUT_DLDO1_VO, vol_reg))
	{
		wlibc_uprintf("sunxi pmu error : unable to set dcdc1\n");
		return -1;
	}

	if(axp_i2c_read(NULL, BOOT_POWER22_OUTPUT_DLDO3_SW, &reg_value))
    {
		wlibc_uprintf("%d\n", __LINE__);
        return -1;
    }
    if(on_off)
    {
		reg_value |= (1 << 3);
	}
	else
	{
		reg_value &= ~(1 << 3);
	}

	if(axp_i2c_write(NULL, BOOT_POWER22_OUTPUT_DLDO3_SW, reg_value))
	{
		wlibc_uprintf("sunxi pmu error : unable to set dcdc1\n");

		return -1;
	}

	return 0;
}

//voltage: 0.1v,   1 voltage = 0.1v
int axp221_set_dldo3(int on_off, int voltage)
{
    u8   reg_value;
    u8   vol_reg;

	voltage = (voltage < 7)? 7 :voltage;
	voltage = (voltage > 33)?33:voltage;
	vol_reg = voltage - 7;
	if(axp_i2c_write(NULL, BOOT_POWER22_OUTPUT_DLDO3_VO, vol_reg))
	{
		wlibc_uprintf("sunxi pmu error : unable to set dcdc1\n");
		return -1;
	}

	if(axp_i2c_read(NULL, BOOT_POWER22_OUTPUT_DLDO3_SW, &reg_value))
    {
		wlibc_uprintf("%d\n", __LINE__);
        return -1;
    }
    if(on_off)
    {
		reg_value |= (1 << 5);
	}
	else
	{
		reg_value &= ~(1 << 5);
	}

	if(axp_i2c_write(NULL, BOOT_POWER22_OUTPUT_DLDO3_SW, reg_value))
	{
		wlibc_uprintf("sunxi pmu error : unable to set dcdc1\n");

		return -1;
	}

	return 0;
}

//voltage: 0.1v,   1 voltage = 0.1v
int axp221_set_eldo3(int on_off, int voltage)
{
    u8   reg_value;
    u8   vol_reg;

	voltage = (voltage < 7)? 7 :voltage;
	voltage = (voltage > 33)?33:voltage;
	vol_reg = voltage - 7;
	if(axp_i2c_read(NULL, BOOT_POWER22_OUTPUT_DLDO3_SW, &reg_value))
    {
		wlibc_uprintf("%d\n", __LINE__);
        return -1;
    }

	if(on_off)
	{
		if(axp_i2c_write(NULL, BOOT_POWER22_OUTPUT_ELDO3_VO, vol_reg))
		{
			wlibc_uprintf("sunxi pmu error : unable to set eldo3\n");
			return -1;
		}
		reg_value |= (1 << 2);
	}
	else
	{
		reg_value &= ~(1 << 2);
	}

	if(axp_i2c_write(NULL, BOOT_POWER22_OUTPUT_DLDO3_SW, reg_value))
	{
		wlibc_uprintf("sunxi pmu error : unable to set dcdc1\n");

		return -1;
	}

	return 0;
}

