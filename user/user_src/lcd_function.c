#include "lcd_function.h"

#define DEBUG_DELAY()

CH_FontInfo GBK12_FontInfo = {12, 12, 12, (TCHAR *)"0:/system/font/GBK12.FON"};

CH_FontInfo GBK16_FontInfo = {16, 16, 16, (TCHAR *)"0:/system/font/GBK16.FON"};

EN_FontInfo ASCII12_FontInfo = {6, 12, 12, (TCHAR *)"0:/system/font/ASCII12.FON"};

EN_FontInfo ASCII16_FontInfo = {8, 16, 16, (TCHAR *)"0:/system/font/ASCII16.FON"};

void LCD_Delay(volatile uint32_t  T)
{
	for(T; T>0; T--);
}

void LCD_Init(void)
{
	LCD_GPIO_Config();
	LCD_FSMC_Config();
	LCD_Reset();
	LCD_REG_Config();
}

void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	/* Enable FSMC Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	/* Enable GPIO Clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD |
				RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* LCD背光控制 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* LCD复位控制引脚 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* 配置FSMC相对应的数据线 FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|
					GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|
					GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/*配置FSMC相对应的控制线
	 *PD3-FMSC_NOE --> LCD_RD	LCD读使能
	 *PD4-FMSC_NWE --> LCD_WR	LCD写使能
	 *PD7-FMSC_NE1 --> LCD_CS	LCD片选
	 *PD11-FMSC_A16 --> LCD_DC	LCD数据命令选择
	 * */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);

}

void LCD_FSMC_Config(void)
{
	FSMC_NORSRAMInitTypeDef		FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef	FSMC_NORSRAMTimingInitStructure;
	
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 2;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 5;
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_B;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	//FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	//FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void LCD_Reset(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	LCD_Delay(0xFFF);
	GPIO_SetBits(GPIOE, GPIO_Pin_1);
	LCD_Delay(0xFFF);
}

void LCD_REG_Config(void)
{
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xCF);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x81);
	LCD_ILI9341_Parameter(0x30);

	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xED);
	LCD_ILI9341_Parameter(0x64);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x12);
	LCD_ILI9341_Parameter(0x81);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xE8);
	LCD_ILI9341_Parameter(0x85);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x78);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xCB);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x2C);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x34);
	LCD_ILI9341_Parameter(0x02);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xF7);
	LCD_ILI9341_Parameter(0x20);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xEA);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xB1);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x1B);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xB6);
	LCD_ILI9341_Parameter(0x0A);
	LCD_ILI9341_Parameter(0xA2);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xC0);
	LCD_ILI9341_Parameter(0x35);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xC1);
	LCD_ILI9341_Parameter(0x11);

	LCD_ILI9341_Cmd(0xC5);
	LCD_ILI9341_Parameter(0x45);
	LCD_ILI9341_Parameter(0x45);

	LCD_ILI9341_Cmd(0xC7);
	LCD_ILI9341_Parameter(0xA2);

	LCD_ILI9341_Cmd(0xF2);
	LCD_ILI9341_Parameter(0x00);

	LCD_ILI9341_Cmd(0x26);
	LCD_ILI9341_Parameter(0x01);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xE0);
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x26);
	LCD_ILI9341_Parameter(0x24);
	LCD_ILI9341_Parameter(0x0B);
	LCD_ILI9341_Parameter(0x0E);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x54);
	LCD_ILI9341_Parameter(0xA8);
	LCD_ILI9341_Parameter(0x46);
	LCD_ILI9341_Parameter(0x0C);
	LCD_ILI9341_Parameter(0x17);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x00);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0xE1);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x19);
	LCD_ILI9341_Parameter(0x1B);
	LCD_ILI9341_Parameter(0x04);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x2A);
	LCD_ILI9341_Parameter(0x47);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x30);
	LCD_ILI9341_Parameter(0x38);
	LCD_ILI9341_Parameter(0x0F);
	
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0x36);
	LCD_ILI9341_Parameter(0xC8);
	
	/* column address control set */
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0x2A);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0xEF);

	/* page address control set */
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0x2B);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x01);
	LCD_ILI9341_Parameter(0x3F);
	
	/*  Pixel Format Set (3Ah)  */
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0x3A);
	LCD_ILI9341_Parameter(0x55);

	/* Sleep Out (11h)  */
	LCD_ILI9341_Cmd(0x11);
	LCD_Delay(0x2bffc);

	/* Display ON (29h) */
	DEBUG_DELAY();
	LCD_ILI9341_Cmd(0x29);

}

