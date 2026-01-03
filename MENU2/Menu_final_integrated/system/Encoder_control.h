#ifndef __ENCODER_CONTROL_H
#define __ENCODER_CONTROL_H

#include "stm32f10x.h"


void encoder_init(void);
void Init_all(void);
void menu_showing(void);
void key_init(void);
uint8_t key_state(void);
void menu_second_level(void);
void lyrics_showing(const char* lyric_lines[]);
void lyrics_showing2(const char* lyric_lines[]);
extern uint8_t minimum_pos;
extern uint8_t maximum_pos;
extern uint8_t last_pos;/* __LIGHT_CONTROL_H */
extern uint8_t curr_pos; 
extern uint8_t last_str;
extern uint8_t curr_str;
extern int32_t encoder_count;
extern char str_array[6][20];
extern uint8_t key_flag;
extern const char* lyric_lines[];

#endif 