#工程名
PROJ_NAME=TFT_CH

#要编译的文件
SRCS  = ./user/user_src/stm32f10x_it.c
SRCS += ./user/user_src/system_stm32f10x.c 
SRCS += ./user/user_src/delay.c 
SRCS += ./user/user_src/lcd_function.c
SRCS += ./user/user_src/main.c
SRCS += ./user/user_src/sdio_function.c
SRCS += ./ff11a/src/diskio.c
SRCS += ./ff11a/src/ff.c
SRCS += ./ff11a/src/option/cc936.c

SRCS += ../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_hd.s

#链接脚本路径
LDSCRIPT_INC = ../ldscript

#标准库位置
STDPERIPHSRC = ../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver

#C编译器
CC=arm-none-eabi-gcc

#链接器
LD=arm-none-eabi-gcc

#打包器
AR=arm-none-eabi-ar

#二进制文件生成
OBJCOPY=arm-none-eabi-objcopy

#SIZE
SIZE=arm-none-eabi-size

#make
MAKE=make

#编译选项
CFLAGS  = -g -nostartfiles -Wl,--gc-sections -mthumb -mcpu=cortex-m3 
CFLAGS += -mfloat-abi=soft -mabi=aapcs
CFLAGS += -I inc -I .
CFLAGS += -I ./user/user_inc
CFLAGS += -I ./ff11a/src
CFLAGS += -I ../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver/inc
CFLAGS += -I ../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/CoreSupport
CFLAGS += -I ../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CFLAGS += -D USE_STDPERIPH_DRIVER -D STM32F10X_HD
CFLAGS += -finput-charset=UTF-8 -fexec-charset=GBK
#链接选项
LDFLAGS = -nostartfiles -Wl,--gc-sections -mthumb -mcpu=cortex-m3 -mfloat-abi=soft -mabi=aapcs

vpath %.c src
vpath %.a $(STDPERIPHSRC)

.PHONY: lib proj

all: lib proj

lib:
	$(MAKE) -C $(STDPERIPHSRC)

proj:$(PROJ_NAME).elf

$(PROJ_NAME).elf:$(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -L$(STDPERIPHSRC) -lstm32f103 -L$(LDSCRIPT_INC) -Tstm32.ld
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(SIZE) $(PROJ_NAME).elf

program: $(PROJ_NAME).bin
	st-flash write $(PROJ_NAME).bin 0x08000000

clean:
	find ./ -name '*~' | xargs rm -f	
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map