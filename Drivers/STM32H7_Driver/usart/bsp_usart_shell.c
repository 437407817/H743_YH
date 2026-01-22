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
  
#include "./usart/bsp_usart_shell.h"
#include "./buffer/p_data_queue_outer.h"
#include "./buffer/que.h"
#include "./sys/sysio.h"




UART_HandleTypeDef huart_shell_Handle;


#if TEST_SHELL_UART

#define RX_BUF_LEN 64
static uint8_t uart_rx_buf[RX_BUF_LEN];  // 多字节接收缓冲区
static volatile uint8_t index = 0;
static volatile uint8_t indexsize = 0;

//uint16_t rx_len = 0;              // 接收长度计数
uint8_t uart_rx_byte;  // 单次接收 1 字节
#endif



 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */  
void USART_Shell_UartInit(void)
{ 
  
    USART_SHELL_CLK_ENABLE();

    huart_shell_Handle.Instance = USART_SHELL;
    huart_shell_Handle.Init.BaudRate = USART_SHELL_BAUDRATE;
    huart_shell_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    huart_shell_Handle.Init.StopBits = UART_STOPBITS_1;
    huart_shell_Handle.Init.Parity = UART_PARITY_NONE;
    huart_shell_Handle.Init.Mode = UART_MODE_TX_RX;
    huart_shell_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_shell_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
    huart_shell_Handle.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
    huart_shell_Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//huart_shell_Handle.Init.FIFOMode = UART_FIFOMODE_DISABLE;
    if (HAL_UART_Init(&huart_shell_Handle) != HAL_OK)
    {
        Error_Handler();
    }

    /* 使能 RXNE + IDLE 中断 */
//// 2. 强行关闭 RX 和 TX 的硬件 FIFO
//HAL_UARTEx_DisableFifoMode(&huart_shell_Handle);
		// 将其清零即可彻底关闭 FIFO
//huart_shell_Handle.Instance->CR1 &= ~(1U << 29);


		
		#if TEST_SHELL_UART
		#if USE_IT_1
		__HAL_UART_ENABLE_IT(&huart_shell_Handle,UART_IT_IDLE);
		__HAL_UART_ENABLE_IT(&huart_shell_Handle, UART_IT_RXNE);		
		#else
		__HAL_UART_ENABLE_IT(&huart_shell_Handle,UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart_shell_Handle, &uart_rx_byte, 1);
		#endif
		#endif
		
		
		
    /* H743：中断优先级必须低 */
    HAL_NVIC_SetPriority(USART_SHELL_IRQ, 5, 0);
    HAL_NVIC_EnableIRQ(USART_SHELL_IRQ);
		
		 
}







/**
  * @brief UART MSP 初始化 
  * @param huart: UART handle
  * @retval 无
  */
void USART_Shell_GpioInit(void)
{  

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};

    USART_SHELL_TX_GPIO_CLK_ENABLE();
    USART_SHELL_RX_GPIO_CLK_ENABLE();

    /* USART1 clock source */
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

    GPIO_InitStruct.Pin = USART_SHELL_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = USART_SHELL_TX_AF;
    HAL_GPIO_Init(USART_SHELL_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART_SHELL_RX_PIN;
    GPIO_InitStruct.Alternate = USART_SHELL_RX_AF;
    HAL_GPIO_Init(USART_SHELL_RX_GPIO_PORT, &GPIO_InitStruct);
	

}


/**
***********************************************************
* @brief USB转串口硬件初始化
* @param
* @return 
***********************************************************
*/
void USART_Shell_ComDrvInit(void)
{
	
		
	USART_Shell_UartInit();
USART_Shell_GpioInit();
	
	

}


#if TEST_SHELL_UART


 #if !(USE_LETTER_SHELL)
void HAL_UART_Shell_RxCpltCallback(void){
	
uart_rx_buf[index++] = uart_rx_byte;
//SYSTEM_DEBUG("+++%c",uart_rx_byte);
HAL_UART_Receive_IT(&huart_shell_Handle, &uart_rx_byte, 1);
		
}




/**
***********************************************************
* @brief 串口1中断服务函数
* @param
* @return 
***********************************************************
*/

#if USE_IT_1
void USART_SHELL_IRQHandler(void)
{

	
	uint8_t uData;
		if (__HAL_UART_GET_FLAG(&huart_shell_Handle, UART_FLAG_RXNE) != RESET)
	{		

		HAL_UART_Receive(&huart_shell_Handle, &uData, 1, 0xFFFF);
				if (index < RX_BUF_LEN)
     {
			 uart_rx_buf[index++] = uData;
		 }
//		SYSTEM_DEBUG("+++%c",uData);
		
	}
			if (__HAL_UART_GET_FLAG(&huart_shell_Handle, UART_FLAG_IDLE) != RESET)
	{		

        // 关键：清除 IDLE 标志（必须手动清除，否则会持续触发中断）
        __HAL_UART_CLEAR_IDLEFLAG(&huart_shell_Handle);
				indexsize=index;
				index=0;
//		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,uart_rx_buf,indexsize,"--%d",indexsize);
	}
	
}
#else
void USART_SHELL_IRQHandler(void){

			if (__HAL_UART_GET_FLAG(&huart_shell_Handle, UART_FLAG_IDLE) != RESET)
	{		

        // 关键：清除 IDLE 标志（必须手动清除，否则会持续触发中断）
        __HAL_UART_CLEAR_IDLEFLAG(&huart_shell_Handle);
				indexsize=index;
				index=0;
//		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,uart_rx_buf,indexsize,"--%d",indexsize);
	}

	HAL_UART_IRQHandler(&huart_shell_Handle);	
}



#endif



#endif

static void Test_USART_SHELL_print(void){
if(indexsize!=0){
	#if USE_IT_1
	//这种方式是有问题的，发送是阻塞的，接收，会导丢失数据，因为运行完后indexsize=0;所以发送过程中接收数据出错，要用环形队列
	HAL_UART_Transmit(&huart_shell_Handle,uart_rx_buf,indexsize,1000);
	#else
	HAL_UART_Transmit(&huart_shell_Handle,uart_rx_buf,indexsize,1000);
	#endif
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,uart_rx_buf,indexsize,"-+%d",indexsize);
	indexsize=0;
	
}
}

void Test_USART_SHELL_while(void){
	while(1){
	Test_USART_SHELL_print();
		HAL_Delay(1000);
		printf("*");
	}
}


#endif






//extern UART_HandleTypeDef huart1;

//重定向c库函数printf到串口USART_A，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口USART_A */
	HAL_UART_Transmit(&huart_shell_Handle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///重定向c库函数scanf到串口USART_A，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&huart_shell_Handle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}





// 发送一个字符
void USART_Shell_SendChar(uint8_t ch)
{
    HAL_UART_Transmit(&huart_shell_Handle, &ch, 1, HAL_MAX_DELAY);
}

// 接收一个字符
uint8_t USART_Shell_ReceiveChar(void)
{
    uint8_t ch;
	while (__HAL_UART_GET_FLAG(&huart_shell_Handle, UART_FLAG_RXNE) == RESET){};
    HAL_UART_Receive(&huart_shell_Handle, &ch, 1, HAL_MAX_DELAY);
    return ch;
}



/*****************  发送字符串 **********************/
void USART_Shell_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit( &huart_shell_Handle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}





























/*********************************************END OF FILE**********************/