void LCD_Clear(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	uint32_t i = 0;
	
	LCD_ILI9341_Cmd(0x2A);
	LCD_ILI9341_Parameter(x >> 8);			//设置x起点坐标
	LCD_ILI9341_Parameter(x & 0xff);
	LCD_ILI9341_Parameter((x+width-1) >> 8);	//设置x终点坐标
	LCD_ILI9341_Parameter((x+width-1) & 0xff);

	LCD_ILI9341_Cmd(0x2B);
	LCD_ILI9341_Parameter(y >> 8);			//设置y起点坐标
	LCD_ILI9341_Parameter(y & 0xff);
	LCD_ILI9341_Parameter((y+height-1) >> 8);	//设置y终点坐标
	LCD_ILI9341_Parameter((y+height-1) & 0xff);

	LCD_ILI9341_Cmd(0x2C);

	for(; i<width*height; i++)
	{
		LCD_WR_D( color );
	}
}

void LCD_SetCursur(uint16_t x, uint16_t y)
{
	//设置x坐标
	LCD_ILI9341_Cmd(0x2A);
	LCD_ILI9341_Parameter(x >> 8);
	LCD_ILI9341_Parameter(x & 0xff);
	LCD_ILI9341_Parameter(x >> 8);
	LCD_ILI9341_Parameter(x & 0xff);
	
	//设置y坐标
	LCD_ILI9341_Cmd(0x2B);
	LCD_ILI9341_Parameter(y >> 8);
	LCD_ILI9341_Parameter(y & 0xff);
	LCD_ILI9341_Parameter(y >> 8);
	LCD_ILI9341_Parameter(y & 0xff);

}

void LCD_SetSize(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	//设置x坐标
	LCD_ILI9341_Cmd(0x2A);
	LCD_ILI9341_Parameter(x >> 8);
	LCD_ILI9341_Parameter(x & 0xff);
	LCD_ILI9341_Parameter((x+width-1) >> 8);
	LCD_ILI9341_Parameter((x+width-1) & 0xff);
	
	//设置y坐标
	LCD_ILI9341_Cmd(0x2B);
	LCD_ILI9341_Parameter(y >> 8);
	LCD_ILI9341_Parameter(y & 0xff);
	LCD_ILI9341_Parameter((y+height-1) >> 8);
	LCD_ILI9341_Parameter((y+height-1) & 0xff);

}

void LCD_SetPoint(uint16_t x, uint16_t y, uint16_t color)	//write point color
{
	LCD_SetCursur(x, y);
	LCD_ILI9341_Cmd(0x2C);		//write the data command
	LCD_WR_D(color);
}

uint16_t LCD_RD_DATA(void)
{
	uint16_t R=0, G=0, B=0;
	
	R = *(volatile uint16_t *)Bank1_LCD_D;
	R = *(volatile uint16_t *)Bank1_LCD_D;		//read the red color
	G = *(volatile uint16_t *)Bank1_LCD_D;		//read the green color
	B = *(volatile uint16_t *)Bank1_LCD_D;		//read the bule color

	return ( ((R >> 11) << 11) + ((G >> 10) << 5) + (B >> 11));
}

uint16_t LCD_GetPoint(uint16_t x, uint16_t y)
{
	LCD_SetCursur(x, y);
	LCD_ILI9341_Cmd(0x2e);		//read data command
	return LCD_RD_DATA();		//return the data
}

