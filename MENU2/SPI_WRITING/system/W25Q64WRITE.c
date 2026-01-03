 #include "stm32f10x.h"
#include "W25Q64_DATASHEET.h"

#define SPI_W_SS(x)		    GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(x))
#define SPI_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)(x))
//#define SPI_W_MISO(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_14, (BitAction)(x))
#define SPI_W_MOSI(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)(x))

uint8_t MYSPI_W_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
}

void MYSPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_15;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI_W_SS(1);
	SPI_W_SCL(0);
}
void SPI_Start(void)
{
	SPI_W_SS(0);
}

void SPI_Stop(void)
{
	SPI_W_SS(1);
}

uint8_t SwapByte(uint8_t Byte)
{
	uint8_t inp=0x00;
	for (uint8_t i=0;i<8;i++)
	{//adjust 相位by 先clk 后做 now mode 0
		SPI_W_MOSI(!!(Byte&(0x80>>i)));
		SPI_W_SCL(1);
		if (MYSPI_W_MISO())
		{
			inp|=(0x80>>i);//have alt way 移入
		}
		else
		{
		}
		SPI_W_SCL(0);//when finish must 0
	}	
	return inp;
}


void W25Q64_ReadID(uint8_t *MID,uint16_t *DID)//when more than one return
{
	SPI_Start();
	SwapByte(W25Q64_JEDEC_ID);//第一个byte is cmd not address or data directly
	*MID=SwapByte(W25Q64_DUMMY_BYTE);//althought same way but differnt outcome due to 时间顺序
	*DID=SwapByte(W25Q64_DUMMY_BYTE);
	*DID<<=8;//remenber =
	*DID|=SwapByte(W25Q64_DUMMY_BYTE);
	SPI_Stop();
}

void W25Q64_waitBusy(void)
{
	uint32_t Timeout;
	SPI_Start();
	SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timeout = 100000;
	while ((SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	SPI_Stop();
}

void W25Q64_WriteEnable(void)
{
	SPI_Start();
	SwapByte(W25Q64_WRITE_ENABLE);
	SPI_Stop();
}

void W25Q64_writepage(uint32_t address,uint8_t *DataArray,uint16_t count)//32 for 24 bit,16 for 0-256
{
	W25Q64_WriteEnable();
	SPI_Start();
	SwapByte(W25Q64_PAGE_PROGRAM);
	SwapByte(address>>16);//0x00123456 高位舍弃,not>>4 is 4*4
	SwapByte(address>>8);
	SwapByte(address);
	for (uint16_t i=0;i<count;i++)
	{
		SwapByte(DataArray[i]);
	}
	SPI_Stop();
	W25Q64_waitBusy();
}

void W25Q64_erase_sector(uint32_t address)
{
	W25Q64_WriteEnable();
	SPI_Start();
	SwapByte(W25Q64_SECTOR_ERASE_4KB);
	SwapByte(address>>16);//0x00123456 高位舍弃,not>>4 is 4*4
	SwapByte(address>>8);
	SwapByte(address);
	SPI_Stop();
	W25Q64_waitBusy();
}


void W25Q64_erase_block(uint32_t address)
{
	W25Q64_WriteEnable();
	SPI_Start();
	SwapByte(W25Q64_BLOCK_ERASE_64KB);
	SwapByte(address>>16);//0x00123456 高位舍弃,not>>4 is 4*4
	SwapByte(address>>8);
	SwapByte(address);
	SPI_Stop();
	W25Q64_waitBusy();
}
void W25Q64_ReadData(uint32_t address,uint8_t *DataArray,uint32_t count)
{
	SPI_Start();
	SwapByte(W25Q64_READ_DATA);
	SwapByte(address>>16);//0x00123456 高位舍弃,not>>4 is 4*4
	SwapByte(address>>8);
	SwapByte(address);
	for (uint32_t i=0;i<count;i++)
	{
		DataArray[i]=SwapByte(W25Q64_DUMMY_BYTE);//adress pointer 自增
	}
	SPI_Stop();
}