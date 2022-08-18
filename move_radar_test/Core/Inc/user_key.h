
#ifndef __USER_KEY__
#define __USER_KEY__

#include "main.h"

#define LONG_PRE 500//long press judge time (ms)

typedef struct
{
	uint8_t  state;
	uint8_t  flag;
	uint8_t  flag_store;
	uint32_t count;
}KEYTYPE;
extern KEYTYPE KeyRamSW1;

/*-----------KEY--------*/
#define KEYCYC     20 //MS

#define KEYINIT    0
#define KEYPS      1 //检测按下
#define KEYPSTRUE  2 //确认按下
#define KEYRLS     3 //检测松手
#define KEYRLSTRUE 4 //确认松手

void StartKeyTask(void const * argument); 
void KeyFunction(KEYTYPE * keyram, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif

