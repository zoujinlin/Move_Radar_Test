

#include "main.h"

void StartFunTask(void const * argument)
{
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	SysRam.M_mode = MANU;// default = manual mode
	SysRam.Move_cnt = 0;
	SysRam.M_state = M_STOP;

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
				osDelay(100);
				HAL_GPIO_TogglePin(M_DIR_GPIO_Port, M_DIR_Pin);
				HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
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

#define DELAY_CFG 75
void Ods_Cfg(UART_HandleTypeDef *uart)
{
	//*****************STANDARD MMWAVE SDK COMMANDS******************
	HAL_UART_Transmit(uart,(uint8_t*)"sensorStop\n", 11, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"flushCfg\n", 9, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"dfeDataOutputMode 1\n", 20, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"channelCfg 15 7 0\n", 18, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"adcCfg 2 1\n", 11, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"adcbufCfg -1 0 1 1 1\n", 21, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"profileCfg 0 60.5 100 25 69 0 0 50 1 256 6000 0 0 30\n", 53, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"chirpCfg 0 0 0 0 0 0 0 1\n", 25, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"chirpCfg 1 1 0 0 0 0 0 2\n", 25, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"chirpCfg 2 2 0 0 0 0 0 4\n", 25, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"frameCfg 0 2 32 0 100 1 0\n", 26, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"lowPower 0 0\n", 13, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"guiMonitor -1 1 0 0 0 0 0\n", 26, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"cfarCfg -1 0 2 8 4 3 0 20 0\n", 28, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"cfarCfg -1 1 0 4 2 3 1 15 0\n", 28, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"multiObjBeamForming -1 1 0.5\n", 29, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"clutterRemoval -1 1\n", 20, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"calibDcRangeSig -1 0 -5 8 256\n", 30, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"extendedMaxVelocity -1 0\n", 25, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"bpmCfg -1 0 0 1\n", 16, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"lvdsStreamCfg -1 0 0 0\n", 23, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"compRangeBiasAndRxChanPhase 0.0 1 0 -1 0 -1 0 1 0 1 0 -1 0 -1 0 1 0 1 0 -1 0 -1 0 1 0\n", 86, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"measureRangeBiasAndRxChanPhase 0 1.5 0.2\n", 41, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"CQRxSatMonitor 0 3 15 125 0\n", 28, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"CQSigImgMonitor 0 115 6\n", 24, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"analogMonitor 0 0\n", 18, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"aoaFovCfg -1 -90 90 -90 90\n", 27, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"cfarFovCfg -1 0 0 11.11\n", 24, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"cfarFovCfg -1 1 -2.04 2.04\n", 27, 1000);osDelay(DELAY_CFG);
	//*****************TRACKING COMMANDS*****************************
	HAL_UART_Transmit(uart,(uint8_t*)"staticBoundaryBox -8 8 0 8 -1 2\n", 32, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"boundaryBox -8 8 0 8 -1 2\n", 26, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"gatingParam 4 2 2 2 10\n", 23, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"stateParam 20 10 10 50 1\n", 25, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"allocationParam 200 155 0.5 10 2 2\n", 35, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"maxAcceleration 0.1 0.1 0.1\n", 28, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"trackingCfg 1 2 250 20 20 260 100 90\n", 37, 1000);osDelay(DELAY_CFG);
	//*****************STATIC DETECTION COMMANDS*********************
	HAL_UART_Transmit(uart,(uint8_t*)"heatmapGenCfg -1 1  0 6 50 60.0 3.0 10\n", 39, 1000);osDelay(DELAY_CFG);
	HAL_UART_Transmit(uart,(uint8_t*)"staticDetectionCfg -1 0 -50.0 +50.0 -30.0 20.0 0.7 5.0 0.2 4 20.0\n", 66, 1000);osDelay(DELAY_CFG);
	//*****************SENSOR START*********************
	HAL_UART_Transmit(uart,(uint8_t*)"sensorStart\n", 12, 1000);osDelay(DELAY_CFG);
}














