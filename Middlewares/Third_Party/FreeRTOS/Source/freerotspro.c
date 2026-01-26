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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "freerotspro.h"  
/* 开发板硬件bsp头文件 */

//#include "./io/IN_io/bsp_gpio_in.h" 
//#include "./usart/bsp_usart.h"
//#include "./extfun/oled/bsp_oled_test.h"
//#include "./controller/TaskRtosFun.h"

//#include "./io/bsp_io_output.h"  
//#include "business.h"



//#include "lv_port_disp_template.h"
//#include "lv_port_indev_template.h"
//#include "lv_demo_stress.h"
#include "./sys/sysio.h"
#include "./TaskTest/Task_mockheap_stack.h"
 #include "./shell_port.h"
#include "./hook/hook_mem.h"
#include "./rtosprintf/frtos_printf.h"
#include "./TaskTest/Task_check.h"
//#include "./usart/bsp_usart.h"
#include "./SEGGER_TOOLKIT.h"
#include "./sys/bsp_systime.h"   





#include "./task/user_SystemTask.h"
#include "./task/user_LVGLTask.h"




/******************************* 宏定义 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些宏定义。
 */
//#define  QUEUE_LEN    4   /* 队列的长度，最大可包含多少个消息 */
//#define  QUEUE_SIZE   4   /* 队列中每个消息大小（字节） */


/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
//static TaskHandle_t AppTaskCreate_Handle = NULL;/* 创建任务句柄 */
////static TaskHandle_t OLED_Task_Handle = NULL;/* LED任务句柄 */
////static TaskHandle_t KEY_Task_Handle = NULL;/* KEY任务句柄 */
//TaskHandle_t LED1_Task_Handle = NULL;/* LED任务句柄 */
//TaskHandle_t LED2_Task_Handle = NULL;/* KEY任务句柄 */
//TaskHandle_t FRS_show_Task_Handle = NULL;
TaskHandle_t tmp_handle;	

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */


/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
#define TASK_PARAMETER_(name)	volatile void *TASK_PARAMETER_##name	
#define vTASK_PARAMETER_(name)	TASK_PARAMETER_###name	

#define TASK_NAME_STR(name)  #name


TASK_PARAMETER_(T_1_Task);
TASK_PARAMETER_(T_2_Task);
//void *TASK_PARAMETER_T_1_Task;
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void T_1_Task(void* pvParameters);/* LED_Task任务实现 */
static void T_2_Task(void* pvParameters);/* KEY_Task任务实现 */
//void SEGGERTask(void *pvParameters);
//QueueHandle_t Test_Queue =NULL;


//void vLvglTask(void *pvParameters);  /* 任务函数 */
/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
uint8_t FreerTosProBegin(void)
{	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  /* 开发板硬件初始化 */

//  lv_init_all();
//vStartStackOverflowTest();
  //printf("按下KEY1挂起任务，按下KEY2恢复任务\n");
  
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )1024,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )20, /* 任务的优先级 */
                        (TaskHandle_t*  )&tmp_handle);/* 任务控制块指针 */ 
	 xReturn = pdPASS;
	
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
		
    /* 启动调度器，开始多任务调度，启动成功则不返回 */
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return 1;  
  printf("XXXXXXXXXXXXXXXXX\n");
  while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/

extern void thread_shell(void *pvParameters);
TaskHandle_t thread_shell_Handle = NULL;/* KEY任务句柄 */


static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
    /* 创建Test_Queue */
SYSTEM_set_rtos_running();

//  if(NULL != Test_Queue)
//    printf("创建Test_Queue消息队列成功!\r\n");
	 vPrintHeapState("Before Creation");
#if USE_OS&&USE_LETTER_SHELL
LetterShell_OS_Init();
	
vPrintStack_TaskCreationResult("shell", xReturn, 256);
#endif

	#if 1
  /* 创建LED_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )T_1_Task, /* 任务入口函数 */
                        (const char*    )"T_1_Task",/* 任务名字 */
                        (uint16_t       )128,   /* 任务栈大小 */
                        (void*          )TASK_PARAMETER_T_1_Task,	/* 任务入口函数参数 */
                        (UBaseType_t    )13,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&tmp_handle);/* 任务控制块指针 */
   vPrintStack_TaskCreationResult("T_1_Task", xReturn, 128);
