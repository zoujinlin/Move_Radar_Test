
#include "main.h"


RADARTYPE RadarRam;


void StartRadarTask(void const * argument)
{
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, SysRam.urbuff, BUFF_LEN);
	//HAL_UARTEx_ReceiveToIdle_IT(&huart3, SysRam.urbuff, BUFF_LEN);
	for(;;)
	{
		if(SysRam.rcv_flag == 1)//data process
		{
			SysRam.rcv_flag = 0;
			
			memcpy(SysRam.spcbuff, SysRam.urbuff, 30);
			HAL_UART_Transmit_DMA(&huart2,(uint8_t*)SysRam.spcbuff, 30);
			
			Radar_Resolver(SysRam.urbuff, &RadarRam);
			memset(SysRam.urbuff, 0x00, 8);//magic word set to 0
			HAL_UARTEx_ReceiveToIdle_IT(&huart3, SysRam.urbuff, BUFF_LEN);
			HAL_UARTEx_ReceiveToIdle_DMA(&huart3, SysRam.urbuff, BUFF_LEN);
			
		}

		SysRam.rcv_timeout++;
		if(SysRam.rcv_timeout >= 125)//up to 250ms, reset the uart3
		{
			//HAL_UARTEx_ReceiveToIdle_IT(&huart3, SysRam.urbuff, BUFF_LEN);
			HAL_UARTEx_ReceiveToIdle_DMA(&huart3, SysRam.urbuff, BUFF_LEN);
		}
		
		if(SysRam.send_cfg == 1)//set radar parameter
		{
			SysRam.send_cfg = 0;
			Ods_Cfg(&huart1);
		}
		
	}
	osDelay(1);
}


void Radar_Resolver(uint8_t *databuff, RADARTYPE *result)
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
			
		}
		else
		{
			
		}
	}
	

}


