#ifndef __buttom_H
#define __buttom_H

#include "stm32f10x.h"
#include "light_control.h"

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}KEY;


void key(KEY* kkey);
uint8_t getkey(KEY *kkey);
uint8_t getkey2(KEY *kkey);
#endif /* __buttom_H */