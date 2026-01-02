#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*字符集定义*/
/*以下两个宏定义只可解除其中一个的注释*/
#define OLED_CHARSET_UTF8			//定义字符集为UTF8
//#define OLED_CHARSET_GB2312		//定义字符集为GB2312

/*字模基本单元*/
typedef struct 
{
	
#ifdef OLED_CHARSET_UTF8			//定义字符集为UTF8
	char Index[5];					//汉字索引，空间为5字节
#endif
	
#ifdef OLED_CHARSET_GB2312			//定义字符集为GB2312
	char Index[3];					//汉字索引，空间为3字节
#endif
	
	uint8_t Data[32];				//字模数据
} ChineseCell_t;

/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];

/*图像数据声明*/
extern const uint8_t Diode[];
/*按照上面的格式，在这个位置加入新的图像数据声明*/
//...
extern const uint8_t saber[];

extern const uint8_t saber2[];

extern const uint8_t anime1[]; //115 63
extern const uint8_t anime2[];//110 60
extern const uint8_t anime3[];//110 58
extern const uint8_t anime4[];//105 62
extern const uint8_t anime5[];//90 61
extern const uint8_t anime6[];//100 61
extern const uint8_t anime7[];//105 62
extern const uint8_t anime8[];//100 60
extern const uint8_t anime9[];//95 62
extern const uint8_t anime10[];//110 61
extern const uint8_t anime11[];//105 64
extern const uint8_t anime12[];//110 62
extern const uint8_t anime13[];//110 62
extern const uint8_t anime14[];//110 62
extern const uint8_t anime15[];//115 61
extern const uint8_t anime16[];//115 60
extern const uint8_t anime17[];//116 62
extern const uint8_t anime18[];//115 60
extern const uint8_t anime19[];//110 62
extern const uint8_t anime20[];//115 60
extern const uint8_t anime21[];//115 61
extern const uint8_t anime22[];//115 60
extern const uint8_t anime23[];//115 60
extern const uint8_t anime24[];//115 60
extern const uint8_t anime25[];//80 60
extern const uint8_t anime26[];//50 62
extern const uint8_t anime27[];//120 63
extern const uint8_t anime28[];//115 63
extern const uint8_t anime29[];//115 63
extern const uint8_t anime30[];//115 62
extern const uint8_t anime31[];//115 60
extern const uint8_t anime32[];//119 64

extern uint8_t *image_ptrs[];
extern uint8_t height[];
extern uint8_t width[];

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
