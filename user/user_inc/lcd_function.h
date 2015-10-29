#ifndef __LCD_FUNCTION_H_
#define __LCD_FUNCTION_H_

#include "stm32f10x.h"
#include "ff.h"

/***************************************************************************************
 * 2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
 * 64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
 * 64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
 * 64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
 * 64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF
 *
 * 选择BANK1-BORSRAM1 连接 TFT，地址范围为0X6000 0000 ~ 0X63FF FFFF
 * FSMC_A16 接LCD的DC(寄存器/数据选择)脚
 * 寄存器基地址 = 0X60000000
 * RAM基地址 = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
 * 当选择不同的地址线时，地址要重新计算  
 * ****************************************************************************************/

#define Bank1_LCD_C ((uint32_t)0x60000000)			//REG ADDR
#define Bank1_LCD_D ((uint32_t)0x60020000)			//DATA ADDR

/*   选定LCD为寄存器  */
#define LCD_WR_C(index) ((*(volatile uint16_t *) Bank1_LCD_C) = ((uint16_t)(index)))

/*   向LCD GRAM写入数据    */
#define LCD_WR_D(val) ((*(volatile uint16_t *) Bank1_LCD_D) = ((uint16_t) (val)))


#define LCD_ILI9341_Cmd(index) 		LCD_WR_C(index)
#define LCD_ILI9341_Parameter(val) 	LCD_WR_D(val)


#define COLUMN		240
#define PAGE		320

#define BACKGROUND	BLACK

#define WHITE		    0xFFFF		/* 白色 */
#define BLACK        	0x0000		/* 黑色 */
#define GREY         	0xF7DE		/* 灰色 */
#define BLUE         	0x001F		/* 蓝色 */
#define BLUE2        	0x051F		/* 浅蓝色 */
#define RED          	0xF800		/* 红色 */
#define MAGENTA      	0xF81F		/* 红紫色，洋红色 */
#define GREEN        	0x07E0		/* 绿色 */
#define CYAN         	0x7FFF		/* 蓝绿色，青色 */
#define YELLOW       	0xFFE0		/* 黄色 */

typedef struct
{
	uint8_t GBK_WIDTH;
	uint8_t GBK_HEIGHT;
	uint8_t GBK_SIZE;
	TCHAR  *GBK_PATH;
}CH_FontInfo;

typedef struct
{
	uint8_t ASCII_WIDTH;
	uint8_t ASCII_HEIGHT;
	uint8_t ASCII_SIZE;
	TCHAR *ASCII_PATH;
}EN_FontInfo;

typedef enum
{
    SIZE_12 = 12,
    SIZE_16 = 16,
}FontSize_Type;

void LCD_Init(void);
void LCD_GPIO_Config(void);
void LCD_FSMC_Config(void);
void LCD_Reset(void);
void LCD_Delay(volatile uint32_t T);
void LCD_REG_Config(void);
void LCD_Clear(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, uint16_t color);
void LCD_SetCursur(uint16_t x, uint16_t y);
void LCD_SetSize(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void LCD_SetPoint(uint16_t x,uint16_t y, uint16_t color);
uint16_t LCD_RD_DATA(void);
uint16_t LCD_GetPoint(uint16_t x, uint16_t y);
uint8_t GetASCIICode(uint8_t *pBuffer, const uint8_t ascii, FontSize_Type fontsize);
void LCD_DisplayChar(uint16_t x, uint16_t y, uint8_t ascii, uint16_t color, FontSize_Type fontsize);
void LCD_DisplayStr(uint16_t x, uint16_t y, const uint8_t *p, uint16_t color, FontSize_Type fontsize);
void LCD_DisplayNum(uint16_t x, uint16_t y, uint32_t num, uint16_t color, FontSize_Type fontsize);
uint8_t GetGBKCode(uint8_t *pBuffer, const uint8_t *pStr, FontSize_Type fontsize);
void LCD_DisplayCH(uint16_t x, uint16_t y,const uint8_t *pStr, uint16_t color, FontSize_Type fontsize);
void LCD_DisplayMultiCH(uint16_t x, uint16_t y,const uint8_t *pStr, uint16_t color, FontSize_Type fontsize);

FIL fsrc;
FATFS fs;
UINT br;
FRESULT res;

#endif
