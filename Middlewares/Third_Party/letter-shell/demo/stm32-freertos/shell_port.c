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
#include "./HAL/hal_ext_uart.h"
Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;
SemaphoreHandle_t shellBinarySem =NULL;
//osSemaphoreId shellBinarySem;
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

volatile uint8_t my_uartshell_redata;

extern ShellRingBuffer_t shellRingBuffer;
HAL_StatusTypeDef RIT_Status;
extern __IO uint32_t NowUse_rtos;

BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//#if (USE_LETTER_SHELL&&USE_OS)
#if (USE_LETTER_SHELL)

void USART_SHELL_IRQHandler(void)
{

//	  uint32_t ulReturn;
//	if(NowUse_rtos==1){
//  /* 进入临界段，临界段可以嵌套 */
//  ulReturn = taskENTER_CRITICAL_FROM_ISR();	
//	}
//	SYSTEM_INFO("+");

	
	
//	    if(huart_shell_Handle.Instance == USART_SHELL)//判断串口号
//    {
//	    uint32_t isr_flags = huart_shell_Handle.Instance->ISR; // 读取中断状态寄存器
//    uint32_t cr1_flags = huart_shell_Handle.Instance->CR1; // 读取控制寄存器1
//    // 1. 优先处理 ORE 错误（H743 需先清错误，再读数据）

//    // 1. 处理接收非空中断（正常接收逻辑）
//    if ((isr_flags & USART_ISR_RXNE_RXFNE) && (cr1_flags & USART_CR1_RXNEIE))
//    {
//        // 读取接收数据（必须读DR寄存器清除RXNE标志）
//         my_uartshell_redata = huart_shell_Handle.Instance->RDR;
//        // 发送到消息队列（复杂处理放任务中，回调只做最小操作）
////        xQueueSendFromISR(uart_rx_queue, &recv_data, NULL);

//			 RingBuffer_WriteByte(&shellRingBuffer.shell_rx_ring,my_uartshell_redata);
////				shellHandler(&shell, my_uartshell_redata);

//			
//        // 确保 RXNEIE 位始终开启（关键：防止被静默错误关闭）
//        huart_shell_Handle.Instance->CR1 |= USART_CR1_RXNEIE;
//			
////        return;
//    }

    // 2. 主动处理串口静默错误（FE/ NE/ ORE）
//    if (isr_flags & (USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE))
//    {
//			uart_debug_dump(&huart_shell_Handle);
//        // 第一步：清除所有错误标志（必须操作，否则错误会一直存在）
//        huart_shell_Handle.Instance->ICR = USART_ICR_FECF | USART_ICR_NECF | USART_ICR_ORECF;
//        
//        // 第二步：强制恢复 RXNEIE 位（核心：避免错误导致中断关闭）
//        huart_shell_Handle.Instance->CR1 |= USART_CR1_RXNEIE;
//        
//        // 可选：记录错误日志，方便排查硬件问题（比如串口电平不稳）
//			SYSTEM_INFO("err :ISR=0x%08X , recocered\r\n", isr_flags);

//    }
		
//	}
	
	
//		    uint32_t ulReturn;

    // 仅处理Shell串口的中断
//    if (huart_shell_Handle.Instance == USART_SHELL)
//    {
//			if(NowUse_rtos==1){
//        // 1. 进入中断级临界区（FreeRTOS专用）
//        ulReturn = taskENTER_CRITICAL_FROM_ISR();
//			}
//			uint32_t isrflags = READ_REG(huart_shell_Handle.Instance->ISR);
//			if ((isrflags & USART_ISR_ORE) != RESET) {
//        // H7 清除 ORE 标志的宏
//        __HAL_UART_CLEAR_IT(&huart_shell_Handle, UART_CLEAR_OREF);
//        // 清除后，RXNE 才能重新正常工作
//    }
			
//			if (isrflags & (USART_ISR_ORE | USART_ISR_NE | USART_ISR_FE | USART_ISR_PE)) {
//				uart_debug_dump(&huart_shell_Handle);
//        WRITE_REG(huart_shell_Handle.Instance->ICR, 0x1F); // 暴力清除所有错误标志
//    }
			
//			if ((isrflags & USART_ISR_RXNE_RXFNE) != RESET) {
//        // 直接读 RDR/DR 寄存器，这会自动清除 RXNE 标志位
//        uint8_t my_uartshell_redata = (uint8_t)(huart_shell_Handle.Instance->RDR);
//        
//        // 存入环形缓冲区并通知任务
//        RingBuffer_WriteByte(&shellRingBuffer.shell_rx_ring, my_uartshell_redata);
//        if(NowUse_rtos==1){
//        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//        xSemaphoreGiveFromISR(shellBinarySem, &xHigherPriorityTaskWoken);
//				
//        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//				}
//        return; // 处理完即返回，不走 HAL 的流程
//    }
//			if ((isrflags & USART_ISR_ORE) != RESET) {
//        // H7 专用：写 ICR 寄存器的 ORECF 位来清除标志
////        __HAL_UART_CLEAR_IT(&huart_shell_Handle, UART_CLEAR_OREF);
//        								uart_debug_dump(&huart_shell_Handle);
//								uart_debug_checkerror(&huart_shell_Handle,ENABLE);
//        /* 额外保险：如果是 HAL 状态机模式，这里返回能防止 HAL 进入 Error 回调关闭中断 */
//        return; 
//    }
		
		#if 0
			
        // 2. 将接收到的字节写入环形缓冲区
       RingBuffer_WriteByte(&shellRingBuffer.shell_rx_ring,my_uartshell_redata);

        // 3. 释放信号量，通知任务有数据可读
        
			if(NowUse_rtos==1){
        xSemaphoreGiveFromISR(shellBinarySem, &xHigherPriorityTaskWoken);
			}
        // 4. 重新开启串口接收中断（关键：持续接收下一个字节）
//        HAL_UART_Receive_IT_UNLOCK(&huart_shell_Handle, (uint8_t *)&my_uartshell_redata, 1);
					   while((RIT_Status=HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t *)&my_uartshell_redata, 1))!= HAL_OK){
       // 中止当前接收
//							SYSTEM_INFO(" RIT BUSY = %d State =%x %x err=%x\r\n",RIT_Status,huart_shell_Handle.RxState,HAL_UART_GetState(&huart_shell_Handle),huart->ErrorCode);
								uart_debug_dump(&huart_shell_Handle);
								uart_debug_checkerror(&huart_shell_Handle,ENABLE);

//							 HAL_StatusTypeDef status = HAL_UART_AbortReceive(&huart_shell_Handle);
//							SET_BIT(huart_shell_Handle.Instance->CR1, USART_CR1_RXNEIE);
//        if (status != HAL_OK) {
//            SYSTEM_INFO("Failed to abort receive: %d\r\n", status);
//        }
//							huart_shell_Handle.RxState = HAL_UART_STATE_READY;
//      __HAL_UNLOCK(&huart_shell_Handle);
    }
			if(NowUse_rtos==1){
        // 5. 退出临界区，并触发任务调度（如果需要）
//        taskEXIT_CRITICAL_FROM_ISR(ulReturn);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			}
			return;
    }
	
	#endif
	
