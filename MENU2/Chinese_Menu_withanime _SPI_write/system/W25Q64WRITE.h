#ifndef __W25Q64WRITE_H
#define __W25Q64WRITE_H
#include "stm32f10x.h"

uint8_t MYSPI_W_MISO(void);
void MYSPI_Init(void);
void SPI_Start(void);
void SPI_Stop(void);
uint8_t SwapByte(uint8_t Byte);
void W25Q64_ReadID(uint8_t *MID,uint16_t *DID);
void W25Q64_waitBusy(void);
void W25Q64_WriteEnable(void);
void W25Q64_writepage(uint32_t address,uint8_t *DataArray,uint16_t count);
void W25Q64_erase_sector(uint32_t address);
void W25Q64_erase_block(uint32_t address);
void W25Q64_ReadData(uint32_t address,uint8_t *DataArray,uint32_t count);


#endif