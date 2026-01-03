#include "buttom.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "light_control.h"


void key(KEY *kkey)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if (kkey->GPIOx==GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	
	GPIO_InitStructure.GPIO_Pin=kkey->GPIO_Pin;    /** void 自定义 now rely more flex**/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /**pull upward mode**/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(kkey->GPIOx,&GPIO_InitStructure);
	/** GPIO_SetBits(kkey->GPIOx, kkey->GPIO_Pin);  no need? **/
}

uint8_t getkey(KEY *kkey)
{
	uint8_t key_num=0;
	if (GPIO_ReadInputDataBit(kkey->GPIOx, kkey->GPIO_Pin)==0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(kkey->GPIOx, kkey->GPIO_Pin)==0);
		Delay_ms(20);
		key_num=1;
	}
	return key_num;
}
uint8_t getkey2(KEY *kkey)
{
	uint8_t keyy_num=1;
	if (GPIO_ReadInputDataBit(kkey->GPIOx, kkey->GPIO_Pin)==0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(kkey->GPIOx, kkey->GPIO_Pin)==0);
		Delay_ms(20);
		keyy_num=0;
	}
	return keyy_num;

}
