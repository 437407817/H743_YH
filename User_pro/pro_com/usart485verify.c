
#include "./pro_com/usart485verify.h"

#include <stdio.h>
#include <stdbool.h>
#include "./pro_com/usart_485_232_send.h"
//#include "./middle_business/usart_485_business.h"
#include "./DataConvert/data_verification.h"
#include <stdlib.h>
#include "./buffer/queue3.h"
 #include "./sys/sysio.h"

 #include "./usart/bsp_usart_COM485.h"
 #include "./middle_business/usart_485_dataGet.h"
 #include "./pro_com/usart_485_232_send.h"
 
/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   LED编号  亮/灭  异或校验数据
        0x55   0xAA    0x05      0x82     0x00     0x01      0xFB
***********************************************************************
*/
#define FRAME_REC_HEAD_0           0x5A  
#define FRAME_REC_HEAD_1           0xA5

#if c485_232_CRC16
#define PACKET_DATA_LEN_MIN    10                  //最小包长度
#define PACKET_DATA_LEN_MAX    136                 //最大包长度
#else
#define PACKET_DATA_LEN_MIN    10                  //最小包长度
#define PACKET_DATA_LEN_MAX    120                 //最大包长度
#endif


#define FUNC_DATA_IDX          3                  //功能字数组下标
#define SLAVE_CTRL_CODE          0x82               //功能字

#define MAX_BUF_SIZE           (PACKET_DATA_LEN_MAX * 8)										//环形队列总长度
static uint8_t g_rcvDataBuf[MAX_BUF_SIZE];
static QueueType_t g_rcvQueue;

typedef struct
{
	uint8_t ledNo;
	uint8_t ledState;
} LedCtrlInfo_t;


static void (*pProcSlaveDataFunc)(void); 

void reg_SlaveComCb(void (*pFunc)(void))
{
	pProcSlaveDataFunc = pFunc;
}




static void ProcUartData(uint8_t data)
{

	QueuePush(&g_rcvQueue, data);
}

///**
//***********************************************************
//* @brief 对数据进行异或运算
//* @param data, 存储数组的首地址
//* @param len, 要计算的元素的个数
//* @return 异或运算结果
//***********************************************************
//*/
//static uint8_t CalXorSum(const uint8_t *data, uint32_t len)
//{
//	uint8_t xorSum = 0;
//	for (uint32_t i = 0; i < len; i++)
//	{
//		xorSum ^= data[i];
//	}
//	return xorSum;
//}


//static uint8_t CalHead( uint8_t *q_data)
//{

//			//SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->len);
//		if((q_data[0]!=FRAME_HEAD_0)||(q_data[1]!=FRAME_HEAD_1)){
//SYSTEM_ERROR("5AA5 data is not right \n");
//	return 1;
//		}

//return 0;
//}



static uint8_t CalCRC16( uint8_t *q_data, uint32_t q_len)
{
	uint16_t crc16_res;
	uint8_t	cl,ch;
//				crc16_res = crc_16(&q_data[3], q_len-5);
		crc16_res = crc_16(&q_data[3], q_len-5);
	cl = (uint8_t)(crc16_res & 0xff);
	ch = (uint8_t)((crc16_res & 0xff00) >> 8);
	
		
		if((q_data[q_len-2]!=cl)||(q_data[q_len-1]!=ch)){
		SYSTEM_ERROR("crc data is not right h=0x%X  l=0x%X \n",ch,cl);
		return 1;
}
return 0;
}



///**
//***********************************************************
//* @brief LED控制处理函数
//* @param ctrlData，结构体指针，传入LED的编号和状态
//* @return 
//***********************************************************
//*/
//static void CtrlLed(LedCtrlInfo_t *ctrlData)
//{
//	ctrlData->ledState != 0 ? TurnOnLed(ctrlData->ledNo) : TurnOffLed(ctrlData->ledNo);
//}