//}
	
	
	
HAL_UART_IRQHandler(&huart_shell_Handle);
	
//if(NowUse_rtos==1){
//taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
//}

}
//CEVENT_EXPORT(EVENT_INIT_STAGE2, LetterShell_OS_Init);



//串口收到数据回调
void HAL_UART_Shell_RxCpltCallback(UART_HandleTypeDef *huart){
	
//	SYSTEM_INFO("+");
//	HAL_Delay(200);
	

	
	
	
	
//		if(huart->Instance==USART1)
//	{
//        osSemaphoreRelease(uart_rx_sem);
//        // xSemaphoreGiveFromISR(uart_rx_sem, &taskWoken);
//		HAL_UART_Receive_IT(&huart1, &shell.recv_data, 1);
//	}
	
	
	
	
			#if 1
			    if(huart->Instance == USART_SHELL)//判断串口号
    {
        // 2. 将接收到的字节写入环形缓冲区
       RingBuffer_WriteByte(&shellRingBuffer.shell_rx_ring,my_uartshell_redata);

        // 3. 释放信号量，通知任务有数据可读
        
			if(NowUse_rtos==1){
        xSemaphoreGiveFromISR(shellBinarySem, &xHigherPriorityTaskWoken);
//				osSemaphoreRelease(shellBinarySem);
			}
        // 4. 重新开启串口接收中断（关键：持续接收下一个字节）
//        HAL_UART_Receive_IT_UNLOCK(&huart_shell_Handle, (uint8_t *)&my_uartshell_redata, 1);
					   while((RIT_Status=HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t *)&my_uartshell_redata, 1))!= HAL_OK){
       // 中止当前接收
//							SYSTEM_INFO(" RIT BUSY = %d State =%x %x err=%x\r\n",RIT_Status,huart_shell_Handle.RxState,HAL_UART_GetState(&huart_shell_Handle),huart->ErrorCode);
								uart_debug_dump(&huart_shell_Handle);
								uart_debug_checkerror(&huart_shell_Handle,ENABLE);

//							 HAL_StatusTypeDef status = HAL_UART_AbortReceive(&huart_shell_Handle);
//							SET_BIT(huart_shell_Handle.Instance->CR1, USART_CR1_RXNEIE);
//        if (status != HAL_OK) {
//            SYSTEM_INFO("Failed to abort receive: %d\r\n", status);
//        }
//							huart_shell_Handle.RxState = HAL_UART_STATE_READY;
//      __HAL_UNLOCK(&huart_shell_Handle);
    }
			if(NowUse_rtos==1){
        // 5. 退出临界区，并触发任务调度（如果需要）
//        taskEXIT_CRITICAL_FROM_ISR(ulReturn);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			}
//			return;
    }
	
	#endif
	
	
	
	
	
	#if 0
    if(huart_shell_Handle.Instance == USART_SHELL)//判断串口号
    {


					SYSTEM_INFO("%c",my_uartshell_redata);
				RingBuffer_WriteByte(&shellRingBuffer.shell_rx_ring,my_uartshell_redata);
        // 重启接收
//			taskENTER_CRITICAL(); // 进入临界区（关闭所有可屏蔽中断）
//				    while((RIT_Status=HAL_UART_Receive_IT_UNLOCK(&huart_shell_Handle, (uint8_t *)&my_uartshell_redata, 1))!= HAL_OK){
							   while((RIT_Status=HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t *)&my_uartshell_redata, 1))!= HAL_OK){
       // 中止当前接收
//							SYSTEM_INFO(" RIT BUSY = %d State =%x %x err=%x\r\n",RIT_Status,huart_shell_Handle.RxState,HAL_UART_GetState(&huart_shell_Handle),huart->ErrorCode);
								uart_debug_dump(&huart_shell_Handle);
								uart_debug_checkerror(&huart_shell_Handle,ENABLE);

//							 HAL_StatusTypeDef status = HAL_UART_AbortReceive(&huart_shell_Handle);
//							SET_BIT(huart_shell_Handle.Instance->CR1, USART_CR1_RXNEIE);
//        if (status != HAL_OK) {
//            SYSTEM_INFO("Failed to abort receive: %d\r\n", status);
//        }
//							huart_shell_Handle.RxState = HAL_UART_STATE_READY;
//      __HAL_UNLOCK(&huart_shell_Handle);
    }
