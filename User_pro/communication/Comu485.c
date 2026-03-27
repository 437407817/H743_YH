#include "./communication/Comu485.h"

#include "./sys/bsp_systime.h"   




//#include "./io/OUT_io/bsp_gpio_out.h" 

#include "./DataConvert/data_convert.h"
#include <stdlib.h>

 #include "./sys/sysio.h"
#include "string.h"

#include "./usart/bsp_usart_COM485.h"


#include "./Sys/systick.h"




/**
***********************************************************
* @brief 485输出任务处理函数（长度单字节）
* @param
* @return 
***********************************************************
*/
void c485_write_cmd(uint8_t *data,uint16_t size, uint8_t cmd,uint8_t OverGetRX) {      

	uint8_t *frame = NULL;
	uint16_t crc16_res;
	
	//frame = (uint8_t *)mymalloc(SRAMIN, size*2 + 8);
	
	#if (c485_CRC16==1)
	frame = (uint8_t *)malloc(size + 6);
	#else
	frame = (uint8_t *)malloc(size + 4);
	#endif
	
	
	if(frame == NULL) {
		SYSTEM_ERROR("CRC16 memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_HEAD_0;
	frame[1] = FRAME_HEAD_1;
	#if (c485_CRC16==1)	
	frame[2] = size + 3;
	#else	
	frame[2] = size + 1;
	#endif	
	frame[3] = cmd;

	//frame[3] = 75;
	for(int i=0;i<size;i++) {
	
		frame[i+4] = (uint8_t)data[i];

	}
	

	
	
	#if (c485_CRC16==1)
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	//crc16_res = crc_16(&frame[3], 3);
	frame[size + 4] = (uint8_t)(crc16_res & 0xff);
	frame[size + 5] = (uint8_t)((crc16_res & 0xff00) >> 8);
	
	//SYSTEM_DEBUG("crc2=0x%x 0x%x 0x%x",crc16_res,frame[size + 5],frame[size + 6]);
	//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size + 6,"crc =");
	_485_A_TX_EN();
	//DelayNclock(200);
	Usart_SendArray(&huart_COM485_Handle,frame,size + 6);
	//DelayNclock(200);
	if(OverGetRX){
	_485_A_RX_EN();
	}
	
		#else
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 6,"Usart_SendArray 82 write TO SCREEN =");
	_485_A_TX_EN();
	DelayNclock(200);
	Usart_SendArray(USART_485_A,frame,size*2 + 6);
	DelayNclock(200);
	_485_A_RX_EN();
  #endif
	//HAL_UART_Transmit(&huart1,frame ,size*2 + 8, 1000);

	
	
	
	
free(frame);
	frame = NULL;
	//myfree(SRAMIN, frame);
}


void c485_write_cmd_print(uint8_t *data,uint16_t size, uint8_t cmd,uint8_t OverGetRX, uint16_t debugprint) {      

	uint8_t *frame = NULL;
	uint16_t crc16_res;
	
	//frame = (uint8_t *)mymalloc(SRAMIN, size*2 + 8);
	
	#if (c485_CRC16==1)
	frame = (uint8_t *)malloc(size + 6);
	#else
	frame = (uint8_t *)malloc(size + 4);
	#endif
	
	
	if(frame == NULL) {
		SYSTEM_ERROR("CRC16 memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_HEAD_0;
	frame[1] = FRAME_HEAD_1;
	#if (c485_CRC16==1)	
	frame[2] = size + 3;
	#else	
	frame[2] = size + 1;
	#endif	
	frame[3] = cmd;

	//frame[3] = 75;
	for(int i=0;i<size;i++) {
	
		frame[i+4] = (uint8_t)data[i];

	}
	

	
	
	#if (c485_CRC16==1)
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	//crc16_res = crc_16(&frame[3], 3);
	frame[size + 4] = (uint8_t)(crc16_res & 0xff);
	frame[size + 5] = (uint8_t)((crc16_res & 0xff00) >> 8);
	
	//SYSTEM_DEBUG("crc2=0x%x 0x%x 0x%x",crc16_res,frame[size + 5],frame[size + 6]);
	//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size + 6,"crc =");
		if(debugprint==1){
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size + 6,"485 send TO slave ***----- %d----",GetSysRunTime());
	}
	_485_A_TX_EN();
	//DelayNclock(200);
	Usart_SendArray(&huart_COM485_Handle,frame,size + 6);
	//DelayNclock(200);
		if(OverGetRX){
	_485_A_RX_EN();
	}
		#else
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 6,"Usart_SendArray 82 write TO SCREEN =");
	_485_A_TX_EN();
	DelayNclock(200);
	Usart_SendArray(USART_485_A,frame,size*2 + 6);
	DelayNclock(200);
	_485_A_RX_EN();
  #endif
	//HAL_UART_Transmit(&huart1,frame ,size*2 + 8, 1000);

	
	
	
	
free(frame);
	frame = NULL;
	//myfree(SRAMIN, frame);
}

















