uint8_t GetASCIICode(uint8_t *pBuffer,const uint8_t ascii, FontSize_Type fontsize)
{
	uint16_t offset;
    EN_FontInfo *Font_Info;

	switch(fontsize)
	{
		case SIZE_12:Font_Info = &ASCII12_FontInfo;break;
		case SIZE_16:Font_Info = &ASCII16_FontInfo;break;
		default:break;
	}

	//获取ASCII在字库中的偏移	
	offset = (ascii - ' ') * Font_Info->ASCII_SIZE;

	//挂载SD卡
	//res = f_mount(&fs, "0:", 1);
	//if(res == FR_OK)
	//{
		res = f_open(&fsrc, Font_Info->ASCII_PATH, FA_OPEN_EXISTING | FA_READ);
		if(res == FR_OK)
		{
			//通过偏移找到ASCII字模首地址
			f_lseek(&fsrc, offset);
			//读从首地址开始的size位数据
			f_read(&fsrc, pBuffer, Font_Info->ASCII_SIZE, &br);
			//关闭文件
			f_close(&fsrc);
			return 1;
		}
		else
			return 0;
	//}
	//else
	//	return 0;
}

void LCD_DisplayChar(uint16_t x, uint16_t y, uint8_t ascii, uint16_t color, FontSize_Type fontsize)
{
	uint8_t column, page;
	uint8_t temp;
	uint8_t buffer[fontsize];
	EN_FontInfo *FontInfo;

	switch(fontsize)
	{
		case SIZE_12:FontInfo = &ASCII12_FontInfo;break;
		case SIZE_16:FontInfo = &ASCII16_FontInfo;break;
		default:break;
	}

	LCD_SetSize(x, y, FontInfo->ASCII_WIDTH, FontInfo->ASCII_HEIGHT);
	LCD_ILI9341_Cmd(0x2C);				//memory write command
	GetASCIICode(buffer, ascii, fontsize);

	for(page=0; page < FontInfo->ASCII_HEIGHT; page++)
	{
		temp = buffer[page];
		for(column=0;column < FontInfo->ASCII_WIDTH; column++)
		{
			if(temp & 0x80)
				LCD_WR_D( color );
			else
				LCD_WR_D( BACKGROUND );
			temp = temp << 1;
		}
	}

}

void LCD_DisplayStr(uint16_t x, uint16_t y,const uint8_t *p, uint16_t color, FontSize_Type fontsize)
{
	EN_FontInfo *Font_Info;

	switch(fontsize)
	{
		case SIZE_12:Font_Info = &ASCII12_FontInfo;break;
		case SIZE_16:Font_Info = &ASCII16_FontInfo;break;
		default:break;
	}
	while( *p != '\0')
	{
		if( x > (COLUMN-Font_Info->ASCII_WIDTH) )
		{
			x = 0;
			y = y + Font_Info->ASCII_HEIGHT;
		}
		if(y > (PAGE-Font_Info->ASCII_HEIGHT))
		{
			y = 0;
			x = 0;
		}
		LCD_DisplayChar(x, y, *p, color, fontsize);
		x  = x + Font_Info->ASCII_WIDTH;
		p++;
	}
}

void LCD_DisplayNum(uint16_t x, uint16_t y, uint32_t num, uint16_t color, FontSize_Type fontsize)
{
	uint8_t length=0;
	uint32_t temp=0;
	EN_FontInfo *Font_Info;

	switch(fontsize)
	{
		case SIZE_12:Font_Info = &ASCII12_FontInfo;break;
		case SIZE_16:Font_Info = &ASCII16_FontInfo;break;
		default:break;
	}

	temp = num;
	if(temp==0)
	{
		LCD_DisplayChar(x, y, '0', color, fontsize);
		return;
	}
	while(temp)
	{
		temp = temp / 10;
		length++;
	}
	while(num)
	{
		LCD_DisplayChar( x+(Font_Info->ASCII_WIDTH*(length--)-Font_Info->ASCII_WIDTH), y, num%10+'0', color, fontsize );
		num = num / 10;
	}
	
}

