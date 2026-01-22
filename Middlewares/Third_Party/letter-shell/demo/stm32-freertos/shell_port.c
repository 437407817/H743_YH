/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */
 #include "./shell_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
//#include "serial.h"
#include "stm32_FH_xxx.h"
//#include "usart.h"
//#include "cevent.h"
#include "log.h"

 #include "./buffer/que.h"
 #include "semphr.h"
#include "./user_config.h"
#include "cmsis_os.h"

 #include "./sys/sysio.h"
 #include "./buffer/user_buffers.h"

Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;
//osSemaphoreId thread_uart_rx_sem;
osThreadId thread_shellTaskHandle;
/* 创建接收队列 */
//creat_que(rx_letter_shell_que, 40);
static char ch;

#include "log.h"
#include "shell.h"
void userLogWrite(char *buffer, short len);
// 定义 Log 对象
Log userLog = {
    .write = userLogWrite,
    .active = 1,      // 必须为 1
    .level = LOG_DEBUG,
//    .shell = &shell
};
//extern Shell shell; // 假设这是您的全局 Shell 对象

// Log 组件的写回调函数
void userLogWrite(char *buffer, short len) {
    if (userLog.shell) { // 仅在用户登录后才输出日志
        // 使用尾行模式接口，buffer 会被插入到命令行上方
        shellWriteEndLine(&shell, buffer, len);
    }
}

uint8_t my_uartshell_redata;

extern ShellRingBuffer_t shellRingBuffer;
HAL_StatusTypeDef RIT_Status;

#if (USE_LETTER_SHELL&&USE_OS)


void USART_SHELL_IRQHandler(void)
{
//SYSTEM_INFO("+");
HAL_UART_IRQHandler(&huart_shell_Handle);	
}
//CEVENT_EXPORT(EVENT_INIT_STAGE2, LetterShell_OS_Init);
//串口收到数据回调
void HAL_UART_Shell_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart_shell_Handle.Instance == USART_SHELL)//判断串口号
    {


//					SYSTEM_INFO("%c",my_uartshell_redata);
				RingBuffer_WriteByte(&shellRingBuffer.shell_rx_ring,my_uartshell_redata);
        // 重启接收
				    while(HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t *)&my_uartshell_redata, 1)!= HAL_OK){
      huart_shell_Handle.RxState = HAL_UART_STATE_READY;
							SYSTEM_INFO(" RIT BUSY = %d \r\n",RIT_Status);
      __HAL_UNLOCK(&huart_shell_Handle);
    }


    }
}


/**
 * @brief 串口错误回调（必须实现，避免错误状态累积）
 */
void HAL_UART_Shell_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART_SHELL) {
        // 1. 读取错误类型（可选）
        uint32_t error_code = huart->ErrorCode;
        
        // 2. 清除所有错误标志（关键步骤！）
        // 注意：不同 HAL 版本 API 不同
        
        // 方法 A：使用宏（推荐，适用于 STM32H7 新版 HAL）
        __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);

        // 方法 B：手动读 RDR + 清 SR（兼容旧版）
        // volatile uint32_t tmp = huart->Instance->RDR; // 强制清 ORE
        // (void)tmp;

        // 3. 重置错误码
        huart->ErrorCode = HAL_UART_ERROR_NONE;

        // 4. 重启接收
        HAL_UART_Receive_IT(huart, (uint8_t *)&my_uartshell_redata, 1);

        SYSTEM_INFO("UART ORE Recovered! Err=0x%lX\r\n", error_code);
    }
}

#endif




#if (USE_LETTER_SHELL&&!USE_OS)
void USART_SHELL_IRQHandler(void){

	HAL_UART_IRQHandler(&huart_shell_Handle);	
}

void HAL_UART_Shell_RxCpltCallback(void)
{
#if USE_OS
        osSignalSet(thread_shellTaskHandle, 0x01);
#endif
//	osThreadFlagsSet(thread_shellTaskHandle, 0x01);
//SYSTEM_DEBUG("111%c",ch);
//		 InQue(rx_letter_shell_que, ch);
	shellHandler(&shell, ch);

		HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t*)&ch, 1);

}
#endif

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
//    serialTransmit(&debugSerial, (uint8_t *)data, len, 0x1FF);
	HAL_UART_Transmit(&huart_shell_Handle, (uint8_t *)data, len, 0xFFFF);
//	HAL_UART_Transmit(&huart_shell_Handle, (uint8_t *)data, len, 0x1FF);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
//short userShellRead(char *data, unsigned short len)
//{
//	HAL_StatusTypeDef status;
////    return serialReceive(&debugSerial, (uint8_t *)data, len, 0);
////	while (__HAL_UART_GET_FLAG(&huart_shell_Handle, UART_FLAG_RXNE) == RESET){};
//	status = HAL_UART_Receive(&huart_shell_Handle, (uint8_t *)data, 1, 0xFFFF);
//	    if (status == HAL_OK) {
//        return 1;
//    } else {
//        return 0;
//    }
//	
//}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    xSemaphoreGiveRecursive(shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void LetterShell_OS_Init(void)
{
		// 在 UART 初始化后，添加：
__HAL_UART_ENABLE_IT(&huart_shell_Handle, UART_IT_ERR);
	HAL_UART_Receive_IT(&huart_shell_Handle,(uint8_t *)&my_uartshell_redata,1);

    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
//    shell.read = userShellRead;
	#if SHELL_USING_LOCK == 1
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
	#endif
    shellInit(&shell, shellBuffer, 512);
	logRegister(&userLog, &shell);
    if (xTaskCreate(shellTask, "shell", 256, &shell, 15, NULL) != pdPASS)
    {
        logError("shell task creat failed");
			SYSTEM_DEBUG("shell task creat failed");
    }
}



void letterShell_no_os_WhileInit(void)
{
	//注册自己实现的写函数
  shell.write = userShellWrite;
	#if (USE_LETTER_SHELL&&!USE_OS)
//	shell.read = userShellRead;
	#endif
	//调用shell初始化函数
    shellInit(&shell, shellBuffer, 512);
//	__HAL_UART_CLEAR_NEFLAG(&huart_shell_Handle);
//volatile uint32_t temp_isr = huart_shell_Handle.Instance->ISR;  // 先读 ISR 寄存器(必须有)
//		(void)temp_isr;
	__HAL_UART_ENABLE_IT(&huart_shell_Handle,UART_IT_RXNE);
}

void letter_Shell_NoOsWhileTask(void)
{
    Shell *tshell = &shell;
    char data;

    while(1)
    {
        if (tshell->read && tshell->read(&data, 1) == 1)
        {
            shellHandler(tshell, data);
        }
    }
}



