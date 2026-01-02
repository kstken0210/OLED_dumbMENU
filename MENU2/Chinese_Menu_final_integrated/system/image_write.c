#include "stm32f10x.h"
#include "W25Q64WRITE.h"
#include "W25Q64_DATASHEET.h"
#include "OLED_Data.h"
#include "OLED.h"
#include "light_control.h"

int16_t storage_num_of_image_point;
uint32_t curr_address=0x000000;  //highest two bit not use in spi no need 0x000000000
uint16_t num_of_256=0;
uint16_t r;
uint16_t check;
uint8_t big_array[1200];
void Init_everythings(void)
{
	MYSPI_Init();
}

uint16_t address_offset(uint8_t ww)
{
	check=0;//bruh no init??
	for (uint16_t g=0;g<ww;g++)
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
	return check;
}
uint8_t* image_reading(uint32_t address,uint8_t *DataArray,uint32_t count)
{
	SPI_Start();
	SwapByte(W25Q64_READ_DATA);
	SwapByte(address>>16);//0x00123456 高位舍弃,not>>4 is 4*4
	SwapByte(address>>8);
	SwapByte(address);
	if (height[count]%8==0)
	{
		r=(height[count]);
	}
	else
	{
		r=(1+height[count]/8)*8;
	}
	storage_num_of_image_point=width[count]*r/8;
	for (uint16_t i=0;i<storage_num_of_image_point;i++)
	{
		DataArray[i]=SwapByte(W25Q64_DUMMY_BYTE);//adress pointer 自增
	}
	SPI_Stop();
	return DataArray;
}//每次只读一张图