//taskEXIT_CRITICAL(); // 退出临界区

    }
		#endif
}


/**
 * @brief 串口错误回调（必须实现，避免错误状态累积）
 */
void HAL_UART_Shell_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART_SHELL) {
			uint32_t isrflags = READ_REG(huart->Instance->ISR); // 也可以用 SR 寄存器，取决于型号
			    if(__HAL_UART_GET_FLAG(huart,UART_FLAG_ORE) != RESET){      
					SYSTEM_INFO("UART_FLAG_ORE Recovered! \r\n");			
					__HAL_UART_CLEAR_OREFLAG(huart);        
					HAL_UART_Receive_IT(huart, (uint8_t *)&my_uartshell_redata, 1);
					}
			
        // 1. 读取错误类型（可选）
//        uint32_t error_code = huart->ErrorCode;
//        
//        // 2. 清除所有错误标志（关键步骤！）
//        // 注意：不同 HAL 版本 API 不同
//        
//        // 方法 A：使用宏（推荐，适用于 STM32H7 新版 HAL）
//        __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);

//        // 方法 B：手动读 RDR + 清 SR（兼容旧版）
//        // volatile uint32_t tmp = huart->Instance->RDR; // 强制清 ORE
//        // (void)tmp;

//        // 3. 重置错误码
//        huart->ErrorCode = HAL_UART_ERROR_NONE;

//        // 4. 重启接收
//        HAL_UART_Receive_IT(huart, (uint8_t *)&my_uartshell_redata, 1);

//        SYSTEM_INFO("UART ORE Recovered! Err=0x%lX\r\n", error_code);
    }
}

#endif




//#if (USE_LETTER_SHELL&&!USE_OS)
//void USART_SHELL_IRQHandler(void){

//	HAL_UART_IRQHandler(&huart_shell_Handle);	
//}

//void HAL_UART_Shell_RxCpltCallback(void)
//{
//#if USE_OS
//        osSignalSet(thread_shellTaskHandle, 0x01);
//#endif
////	osThreadFlagsSet(thread_shellTaskHandle, 0x01);
////SYSTEM_DEBUG("111%c",ch);
////		 InQue(rx_letter_shell_que, ch);
//	shellHandler(&shell, ch);

