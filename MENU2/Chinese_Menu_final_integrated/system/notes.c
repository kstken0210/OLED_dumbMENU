//#include "stm32f10x.h"
//#include "W25Q64WRITE.h"
//#include "W25Q64_DATASHEET.h"
//#include "OLED_Data.h"
//#include "OLED.h"
//#include "light_control.h"
//
//int16_t storage_num_of_image_point;
//uint32_t curr_address=0x000000;  //highest two bit not use in spi
//uint16_t num_of_256=0;
//void Init_everythings(void)
//{
//	MYSPI_Init();
//}
//
//void image_writting(uint32_t address,uint8_t **DataArray,uint16_t counts)
//{
//	num_of_256=0;
//	curr_address=address;
//	//for (uint16_t w=0;w<counts;w++)
//	//{
//	//	storage_num_of_image_point=width[w]*height[w];
//		storage_num_of_image_point=width[counts]*height[counts];
//		while(storage_num_of_image_point>0)
//		{
//
//			if (storage_num_of_image_point>255)
//			{
//				W25Q64_WriteEnable();
//				SPI_Start();
//				SwapByte(W25Q64_PAGE_PROGRAM);
//				SwapByte(curr_address>>16);//0x00123456 高位舍弃,not>>4 is 4*4
//				SwapByte(curr_address>>8);
//				SwapByte(curr_address);
//				for (uint16_t i=0;i<256;i++)
//				{
//					SwapByte(DataArray[counts][i+(256*num_of_256)]);
//				}
//				SPI_Stop();
//				W25Q64_waitBusy();
//			}
//			else
//			{
//				W25Q64_WriteEnable();
//				SPI_Start();
//				SwapByte(W25Q64_PAGE_PROGRAM);
//				SwapByte(curr_address>>16);//0x00123456 高位舍弃,not>>4 is 4*4
//				SwapByte(curr_address>>8);
//				SwapByte(curr_address);
//				for (uint16_t i=0;i<storage_num_of_image_point;i++)
//				{
//					SwapByte(DataArray[counts][i+(256*num_of_256)]);
//				}
//				SPI_Stop();
//				W25Q64_waitBusy();
//			}
//			curr_address+=256;
//			storage_num_of_image_point-=256;
//			num_of_256+=1;
//		}
//	//}
//}
//
//void image_reading(uint32_t address,uint8_t *DataArray,uint32_t count)
//{
//	SPI_Start();
//	SwapByte(W25Q64_READ_DATA);
//	SwapByte(address>>16);//0x00123456 高位舍弃,not>>4 is 4*4
//	SwapByte(address>>8);
//	SwapByte(address);
//	storage_num_of_image_point=width[count]*height[count];
//	for (uint16_t i=0;i<storage_num_of_image_point;i++)
//	{
//		DataArray[i]=SwapByte(W25Q64_DUMMY_BYTE);//adress pointer 自增
//	}
//	SPI_Stop();
//}//每次只读一张图