//		vPrintHeapState("After T_1_Task Creation");										
  /* 创建KEY_Task任务 *///后创建先调用
  xReturn = xTaskCreate((TaskFunction_t )T_2_Task,  /* 任务入口函数 */
                        (const char*    )"T_2_Task",/* 任务名字 */
                        (uint16_t       )128,  /* 任务栈大小 */
                        (void*          )TASK_PARAMETER_T_2_Task,/* 任务入口函数参数 */
                        (UBaseType_t    )12, /* 任务的优先级 */
                        (TaskHandle_t*  )&tmp_handle);/* 任务控制块指针 */ 
 vPrintStack_TaskCreationResult("T_2_Task", xReturn, 128);
//		 vPrintHeapState("After T_2_Task Creation");											
												/* 创建LVGL任务 */
    xTaskCreate((TaskFunction_t )vLvglTask,
                (const char*    )"vLvglTask",
                (uint16_t       )8192, 
                (void*          )NULL,
                (UBaseType_t    )tskIDLE_PRIORITY+5,
                (TaskHandle_t*  )&tmp_handle);
								
								
			 if(pdPASS == xReturn)
    vPrintStack_TaskCreationResult("vLvglTask", xReturn, 8192);						

			 
	    xTaskCreate((TaskFunction_t )SEGGERTask,
                (const char*    )"SEGGERTask",
                (uint16_t       )256, 
                (void*          )NULL,
                (UBaseType_t    )tskIDLE_PRIORITY+4,
                (TaskHandle_t*  )&tmp_handle);
vPrintStack_TaskCreationResult("SEGGERTask", xReturn, 256);				 
			 
//    xTaskCreate((TaskFunction_t )uart_health_check_task,
//                (const char*    )"uart_health_check_task",
//                (uint16_t       )256, 
//                (void*          )NULL,
//                (UBaseType_t    )tskIDLE_PRIORITY+4,
//                (TaskHandle_t*  )&tmp_handle);
//vPrintStack_TaskCreationResult("uart_health_check_task", xReturn, 256);		

			 
//			 vPrintHeapState("After vLvglTask Creation");									
	#endif
// CreateMemTestTasks();
	vPrintHeapState("After Creation");
//	    xTaskCreate((TaskFunction_t )thread_shell,     
//                (const char*    )"thread_shell",   
//                (uint16_t       )128*4,
//                (void*          )NULL,
//                (UBaseType_t    )10,
//                (TaskHandle_t*  )&thread_shell_Handle); 
	
	//xTaskCreate(FRS_show_task_info,"FRS_show_task_info",128,(void *)0,1,&FRS_show_Task_Handle);
//	 xTaskCreate(busi_process_task_entry,"busi_pro_task",128,(void *)0,7,&tmp_handle);

  vTaskDelete(xTaskGetHandle("AppTaskCreate")); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}

void StartNeedDeleteTask(void *pvParameters)
{


    
    while(1)
    {

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}




#include "log.h"


#include "./TaskTest/Task_check.h"

#include "stdarg.h"
volatile uint16_t timedelar_T_1_Task;
static void T_1_Task(void* parameter)
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
				logInfo("T_2_Task:-----\n");
		SYSTEM_I_PRINT("Task is running, count: %d", count++);
		taskYIELD();
		SYSTEM_D_PRINT("Task is running, count: %d", count++);
		
//		Check_Usart1_enable();
//		Check_Usart1_clock_enable();
		
//		printf("xxxxxx\r\n");
		 SYSTEM_DEBUG(" T_1_Task !\r\n");
    vTaskDelay(timedelar_T_1_Task/portTICK_PERIOD_MS);   /* 延时1000个ms */
  }
}

#include <elog.h>

extern Shell shell;
volatile uint16_t timedelar_T_2_Task;
static void T_2_Task(void* parameter)
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
#include "freertos.h"
#include "task.h"
#include "string.h"




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