static uint8_t readBuf[PACKET_DATA_LEN_MAX] = {0};
//s485dataswitch(&f_data,s_num,s_command_num);
void s485_convert_queue(uint8_t **outalldata,uint16_t* Command_address,uint8_t** OutData,uint8_t* Datanum){

*outalldata=readBuf;
*Command_address=((uint16_t)*(*outalldata+4)<<8)|(*(*outalldata+5));
//	SYSTEM_INFO("0x%4x ",(uint16_t)*(*outalldata+4)<<8);
//	SYSTEM_INFO("0x%4x ",(uint16_t)*(*outalldata+5));
//	SYSTEM_INFO("0x%4x ",((uint16_t)*(*outalldata+4)<<8)|(*(*outalldata+5)));
	*OutData=(*outalldata)+7;
	*Datanum=*(*outalldata+2)-4;
	if(*Datanum!=0){
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)*outalldata,*(*outalldata+2)+2,"485 GET-D-PACK -count: %d\n",*(*outalldata+2)+2);
	}
}




/**
***********************************************************
* @brief USB转串口任务处理函数
* @param
* @return 
***********************************************************
*/
void Usart485ComTask(void)
{
	
	static uint32_t qcp=0;
//	if(QueueCount(&g_rcvQueue)>=PACKET_DATA_LEN_MIN){
//	
//	}
	
//	SYSTEM_ERROR("num count --------,%d \n",QueueCount(&g_rcvQueue));
	while ((QueueCount(&g_rcvQueue)>=PACKET_DATA_LEN_MIN)&&(QueuePop(&g_rcvQueue, &readBuf[0]) == QUEUE_OK))
	{
		if (readBuf[0] != FRAME_REC_HEAD_0)
		{
//			SYSTEM_ERROR("5A XX data is not right 0X%x\n",readBuf[0]);
			continue;
		}
//		SYSTEM_ERROR("num count --------,yes \n");
		if ((QueuePop(&g_rcvQueue, &readBuf[1]) == QUEUE_EMPTY) || (readBuf[1] != FRAME_REC_HEAD_1))
		{
			SYSTEM_ERROR("XX A5 data is not right 0X%x\n",readBuf[1]);
			continue;
		}
		if ((QueuePop(&g_rcvQueue, &readBuf[2]) == QUEUE_EMPTY) || (readBuf[2] > PACKET_DATA_LEN_MAX))
	    {
		SYSTEM_ERROR("num count data is not right1,%d \n",readBuf[2]);
			continue;
		}
			qcp=QueuePopArray(&g_rcvQueue, &readBuf[3], readBuf[2]);
		if (qcp!= readBuf[2])
		{
			SYSTEM_ERROR("num count data is not right2,%d ,%d \n",qcp,readBuf[2]);
			continue;
		}
		#if c485_232_CRC16
//		if (CalCRC16(readBuf, readBuf[2] + 3) != readBuf[readBuf[2] + 3])
		if (CalCRC16(readBuf, readBuf[2] + 3) != 0)
		{
			SYSTEM_ERROR("CalCRC16 ERROR\n");
			continue;
		}
		#endif
//		SYSTEM_DEBUG("num count --------,yes12   0x%0x ,%d\n",readBuf[FUNC_DATA_IDX],FUNC_DATA_IDX);
		if (readBuf[FUNC_DATA_IDX] == SLAVE_CTRL_CODE)
		{
//			SYSTEM_ERROR("num count --------,yes3 \n");
//			CtrlLed((LedCtrlInfo_t *)(&readBuf[FUNC_DATA_IDX + 1]));
			pProcSlaveDataFunc();//reg_SlaveComCb(pull_data_from_485);
//			pProcUartDataFunc(ucCh);
		}
		
//		for (uint32_t i = 0; i < readBuf[2] + 4; i++)
//		{
//			printf("%02x ", readBuf[i]);
//		}
//		printf("\n");
	}
}

/**
***********************************************************
* @brief USB转串口应用初始化函数
* @param
* @return 
***********************************************************
*/
void Usart485ComAppInit(void)
{
	reg485ComCb(ProcUartData);
	reg_SlaveComCb(pull_data_from_485);
QueueInit(&g_rcvQueue, g_rcvDataBuf, MAX_BUF_SIZE);
}