//		HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t*)&ch, 1);

//}
//#endif

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

int userShellMutexUnlock(void){
if (xSemaphoreTake(shellBinarySem, portMAX_DELAY) == pdTRUE){
//if (osSemaphoreWait(shellBinarySem, portMAX_DELAY) == pdTRUE){
return 1;
}
return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void LetterShell_OS_Init(void)
{

//	lpuart_disable_fifo_hal(&huart_shell_Handle); // 关闭FIFO
		// 在 UART 初始化后，添加：
__HAL_UART_ENABLE_IT(&huart_shell_Handle, UART_IT_ERR);
	HAL_UART_Receive_IT(&huart_shell_Handle,(uint8_t *)&my_uartshell_redata,1);
//	HAL_UART_Receive_IT_UNLOCK(&huart_shell_Handle,(uint8_t *)&my_uartshell_redata,1);
//__HAL_UART_ENABLE_IT(&huart_shell_Handle, UART_IT_RXNE);
    shellMutex = xSemaphoreCreateMutex();
	
			shellBinarySem = xSemaphoreCreateBinary();	 
//	   osSemaphoreDef(shellBinarySem);
//    shellBinarySem = osSemaphoreCreate(osSemaphore(shellBinarySem),1);
	
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

//#include "ringbuffer.h"  // 确保包含环形缓冲区的头文件

// 声明环形缓冲区实例（需和中断中使用的是同一个）
//extern RingBuffer_t shellRingBuffer;
    unsigned short read_count = 0;  // 实际读取字节数
    uint8_t read_byte;              // 临时存储读取的单个字节
/**
 * @brief 从shell环形缓冲区读取数据
 * @param data 存储读取数据的缓冲区
 * @param len 期望读取的最大字节数
 * @return 实际读取到的字节数
 */
short userShellRead(char *data, unsigned short len)
{
    // 入参合法性检查
    if (data == NULL || len == 0)
    {
        return 0;
    }
read_count=0;
//        if (RingBuffer_ReadByte(&shellRingBuffer.shell_rx_ring, &read_byte) == true)
//        {
//            data[read_count] = (char)read_byte;  // 存入数据缓冲区
//            read_count++;                        // 计数+1
//					SYSTEM_INFO("%c",read_byte);
//        }

    // 循环读取：直到读满len个字节 或 缓冲区为空
//    while (len>0)
//    {
//        // 从环形缓冲区读取一个字节
//        // RingBuffer_ReadByte返回值一般为0表示成功，非0表示空/失败
//        if (RingBuffer_ReadByte(&shellRingBuffer.shell_rx_ring, &read_byte) == true)
//        {
//            data[read_count] = (char)read_byte;  // 存入数据缓冲区
//            read_count++;                        // 计数+1
//        }
//        else
//        {
//            break;  // 缓冲区无数据，退出循环
//        }
//    }
        if (RingBuffer_ReadByte(&shellRingBuffer.shell_rx_ring, &read_byte) == true)
        {
            *data = (char)read_byte;  // 存入数据缓冲区
            read_count++;                        // 计数+1
        }

    // 返回实际读取的字节数
    return read_count;
}

void letterShell_no_os_WhileInit(void)
{
	
	//注册自己实现的写函数
  shell.write = userShellWrite;
	
	shell.read = userShellRead;
	

	//调用shell初始化函数
    shellInit(&shell, shellBuffer, 512);
	logRegister(&userLog, &shell);
//	__HAL_UART_CLEAR_NEFLAG(&huart_shell_Handle);
//volatile uint32_t temp_isr = huart_shell_Handle.Instance->ISR;  // 先读 ISR 寄存器(必须有)
//		(void)temp_isr;
//	__HAL_UART_ENABLE_IT(&huart_shell_Handle,UART_IT_RXNE);
	HAL_UART_Receive_IT(&huart_shell_Handle,(uint8_t *)&my_uartshell_redata,1);
//	__HAL_UART_ENABLE_IT(&huart_shell_Handle, UART_IT_RXNE);
}

void letter_Shell_NoOsWhileTask(void)
{
    Shell *tshell = &shell;
    uint8_t data;

//    while(1)
    {
        if (tshell->read && tshell->read(&data, 1) == 1)
        {
            shellHandler(tshell, data);
        }
			
//        if (RingBuffer_ReadByte(&shellRingBuffer.shell_rx_ring, &data))
//        {
//            shellHandler(tshell, data);
//        }
    }
}



