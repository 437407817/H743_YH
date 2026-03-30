#ifndef __USART_485_DATAGET_H
#define	__USART_485_DATAGET_H


#include "./stm32_FH_xxx_hal.h"











//void screendataswitch(uint16_t addr,uint8_t num,uint8_t** p_data);
void screendataswitch(uint16_t addr,uint8_t** p_data,uint8_t num);

void pull_data_from_485(void);

void s485dataswitch(uint8_t** p_data,uint8_t num,uint8_t command_num);









#endif /* __USART1_H */
