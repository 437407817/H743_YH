/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   使用串口1，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F103 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./middle_business/usart_485_dataMethod.h"
//#include "./usart/usart485verify.h"
#include "./pro_com/usart485verify.h"
#include "./sys/sysio.h"
//#include "./middle_business/usart_485_address.h"
//#include "./middle_business/usart_485_driver.h"
//#include "./global/GV_enum.h" 
#include "./DataConvert/data_convert.h"
#include "./global/GV_variable.h" 
#include "string.h"




extern STR_GET_VOL_Data_t GV_get_vol_485_232_Bigdata;

void Getdata485_voldata_process(uint8_t num,uint8_t** p_data){

//VpChange16HL((uint16_t*)*p_data,num);
	
	memmove(&GV_get_vol_485_232_Bigdata,*p_data,num);
	
EndianSwap_VpChange64HL_CM7((uint64_t *)&GV_get_vol_485_232_Bigdata);

EndianSwap_VpChange16HL((uint16_t *)&GV_get_vol_485_232_Bigdata.Bat_Vol,BATNUM*2);
	
	
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(PRINT_HEX,(uint8_t *)&GV_get_vol_485_232_Bigdata,num,"485 GET REAL DATA = ");
	

}









/*********************************************END OF FILE**********************/
