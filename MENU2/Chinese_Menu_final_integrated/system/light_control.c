#include "light_control.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"


void Delay_ms(uint32_t nTime) {
    uint32_t i;
    for(; nTime > 0; nTime--) {
        for(i = 0; i < 8000; i++); // 粗略的1ms延迟
    }
}

// 初始化GPIO
void Light_Init(Light_Config *config) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 启用GPIO时钟
    if(config->GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } 
    // 可扩展其他GPIO端口...
    
    // 配置为推挽输出
    GPIO_InitStructure.GPIO_Pin = config->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(config->GPIOx, &GPIO_InitStructure);
    
    // 初始状态为关闭
    Light_Off(config);
}

// 点亮LED（根据ActiveLevel决定电平）
void Light_On(Light_Config *config) {
    if(config->ActiveLevel) {
        GPIO_SetBits(config->GPIOx, config->GPIO_Pin); // 高电平有效
    } else {
        GPIO_ResetBits(config->GPIOx, config->GPIO_Pin); // 低电平有效
    }
}

// 熄灭LED
void Light_Off(Light_Config *config) {
    if(config->ActiveLevel) {
        GPIO_ResetBits(config->GPIOx, config->GPIO_Pin); // 高电平有效时，低电平熄灭
    } else {
        GPIO_SetBits(config->GPIOx, config->GPIO_Pin); // 低电平有效时，高电平熄灭
    }
}

// 渐亮效果（支持任意引脚）
void Light_FadeIn(Light_Config *config, uint32_t duration_ms) {
    uint32_t cycleTime = duration_ms / 100;
    uint8_t duty;
    
    for(duty = 1; duty <= 100; duty++) {
        uint32_t onTime = cycleTime * duty / 100;
        uint32_t offTime = cycleTime - onTime;
        
        if(onTime > 0) {
            Light_On(config);
            Delay_ms(onTime);
        }
        if(offTime > 0) {
            Light_Off(config);
            Delay_ms(offTime);
        }
    }
    Light_On(config); // 最后保持全亮
}

// 渐暗效果（支持任意引脚）
void Light_FadeOut(Light_Config *config, uint32_t duration_ms) {
    uint32_t cycleTime = duration_ms / 100;
    uint8_t duty;
    
    for(duty = 100; duty > 0; duty--) {
        uint32_t onTime = cycleTime * duty / 100;
        uint32_t offTime = cycleTime - onTime;
        
        if(onTime > 0) {
            Light_On(config);
            Delay_ms(onTime);
        }
        if(offTime > 0) {
            Light_Off(config);
            Delay_ms(offTime);
        }
    }
    Light_Off(config); // 最后保持关闭
}

// 闪烁控制
void Light_Blink(Light_Config *config, uint32_t onTime, uint32_t offTime, uint8_t repeat) {
    uint8_t i;
    for(i = 0; i < repeat; i++) {
        Light_On(config);
        Delay_ms(onTime);
        Light_Off(config);
        Delay_ms(offTime);
    }
}