/**
 * @brief      FreeRTOS Tick 钩子函数
 * @note       在系统Tick中断中调用，用于精确计时
 * @retval     无
 */
void vApplicationTickHook(void)
{
    lv_tick_inc(1);  
}

//static void AppTaskCreate(void)
//{
//  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
//  
//  taskENTER_CRITICAL();           //进入临界区
//    /* 创建Test_Queue */
//  Test_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* 消息队列的长度 */
//                            (UBaseType_t ) QUEUE_SIZE);/* 消息的大小 */
//  if(NULL != Test_Queue)
//    printf("创建Test_Queue消息队列成功!\r\n");
//  /* 创建LED_Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )OLED_Task, /* 任务入口函数 */
//                        (const char*    )"OLED_Task",/* 任务名字 */
//                        (uint16_t       )512,   /* 任务栈大小 */
//                        (void*          )NULL,	/* 任务入口函数参数 */
//                        (UBaseType_t    )2,	    /* 任务的优先级 */
//                        (TaskHandle_t*  )&OLED_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("创建OLED_Task任务成功!\r\n");
//  /* 创建KEY_Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )KEY_Task,  /* 任务入口函数 */
//                        (const char*    )"KEY_Task",/* 任务名字 */
//                        (uint16_t       )512,  /* 任务栈大小 */
//                        (void*          )NULL,/* 任务入口函数参数 */
//                        (UBaseType_t    )3, /* 任务的优先级 */
//                        (TaskHandle_t*  )&KEY_Task_Handle);/* 任务控制块指针 */ 
//  if(pdPASS == xReturn)
//    printf("创建KEY_Task任务成功!\r\n");
//  
//  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
//  
//  taskEXIT_CRITICAL();            //退出临界区
//}



/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
//static void OLED_Task(void* parameter)
//{	
//	BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdTRUE */
//	 uint32_t r_queue;	/* 定义一个接收消息的变量 */
//  while (1)
//  {
//		xReturn = xQueueReceive( Test_Queue,    /* 消息队列的句柄 */
//                             &r_queue,      /* 发送的消息内容 */
//                             portMAX_DELAY); /* 等待时间 一直等 */
//		//OLED_TestQ(r_queue);
//      if(pdTRUE == xReturn)
//      printf("本次接收到的数据是%d\n\n",r_queue);
//    else
//      printf("数据接收出错,错误代码0x%lx\n",xReturn);
//   
//    printf("OLED_Task Running,OLED\r\n");
//    vTaskDelay(1000/portTICK_PERIOD_MS);   /* 延时1000个ms */
//  }
//}

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
//static void KEY_Task(void* parameter)
//{	
//	
//	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
//  uint32_t send_data1 = 0;
//  while (1)
//  {
//		//printf("T_2_Task Running,OLED\r\n");
//    if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
//    {/* K1 被按下 */
////      printf("挂起OLED任务！\n");
////      vTaskSuspend(OLED_Task_Handle);/* 挂起LED任务 */
////      printf("挂起OLED任务成功！\n");
//			send_data1++;
//			printf("发送消息send_data1加1！\n");
//      xReturn = xQueueSend( Test_Queue, /* 消息队列的句柄 */
//                            &send_data1,/* 发送的消息内容 */
//                            0 );        /* 等待时间 0 */
//      if(pdPASS == xReturn)
//        printf("消息send_data1发送成功!\n\n");
//    } 
//		
//    if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON )
//    {/* K2 被按下 */
////      printf("恢复OLED任务！\n");
////      vTaskResume(OLED_Task_Handle);/* 恢复LED任务！ */
////      printf("恢复OLED任务成功！\n");
//			
//			      printf("发送消息send_data1减1！\n");
//			send_data1--;
//      xReturn = xQueueSend( Test_Queue, /* 消息队列的句柄 */
//                            &send_data1,/* 发送的消息内容 */
//                            0 );        /* 等待时间 0 */
//      if(pdPASS == xReturn)
//        printf("消息send_data1发送成功!\n\n");
//    
//			
//    }
//    vTaskDelay(20);/* 延时20个tick */
//  }
//}


/********************************END OF FILE****************************/
