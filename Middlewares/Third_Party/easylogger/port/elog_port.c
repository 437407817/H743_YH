/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */
 
#include <elog.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "user_config.h"
#include "./SEGGER_RTT.h"
#include "cmsis_os.h"
//#include "cmsis_os2.h"
#include "log.h" 
#include "./usart/bsp_usart_shell.h"
//! 日志互斥信号量句柄
static SemaphoreHandle_t LogMutexSemaphore = NULL; //freertos
//static osSemaphoreId elog_lockHandle;	//cmsis_os
//static osSemaphoreId_t elog_lockHandle;	//cmsis_os
#ifdef ELOG_ASYNC_OUTPUT_ENABLE
static osSemaphoreId elog_asyncHandle= NULL;;
#endif
/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    /* add your code here */
        //! 创建互斥信号值
#if USE_OS
    LogMutexSemaphore = xSemaphoreCreateMutex();//! 创建互斥信号值freertos
	#ifdef ELOG_ASYNC_OUTPUT_ENABLE
	elog_asyncHandle = xSemaphoreCreateMutex();
	#endif
//			osSemaphoreDef(elog_lockHandle);
    if (LogMutexSemaphore == NULL) {
        printf("elog sem create fail\r\n");
        result =  ELOG_SEM_FAIL;	//!< 注意：ElogErrCode 枚举中没有这个类型，需要添加一个错误类型
    }
#endif
	
	
	
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {

    /* add your code here */

}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    
    /* add your code here */
        //! %s 表示字符串输出，
    //! .<十进制数> 是精度控制格式符，输出字符时表示输出字符的位数，
    //! 在精度控制时，小数点后的十进制数可以使用 * 来占位，
    //! 在后面提供一个变量作为精度控制的具体值
	#if 1
//	printf("%.*s", size, log);//打印在串口端
	logDebug("%.*s", size, log);
	
//	    for (size_t i = 0; i < size; i++)
//    {
//        while((USART_SHELL->ISR & 0X40) == 0);

//        USART_SHELL->TDR = (uint8_t)log[i];
//    }
	#endif
	#if 0
	//SEGGER_RTT_printf(0,RTT_CTRL_RESET"%s", log);


	SEGGER_RTT_SetTerminal(3);
	SEGGER_RTT_Write(0,log,size);//打印在segger端
	#endif
}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    
    /* add your code here */
#if USE_OS
    if (NULL != LogMutexSemaphore)
    {
        xSemaphoreTake(LogMutexSemaphore, portMAX_DELAY); //!< 等待互斥信号量freertos
//			osSemaphoreWait(elog_lockHandle,osWaitForever);
//			osSemaphoreAcquire(elog_lockHandle, osWaitForever); //!< 等待互斥信号量cmsis_V2
    }
#else
		    __disable_irq();
#endif
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    
    /* add your code here */
#if USE_OS
    if (NULL != LogMutexSemaphore)
    {
        xSemaphoreGive(LogMutexSemaphore); //!< 发送互斥信号量freertos
//			osSemaphoreRelease(elog_lockHandle);//!< 发送互斥信号量cmsis_V1 V2
    }
#else

    __enable_irq();

		
#endif
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    
    /* add your code here */
#if USE_OS
	
    static char cur_system_time[16] = { 0 };
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
#endif
        TickType_t tick = xTaskGetTickCount();
        snprintf(cur_system_time, 16, "%d.%.3d", (tick / configTICK_RATE_HZ), tick % configTICK_RATE_HZ);
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    }
#endif 
//		snprintf(cur_system_time, 16, "%lu", osKernelSysTick());
    return cur_system_time;
		
#else
  //static char cur_system_time[16] = { 0 };

	//snprintf(cur_system_time, 16, "%d.%.3d", (tick / configTICK_RATE_HZ), tick % configTICK_RATE_HZ);
    return "10:08:12";


		
#endif
		
		
		
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    
    /* add your code here */
	    return "pid:1008";
    //return "";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    
    /* add your code here */
#if USE_OS
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
#endif
        return pcTaskGetName(xTaskGetCurrentTaskHandle());
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    }
#endif 

    return "";
#else

return "tid:24";

		
		
#endif 
}

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
void elog_async_output_notice(void) {
//    osSemaphoreRelease(elog_asyncHandle);
	xSemaphoreGive(elog_asyncHandle);
}
#endif



#ifdef ELOG_ASYNC_LINE_OUTPUT
    static char poll_get_buf[ELOG_LINE_BUF_SIZE - 4];
#else
    static char poll_get_buf[ELOG_ASYNC_OUTPUT_BUF_SIZE - 4];
#endif

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
void elog_entry(void *para) {

    size_t get_log_size = 0;

    for(;;)
    {
        /* waiting log */
        // osSemaphoreAcquire(elog_asyncHandle, osWaitForever);

        osSemaphoreWait(elog_asyncHandle,osWaitForever);
			
        /* polling gets and outputs the log */
        while (1) {
#ifdef ELOG_ASYNC_LINE_OUTPUT
            get_log_size = elog_async_get_line_log(poll_get_buf, sizeof(poll_get_buf));
#else
            get_log_size = elog_async_get_log(poll_get_buf, sizeof(poll_get_buf));
#endif
            if (get_log_size) {
                elog_port_output(poll_get_buf, get_log_size);
            } else {
                break;
            }
        }
    }
}
#endif


#include "./hook/hook_mem.h"
/**
 * init easylogger,这个函数是我们自己添加的，便于用户直接调用，需要在elog.h中添加声明
 */
void easylogger_init(void)
{
	
	
	#if USE_OS
	//异步输出应用于RTOS，而同步输出应用于裸机
	#ifdef ELOG_ASYNC_OUTPUT_ENABLE
	TaskHandle_t tmp_handle;	
	BaseType_t xReturn = pdPASS;
			xTaskCreate(elog_entry,"elog_entry",1024,(void *)0,7,&tmp_handle);				
			vPrintStack_TaskCreationResult("elog_entry", xReturn, 1024);		
	#endif
	#endif
    /* init Easylogger */
	elog_init();

	/* set EasyLogger log format */
  elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_T_INFO);
	elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_T_INFO);
	elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL);
	elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);

	
//	   elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
//     elog_set_fmt(ELOG_LVL_ERROR,  ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
//     elog_set_fmt(ELOG_LVL_WARN,   ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
//     elog_set_fmt(ELOG_LVL_INFO,   ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
//     elog_set_fmt(ELOG_LVL_DEBUG,  ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
	
	/*Eenbale color*/
	elog_set_text_color_enabled(true);

	/* start EasyLogger */
	elog_start();
    
}

