

#include "./TaskTest/Task_check.h"
//#include "./usart/bsp_usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "./hook/hook_mem.h"
#include "./sys/sysio.h"

 #include "./buffer/user_buffers.h"
#include "./usart/bsp_usart_shell.h"

#include "stm32_FH_xxx.h"
#include "./HAL/hal_ext_uart.h"


void Check_Usart1_enable(void){

// 假设使用的是 USART1
if (USART1->CR1 & USART_CR1_UE) {
    // USART1 已使能
    SYSTEM_INFO("USART1 is ENABLED\r\n");
} else {
    // USART1 被禁用
    SYSTEM_INFO("USART1 is DISABLED!\r\n");
}

}


void Check_Usart1_clock_enable(void){

// 假设使用的是 USART1
if (RCC->APB2ENR & RCC_APB2ENR_USART1EN) {
    // USART1 时钟已使能
    SYSTEM_INFO("USART1 clock is ENABLED\r\n");
} else {
    // USART1 时钟被禁用！
    SYSTEM_INFO("USART1 clock is DISABLED!\r\n");
}

}
 #include "./shell_port.h"
#include "./usart/bsp_usart_shell.h"
extern uint8_t my_uartshell_redata;
extern ShellRingBuffer_t shellRingBuffer;
// UART 健康检查任务（优先级高于shell任务）
void uart_health_check_task(void *arg)
{
	SYSTEM_INFO("uart_health_check_task start...\r\n");
    while (1)
    {
        // 检查是否锁定为 BUSY_RX 状态
        if (huart_shell_Handle.RxState == HAL_UART_STATE_BUSY_RX)
        {
            SYSTEM_INFO("UART Locked in BUSY_RX! Reset...\r\n");
            // 强制重置状态 + 重启接收
            __HAL_UART_CLEAR_FLAG(&huart_shell_Handle,  UART_FLAG_RXNE | UART_FLAG_ORE | UART_FLAG_PE | UART_FLAG_FE);
            huart_shell_Handle.RxState = HAL_UART_STATE_READY;
            huart_shell_Handle.ErrorCode = HAL_UART_ERROR_NONE;
            HAL_UART_AbortReceive(&huart_shell_Handle);
            HAL_UART_Receive_IT(&huart_shell_Handle, (uint8_t *)&my_uartshell_redata, 1);
        }
        vTaskDelay(pdMS_TO_TICKS(30000)); // 1秒检查一次
    }
}


void uart_unlock_task(void){
SYSTEM_INFO("uart_unlock_task...\r\n");
__HAL_UNLOCK(&huart_shell_Handle);


}


void uart_reableIRQ_task(void){
SYSTEM_INFO("uart_rein_task...\r\n");
HAL_NVIC_DisableIRQ(USART_SHELL_IRQ);
HAL_NVIC_EnableIRQ(USART_SHELL_IRQ);


}


void uart_reable_task(void){
SYSTEM_INFO("uart_reable_task...\r\n");
USART_Shell_ComDrvInit();
__HAL_UART_ENABLE_IT(&huart_shell_Handle, UART_IT_ERR);
	
	HAL_UART_Receive_IT_UNLOCK(&huart_shell_Handle,(uint8_t *)&my_uartshell_redata,1);

}






void uart_debug_error(void){

uart_debug_dump(&huart_shell_Handle);
uart_debug_checkerror(&huart_shell_Handle,DISABLE);	
	
}

void uart_debug_fixerror(void){


uart_debug_checkerror(&huart_shell_Handle,ENABLE);
	
}



/**************************END OF FILE************************************/











