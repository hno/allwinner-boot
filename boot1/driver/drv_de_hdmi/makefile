#/*
#*********************************************************************************************************
#*                                                    MELIS
#*                                    the Easy Portable/Player Develop Kits
#*                                               makefile Module
#*
#*                                    (c) Copyright 2006-2010, kevin.z China
#*                                             All Rights Reserved
#*
#* File    : makefile
#* By      : kevin.z
#* Version : v1.0
#* Date    : 2010-9-3 9:39
#* Descript:
#* Update  : date                auther      ver     notes
#*********************************************************************************************************
#*/


#导入编译器配置
include make.cfg


#从所有子目录中得到源代码的列表
SRCCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
SRCSS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.S))

#得到源代码对应的目标文件的列表
OBJS=$(SRCCS:.c=.o) $(SRCSS:.S=.o)


#生成输出目标文件
all:$(LOCALTARGET)
	$(LOAD) $(LDFLAGS) $(LOCALTARGET) $(LTARGET)
	-cp $(LTARGET) $(TARGET0)
	cp  $(LTARGET) $(TARGET1)
	cp  $(LTARGET) $(TARGET2)
	-rm $(OBJS) $(LOCALTARGET) $(LTARGET)
	@echo ----------------------------
	@echo target make finish
	@echo ----------------------------

#生成本地目标文件
$(LOCALTARGET):$(OBJS)
	$(LINK) $(LKFLAGS) -o $@ $(filter %.o ,$+) $(LIBS)


# 删除生成的中间文件
clean:
	-rm $(OBJS) $(LOCALTARGET)
