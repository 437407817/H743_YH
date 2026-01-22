#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "./touch/touch_drv.h"
#include "./touch/bsp_i2c_touch.h"
#include "./Sys/systick.h"
#include "./Sys/sys_dwt.h"
#include "./sys/bsp_systime.h" 
#include "./sys/sysio.h"



#define DelayNus	delay_us
#define DelayNms	delay_ms





#if 1
/**
*******************************************************************
* @function 产生IIC起始时序，准备发送或接收数据前必须由起始序列开始 
* @param
* @return 
* @brief    SCL为高电平时，SDA由高电平向低电平跳变，开始传输数据 
*           生成下图所示的波形图，即为起始时序 
*                1 2    3     4   
*                    __________     
*           SCL : __/          \_____ 
*                 ________          
*           SDA :         \___________ 
*******************************************************************
*/
static void I2CStart(void)
{
	SET_I2C_SDA();          // 1#数据线SDA输出高电平
	SET_I2C_SCL();          // 2#时钟线SCL输出高电平   
	DelayNus(4);            // 延时4us
	CLR_I2C_SDA();          // 3#数据线SDA输出低电平 
	DelayNus(4);            // 延时4us
	CLR_I2C_SCL();          // 4#时钟线SCL输出低电平，保持I2C的时钟线SCL为低电平，准备发送或接收数据 
	DelayNus(4);            // 延时4us
}

/**
*******************************************************************
* @function 产生IIC停止时序  
* @param
* @return 
* @brief    SCL为高电平时，SDA由低电平向高电平跳变，结束传输数据 
*          生成下图所示的波形图，即为停止时序 
*                1 2   3  4   
*                       _______________     
*          SCL : ______/          
*                __        ____________  
*          SDA:    \______/
*******************************************************************
*/
static void I2CStop(void)
{
	CLR_I2C_SDA();          //2#数据线SDA输出低电平
	DelayNus(4);            //延时4us
	SET_I2C_SCL();          //3#时钟线SCL输出高电平
	DelayNus(4);  
	SET_I2C_SDA();          //4#数据线SDA输出高电平，发送I2C总线结束信号
}

/**
*******************************************************************
* @function 发送一字节，数据从高位开始发送出去
* @param    byte
* @return 
* @brief    下面是具体的时序图 
*                1 2     3      4
*                         ______
*           SCL: ________/      \______    
*                ______________________    
*           SDA: \\\___________________
*******************************************************************
*/
static void I2CSendByte(uint8_t byte)
{                          
	for (uint8_t i = 0; i < 8; i++)   // 循环8次，从高到低取出字节的8个位
	{     
		if ((byte & 0x80))            // 2#取出字节最高位，并判断为‘0’还是‘1’，从而做出相应的操作
		{
			SET_I2C_SDA();            // 数据线SDA输出高电平，数据位为‘1’
		}
		else
		{  
			CLR_I2C_SDA();      	  // 数据线SDA输出低电平，数据位为‘0’
		}
		
		byte <<= 1;            		  // 左移一位，次高位移到最高位
		
		DelayNus(4);          		  // 延时4us
		SET_I2C_SCL();                // 3#时钟线SCL输出高电平
		DelayNus(4);          		  // 延时4us
		CLR_I2C_SCL();        		  // 4#时钟线SCL输出低电平
		DelayNus(4);                  // 延时4us  
	}  
}

/**
*******************************************************************
* @function 读取一字节数据
* @param    
* @return   读取的字节
* @brief    下面是具体的时序图
*                       ______
*           SCL: ______/      \___        
*                ____________________    
*           SDA: \\\\______________\\\
*******************************************************************
*/
static uint8_t I2CReadByte(void)
{
	uint8_t byte = 0;           		// byte用来存放接收的数据
	SET_I2C_SDA();                      // 释放SDA
	for (uint8_t i = 0; i < 8; i++)     // 循环8次，从高到低读取字节的8个位
	{
		SET_I2C_SCL();          		// 时钟线SCL输出高电平
		DelayNus(4);            		// 延时4us
		byte <<= 1;          			// 左移一位，空出新的最低位

		if (GET_I2C_SDA())       		// 读取数据线SDA的数据位
		{
			byte++;            			// 在SCL的上升沿后，数据已经稳定，因此可以取该数据，存入最低位
		}
		CLR_I2C_SCL();          		// 时钟线SCL输出低电平
		DelayNus(4);            		// 延时4us
	} 

	return byte;           				// 返回读取到的数据
}

