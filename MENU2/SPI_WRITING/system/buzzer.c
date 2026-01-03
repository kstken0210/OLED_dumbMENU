#include "buttom.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "light_control.h"
#include "buzzer.h"

void buzzer_init(buzzer *buzzer_pointer)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=buzzer_pointer->GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /**pull upward mode**/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(buzzer_pointer->GPIOx,&GPIO_InitStructure);
}
void buzzer_init2(buzzer *buzzer_pointer)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=buzzer_pointer->GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /**pull upward mode**/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(buzzer_pointer->GPIOx,&GPIO_InitStructure);
}
uint8_t lighting(buzzer *buzzer_pointer)
{
	return GPIO_ReadInputDataBit(buzzer_pointer->GPIOx, buzzer_pointer->GPIO_Pin);
}