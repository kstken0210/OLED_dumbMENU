#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "light_control.h"
#include "buttom.h"
#include "buzzer.h"
#include "OLED.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h.>
#include "W25Q64WRITE.h"
#include "W25Q64_DATASHEET.h"
#include "image_write.h"
#include "Encoder_control.h"

uint8_t MID;
uint16_t DID;

int main()
{
	OLED_Init();
	Init_everythings();//spi
	key_init();//key
	Init_all();//ui and encode
	OLED_Reverse();
	OLED_Clear();
	while(1)
	{
		if (key_flag==0)
		{
			menu_showing();
		}
		else if (key_flag==1)
		{
			menu_second_level();
		}
	}
}