/**
*******************************************************************
* @function 等待接收端的应答信号
* @param    
* @return   1，接收应答失败；0，接收应答成功
* @brief    当SDA拉低后，表示接收到ACK信号，然后，拉低SCL，
*           此处表示发送端收到接收端的ACK
*                _______|____     
*           SCL:        |    \_________    
*                _______|     
*           SDA:         \_____________ 
*******************************************************************
*/
static bool I2CWaitAck(void)
{
	uint8_t errTimes = 0;
	
	SET_I2C_SDA();             // 释放SDA总线,很重要
	DelayNus(4);               // 延时4us
	
	SET_I2C_SCL();             // 时钟线SCL输出高电平
	DelayNus(4);               // 延时4us

	while (GET_I2C_SDA())      // 读回来的数据如果是高电平，即接收端没有应答
	{
		errTimes++;            // 计数器加1

		if (errTimes > 250)    // 如果超过250次，则判断为接收端出现故障，因此发送结束信号
		{
			I2CStop();         // 产生一个停止信号
			return false;      // 返回值为1，表示没有收到应答信号
		}
	}

	CLR_I2C_SCL();             // 表示已收到应答信号，时钟线SCL输出低电平
	DelayNus(4);               // 延时4us
	
	return true;               // 返回值为0，表示接收应答成功  
}

/**
*******************************************************************
* @function 发送应答信号
* @param    
* @return   
* @brief    下面是具体的时序图 
*                 1 2     3      4      5     
*                         ______
*           SCL: ________/      \____________    
*                __                     ______
*           SDA:   \___________________/        
*******************************************************************
*/
static void I2CSendAck(void)
{
	CLR_I2C_SDA();          // 2#数据线SDA输出低电平
	DelayNus(4);            // 延时4us
	SET_I2C_SCL();          // 3#时钟线SCL输出高电平,在SCL上升沿前就要把SDA拉低，为应答信号
	DelayNus(4);            // 延时4us
	CLR_I2C_SCL();          // 4#时钟线SCL输出低电平
	DelayNus(4);            // 延时4us
	SET_I2C_SDA();          // 5#数据线SDA输出高电平，释放SDA总线,很重要
}

/**
*******************************************************************
* @function 发送非应答信号
* @param    
* @return   
* @brief    下面是具体的时序图 
*               1 2     3      4
*                        ______
*          SCL: ________/      \______    
*               __ ___________________    
*          SDA: __/
*******************************************************************
*/
static void I2CSendNack(void)
{
	SET_I2C_SDA();          // 2#数据线SDA输出高电平
	DelayNus(4);            // 延时4us
	SET_I2C_SCL();          // 3#时钟线SCL输出高电平，在SCL上升沿前就要把SDA拉高，为非应答信号
	DelayNus(4);            // 延时4us
	CLR_I2C_SCL();          // 4#时钟线SCL输出低电平
	DelayNus(4);            // 延时4us
}

#endif




/**
*******************************************************************
* @function 配置触摸芯片的设备地址 0x28/0x29
* @param    
* @return                                                        
*******************************************************************
*/
static void ConfigDevAddr(void)            
{
//	gpio_bit_reset(GPIOB, GPIO_PIN_9);    	// RST拉低，复位GT911

	
		GPIO_InitTypeDef GPIO_InitStructure;

	/*配置 INT引脚，下拉推挽输出，方便初始化 */   
	GPIO_InitStructure.Pin = GTP_INT_GPIO_PIN ;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull  = GPIO_PULLDOWN;       //设置为下拉，方便初始化
	HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GTP_RST_GPIO_PIN ;
	HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);

	 CLR_RST_IO();
	CLR_INT_IO();
	DelayNms(1);  	// 延时1毫秒
	SET_INT_IO();
	DelayNms(1);  	// 延时1毫秒
 SET_RST_IO();
	DelayNms(10);  	// 延时10毫秒
	GPIO_InitStructure.Pin = GTP_INT_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);


}




#if !SOFT_IIC
extern I2C_HandleTypeDef  I2C_Handle; 


void I2Cx_Reset(I2C_HandleTypeDef  I2Cx_Handle)
{
    // 关闭 I2C
    __HAL_I2C_DISABLE(&I2Cx_Handle);

    // 复位 I2C2 外设
    GTP_RCC_II2C_FORCE_RESET();
    delay_ms(2);
    GTP_RCC_II2C_RELEASE_RESET();

    // 重新初始化
    HAL_I2C_Init(&I2Cx_Handle);
}

