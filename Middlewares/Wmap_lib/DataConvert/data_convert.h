#ifndef __DATA_CONVERT_H
#define	__DATA_CONVERT_H

#include "./stm32_FH_xxx_hal.h"
#include <stdint.h>



#define FLIPW(h) ((((uint16_t)h << 8)&0xFF00) | ((uint16_t)h >> 8))













void EndianSwap_VpChange16HL(uint16_t* pt,uint16_t num);

void EndianSwap_VpChange64HL(uint64_t* pt);

void EndianSwap_VpChange64HL_CM7(uint64_t* pt);


void StructCopyAndVpReserve(uint8_t *aim_struct,uint8_t *origin_struct,uint8_t stuct_size,uint16_t *aim_struct_fromChange,uint8_t count);

void StructOnlyVpReserve(uint8_t *aim_struct_fromChange,uint8_t count);





uint16_t crc_16(uint8_t* buf, uint16_t length);
void U16CovertToU8lH(uint16_t* a16,uint8_t* bL8,uint8_t* bH8);

void copyArray(uint8_t* original , uint8_t* copied , uint8_t num);
//void DataArrayPrintf(uint8_t* arraydata, uint8_t num);
uint8_t HexToAscii(uint8_t *pHexStr,uint8_t *pAscStr,uint8_t Len);


uint8_t  AsciiToHex(uint8_t * pAscii, uint8_t * pHex, uint8_t nLen);
char  AsciiToHex02(unsigned char * pAscii, unsigned char * pHex, int nLen);
uint8_t  AsciiToHex03(uint8_t * pAscii, uint8_t* pHex, uint8_t nLen);



	



void  AsciiRemoveSpace01(uint8_t **pAscii,uint16_t nLen,uint16_t* retlen);

void  AsciiRemoveSpace02(uint8_t *pAscii,uint16_t nLen,uint16_t* retlen);

uint8_t convert_string_to_chars(const char *input, char *output);
void memset_uint16_Big(uint16_t* stru_data, uint16_t data, uint16_t count);

void memset_uint16_Small(uint16_t* stru_data,uint16_t data ,uint16_t count);


void ConvertCNtoGBKhexReverse(char *org_str ,uint16_t sLen,char *aim_array);
//void ConvertCNtoGBKhexReverse(char *org_str ,uint16_t sLen,char *aim_array);

void ConvertCNtoGBKhex(char *org_str,uint16_t sLen,char *aim_array);

#endif







