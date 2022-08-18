
#include "main.h"


KEYTYPE KeyRamSW1;

/*------------------------------ KEY TASK ------------------------------------*/
void KeyFunction(KEYTYPE * keyram, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
		switch(keyram->state)
		{
			case KEYINIT:
				if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 0)//press down
					keyram->state = KEYPS;
				break;
			case KEYPS:
				if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 0)//press down
				{
					keyram->state = KEYPSTRUE;				
					keyram->flag  = 1;
				}
				else
					keyram->state = KEYINIT;
				break;
			case KEYPSTRUE:
				if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 1)//release
					keyram->state = KEYRLS;				
				else
				{
					if(keyram->count < 5000)
						keyram->count += KEYCYC;
				}
				break;
			case KEYRLS:
				if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 1)//release
				{
					if(keyram->count < LONG_PRE)
					{
						keyram->state = KEYRLSTRUE;		
					}
					else
					{
						keyram->state = KEYINIT;
					}
					keyram->flag  = 0;
					keyram->count = 0;
				}
				else
					keyram->state = KEYPSTRUE;
				break;
			case KEYRLSTRUE:
				keyram->state = KEYINIT;
				break;
			default:
				keyram->state = KEYINIT;
				break;
		}
}

void StartKeyTask(void const * argument)
{
	memset(( void * )&KeyRamSW1, 0x00, sizeof(KEYTYPE));

	for(;;)
	{
		
		KeyFunction(&KeyRamSW1, KEY_B_GPIO_Port, KEY_B_Pin);	
		
		if(KeyRamSW1.count == LONG_PRE)
		{
			if(SysRam.M_mode == AUTO)
				SysRam.M_mode = MANU;
			else if(SysRam.M_mode == MANU)
				SysRam.M_mode = AUTO;
		}
		
		osDelay(KEYCYC);
	}
}	



