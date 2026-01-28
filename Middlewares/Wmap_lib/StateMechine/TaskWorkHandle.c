

#include "./TaskWorkHandle.h"
#include "./sys/sysio.h"
#include "./sys/systick.h"


#include "./buffer/p_data_queue_outer.h"

//#include "./usart/p_data_queue.h"
//#include "./usart/p_data_queue_outer.h"

#include "./Sys/sys_dwt.h"
//#include "./iwdg/bsp_iwdg.h"   
//#include "./io/OUT_io/bsp_gpio_out.h"   

//extern str_ComuBoardState GV_ComuBoardState;
#include "./shell_port.h"
 #include "./touch/touch_drv.h"
#include "./DataCache/data_heap.h"

void Handle_letter_Shell_NoOsWhile(void){

	 letter_Shell_NoOsWhileTask();
//	SYSTEM_DEBUG("----ssss---");
	
}

 void Handle_test3(void){
static uint16_t i=0;
	uint64_t ii=0;
	ii=(uint64_t)GetSysRunTime();
//	SYSTEM_DEBUG("Handle_test2 is i=%d---%llu",i++,ii);
//	 SYSTEM_DEBUG("Handle_test2 is i=%d---0x%08X%08X", i++, (uint32_t)(ii >> 32), (uint32_t)ii);
//	
//	SYSTEM_BIGNUM_HEX(ii,"");
	SYSTEM_DEBUG("");
  SYSTEM_BIGNUM_DEC(1,ii,"Handle_test2 is i=%d  ",i++);
//	printf("Handle_test2 is i=%d---%llu",i++,ii);
//printf("xxxxxxxxxxxxxxxxx------------- ");
}


void Handle_IIC_TOUCH(void){

//	printf("----ssss---\r\n");
	touch_drv_while_test();
	
}
void Handle_pwd(void){

	
	
	
}

void Handle_DWT(void){
uint32_t ia=dwt_getCurrentTick();
	
	
	SYSTEM_DEBUG("dwt1= %u", ia);
	DWT_Delay_us(10);
	ia=dwt_getCurrentTick();
	SYSTEM_DEBUG("dwt2= %u", ia);
	
}

void Handle_watchdog_feed(void){

//	IWDG_Feed();
}


void Handle_usart(void){

//Test_get_all_data_from_queue(USARTx_A);
Test_all_queue();
	
}

#include "./rtosprintf/frtos_printf.h"
void Handle_led(void){


//LED00_Toggle;
	SYSTEM_I_PRINT("*******************************\r\n");
}



#include "./usart/bsp_usart_dma.h"

void Handle_DMA_USART_Save(void){//success
#if TESTUsartDMASendSaveAndSend	
	Test_Usart_DMA_SendSaveFun();
#endif	
// UartSendDataDMAtest01();
//	IWDG_Feed();
}
void Handle_DMA_USART_Send(void){//success
#if TESTUsartDMASendSaveAndSend	
	Usart_SendFUN_ALL();
#endif
//	static uint8_t txBuffer[] = "Hello from STM32F429 UART1 DMA!\r\n";
	
//	HAL_UART_Transmit_DMA(&huart_shell_Handle, txBuffer, strlen((char*)txBuffer));
// UartSendDataDMAtest01();
//	IWDG_Feed();
}


#include "./test/bsp_usart_test.h"
void Handle_DMA_USART(void){
	
	
	
// UartSendDataDMAtest01();
//	IWDG_Feed();
}
