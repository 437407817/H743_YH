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
  * 实验平台:野火 STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_usart_callback.h"

#include "./usart/bsp_usart_dma.h"
#include "./usart/bsp_usart_shell.h"
#include "./usart/bsp_usart.h"
 #include "./shell_port.h"



// DMA 接收完成回调（循环模式下，缓冲区满后触发）
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//  if (huart == &huart_a) {
//    HAL_UART_a_RxCpltCallback();
//  }else 
#if !USE_OS	
#if TEST_SHELL_UART||USE_LETTER_SHELL
	if(huart == &huart_shell_Handle){
	HAL_UART_Shell_RxCpltCallback();
	}
	
#endif
	
#endif
//	SYSTEM_INFO("-");
	#if USE_LETTER_SHELL&&USE_OS
	if(huart == &huart_shell_Handle){
//		SYSTEM_INFO("*");
	HAL_UART_Shell_RxCpltCallback(huart);
	}
	#endif
	
#if USE_UARTx_DMA&&USE_UART_DMA_RX
if (huart == &huart_DMA_Handle) {
HAL_USARTx_DMA_RxCpltCallback();
}
#endif

}


//发送0个数据不会调用该函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
#if USE_UARTx_DMA
  if (huart == &huart_DMA_Handle) {
HAL_USARTx_DMA_TxCpltCallback();
  }
#endif
	
}






void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
#if USE_UARTx_DMA&&!USE_LETTER_SHELL&&!USE_OS
if (huart == &huart_DMA_Handle) {
	HAL_USARTx_DMA_ErrorCallback();
  }
#endif
	SYSTEM_INFO("--------------------err-------------------");
	#if USE_LETTER_SHELL&&USE_OS
	if(huart == &huart_shell_Handle){
	HAL_UART_Shell_ErrorCallback(huart);
	}
	#endif
}


















/*********************************************END OF FILE**********************/
