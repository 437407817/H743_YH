/**
  ******************************************************************************
  * @file    data_convert.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   can驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103-霸道 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./DataConvert/data_convert.h"
//#include "./usart/bsp_usart.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>









void EndianSwap_VpChange16HL(uint16_t* pt,uint16_t num){
	//uint16_t* vp =pt;
	
uint16_t count=num/2;
	if(num%2!=0)return;
	
	for(uint16_t i=0;i<count;i++){
		*(pt+i)=(((*(pt+i) << 8)&0xFF00) | (*(pt+i)  >> 8));
//*((pt)+i)=0;
	}

}
	
//	uint16_t a[3]={0x1234,0x5678,0x6789};
//	uint16_t *frame =&a[0];
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,6," vxcxcxcxc=");
//	VpChange16HL(frame,6);
//		SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,6," vxcxcxcxc=");
//	
//	
void EndianSwap_VpChange64HL(uint64_t* pt) {
    if (pt == NULL) return;
    
    uint64_t val = *pt;
    val = ((val & 0xFF00000000000000ULL) >> 56) |
          ((val & 0x00FF000000000000ULL) >> 40) |
          ((val & 0x0000FF0000000000ULL) >> 24) |
          ((val & 0x000000FF00000000ULL) >> 8)  |
          ((val & 0x00000000FF000000ULL) << 8)  |
          ((val & 0x0000000000FF0000ULL) << 24) |
          ((val & 0x000000000000FF00ULL) << 40) |
          ((val & 0x00000000000000FFULL) << 56);
    *pt = val;
}




void EndianSwap_VpChange64HL_CM7(uint64_t* pt) {
    if (pt == NULL) return;
    // 使用编译器内置函数，直接映射到硬件 REV 指令
    *pt = __builtin_bswap64(*pt);
}



void StructCopyAndVpReserve(uint8_t *aim_struct,uint8_t *origin_struct,uint8_t stuct_size,uint16_t *aim_struct_fromChange,uint8_t count){

	
	memmove(&*aim_struct,&*origin_struct,stuct_size);
	
	EndianSwap_VpChange16HL((uint16_t *)aim_struct_fromChange,count);
	
	
 //VpChange16HL(BigD_GV_CommSlaveBoardState.StopParalleltChargeVol,16*12);

	
}












void StructOnlyVpReserve(uint8_t *aim_struct_fromChange,uint8_t count){

	
	
	EndianSwap_VpChange16HL((uint16_t *)aim_struct_fromChange,count);
	
	
}






void U16CovertToU8lH(uint16_t* a16,uint8_t* bL8,uint8_t* bH8){
	
(*bL8) = (uint8_t)(*a16);	
(*bH8) = (uint8_t)((*a16)>>8);
	
}

//复制数组
void copyArray(uint8_t* original , uint8_t* copied , uint8_t num){

    for (uint8_t loop = 0; loop < num; loop++) {
        copied[loop] = original[loop];
      //  printf("%x  %x  \n",  original[loop],copied[loop]);
    }

}

////数组打印
//void DataArrayPrintf(uint8_t* arraydata, uint8_t num){
//    for (uint8_t loop = 0; loop < num; loop++) {
//        
//       printf("%x ",  arraydata[loop]);
//    }

//}

//https://blog.csdn.net/u010761559/article/details/83508834
//HEX是256数，ASCII是字符
//HEX转ASCII函数,计算机存储是HEX存储256，但要显示成0xFF的话，需转成ASCII，不带空格
//串口软件ASCII是一个字符代表一个位7F代表两个字符，HEX是7F带表一个字符，传输过程中都是一个字符一个字符传。
uint8_t HexToAscii(uint8_t *pHexStr,uint8_t *pAscStr,uint8_t Len)
{
	char Nibble[2];
	uint8_t Buffer[2048];
	uint8_t i = 0;
	uint8_t j = 0;

	for(i=0;i<Len;i++)
	{
		Nibble[0]=pHexStr[i] >> 4 & 0X0F;
		Nibble[1]=pHexStr[i] & 0x0F;
		for(j=0;j<2;j++)
		{
			if(Nibble[j]<10)
			{
				Nibble[j]=Nibble[j]+0x30;
			}
			else if(Nibble[j]<16)
			{
				Nibble[j]=Nibble[j]-10+'A';
			}
			else
			{
				return 0;
			}
		}
		memcpy(Buffer+i*2,Nibble,2);
	}
	Buffer[2*Len]=0x00;
	memcpy(pAscStr,Buffer,2*Len);
	pAscStr[2*Len]=0x00;
	return 1;
}


//ASCII转HEX函数,把显示的0x('FF''FE''FC')转计算机存储是HEX存储256，不带空格
uint8_t  AsciiToHex(uint8_t * pAscii, uint8_t* pHex, uint8_t nLen)
{
	uint16_t nHexLen = nLen / 2;
	uint8_t Nibble[2] = {0};
	int i = 0;
	int j = 0;

	if (nLen%2)
	{
		return 1;
	}

	for (i = 0; i < nHexLen; i ++)
	{
		Nibble[0] = *pAscii ++;		
		Nibble[1] = *pAscii ++;
		for (j = 0; j < 2; j ++)
		{
			if (Nibble[j] <= 'F' && Nibble[j] >= 'A')
				Nibble[j] = Nibble[j] - 'A' + 10;
			else if (Nibble[j] <= 'f' && Nibble[j] >= 'a')
				Nibble[j] = Nibble[j] - 'a' + 10;
			else if (Nibble[j] >= '0' && Nibble[j] <= '9')
				Nibble [j] = Nibble[j] - '0';
			else
				return 1;//Nibble[j] = Nibble[j] - 'a' + 10;
			
		}	// for (int j = ...)
		pHex[i] = Nibble[0] << 4;	// Set the high nibble
		pHex[i] |= Nibble[1];	//Set the low nibble
	}	// for (int i = ...)
	
	
	return 0;
}

//pHex内部创建一个指针指向他，则传入一级指针即可，但使用后需要释放内存
uint8_t  AsciiToHex03(uint8_t * pAscii, uint8_t* pHex, uint8_t nLen)
{
	uint16_t nHexLen = nLen / 2;
	uint8_t Nibble[2] = {0};
	 uint8_t* pHexTMP = pHex;
	int i = 0;
	int j = 0;

	if (nLen%2)
	{
		return 1;
	}

	for (i = 0; i < nHexLen; i ++)
	{
		Nibble[0] = *pAscii ++;		
		Nibble[1] = *pAscii ++;
		for (j = 0; j < 2; j ++)
		{
			if (Nibble[j] <= 'F' && Nibble[j] >= 'A')
				Nibble[j] = Nibble[j] - 'A' + 10;
			else if (Nibble[j] <= 'f' && Nibble[j] >= 'a')
				Nibble[j] = Nibble[j] - 'a' + 10;
			else if (Nibble[j] >= '0' && Nibble[j] <= '9')
				Nibble [j] = Nibble[j] - '0';
			else
				return 1;//Nibble[j] = Nibble[j] - 'a' + 10;
			
		}	// for (int j = ...)
		pHexTMP[i] = Nibble[0] << 4;	// Set the high nibble
		pHexTMP[i] |= Nibble[1];	//Set the low nibble
	}	// for (int i = ...)
	
	
	return 0;
}






//pHex由于是一维指针，并且内部没有创建指针，则传参时pHex必须是数组
char  AsciiToHex02(unsigned char * pAscii, unsigned char * pHex, int nLen)
{
	int nHexLen = nLen / 2;
	unsigned char Nibble[2] = {0};
	int i = 0;
	int j = 0;

	if (nLen%2)
	{
		return 1;
	}

	for (i = 0; i < nHexLen; i ++)
	{
		Nibble[0] = *pAscii ++;		
		Nibble[1] = *pAscii ++;
		for (j = 0; j < 2; j ++)
		{
			if (Nibble[j] <= 'F' && Nibble[j] >= 'A')
				Nibble[j] = Nibble[j] - 'A' + 10;
			else if (Nibble[j] <= 'f' && Nibble[j] >= 'a')
				Nibble[j] = Nibble[j] - 'a' + 10;
			else if (Nibble[j] >= '0' && Nibble[j] <= '9')
				Nibble [j] = Nibble[j] - '0';
			else
				return 1;//Nibble[j] = Nibble[j] - 'a' + 10;
			
		}	// for (int j = ...)
		pHex[i] = Nibble[0] << 4;	// Set the high nibble
		pHex[i] |= Nibble[1];	//Set the low nibble
	}	// for (int i = ...)
	return 0;
}







//ASCII去空格函数
void  AsciiRemoveSpace01(uint8_t **pAscii,uint16_t nLen,uint16_t* retlen){
uint16_t spacenum=0;
	uint16_t i;
	for( i=0;i<nLen;i++){
	
			if ((pAscii[0][i] <= 'F' && pAscii[0][i] >= 'A')||(pAscii[0][i] <= 'f' && pAscii[0][i] >= 'a')||(pAscii[0][i] >= '0' && pAscii[0][i] <= '9'))
				pAscii[0][i-spacenum]=pAscii[0][i];
			else
				spacenum++;

//		if(pAscii[0][i]==' '){
//		spacenum++;
//		}else{
//		pAscii[0][i-spacenum]=pAscii[0][i];
//		}
}
*retlen =i-spacenum;


}



//ASCII去空格函数
void  AsciiRemoveSpace02(uint8_t *pAscii,uint16_t nLen,uint16_t* retlen){
uint16_t spacenum=0;
	uint8_t *pAsciitmp =pAscii;
	uint16_t i;
	for( i=0;i<nLen;i++){

if ((pAsciitmp[i] <= 'F' && pAsciitmp[i] >= 'A')||(pAsciitmp[i] <= 'f' && pAsciitmp[i] >= 'a')||(pAsciitmp[i] >= '0' && pAsciitmp[i] <= '9'))
		pAsciitmp[i-spacenum]=pAsciitmp[i];
else
		spacenum++;	

//		if(pAscii[i]==' '){
//		spacenum++;
//		}else{
//		pAscii[i-spacenum]=pAscii[i];
//		}

}
*retlen =i-spacenum;


}




// 将十六进制字符转换为对应的整数值
int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return 1;
}

// 将输入的字符串每两个字符组合成一个 char 类型数据
uint8_t convert_string_to_chars(const char *input, char *output) {
    int len = strlen(input);
    if (len % 2 != 0) {
//        fprintf(stderr, "The length of the input string must be even.\n");
        return 1;
    }
    for (int i = 0; i < len; i += 2) {
        int high = hex_char_to_int(input[i]);
        int low = hex_char_to_int(input[i + 1]);
        if (high == 1 || low == 1) {
//            fprintf(stderr, "The input string contains non - hexadecimal characters.\n");
            return 2;
        }
        output[i / 2] = (high << 4) | low;
    }
		return 0;
}


//设置大端uint16数据整体为某值
void memset_uint16_Big(uint16_t* stru_data, uint16_t data, uint16_t count) {
    for (uint16_t i = 0; i < count; i++) {
        // 计算每个元素的地址
        uint8_t* elem = (uint8_t*)(&stru_data[i]);

        // 设置高字节
        elem[0] = (data >> 8) & 0xFF;
        // 设置低字节
        elem[1] = data & 0xFF;
    }
			 
}

//设置小端uint16数据整体为某值//stru_data需要修改的地址起始指针，data需要修改成什么数，count需要修改多少个
void memset_uint16_Small(uint16_t* stru_data,uint16_t data ,uint16_t count){

					   
																			
				 
	 
    for (uint16_t i = 0; i < count; i++) {
        // 计算每个元素的地址
        uint8_t* elem = (uint8_t*)(&stru_data[i]);

        // 设置低字节
        elem[0] = (data ) & 0xFF;
        // 设置高字节
        elem[1] = (data >> 8) & 0xFF;
    }

}





void ConvertCNtoGBKhexReverse(char *org_str ,uint16_t sLen,char *aim_array){

//str="电池正常\0";
//uint16_t sLen=strlen(str);
	
for(uint8_t i=0;i<sLen/2;i++)
	{
		aim_array[i*2+1]=*org_str++;//0
		aim_array[i*2]=*org_str++;//0
	}

}




void ConvertCNtoGBKhex(char *org_str,uint16_t sLen,char *aim_array){

//str="电池正常\0";
//		sLen=strlen(str);
for(uint16_t i=0;i<sLen;i++)
	{
		aim_array[i*2]=*org_str++;//0
//		Buff[i*2+1]=*str++;//0
	}
//char* str


}
/**************************END OF FILE************************************/











