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
  
#include "./middle_business/usart_485_business.h"
//#include "./usart/usart485verify.h"
#include "./pro_com/usart485verify.h"
#include "./sys/sysio.h"
//#include "./middle_business/usart_485_address.h"
//#include "./middle_business/usart_485_driver.h"






void pull_data_from_485(void)
{
	
uint8_t *frame = NULL;
uint8_t* o_data=NULL;	
uint8_t o_leng=0;//必须为0
	
uint16_t comand_addr=0;

uint8_t s_num = 0;
	uint8_t s_command_num=0;
//copy_data_from_queue(&frame,&leng);

	
//			s485_copy_data_from_queue(&GV_qdf_485_1,&frame,&s_leng,&f_data,&s_num,&s_command_num);
//	SYSTEM_ERROR("num pull_data_from_485 --------,yes3 \n");
//	 s485_convert_queue(&frame,&f_data,&s_num,&s_command_num);
//	void s485_convert_queue(uint8_t **outalldata,uint8_t* Command_address,uint8_t** OutData,uint8_t* Datanum);
	 s485_convert_queue(&frame,&comand_addr,&o_data,&o_leng);
	if(o_leng!=0){
//		 SYSTEM_DEBUG("485received data：%s  ,length: %d",frame,s_leng);
		//SYSTEM_DEBUG("received data：s_addr %x  ,length: %d",s_addr,s_num);
#if 0			 
   SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,s_leng,"485 get data : ,length: %d",s_num);
#endif		
		
		screendataswitch(comand_addr,&o_data,o_leng);



	}	
//		free(frame);
//	free(f_data);
	o_data= NULL;
	frame = NULL;
	o_leng=0;

}



void screendataswitch(uint16_t addr,uint8_t** p_data,uint8_t num){
	
SYSTEM_DEBUG_ARRAY_MESSAGE(*p_data,num,"   screen_ADDR =%x ",addr);
	
switch(addr){
#if 0
	case EM_S_SetMaintainLimitVoltageComplete_Add: set_setting_Dispose(num,*p_data); break;//点击设置完成 5A A5 06 83 0000 01 0001
	case EM_S_SetBeginMaintain_Add: set_BeginMaintain_Dispose(num,*p_data); break;//5A A5 06 83 00 04 01 00 00
	case EM_S_SetBatteryCurrent_Add: set_SetBatteryCurrent_Dispose(num,*p_data); break;//5A A5 06 83 40 00 01 00 00
	case EM_DS_MaintainUpperLimitVoltage_Add: set_SetBatteryLimitVoltage_SettingSave_Dispose(num,p_data); break;//5A A5 05 83 1000 02 0001 0002
//	case EM_S_SetBatteryCurrent_Add:
	#endif
	default:  break;

}

}












/*********************************************END OF FILE**********************/