bool WriteTouchReg(uint16_t reg, uint8_t *pBuffer, uint8_t numToWrite){

	uint16_t i;
	i=HAL_I2C_Mem_Write(&I2C_Handle,TOUCH_DEV_ADDR,reg,2,pBuffer,numToWrite,1000);
if(i==0){
return true;
}
SYSTEM_DEBUG(" HAL_I2C_Mem_Write error,write again");
I2C_Recover_Bus_Stuck(&I2C_Handle);
	i=HAL_I2C_Mem_Write(&I2C_Handle,TOUCH_DEV_ADDR,reg,2,pBuffer,numToWrite,1000);
if(i==0){
return true;
}
SYSTEM_DEBUG(" HAL_I2C_Mem_Write error");
return false;
}

 bool ReadTouchReg(uint16_t reg, uint8_t *pBuffer, uint8_t numToRead)
{	
//HAL_I2C_ResetBus(&hi2c1);
	uint16_t i;
	static uint32_t read_count = 0;
read_count++;
	        if (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
        {
            I2Cx_Reset(I2C_Handle);
					
					
        }
	i=HAL_I2C_Mem_Read(&I2C_Handle,TOUCH_DEV_ADDR,reg,2,pBuffer,numToRead,1000);
if(i==0){
return true;
}
SYSTEM_BIGNUM_DEC(1,read_count,"  HAL_I2C_Mem_Read error,read again ");
read_count=0;
//SYSTEM_DEBUG(" HAL_I2C_Mem_Read error，read again");
I2C_Recover_Bus_Stuck(&I2C_Handle);
	i=HAL_I2C_Mem_Read(&I2C_Handle,TOUCH_DEV_ADDR,reg,2,pBuffer,numToRead,1000);
if(i==0){
	
return true;
}
SYSTEM_DEBUG(" HAL_I2C_Mem_Read error");
return false;

}
#else

/**
*******************************************************************
* @function 向触摸芯片的寄存器写入数据
* @param    reg,寄存器地址
* @param    pBuffer,数组首地址
* @param    numToWrite,要写入的数据个数,不大于256
* @return   写入是否成功                                                      
*******************************************************************
*/
static bool WriteTouchReg(uint16_t reg, uint8_t *pBuffer, uint8_t numToWrite)
{
    I2CStart();

    I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_WR);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}

    I2CSendByte((uint8_t)(reg >> 8) & 0xFF);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}
    I2CSendByte((uint8_t)reg & 0xFF);
    if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}

    for (uint8_t i = 0; i < numToWrite; i++)
    {
        I2CSendByte(pBuffer[i]);
        if (!I2CWaitAck())
		{
			goto i2c_err;	// 器件无应答
		}
    }

	I2CStop();
	return true;

i2c_err:          // 命令执行失败后，要发送停止信号，避免影响I2C总线上其他设备
	I2CStop();
		
	return false;
}

/**
*******************************************************************
* @function 指定地址开始读出指定个数的数据
* @param    readAddr,读取地址，0~255
* @param    pBuffer,数组首地址
* @param    numToRead,要读出的数据个数,不大于256
* @return   
*******************************************************************
*/
static bool ReadTouchReg(uint16_t reg, uint8_t *pBuffer, uint8_t numToRead)
{	
	I2CStart();                       				  // 发送起始信号
	
	I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_WR);      // 发送器件地址和读写模式
	if (!I2CWaitAck())                                 // 等待应答
	{
		goto i2c_err;
	}
	
    I2CSendByte((uint8_t)(reg >> 8) & 0xFF);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}
    I2CSendByte((uint8_t)reg & 0xFF);
    if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}
				 
	I2CStart();                                        // 发送起始信号          
	I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_RD);      // 发送器件地址和读写模式      
	if (!I2CWaitAck())                                 // 等待应答
	{
		goto i2c_err;
	}
	
	numToRead--;	
	while(numToRead--)                                // 数据未读完
	{
		*pBuffer++ = I2CReadByte();                   // 逐字节读出存放到数据数组
		I2CSendAck(); 
	}
	*pBuffer = I2CReadByte();                         // 最后一个字节发送非应答
	I2CSendNack();  
	
	I2CStop(); 
	return true;
	
i2c_err:
	I2CStop(); 
	return false;
}
#endif

