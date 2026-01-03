#include "stm32f10x.h"
#include "OLED.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h.>
#include "light_control.h"
#include "image_write.h"

uint8_t minimum_pos;
uint8_t maximum_pos;
uint8_t last_pos;
uint8_t curr_pos;

uint8_t last_str;
uint8_t curr_str;
int32_t encoder_count;

const char* lyric_lines[17] = {
    "第一天",//0
	"我存在",//4
	"第一次呼吸暢快",//8
    "站在地上的腳踝",//12
	"因為你",//16
	"而有真實感",//20
    "第一天",//24
	"我存在",//28
	"第一次",//32
	"能飛起來",//36
    "愛是",//0
	"騰空的魔幻",//44
	"第一天的純真色彩",//48
    "它總是",//52
	"永遠",//56
	"那麼",//60
	"燦爛",//64
};//each elements is a char array,so char* char as loop by byte/char
// still lyrics_line=n string with n character char strr[100][20]=100 string with 20 byte each

char str_array[6][20];//not uin8_t as transform as character not0-12 integer

uint8_t key_flag;
// mistake curr and last use 16 but min and max use 4
void encoder_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1 ;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=68-1; //from24->100
	TIM_TimeBaseInitStruct.TIM_Prescaler=1-1; 
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;//TIM_ICSelection_DirectTI
	//no need direct indir
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_IndirectTI;
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);

	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);
}

void key_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd= ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2 ); //can not repeat inittialize nvic
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel= EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_InitStruct);
}


