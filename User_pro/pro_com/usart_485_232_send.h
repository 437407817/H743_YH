#ifndef __USART_485_SEND_H
#define	__USART_485_SEND_H


#include "stm32_FH_xxx.h"
#include "./usart/bsp_usart_COM485.h"

/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   LED编号  亮/灭  异或校验数据
        0x55   0xAA    0x03      0x06     0x00     0x01      0xFB
***********************************************************************
*/
/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   数据    CRC低   CRC高
        0x55   0xAA    0x03      0x06    0x00    0x01     0x01 
***********************************************************************
*/



#define c485_232_CRC16 1
#define USE_c485 0


#define COM_DW_WRITE_CMD  0X82
#define COM_DW_READ_CMD 	0X83



#define huart_COM_DW_Handle huart_COM485_Handle

#define Usart_DW_SendArray this_com485_Usart_Send



void com_DW_write_cmd(uint16_t addr, uint16_t *buf, uint16_t size);
void DW_SCREEN_write_cmd_print(uint16_t addr, uint16_t *buf, uint16_t size, uint16_t debugprint);
//void DW_SCREEN_read_cmd(uint16_t addr, uint8_t num);
void DW_SCREEN_read_cmd_print(uint16_t addr, uint8_t num, uint16_t debugprint);
//void DW_DGUSI_SCREEN_write_Reg_cmd_print(uint8_t REGaddr, uint16_t REGADATA,  uint16_t debugprint);





#endif /* __Comu485Dis_H */