uint8_t GetGBKCode(uint8_t *pBuffer,const uint8_t *pStr, FontSize_Type fontsize)
{
	uint32_t offset;
	uint8_t GBKH, GBKL;
    CH_FontInfo *Font_Info;

	GBKH = *pStr;			//取高8位数据
	GBKL = *(pStr+1);		//取低8位数据
	
	switch(fontsize)
	{
		case SIZE_12:Font_Info = &GBK12_FontInfo;break;
		case SIZE_16:Font_Info = &GBK16_FontInfo;break;
		default:break;
	}

	//获取汉字在字库中的偏移	
	if(GBKL < 0x7f)
		offset = ((GBKH - 0x81) * 190 + (GBKL - 0x40)) * (Font_Info->GBK_SIZE * 2);
	else
		offset = ((GBKH - 0x81) * 190 + (GBKL - 0x41)) * (Font_Info->GBK_SIZE * 2);
	//挂载SD卡
	//res = f_mount(&fs, "0:", 1);
	//if(res == FR_OK)
	//{
		res = f_open(&fsrc, Font_Info->GBK_PATH, FA_OPEN_EXISTING | FA_READ);
		if(res == FR_OK)
		{
			//通过偏移找到汉字字模首地址
			f_lseek(&fsrc, offset);
			//读从首地址开始的size*2位数据
			f_read(&fsrc, pBuffer, Font_Info->GBK_SIZE *2, &br);
			//关闭文件
			f_close(&fsrc);
			return 1;
		}
		else
			return 0;
	//}
	//else
	//	return 0;
}

void LCD_DisplayCH(uint16_t x, uint16_t y, const uint8_t *pStr, uint16_t color, FontSize_Type fontsize)
{
	uint16_t column, page;
	uint8_t buffer[fontsize*2];
	uint16_t temp;
    CH_FontInfo *Font_Info;
	
	switch(fontsize)
	{
		case SIZE_12:Font_Info = &GBK12_FontInfo;break;
		case SIZE_16:Font_Info = &GBK16_FontInfo;break;
		default:break;
	}

   	LCD_SetSize(x, y, Font_Info->GBK_WIDTH, Font_Info->GBK_HEIGHT);
	LCD_ILI9341_Cmd(0x2C);
	
	if(GetGBKCode(buffer, pStr, fontsize))
	{
		for(page = 0; page < Font_Info->GBK_HEIGHT; page++)
		{
			temp = buffer[page*2];
			temp = (temp<<8) | buffer[page*2+1];	//将高8位和低8位拼接为16位

			for(column = 0; column < Font_Info->GBK_WIDTH; column++)
			{
				if(temp & 0x8000)
					LCD_WR_D(color);
				else 
					LCD_WR_D(BACKGROUND);
				temp = temp << 1;
			}
		}
	}	
}

void LCD_DisplayMultiCH(uint16_t x, uint16_t y, const uint8_t *pStr, uint16_t color, FontSize_Type fontsize)
{
	CH_FontInfo *Font_Info;

	switch(fontsize)
	{
		case SIZE_12:Font_Info = &GBK12_FontInfo;break;
		case SIZE_16:Font_Info = &GBK16_FontInfo;break;
		default:break;
	}

	while(*pStr != '\0')
	{
		
		if(*pStr > 0x7f)			//判断中文字符和英文字符，中文第一个字节从0x81开始
		{
			if(x > COLUMN - Font_Info->GBK_WIDTH)		//自动换行
			{
				x = 0;
				y += Font_Info->GBK_HEIGHT;
			}
			if(y > PAGE - Font_Info->GBK_HEIGHT)
			{
				y = 0;
				x = 0;
			}
			LCD_DisplayCH(x, y, pStr, color, fontsize); 
			x += Font_Info->GBK_WIDTH;
			pStr += 2;
		}
		else						//英文字符高度一样，宽度位中文的一半
		{
			if(x > COLUMN - Font_Info->GBK_WIDTH/2)		//自动换行
			{
				x = 0;
				y += Font_Info->GBK_HEIGHT;
			}
			if(y > PAGE - Font_Info->GBK_HEIGHT)
			{
				y = 0;
				x = 0;
			}
			LCD_DisplayChar(x, y, *pStr, color, fontsize);
			x += Font_Info->GBK_WIDTH/2;
			pStr += 1;
		}
	}
}
