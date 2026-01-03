#ifndef __IMAGE_WRITE_H
#define __IMAGE_WRITE_H
#include "stm32f10x.h"

void Init_everythings(void);
void image_writting(uint32_t address,uint8_t *DataArray,uint16_t count);
void image_reading(uint32_t address,uint8_t *DataArray,uint32_t count);


extern uint32_t curr_address;
extern uint16_t num_of_256;
extern uint16_t r;
#endif