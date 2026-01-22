

#include "./TaskTest/Task_check.h"
//#include "./usart/bsp_usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "./hook/hook_mem.h"
#include "./sys/sysio.h"







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























/**************************END OF FILE************************************/