void EXTI1_IRQHandler(void)	
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET)
	{
        Delay_ms(10); // 消抖
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) 
		{
			if (key_flag==0)
			{
				key_flag=1;
			}
			else if (key_flag==1)
			{
				key_flag=0;
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}
uint8_t key_state(void)
{
	return key_flag;
}


void Init_all(void)
{
	encoder_count=0;
	minimum_pos=0;
	maximum_pos=12;
	last_pos=0;
	curr_pos=0;
	
	last_str=0;
	curr_str=0;
	
	OLED_Init();
	encoder_init();
	strcpy(str_array[0], "Alcohol mode");
	strcpy(str_array[1], "Testing1");
	strcpy(str_array[2], "Testing2");
	strcpy(str_array[3], "Music App mode");
	strcpy(str_array[4], "Karaoke mode");
	strcpy(str_array[5], "Exit");
	for (uint8_t i=0;i<27;i++)
	{
		OLED_ShowImage(0, 0, width[i], height[i],image_reading(0x000000+address_offset(i),big_array,i)); //i=count=第幾張圖
		OLED_Reverse();
		OLED_Update();
		Delay_ms(80);
		if (i>=25)
		{
			Delay_ms(450);
		}
		OLED_Clear();
	}
	
	last_str=strlen(str_array[minimum_pos/4+0]);
	last_pos=(encoder_count - minimum_pos) / 4 * 16;
	Delay_ms(1000);
}

void menu_showing(void)
{
	encoder_count=TIM_GetCounter(TIM3);
	encoder_count%=24;
	if (encoder_count>maximum_pos+3)   /// as jump is from 1,2,3 not +4 diretly
	{
		minimum_pos+=4;
		maximum_pos+=4;
	}
	if (encoder_count<minimum_pos-3)    /// as jump is from 1,2,3 not +4 diretly
	{
		minimum_pos-=4;
		maximum_pos-=4;
	}//similat like hall sensor(late response)
	OLED_Clear();     // have it so always 反显 refer to 187 line
	
	
	OLED_ShowString(0, 0, str_array[minimum_pos/4+0], OLED_8X16);   //有翻页要思考 so mini/4+    curr line and accumulate value
	OLED_ShowString(0, 16, str_array[(minimum_pos/4)+1], OLED_8X16);
	OLED_ShowString(0, 32, str_array[(minimum_pos/4)+2], OLED_8X16);
	OLED_ShowString(0, 48, str_array[(minimum_pos/4)+3], OLED_8X16);
	OLED_ShowNum(104, 0,curr_pos, 3, OLED_8X16);
	
	curr_pos=(encoder_count - minimum_pos) / 4 * 16;
	curr_str=strlen(str_array[(encoder_count-minimum_pos)/4+minimum_pos/4])*8;    //why must at here for next part bro encoder pass value to here

	if (abs(last_pos-curr_pos)>=4)
	{		
		if (curr_pos > last_pos)
		{
			for (uint8_t i=0;i<10;i++)  // 反显两次不用都show  
			{
				OLED_ReverseArea(0,last_pos+i,(int)(last_str-((last_str-curr_str)/(12-i))),16);
				OLED_Update();                      //idk why update put in middle ok xDDD
				OLED_ReverseArea(0,last_pos+i,(int)(last_str-((last_str-curr_str)/(12-i))),16);
			}
		}
		else if (curr_pos < last_pos)  //change to else if
		{
			for (uint8_t i=0;i<10;i++)  // 反显两次不用都show  
			{
				OLED_ReverseArea(0,last_pos-i,(int)(last_str-((last_str-curr_str)/(12-i))),16);
				OLED_Update();                      //idk why update put in middle ok xDDD
				OLED_ReverseArea(0,last_pos-i,(int)(last_str-((last_str-curr_str)/(12-i))),16);
			}
		}
		
	}
	
	last_pos=curr_pos;
	last_str=curr_str;
	
	OLED_ReverseArea(0, curr_pos, curr_str, 16);
	OLED_Update();
	
	//remember the sequence
}

void lyrics_showing(const char* lyric_lines[])//middle function [] for 多個string array
{
	OLED_Clear();
	uint8_t new_encoder_count=0;
	//TIM_SetCounter(TIM3, 0); use when encoder_mode,looping no need
	while (key_flag==1)
	{
		//new_encoder_count=TIM_GetCounter(TIM3)%68; //base on *4
		new_encoder_count%=68;
		//y->6,6+12+6(24),6+12+6+16+6(46)
		OLED_Clear();
		if (new_encoder_count/4==0)
		{
			if (strlen(lyric_lines[(new_encoder_count+4)/4])%2==0)
			{
				//OLED_ClearArea(64-12*strlen(lyric_lines[(new_encoder_count+4)/4])/3/2, 46, 127, 12);
				OLED_ShowString(64-12*strlen(lyric_lines[(new_encoder_count+4)/4])/3/2, 46, lyric_lines[(new_encoder_count+4)/4], 12);
			}
			else
			{
				//OLED_ClearArea(64-12*strlen(lyric_lines[(new_encoder_count+4)/4])/3/2-5, 46, 127, 12);
				OLED_ShowString(64-12*strlen(lyric_lines[(new_encoder_count+4)/4])/3/2, 46, lyric_lines[(new_encoder_count+4)/4], 12);
			}
			if (strlen(lyric_lines[new_encoder_count/4])%2==0)
			{
				OLED_ShowString(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2, 24, lyric_lines[new_encoder_count/4], 8);
				OLED_ReverseArea(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2-10, 24, strlen(lyric_lines[new_encoder_count/4])/3*16+20, 16);
			}
			else
			{
				OLED_ShowString(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2, 24, lyric_lines[new_encoder_count/4], 8);
				OLED_ReverseArea(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2-10, 24, strlen(lyric_lines[new_encoder_count/4])/3*16+20, 16);
			}
		}
		else if (new_encoder_count/4==16)
		{
			if (strlen(lyric_lines[(new_encoder_count-4)/4])%2==0)
			{
				//OLED_ClearArea(64-12*strlen(lyric_lines[(new_encoder_count-4)/4])/3/2, 46, 127, 12);
				OLED_ShowString(64-12*strlen(lyric_lines[(new_encoder_count-4)/4])/3/2, 6, lyric_lines[(new_encoder_count-4)/4], 12);
			}
			else
			{
				OLED_ShowString(64-12*strlen(lyric_lines[(new_encoder_count-4)/4])/3/2, 6, lyric_lines[(new_encoder_count-4)/4], 12);
			}
			if (strlen(lyric_lines[new_encoder_count/4])%2==0)
			{
				
				OLED_ShowString(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2, 24, lyric_lines[new_encoder_count/4], 8);
				OLED_ReverseArea(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2-10, 24, strlen(lyric_lines[new_encoder_count/4])/3*16+20, 16);
			}
			else
			{
				OLED_ShowString(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2, 24, lyric_lines[new_encoder_count/4], 8);
				OLED_ReverseArea(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2-10, 24, strlen(lyric_lines[new_encoder_count/4])/3*16+20, 16);
			}
		}
		else
		{
			if (strlen(lyric_lines[(new_encoder_count-4)/4])%2==0)
			{
				OLED_ShowString(64-12*strlen(lyric_lines[(new_encoder_count-4)/4])/3/2, 6, lyric_lines[(new_encoder_count-4)/4], 12);
			}
			else
			{
				OLED_ShowString(64-12*strlen(lyric_lines[(new_encoder_count-4)/4])/3/2, 6, lyric_lines[(new_encoder_count-4)/4], 12);
				OLED_ReverseArea(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2, 24, strlen(lyric_lines[new_encoder_count/4])/3*16, 16);
			}
			if (strlen(lyric_lines[(new_encoder_count+4)/4])%2==0)
			{
				//OLED_ClearArea(64-12*strlen(lyric_lines[(new_encoder_count+4)/4])/3/2, 46, 127, 12);
				OLED_ShowString(64-12*strlen(lyric_lines[(new_encoder_count+4)/4])/3/2, 46, lyric_lines[(new_encoder_count+4)/4], 12);
			}
			else
			{
				//OLED_ClearArea(64-12*strlen(lyric_lines[(new_encoder_count+4)/4])/3/2-5, 46, 127, 12);
				OLED_ShowString(64-12*strlen(lyric_lines[(new_encoder_count+4)/4])/3/2, 46, lyric_lines[(new_encoder_count+4)/4], 12);
			}
			if (strlen(lyric_lines[new_encoder_count/4])%2==0)
			{
				OLED_ShowString(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2, 24, lyric_lines[new_encoder_count/4], 8);
				OLED_ReverseArea(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2-10, 24, strlen(lyric_lines[new_encoder_count/4])/3*16+20, 16);
			}
			else
			{
				OLED_ShowString(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2, 24, lyric_lines[new_encoder_count/4], 8);
				OLED_ReverseArea(64-16*strlen(lyric_lines[new_encoder_count/4])/3/2-10, 24, strlen(lyric_lines[new_encoder_count/4])/3*16+20, 16);
			}
		}
		OLED_Update();

		Delay_ms(strlen(lyric_lines[new_encoder_count/4])/3*110);//x*110
		new_encoder_count+=4;
	}
	
}
void lyrics_showing2(const char* lyric_lines[])
{
	OLED_Clear();
	OLED_Update();
	while(key_flag==1)
	{
		OLED_ShowString(0, 10, lyric_lines[0], 8);
		for (uint8_t v=0;v<strlen(lyric_lines[0])/3*16/3+1;v++) //like 3 word 9 byte 48 column =3*v +1 for marginal
			{
				OLED_UpdateArea(0,10, 3*v, 16); //final width=strlen(lyric_lines[z])/3*16 one word delay110ms one word 16 column=one column delay 7ms
				Delay_ms(21);
			}
			Delay_ms(150);

		for (uint8_t z=1;z<16;z++)
		{
			if (z%2==1)
			{
				OLED_ClearArea(0, 38, 128, 16);
				OLED_UpdateArea(0,38, 128, 16);
				OLED_ShowString(127-strlen(lyric_lines[z])/3*16, 38, lyric_lines[z], 8);
				for (uint8_t v=0;v<strlen(lyric_lines[z])/3*16/3+1;v++)  
				{
					OLED_UpdateArea(127-strlen(lyric_lines[z])/3*16, 38, 3*v, 16); //final width=strlen(lyric_lines[z])/3*16 one word delay110ms one word 16 column=one column delay 7ms
					Delay_ms(21);
				}
				Delay_ms(150);
			}
			else if (z%2==0)
			{
				OLED_ClearArea(0, 10,128, 16);
				OLED_UpdateArea(0,10, 128, 16);
				OLED_ShowString(0, 10, lyric_lines[z], 8);
				for (uint8_t v=0;v<strlen(lyric_lines[z])/3*16/3+1;v++)
				{
					OLED_UpdateArea(0,10, 3*v, 16); //final width=strlen(lyric_lines[z])/3*16 one word delay110ms one word 16 column=one column delay 7ms
					Delay_ms(21);
				}
				Delay_ms(150);
			}
		}
		OLED_ClearArea(0, 10,128, 16);
		OLED_UpdateArea(0,10, 128, 16);
		OLED_ShowString(0, 10, lyric_lines[16], 8);
		for (uint8_t v=0;v<strlen(lyric_lines[16])/3*16/3+1;v++)
			{
				OLED_UpdateArea(0, 10, 3*v, 16); //final width=strlen(lyric_lines[z])/3*16 one word delay110ms one word 16 column=one column delay 7ms
				Delay_ms(21);
			}
			Delay_ms(150);
	}
	key_flag=0;
}
void menu_second_level(void)
{
	if ((encoder_count-minimum_pos)/4+minimum_pos/4==0)
	{
		for (uint8_t j=0;j<10;j++)
		{
			for (uint8_t k=27;k<31;k++)
			{
				OLED_ShowImage(0, 0, width[k], height[k],image_reading(0x000000+address_offset(k),big_array,k));
				OLED_Reverse();
				OLED_Update();
				Delay_ms(80);
				OLED_Clear();
			}
		}
		key_flag=0;
	}
	else if ((encoder_count-minimum_pos)/4+minimum_pos/4==5)  //currlast use actual coordinate but minimumpos and count is 0~20
	{
		OLED_Clear();
		OLED_ShowImage(0, 0, width[31], height[31],image_reading(0x000000+address_offset(31),big_array,31));
		OLED_Update();
		for (uint8_t j=0;j<81;j++)
		{
			OLED_ReverseArea(20,34,j,12);
			OLED_Update();

			OLED_ClearArea(20,34,81,12);
		}
		key_flag=2;
		OLED_Clear();
		OLED_Update();
	}
	else if ((encoder_count-minimum_pos)/4+minimum_pos/4==4)  //currlast use actual coordinate but minimumpos and count is 0~20主+副 good determination
	{
		lyrics_showing(lyric_lines);//counter will back to 0
		key_flag=0;
		//OLED_Clear();
		//OLED_Update();
	}
	else if ((encoder_count-minimum_pos)/4+minimum_pos/4==3)  
	{
		lyrics_showing2(lyric_lines);
		//OLED_Clear();
		//OLED_Update();
	}
}