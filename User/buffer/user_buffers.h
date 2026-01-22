#ifndef __USER_BUFFERS_H
#define __USER_BUFFERS_H



#include "stm32_FH_xxx_hal.h"
#include "./buffer/queue2.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>




#define SHELL_RX_BUF_SIZE 64
//static uint8_t shell_rx_buffer[SHELL_RX_BUF_SIZE];
//static RingBuffer_t shell_rx_ring;



typedef struct {
    uint8_t shell_rx_buffer[SHELL_RX_BUF_SIZE];
		RingBuffer_t shell_rx_ring;
	
} ShellRingBuffer_t;






void user_all_buffer_create(void);









#endif
