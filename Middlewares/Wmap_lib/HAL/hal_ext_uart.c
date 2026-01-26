


#include "./HAL/hal_ext_uart.h"
#include "stm32h7xx_hal.h"
#include "./sys/sysio.h"


#include "stm32h743xx.h"


static uint8_t is_uart_configured(UART_HandleTypeDef *huart)
{
    // 1. CR1 需使能 UART + 接收 + 发送
    if ((huart->Instance->CR1 & (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE)) != 
        (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE))
    {
        return 0;
    }
    // 2. BRR 波特率寄存器不能为0（初始化必配置）
    if (huart->Instance->BRR == 0)
    {
        return 0;
    }
    return 1;
}
void uart_debug_dump(UART_HandleTypeDef *huart)
{
    SYSTEM_INFO("\r\n========= UART Deadlock Diagnosis Snapshot =========\r\n");
    // 1. HAL 状态机核心变量
    SYSTEM_INFO("1. HAL State:\r\n");
    SYSTEM_INFO("   - Global gState: 0x%04X (RESET=0x00, READY=0x20, BUSY=0x21)\r\n", huart->gState);
    SYSTEM_INFO("   - RX Substate: 0x%04X (READY=0x00, BUSY_RX=0x20)\r\n", huart->RxState);
    SYSTEM_INFO("   - Error Code: 0x%04X\r\n", huart->ErrorCode);
    SYSTEM_INFO("   - Lock Status: %s (0=UNLOCKED,1=LOCKED)\r\n", huart->Lock==HAL_UNLOCKED?"UNLOCKED":"LOCKED");
    
    // 2. 硬件寄存器标志位（H7 用 ISR 替代 SR）
    SYSTEM_INFO("2. Hardware Flags:\r\n");
    uint32_t isr = huart->Instance->ISR;
    SYSTEM_INFO("   - ISR Register: 0x%04X\r\n", isr);
    SYSTEM_INFO("   - ORE(Overrun): %d, PE(Parity): %d, FE(Frame): %d, NE(Noise): %d\r\n",
                (isr & UART_FLAG_ORE) ? 1:0,
                (isr & UART_FLAG_PE) ? 1:0,
                (isr & UART_FLAG_FE) ? 1:0,
                (isr & UART_FLAG_NE) ? 1:0);
    SYSTEM_INFO("   - RXNE(Receive Ready): %d, TXE(Transmit Ready): %d\r\n",
                (isr & UART_FLAG_RXNE) ? 1:0,
                (isr & UART_FLAG_TXE) ? 1:0);
    
    // 3. 中断使能状态
    SYSTEM_INFO("3. Interrupt Enable Status:\r\n");
    uint32_t cr1 = huart->Instance->CR1;
    SYSTEM_INFO("   - RXNE Interrupt: %d, Error Interrupt(ERR): %d\r\n",
                (cr1 & UART_IT_RXNE) ? 1:0,
                (cr1 & UART_IT_ERR) ? 1:0);
    
    // 4. 初始化状态（替代 IsInitialized）
    SYSTEM_INFO("4. Initialization Status:\r\n");
    SYSTEM_INFO("   - HAL Init Status: %s \r\n", //(RESET=Not Initialized)
                (huart->gState != HAL_UART_STATE_RESET) ? "Initialized" : "Not Initialized");
    SYSTEM_INFO("   - Clock Enabled: %s\r\n", __HAL_RCC_USART1_IS_CLK_ENABLED()?"Enabled":"Disabled");
    SYSTEM_INFO("   - UART Configured: %s\r\n", is_uart_configured(huart)?"Yes":"No");
    SYSTEM_INFO("======================================\r\n");
}




void uart_debug_checkerror(UART_HandleTypeDef *huart,FunctionalState ablestate){

// 检查并修复：强制使能 RXNE 中断
if ((huart->Instance->CR1 & USART_CR1_RXNEIE) == 0) {
    SYSTEM_INFO("RXNE Interrupt Disabled! \r\n");
	if(ablestate==ENABLE){
	__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE); // 使能RXNE中断
	}
  
}
// 检查并重置状态机
if (huart->gState != HAL_UART_STATE_READY || 
    huart->RxState == HAL_UART_STATE_BUSY_RX) {
    SYSTEM_INFO("HAL State Locked! gState=0x%04X, RxState=0x%04X\r\n",huart->gState, huart->RxState);
				if(ablestate==ENABLE){
    // 强制重置状态
    huart->gState = HAL_UART_STATE_READY;
    huart->RxState = HAL_UART_STATE_READY;
    __HAL_UNLOCK(huart);	
	}

}

// 检查并开启接收功能
if ((huart->Instance->CR1 & USART_CR1_RE) == 0) {
    SYSTEM_INFO("UART Receive Disabled! \r\n");
		if(ablestate==ENABLE){
    huart->Instance->CR1 |= USART_CR1_RE; // 开启接收
	}

}
	if(ablestate==ENABLE){
	SYSTEM_INFO("enabling...\r\n");
	}
//if ((huart_shell_Handle.Instance->CR1 & USART_CR1_RXNEIE) == 0) {
//    __HAL_UART_ENABLE_IT(&huart_shell_Handle, UART_IT_RXNE);
//}

}


// 适配所有 H743 HAL 库版本的 LPUART1 FIFO 关闭代码
// 无需依赖 LPUART_CR3_FIFOEN/__HAL_LPUART_DISABLE 等宏
//void lpuart1_disable_fifo(void)
//{
//    // 1. 先禁用 LPUART1 外设（等价于 __HAL_LPUART_DISABLE）
//    // LPUART1->CR1 的 UE 位（位0）：置0禁用外设，置1启用
//    LPUART1->CR1 &= ~(1 << 0);
//    
//    // 2. 关闭 FIFO 功能（等价于清除 LPUART_CR3_FIFOEN）
//    // LPUART1->CR3 的 FIFOEN 位（位0）：置0关闭FIFO，置1开启
//    LPUART1->CR3 &= ~(1 << 0);
//    
//    // 3. 可选：清空接收/发送 FIFO（避免残留数据）
//    // RFCMD 位（CR3的1~2位）：置 01 清空接收FIFO
//    LPUART1->CR3 |= (0x01 << 1);
//    // TFCMD 位（CR3的3~4位）：置 01 清空发送FIFO
//    LPUART1->CR3 |= (0x01 << 3);
//    
//    // 4. 重新启用 LPUART1 外设（等价于 __HAL_LPUART_ENABLE）
//    LPUART1->CR1 |= (1 << 0);
//}



void lpuart_disable_fifo_hal(UART_HandleTypeDef *huart)
{
    // 1. 禁用 LPUART1（用 UART 宏替代 __HAL_LPUART_DISABLE）
    __HAL_UART_DISABLE(huart);
    
    // 2. 关闭 FIFO（直接操作 CR3 寄存器，避开未定义的 LPUART_CR3_FIFOEN）
    huart->Instance->CR3 &= ~(1 << 0);  // FIFOEN 是 CR3 的第0位
    
    // 3. 清空 FIFO 缓冲区
    huart->Instance->CR3 |= (0x01 << 1);  // 清空接收FIFO
    huart->Instance->CR3 |= (0x01 << 3);  // 清空发送FIFO
    
    // 4. 启用 LPUART1（用 UART 宏替代 __HAL_LPUART_ENABLE）
    __HAL_UART_ENABLE(huart);
}
















