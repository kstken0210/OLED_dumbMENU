#ifndef __buzzer_H
#define __buzzer_H

#include "stm32f10x.h"
#include "light_control.h"

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}buzzer;

void buzzer_init(buzzer *buzzer_pointer);
void buzzer_init2(buzzer *buzzer_pointer);
uint8_t lighting(buzzer *buzzer_pointer);
#endif /* __buzzer_H */