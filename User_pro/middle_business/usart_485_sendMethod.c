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
  
#include "./middle_business/usart_485_sendMethod.h"
//#include "./usart/usart485verify.h"
#include "./pro_com/usart485verify.h"
#include "./sys/sysio.h"
//#include "./middle_business/usart_485_address.h"
//#include "./middle_business/usart_485_driver.h"
//#include "./global/GV_enum.h" 
#include "./DataConvert/data_convert.h"
#include "./global/GV_variable.h" 
#include "string.h"
#include "./pro_com/usart_485_232_send.h"

#include "./global/GV_enum.h" 


extern STR_SEND_RUN_DATA_t GV_send_run_state_data;
extern STR_SEND_RUN_DATA_t GV_send_run_state_bigdata;



void run_machine(void){
	
	GV_send_run_state_data.Machine_RUN=1;
	
memmove(&GV_send_run_state_bigdata,&GV_send_run_state_data,2);
EndianSwap_VpChange16HL((uint16_t *)&GV_send_run_state_bigdata.Machine_RUN,2);
com_DW_write_cmd_print((uint16_t)EM_SEND_RUNSTOP_ADDRESS,(uint16_t *)&GV_send_run_state_bigdata,sizeof(GV_send_run_state_bigdata)/2,1);
	
	
}



void stop_machine(void){
	
	GV_send_run_state_data.Machine_RUN=0;
	
memmove(&GV_send_run_state_bigdata,&GV_send_run_state_data,2);
EndianSwap_VpChange16HL((uint16_t *)&GV_send_run_state_bigdata.Machine_RUN,2);
com_DW_write_cmd_print((uint16_t)EM_SEND_RUNSTOP_ADDRESS,(uint16_t *)&GV_send_run_state_bigdata,sizeof(GV_send_run_state_bigdata)/2,1);
	
	
}

#include "shell.h"
int runfunc(int i)
{
	if(i==1){
	run_machine();
	}else{
	stop_machine();
	
	}
    printf("runfunc input int: %d, \r\n", i);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), t01, runfunc, test01);









/*********************************************END OF FILE**********************/
