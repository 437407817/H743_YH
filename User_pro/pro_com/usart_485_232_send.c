#include "./pro_com/usart_485_232_send.h"

#include "./sys/bsp_systime.h"   




#include "./io/bsp_io_output.h"   

#include "./DataConvert/data_verification.h"
//#include "./converter/data_convert.h"
#include <stdlib.h>

 #include "./sys/sysio.h"
#include "string.h"

#include "./usart/bsp_usart_COM485.h"
#include "./sys/systick.h"
#include "./usart/bsp_usart.h"

 

#define FRAME_SEND_HEAD_0           0x5a  
#define FRAME_SEND_HEAD_1           0xa5

#define DW_CRC16   c485_232_CRC16



//5AA5 04 83 1002 01是读单个1002
//5AA5 04 83 1002 02是读1002，1003，连续的
//如果想写多个，5AA5 07 82 1002 1234 5678
//5AA5 04 83 1002 02是读1002，1003，连续的,返回,5AA5 08 83 1002 02 0001 0002







/**
***********************************************************
* @brief 485输出任务处理函数（长度单字节）
* @param
* @return 
***********************************************************
*/
//单片机是小端数据模式，该函数会自动转化成大端模式后发送
void com_DW_write_cmd(uint16_t addr, uint16_t *buf, uint16_t size) {      //82

//	Cal_start_ticktime(1);
	uint8_t *frame = NULL;
	#if (DW_CRC16==1)	
	uint16_t crc16_res;
		#endif
	//frame = (uint8_t *)mymalloc(SRAMIN, size*2 + 8);
	
	#if (DW_CRC16==1)
	frame = (uint8_t *)malloc(size*2 + 8);
	#else
	frame = (uint8_t *)malloc(size*2 + 6);
	#endif
//	SYSTEM_INFO("SCREEN_write_cmd 1  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
	
	if(frame == NULL) {
		SYSTEM_ERROR("DW_CRC16 memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_SEND_HEAD_0;
	frame[1] = FRAME_SEND_HEAD_1;
	#if (DW_CRC16==1)	
	frame[2] = size*2 + 5;
	#else	
	frame[2] = size*2 + 3;
	#endif	
	frame[3] = COM_DW_WRITE_CMD;
	frame[4] = (uint8_t)((addr & 0xff00) >> 8);
	frame[5] = (uint8_t)(addr & 0xff);
	
	for(int i=0;i<size;i++) {
		frame[2*i+6] = (uint8_t)((buf[i] & 0xff00) >> 8);
		frame[2*i+7] = (uint8_t)(buf[i] & 0xff);
	}
	#if USE_c485
		_485_A_TX_EN();
	#endif
	#if (DW_CRC16==1)
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	frame[size*2 + 6] = (uint8_t)(crc16_res & 0xff);
	frame[size*2 + 7] = (uint8_t)((crc16_res & 0xff00) >> 8);
	
//SYSTEM_DEBUG("crc2=0x%x 0x%x 0x%x",crc16_res,frame[size*2 + 6],frame[size*2 + 7]);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 8,"write TO SCREEN =");

	Usart_DW_SendArray(&huart_COM_DW_Handle,frame,size*2 + 8);
	
		#else
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 6,"SCREEN write 82 write TO SCREEN =");
//	SYSTEM_INFO("SCREEN_write_cmd 2  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
//	Cal_diff_ticktime(1);
	Usart_DW_SendArray(&huart_485_Handle,frame,size*2 + 6);
  #endif
	//HAL_UART_Transmit(&huart1,frame ,size*2 + 8, 1000);


//	SYSTEM_INFO("SCREEN_write_cmd 3  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
	#if USE_c485
	_485_A_RX_EN();
	 #endif
	
	
free(frame);
	frame = NULL;
	//myfree(SRAMIN, frame);
}



//单片机是小端数据模式，该函数会自动转化成大端模式后发送
void DW_SCREEN_write_cmd_print(uint16_t addr, uint16_t *buf, uint16_t size, uint16_t debugprint) {      //82

	uint8_t *frame = NULL;
	#if (DW_CRC16==1)	
	uint16_t crc16_res;
		#endif
	//frame = (uint8_t *)mymalloc(SRAMIN, size*2 + 8);
	
	#if (DW_CRC16==1)
	frame = (uint8_t *)malloc(size*2 + 8);
	#else
	frame = (uint8_t *)malloc(size*2 + 6);
	#endif
	
	
	if(frame == NULL) {
		SYSTEM_ERROR("DW_CRC16 memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_SEND_HEAD_0;
	frame[1] = FRAME_SEND_HEAD_1;
	#if (DW_CRC16==1)	
	frame[2] = size*2 + 5;
	#else	
	frame[2] = size*2 + 3;
	#endif	
	frame[3] = COM_DW_WRITE_CMD;
	frame[4] = (uint8_t)((addr & 0xff00) >> 8);
	frame[5] = (uint8_t)(addr & 0xff);
	
	for(int i=0;i<size;i++) {
		frame[2*i+6] = (uint8_t)((buf[i] & 0xff00) >> 8);
		frame[2*i+7] = (uint8_t)(buf[i] & 0xff);
	}
	#if USE_c485
	_485_A_TX_EN();
	#endif
	
	#if (DW_CRC16==1)
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	frame[size*2 + 6] = (uint8_t)(crc16_res & 0xff);
	frame[size*2 + 7] = (uint8_t)((crc16_res & 0xff00) >> 8);
	
//SYSTEM_DEBUG("crc2=0x%x 0x%x 0x%x",crc16_res,frame[size*2 + 6],frame[size*2 + 7]);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 8,"write TO SCREEN =");
	
	Usart_DW_SendArray(&huart_COM_DW_Handle,frame,size*2 + 8);
	
		#else
	if(debugprint==1){
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 6,"SCREEN write 82 data TO SCREEN ===");
	}
//	
	Usart_DW_SendArray(&huart_485_Handle,frame,size*2 + 6);
  #endif
	
	#if USE_c485
	_485_A_RX_EN();
	#endif
	//HAL_UART_Transmit(&huart1,frame ,size*2 + 8, 1000);

	
	
	
	
free(frame);
	frame = NULL;
	//myfree(SRAMIN, frame);
}






void DW_SCREEN_read_cmd_print(uint16_t addr, uint8_t num, uint16_t debugprint) {      //83

	uint8_t *frame = NULL;
	#if (DW_CRC16==1)	
	uint16_t crc16_res;
	#endif
	if(num > 0x7c) {
				SYSTEM_ERROR("Data read is too much than 0x7c\n");
		return;
	}
	
	
	#if (DW_CRC16==1)	
	frame = (uint8_t *)malloc(9);
	#else
	frame = (uint8_t *)malloc(7);
	#endif	
	if(frame == NULL) {
				SYSTEM_ERROR("memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_SEND_HEAD_0;
	frame[1] = FRAME_SEND_HEAD_1;
#if (DW_CRC16==1)	
	frame[2] = 0x06;
#else
	frame[2] = 0x04;
#endif	
	frame[3] = COM_DW_READ_CMD;
	frame[4] = (uint8_t)((addr & 0xff00) >> 8);
	frame[5] = (uint8_t)(addr & 0xff);
	frame[6] = num;
	
	#if USE_c485
	_485_A_TX_EN();
	#endif
	
	#if (DW_CRC16==1)	
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	frame[7] = (uint8_t)(crc16_res & 0xff);	
	frame[8] = (uint8_t)((crc16_res & 0xff00) >> 8);
		
		Usart_DW_SendArray(&huart_COM_DW_Handle,frame,9);
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,9,"Usart_DW_SendArray 83 read TO SCREEN =");
	#else
	
		Usart_DW_SendArray(&huart_485_Handle,frame,7);
			if(debugprint==1){
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,7,"SCREEN write 0x83 read data TO SCREEN ===");
	}
	//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,7,"SCREEN read 83 read TO SCREEN =");
  #endif
	#if USE_c485
	_485_A_RX_EN();
#endif
free(frame);
	frame = NULL;
}

