void TouchDrvInit(void)
{
		I2C_Touch_Init();

	uint8_t id[5];
	if (ReadTouchReg(GT911_PID_REG, id, 4))
	{
		id[4] = '\0';
		SYSTEM_INFO("touch success--Touch ID: %s\n", id);
		return;
	}
	SYSTEM_INFO("Touch init error\n");
}
#if 0
void TouchScan(TouchInfo_t *touchInfo)
{
	uint8_t statRegVal;
	uint8_t buff[6];
		
	if (!ReadTouchReg(GT911_STATUS_REG, &statRegVal, 1))
	{
		printf("read GT911_STATUS_REG error\n");
		touchInfo->state = UP;
		return;
	}
	
	if ((statRegVal & 0x80) == 0)  // 读取最高位查看是否有按下
	{
		printf("It has not been touched\n");
		touchInfo->state = UP;
		return;
	}
	
	uint8_t touchNums = statRegVal & 0x0F;
    printf("It has been touched, touch nums = %d\n", touchNums);
	
	statRegVal = 0;
	WriteTouchReg(GT911_STATUS_REG, &statRegVal, 1); // 清除数据标志位
	
	if (touchNums == 0 || touchNums > TOUCH_POINT_MAX) 
	{
		touchInfo->state = UP;
		return;
	}
	
	ReadTouchReg(GT911_TP1_REG, buff, 6);	// 读出触摸点x y坐标和面积
	touchInfo->point.x = (uint16_t)(buff[1] << 8) | buff[0];
	touchInfo->point.y = (uint16_t)(buff[3] << 8) | buff[2];
	touchInfo->point.size = (uint16_t)(buff[5] << 8) | buff[4];
//	printf("point[%d].x = %d, point[%d].y = %d, point[%d].size = %d\n", \
//			0, touchInfo->point.x, 0, touchInfo->point.y, 0, touchInfo->point.size);
		
	touchInfo->state = DOWN;
	return;	
}
#endif


void TouchScan(TouchInfo_t *touchInfo)
{
	static TouchInfo_t lastTouchInfo = {UP};
	static uint64_t lastSysTime = 0;
	static uint32_t TouchScanNUM=0;
	if ((GetSysRunTime() - lastSysTime) < DETECT_INTERVAL_TIME)  // 判断两次调用接口函数的时间间隔是否小于DETECT_INTERVAL_TIME ms
	{
		*touchInfo = lastTouchInfo;
//		printf("xxx1");
		return;
	}
	lastSysTime = GetSysRunTime();

	uint8_t statRegVal;
	uint8_t buff[6];
		TouchScanNUM++;
	if (!ReadTouchReg(GT911_STATUS_REG, &statRegVal, 1))
	{
		SYSTEM_INFO("read GT911_STATUS_REG error\n");
		SYSTEM_DEBUG("--%d \r\n",TouchScanNUM);
		touchInfo->state = UP;
		lastTouchInfo = *touchInfo;
		return;
	}

	if ((statRegVal & 0x80) == 0)  // 读取最高位查看是否有按下
	{
		//printf("It has not been touched\n");
		touchInfo->state = UP;
		lastTouchInfo = *touchInfo;
//			SYSTEM_INFO("2--%d \r\n",TouchScanNUM);
		return;
	}

	uint8_t touchNums = statRegVal & 0x0F;
//	printf("touch nums = %d\n", touchNums);
	
	statRegVal = 0;
	WriteTouchReg(GT911_STATUS_REG, &statRegVal, 1);//清除数据标志位
	
	if (touchNums == 0 || touchNums > TOUCH_POINT_MAX) 
	{
		touchInfo->state = UP;
		lastTouchInfo = *touchInfo;
//			SYSTEM_INFO("3-%d \r\n",TouchScanNUM);
		return;
	}
	
	ReadTouchReg(GT911_TP1_REG, buff, 6);	// 读出触摸点x y坐标和面积
	touchInfo->point.x = (uint16_t)(buff[1] << 8) | buff[0];
	touchInfo->point.y = (uint16_t)(buff[3] << 8) | buff[2];
	touchInfo->point.size = (uint16_t)(buff[5] << 8) | buff[4];
	//printf("point[%d].x = %d, point[%d].y = %d, point[%d].size = %d\n", \
			0, touchInfo->point.x, 0, touchInfo->point.y, 0, touchInfo->point.size);
		
	touchInfo->state = DOWN;
	lastTouchInfo = *touchInfo;
//	SYSTEM_INFO("4--%d \r\n",TouchScanNUM);
	return;
}



TouchInfo_t touchInfo2;  // 用于存储触摸信息
void touch_drv_while_test(void){
	
         TouchScan(&touchInfo2);

    if (touchInfo2.state == DOWN) 
    {
        // 触摸事件发生
        SYSTEM_INFO("Touch at (%d, %d), size: %d\n", touchInfo2.point.x, touchInfo2.point.y, touchInfo2.point.size);
    }
    else 
    {
        // 没有触摸
        SYSTEM_INFO("No touch detected.\n");
    }


}





