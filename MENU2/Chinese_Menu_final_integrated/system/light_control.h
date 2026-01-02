#ifndef __LIGHT_CONTROL_H
#define __LIGHT_CONTROL_H

#include "stm32f10x.h"

// 灯光配置结构体
typedef struct {
    GPIO_TypeDef* GPIOx;      // GPIO端口（如GPIOA）
    uint16_t GPIO_Pin;        // 引脚号（如GPIO_Pin_0）
    uint8_t ActiveLevel;      // 有效电平（0=低电平点亮，1=高电平点亮）
} Light_Config;

// 初始化函数（需传入配置）
void Light_Init(Light_Config *config);
void Light_On(Light_Config *config);
void Light_Off(Light_Config *config);
void Light_Blink(Light_Config *config, uint32_t onTime, uint32_t offTime, uint8_t repeat);
void Light_FadeIn(Light_Config *config, uint32_t duration_ms);
void Light_FadeOut(Light_Config *config, uint32_t duration_ms);
void Delay_ms(uint32_t nTime);

#endif /* __LIGHT_CONTROL_H */