all:
	make -f make_nand  -C boot1/core -j8
	make -f make_sdmmc -C boot1/core -j8

	make -C boot1/apps/Boot_Android -j8
	make -C boot1/apps/Boot_Burn -j8
	make -C boot1/apps/Boot_Signature -j8
	make -C boot1/apps/Card_Android -j8
	make -C boot1/driver/drv_de -j8
