/*延时函数
 *有延时nms
 *有延时nus
 * */
#include"delay.h"

/*----   延时nms    ---------*/
void delay_nms(u16 nms)
{
	        SysTick->LOAD = (u32)nms*(SystemCoreClock/8/1000);              //T = nms*9000/(72MHz/8)  --72MHz/8 = 9Mhz
		SysTick->CTRL |= 0x00000001;                                    //开启计数
		while( !(SysTick->CTRL & (1<<16)) );                            //等待时间
		SysTick->CTRL &= 0xfffffffe;                                    //关闭计数
		SysTick->VAL = 0;                                               //清空计数器
}

/*-----   延时nus    ---------*/
void delay_nus(u32 nus)
{
	        SysTick->LOAD = nus*(SystemCoreClock/8/1000)/1000;              //T = nus*9000/(72MHz/8)/1000
		SysTick->CTRL |= 0x00000001;                                    //开启计数
		while( !(SysTick->CTRL & (1<<16)) );                            //等待时间
		SysTick->CTRL &= 0xfffffffe;                                    //关闭计数
		SysTick->VAL = 0;                                               //清空计数
}

