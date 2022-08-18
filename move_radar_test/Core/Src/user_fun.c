

#include "main.h"


void StartFunTask(void const * argument)
{

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	SysRam.M_mode = MANU;// default = manual mode
	SysRam.Move_cnt = 0;

	for(;;)
	{
		if(SysRam.M_mode == AUTO)
		{
			HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET);//auto mode
			
			SysRam.Move_cnt++;
			if(SysRam.Move_cnt >= MAX_MV_TIME)
			{
				SysRam.Move_cnt = 0;

//				HAL_TIM_Base_Stop_IT(&htim3);
//				osDelay(50);
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
				osDelay(100);
				HAL_GPIO_TogglePin(M_DIR_GPIO_Port, M_DIR_Pin);
				HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
//				HAL_TIM_Base_Start_IT(&htim3);

			}			
		}
		else
		{
			HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET);//manual mode 
			HAL_GPIO_WritePin(M_DIR_GPIO_Port, M_DIR_Pin, GPIO_PIN_SET);//far away from the motor
			SysRam.Move_cnt = 0;
		}
				
		osDelay(1);
	}
	
}