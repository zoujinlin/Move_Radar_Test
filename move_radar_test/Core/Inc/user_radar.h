#ifndef __USER_RADAR__
#define __USER_RADAR__
#include "main.h"


//radar received data type
typedef struct
{
	uint64_t magic;//magic word
	uint32_t tpl;//total packet length, include magic word
	uint32_t pll;//payload len
	uint32_t TLVs;//num TLVs
	uint32_t T_type;//TLV type
	uint32_t T_len;//TLV len
	
}RADARTYPE;
extern RADARTYPE RadarRam;


extern void StartRadarTask(void const * argument);
extern void Radar_Resolver(uint8_t *databuff, RADARTYPE *result);



#endif

