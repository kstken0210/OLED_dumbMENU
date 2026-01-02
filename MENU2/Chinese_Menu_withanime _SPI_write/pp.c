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

uint8_t MID;
uint16_t DID;
uint16_t check;
int main()
{
	
	uint8_t big_array[1200];//128*64
	
	OLED_Init();
	Init_everythings();
	
	
	W25Q64_ReadID(&MID,&DID);//use &
	OLED_ShowHexNum(0,0,MID,2,OLED_8X16);
	OLED_ShowHexNum(0,16,DID,4,OLED_8X16);
	OLED_Update();
	W25Q64_erase_block(0x000000);
	image_writting(0x000000,image_ptrs,31);// not index just pos

	for (uint8_t g=0;g<25;g++)
	{
		if (height[g]%8==0)
		{
			r=(height[g]);
		}
		else
		{
			r=(1+height[g]/8)*8;
		}
		if ((width[g]*r/8)%256==0)
		{
			check+=(width[g]*r/8);
		}
		else
		{
			check+=((width[g]*r/8)/256+1)*256; //// as each image start address always at the start of new page
		}
			
	}
	image_reading(0x000000+check,big_array,25); //index  6930+256-18  is it like width[0]*height[0]+256*1-(width[0]*height[0])%256 for second one
	OLED_ShowImage(0, 0, width[25], height[25], big_array);  //index
	OLED_Update();
	Delay_ms(1000);
	OLED_Clear();
	OLED_ShowImage(0, 0, width[2], height[2], image_ptrs[2]);  //index
	OLED_Update();
	
	
}
