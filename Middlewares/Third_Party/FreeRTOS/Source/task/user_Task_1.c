/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS V9.0.0  + STM32 任务管理
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 全系列开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
/* FreeRTOS头文件 */
#include "./task/user_Task_1.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "freerotspro.h"  
/* 开发板硬件bsp头文件 */

#include "./sys/sysio.h"

#include "./SEGGER_TOOLKIT.h"
#include "./sys/bsp_systime.h"   



#include "./rtosprintf/frtos_printf.h"



//#include <elog.h>

#include "log.h"


#include "./TaskTest/Task_check.h"

#include "stdarg.h"
volatile uint16_t timedelar_T_1_Task;
 void T_1_Task(void* parameter)
{	
//	BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdTRUE */
//	 uint32_t r_queue;	/* 定义一个接收消息的变量 */
	int count = 0;
	timedelar_T_1_Task=3000;
	if(parameter!=NULL){
	SYSTEM_I_PRINT("1 parameter: %d", (uint16_t*)parameter);
	}else{
	SYSTEM_I_PRINT("1 err parameter: %d", (uint16_t*)parameter);
	}
  while (1)
  {
//LED1_TOGGLE;
		Handle_test3();
				logInfo("T_1_Task:-----\n");
//		SYSTEM_I_PRINT("Task is running, count: %d", count++);
		taskYIELD();
//		SYSTEM_D_PRINT("Task is running, count: %d", count++);
//		SYSTEM_D_PRINT("T+++");
//		Check_Usart1_enable();
//		Check_Usart1_clock_enable();
		
//		printf("xxxxxx\r\n");
//		 SYSTEM_DEBUG(" T_1_Task !\r\n");
    vTaskDelay(timedelar_T_1_Task/portTICK_PERIOD_MS);   /* 延时1000个ms */
  }
}

extern Shell shell;
volatile uint16_t timedelar_T_2_Task;

 void T_2_Task(void* parameter)
{	
//	BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdTRUE */
//	 uint32_t r_queue;	/* 定义一个接收消息的变量 */
	timedelar_T_2_Task=3000;
		if(parameter!=NULL){
	SYSTEM_I_PRINT("2 parameter: %d", (uint16_t*)parameter);
	}else{
	
	SYSTEM_I_PRINT("2 err parameter: %d", (uint16_t*)parameter);
	}
	
  while (1)
  {

    vTaskDelay(pdMS_TO_TICKS(timedelar_T_2_Task));   /* 延时1000个ms */
  }
}



int changetime(char *pcTaskName,int i)
{
	  if (pcTaskName == NULL || *pcTaskName == '\0') {
        return NULL;
    }
 
 TaskHandle_t tmp_handle;
 TaskStatus_t xTaskDetails;
	tmp_handle = xTaskGetHandle(pcTaskName);
	if(NULL==tmp_handle){
	SYSTEM_I_PRINT("task name is wrong!\r\n");
		return 0;
	}

 vTaskGetInfo(tmp_handle,&xTaskDetails,1,eInvalid);

  SYSTEM_I_PRINT("string: %s ,%d  %s \r\n",pcTaskName, i,xTaskDetails.pcTaskName);
	
	if (strcmp("T_2_Task", pcTaskName)==0){
	timedelar_T_2_Task=i;
	}else if(strcmp("T_1_Task", pcTaskName)==0){
		timedelar_T_1_Task=i;
	}else{
	SYSTEM_I_PRINT("no name \r\n");
	}
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), ct, changetime, test);





/********************************END OF FILE****************************/
