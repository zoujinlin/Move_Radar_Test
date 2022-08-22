
#include "main.h"


RADARTYPE RadarRam;

void Radar_Resolver(uint8_t *databuff, RADARTYPE *result, SYSRAMTYPE *sysram);

void StartRadarTask(void const * argument)
{
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, SysRam.urbuff, BUFF_LEN);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, SysRam.ur2buff, UR2R_LEN);
	for(;;)
	{
		if(SysRam.rcv_flag == 1)//data process
		{
			SysRam.rcv_flag = 0;
			
			//memcpy(SysRam.spcbuff, SysRam.urbuff, 30);
			//HAL_UART_Transmit_DMA(&huart2,(uint8_t*)SysRam.spcbuff, 30);
			
			Radar_Resolver(SysRam.urbuff, &RadarRam, &SysRam);
			memset(SysRam.urbuff, 0x00, 8);//magic word set to 0
			HAL_UARTEx_ReceiveToIdle_DMA(&huart3, SysRam.urbuff, BUFF_LEN);
			
		}

		SysRam.rcv_timeout++;
		if(SysRam.rcv_timeout >= 125)//up to 250ms, reset the uart3
		{
			HAL_UARTEx_ReceiveToIdle_DMA(&huart3, SysRam.urbuff, BUFF_LEN);
		}
		
		if(SysRam.send_cfg == 1)//set radar parameter
		{
			SysRam.send_cfg = 0;
			Ods_Cfg(&huart1);
		}
		
		if(SysRam.rcv2_flag == 1)
		{
			SysRam.rcv2_flag = 0;
			
			if(SysRam.ur2buff[0] == 0xAA)
			{
					SysRam.move_com = SysRam.ur2buff[1];
			}
			
			HAL_UARTEx_ReceiveToIdle_DMA(&huart2, SysRam.ur2buff, UR2R_LEN);
		}
		osDelay(1);
	}
}


void Radar_Resolver(uint8_t *databuff, RADARTYPE *result, SYSRAMTYPE *sysram)
{
	uint8_t *ptr;
	uint32_t i;
	
	ptr = databuff;
	memcpy((uint8_t *)&result->magic, ptr, 8); ptr += 8;//magic word
	if(result->magic != 0x0708050603040102)
	{
		return;
	}
	ptr += 4;//version
	memcpy((uint8_t *)&result->tpl, ptr, 4);ptr += 4;//total packet length
	result->pll = result->tpl - 44;//payload len
	ptr += 4;//platform
	ptr += 4;//frame number
	ptr += 4;//time(in cpu cycles)
	ptr += 4;//num detected obj
	memcpy((uint8_t *)&result->TLVs, ptr, 4);ptr += 4;//num TLVs
	ptr += 4;//subframe number
	ptr += 4;//num static detected obj
	for(i=result->TLVs; i>0; i--)
	{
		memcpy((uint8_t *)&result->T_type, ptr, 4);ptr += 4;//TLV type
		memcpy((uint8_t *)&result->T_len, ptr, 4);ptr += 4;//TLV len
		if(result->T_type == 10)//tracked object 
		{
			memcpy((uint8_t *)&result->T_id, ptr, 4);ptr += 4;//tracked object id
			//memcpy((uint8_t *)&result->posX, ptr, 4);ptr += 4;
			ptr += 4;
			memcpy((uint8_t *)&result->posY, ptr, 4);ptr += 4;
			//memcpy((uint8_t *)&result->velX, ptr, 4);ptr += 4;
			ptr += 4;
			memcpy((uint8_t *)&result->velY, ptr, 4);ptr += 4;
			
			if(sysram->move_com == 1)//
			{
				switch(sysram->M_dir)
				{
					case M_STOP:
						if(sysram->M_pos_end == 1)
							result->posY += 0.25;
						break;
					case M_FAR:
						result->posY += sysram->Move_cnt*0.00025;
						result->velY += 0.25;
						break;
					case M_NEAR:
						result->velY -= 0.25;
						result->posY += (MAX_MV_TIME - sysram->Move_cnt)*0.00025;
						break;
					default:
						break;
				}
			}
			printf("id=%d,posY=%f,velY=%f,dir=%d,com=%d\n",result->T_id,result->posY,result->velY,sysram->M_dir,sysram->move_com);
			ptr += 20; 
		}
		else
		{
			ptr += result->T_len;
		}
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

