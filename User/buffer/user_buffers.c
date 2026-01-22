#include "./buffer/user_buffers.h"



#include "./sys/sysio.h"





ShellRingBuffer_t shellRingBuffer;


void user_all_buffer_create(void){

RingBuffer_Init(&shellRingBuffer.shell_rx_ring, shellRingBuffer.shell_rx_buffer, SHELL_RX_BUF_SIZE);
	
	
	
	

}


















