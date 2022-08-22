

#include "main.h"

void StartFunTask(void const * argument)
{
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	SysRam.M_mode = MANU;// default = manual mode
	SysRam.Move_cnt = 0;
	SysRam.M_pos_end = 0;
	for(;;)
	{
		if(SysRam.M_mode == AUTO)
		{
			HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET);//auto mode

			SysRam.Move_cnt++;
			if(SysRam.Move_cnt >= MAX_MV_TIME)
			{
				SysRam.Move_cnt = 0;

				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
				SysRam.M_dir = M_STOP;
				SysRam.M_pos_end = SysRam.M_pos_end == 0 ? 1:0;
				osDelay(1000);
				HAL_GPIO_TogglePin(M_DIR_GPIO_Port, M_DIR_Pin);
				SysRam.M_dir = HAL_GPIO_ReadPin(M_DIR_GPIO_Port, M_DIR_Pin) == GPIO_PIN_SET ? M_FAR:M_NEAR;
				HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
			}			
		}
		else
		{
			HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET);//manual mode 
			HAL_GPIO_WritePin(M_DIR_GPIO_Port, M_DIR_Pin, GPIO_PIN_SET);//far away from the motor
			SysRam.Move_cnt = 0;
			SysRam.M_dir = M_FAR;
		}
		
		osDelay(1);
	}
	
}















