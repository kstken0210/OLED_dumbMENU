#ifndef __IMAGE_WRITE_H
#define __IMAGE_WRITE_H
#include "stm32f10x.h"

void Init_everythings(void);
uint8_t* image_reading(uint32_t address,uint8_t *DataArray,uint32_t count);
uint16_t address_offset(uint8_t g);

extern uint32_t curr_address;
extern uint16_t num_of_256;
extern uint16_t r;
extern uint16_t check;
extern uint8_t big_array[1200];

#endif