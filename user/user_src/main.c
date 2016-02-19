/**
  ******************************************************************************
  * @file    FATS/user/user_src/main.c
  * @author  wangxian
  * @version V1.0
  * @date    08-April-2015
  * @brief   Main program body
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "lcd_function.h"
#include "sdio_function.h"
#include "ff.h"

FATFS fs;
FRESULT res;
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	SD_GPIO_Configuration();
	SD_NVIC_Configuration();
	LCD_Init();
	LCD_Clear(0, 0, 240, 320, BACKGROUND);
	res = f_mount(&fs, "0:", 1);			//挂载SD卡
	if(res == FR_OK)
	{
		LCD_DisplayMultiCH(0, 0, (uint8_t *)"“王献”你好啊！Welcome Mr.Wang! 666", BLUE, SIZE_12);
		LCD_DisplayMultiCH(0, 16, (uint8_t *)"王献你是最棒的！you are the best!", GREEN, SIZE_16);
	}
	while (1)
	{

	}
}

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
