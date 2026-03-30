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
  
#include "./usart/bsp_usart_COM485.h"
#include "./buffer/p_data_queue_outer.h"
#include "./buffer/que.h"
#include "./sys/sysio.h"
#include "./pro_com/usart485verify.h"



// 声明全局执行指针
UsartSendPtr this_com485_Usart_Send = NULL;

UART_HandleTypeDef huart_COM485_Handle;




#if TEST_COM485_UART

#define RX_BUF_LEN 64
//static uint8_t uart_rx_buf[RX_BUF_LEN];  // 多字节接收缓冲区
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
void USART_COM485_UartInit(void)
{ 
  
    USART_COM485_CLK_ENABLE();

    huart_COM485_Handle.Instance = USART_COM485;
    huart_COM485_Handle.Init.BaudRate = USART_COM485_BAUDRATE;
    huart_COM485_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    huart_COM485_Handle.Init.StopBits = UART_STOPBITS_1;
    huart_COM485_Handle.Init.Parity = UART_PARITY_NONE;
    huart_COM485_Handle.Init.Mode = UART_MODE_TX_RX;
    huart_COM485_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_COM485_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
    huart_COM485_Handle.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
    huart_COM485_Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//huart_COM485_Handle.Init.FIFOMode = UART_FIFOMODE_DISABLE;
    if (HAL_UART_Init(&huart_COM485_Handle) != HAL_OK)
    {
        Error_Handler();
    }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart_COM485_Handle, UART_TXFIFO_THRESHOLD_1_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart_COM485_Handle, UART_RXFIFO_THRESHOLD_1_2) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_UARTEx_DisableFifoMode(&huart_COM485_Handle) != HAL_OK)
//  if (HAL_UARTEx_EnableFifoMode(&huart_COM485_Handle) != HAL_OK)//如果不使能fifo，有的情况命令行按键报错，看情况选择
  {
    Error_Handler();
  }
    /* 使能 RXNE + IDLE 中断 */
//// 2. 强行关闭 RX 和 TX 的硬件 FIFO
//HAL_UARTEx_DisableFifoMode(&huart_COM485_Handle);
		// 将其清零即可彻底关闭 FIFO
//huart_COM485_Handle.Instance->CR1 &= ~(1U << 29);


		
	
		#if USE_COM485_IT_1
		__HAL_UART_ENABLE_IT(&huart_COM485_Handle,UART_IT_IDLE);
		__HAL_UART_ENABLE_IT(&huart_COM485_Handle, UART_IT_RXNE);		
		#else
		__HAL_UART_ENABLE_IT(&huart_COM485_Handle,UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart_COM485_Handle, &uart485_rx_byte, 1);
		#endif

		
		
		
    /* H743：中断优先级必须低 */
    HAL_NVIC_SetPriority(USART_COM485_IRQ, 10, 0);// 7 位于 5~15 之间，安全
    HAL_NVIC_EnableIRQ(USART_COM485_IRQ);
		
		 
}


void USART_COM485_UartDeInit(void)
{ 

HAL_UART_DeInit(&huart_COM485_Handle);
	HAL_NVIC_DisableIRQ(USART_COM485_IRQ);
}

/**
  * @brief UART MSP 初始化 
  * @param huart: UART handle
  * @retval 无
  */
void USART_COM485_GpioInit(void)
{  

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};

    USART_COM485_TX_GPIO_CLK_ENABLE();
    USART_COM485_RX_GPIO_CLK_ENABLE();

    /* USART1 clock source */
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

    GPIO_InitStruct.Pin = USART_COM485_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = USART_COM485_TX_AF;
    HAL_GPIO_Init(USART_COM485_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART_COM485_RX_PIN;
    GPIO_InitStruct.Alternate = USART_COM485_RX_AF;
    HAL_GPIO_Init(USART_COM485_RX_GPIO_PORT, &GPIO_InitStruct);
	

}


/**
***********************************************************
* @brief USB转串口硬件初始化
* @param
* @return 
***********************************************************
*/
void USART_COM485_232_ComDrvInit(void)
{
	
		Usart485ComAppInit();//如果无此，pProcUartDataFunc(recv_byte);会报错
	
	
	USART_COM485_UartInit();

USART_COM485_GpioInit();

	Usart_COM485_send_Config_Init();

}


static void (*pProcUartDataFunc)(uint8_t data);   //函数指针变量，保存应用层回调函数地址
/**
***********************************************************
* @brief 注册回调函数
* @param pFunc，函数指针变量，接收传入的回调函数地址
* @return 
***********************************************************
*/
void reg485ComCb(void (*pFunc)(uint8_t data))
{
	pProcUartDataFunc = pFunc;
}






