#ifndef __LCD_VVX07H005A00_H__
#define __LCD_VVX07H005A00_H__
#include "../lcd_panel_cfg.h"

void vvx07h005a00_init(__panel_para_t * info);
void vvx07h005a00_exit(void);

#define spi_csx_set(v)	(LCD_GPIO_write(0, 3, v))
#define spi_sck_set(v)  (LCD_GPIO_write(0, 0, v))
#define spi_sdi_set(v)  (LCD_GPIO_write(0, 1, v))

#define ssd2828_rst(v)  (LCD_GPIO_write(0, 2, v))
#define ssd2828_shutdown(v)  (LCD_GPIO_write(0, 4, v))

#endif