//void HAL_UART_COM485_RxCpltCallback(UART_HandleTypeDef *huart){
//	
//uart_rx_buf[index++] = uart_rx_byte;
////SYSTEM_DEBUG("+++%c",uart_rx_byte);
//HAL_UART_Receive_IT(&huart_COM485_Handle, &uart_rx_byte, 1);
//		
//}




/**
***********************************************************
* @brief 串口1中断服务函数
* @param
* @return 
***********************************************************
*/

#if USE_COM485_IT_1
void USART_COM485_IRQHandler(void)
{

	 uint8_t recv_byte = 0;

		if (__HAL_UART_GET_FLAG(&huart_COM485_Handle, UART_FLAG_RXNE) != RESET)
	{		
    // 清除RXNE标志（必须！否则中断反复触发）2
    __HAL_UART_CLEAR_FLAG(&huart_COM485_Handle, UART_FLAG_RXNE);
		    /* 读取接收到的1个字节 */
    recv_byte = (uint8_t)(huart_COM485_Handle.Instance->RDR);
		SYSTEM_INFO("+%02x",recv_byte);
		pProcUartDataFunc(recv_byte);
//				if (index < RX_BUF_LEN)
//     {
////			 uart_rx_buf[index++] = uData;
//			 
//		 }
		
		
	}
			if (__HAL_UART_GET_FLAG(&huart_COM485_Handle, UART_FLAG_IDLE) != RESET)
	{		

        // 关键：清除 IDLE 标志（必须手动清除，否则会持续触发中断）
        __HAL_UART_CLEAR_IDLEFLAG(&huart_COM485_Handle);
//				indexsize=index;
//				index=0;
//		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,uart_rx_buf,indexsize,"--%d",indexsize);
	}
	
}

//void USART_COM485_IRQHandler(void){

//			if (__HAL_UART_GET_FLAG(&huart_COM485_Handle, UART_FLAG_IDLE) != RESET)
//	{		

//        // 关键：清除 IDLE 标志（必须手动清除，否则会持续触发中断）
//        __HAL_UART_CLEAR_IDLEFLAG(&huart_COM485_Handle);
//				indexsize=index;
//				index=0;
////		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,uart_rx_buf,indexsize,"--%d",indexsize);
//	}

//	HAL_UART_IRQHandler(&huart_COM485_Handle);	
//}
#endif





void Usart_COM485_send_Config_Init(void)
{
#if USE_COM485_DMA_SEND
    this_com485_Usart_Send = Usart_COM485_SendArray_DMA;
#else
    this_com485_Usart_Send = Usart_COM485_SendArray;
#endif
}







void Usart_COM485_SendArray(UART_HandleTypeDef *huart, uint8_t *array, uint16_t num)
{
    // 直接发送整个数组（阻塞式）
    HAL_UART_Transmit(huart, array, num, HAL_MAX_DELAY);

    // 等待发送完成（可选，HAL_UART_Transmit 已等发送为空，再加更保险）
    while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);
}

/**
 * @brief  使用 DMA 异步发送串口数组
 * @note   注意：在 H7 系列上，如果开启了 D-Cache，需处理 Cache 一致性
 */
void Usart_COM485_SendArray_DMA(UART_HandleTypeDef *huart, uint8_t *array, uint16_t num)
{
    // 1. 确保上一次 DMA 发送已经完成
    // 如果不检查状态直接调用，可能会返回 HAL_BUSY
    while (huart->gState != HAL_UART_STATE_READY);

    /* 2. 对于 STM32H7 系列（Cortex-M7 内核）：
       如果 array 位于可缓存的内存区域（如 AXI SRAM），必须先清理缓存，
       确保 DMA 读取的是内存中最新的数据，而不是 Cache 里的旧数据。*/
    // SCB_CleanDCache_by_Addr((uint32_t*)array, num); 

    // 3. 开启 DMA 发送
    if (HAL_UART_Transmit_DMA(huart, array, num) != HAL_OK)
    {
        // 错误处理逻辑
        Error_Handler();
    }
}

#if 0
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1) // 假设是串口1
    {
        // DMA 发送完成后的逻辑
    }
}

#endif


//static void Test_USART_COM485_print(void){
//if(indexsize!=0){
//	#if USE_IT_1
//	//这种方式是有问题的，发送是阻塞的，接收，会导丢失数据，因为运行完后indexsize=0;所以发送过程中接收数据出错，要用环形队列
//	HAL_UART_Transmit(&huart_COM485_Handle,uart_rx_buf,indexsize,1000);
//	#else
//	HAL_UART_Transmit(&huart_COM485_Handle,uart_rx_buf,indexsize,1000);
//	#endif
//	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,uart_rx_buf,indexsize,"-+%d",indexsize);
//	indexsize=0;
//	
//}
//}

//void Test_USART_COM485_while(void){
////	while(1){
//	Test_USART_COM485_print();
////		HAL_Delay(1000);
////		printf("*");
////	}
//}





































/*********************************************END OF FILE**********************/
