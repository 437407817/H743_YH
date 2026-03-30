

#include "./global/GV_variable.h" 






STR_GET_VOL_Data_t GV_get_vol_real_data;

STR_GET_VOL_Data_t GV_get_vol_485_232_Bigdata;

STR_SEND_RUN_DATA_t GV_send_run_state_data;

STR_SEND_RUN_DATA_t GV_send_run_state_bigdata;


STR_SEND_SETTING_DATA_t gv_send_setting_data_t;

////GV_HardwareState.HW_NeedWorkingState=0;
//uint8_t checkNeedStateNotEquRealState(void){

//	
//	
//	if(GV_HardwareState.HW_NowRealWorkingState!=GV_HardwareState.HW_NeedWorkingState){
//	return 1;
//	}else{
//	return 0;
//	}

//}

//#include "./Converter/data_find.h"

//void Setboadnumfun(void){

////	uint16_t tmp_hard_board_count;
//	GV_HardwareState.hard_board_whichIsActiveBit=0;
//	
//	
//for(uint16_t i=0;i<BOARD_COUNT;i++)
//{
//	if(GV_ScreenSettingDataState.Set_Bat_BattNum[i]!=0){
////	tmp_hard_board_count=i+1;
//		
//	GV_HardwareState.hard_board_whichIsActiveBit |= (1 << i);
//	}
//	
//	if((GV_ScreenSettingDataState.Set_Bat_SingleBattRun[i]&0x7fff)==0){
//		GV_CommSendToSlaveBoardState.S_Run[i]=0;	
//	};
//}


//GV_HardwareState.hard_board_HightActiveBitNum=findHighestSetBit(GV_HardwareState.hard_board_whichIsActiveBit);
//SYSTEM_DEBUG_BINARY(GV_HardwareState.hard_board_whichIsActiveBit,"");

//}




////如果进入该函数，代表从机规定时间没有返回数据
//void AskUploadDataovertimefun(void){//如果发送1号板询问数据，超时时间达到，但没有接收1号板数据，则触发该函数

////GV_HardwareState.hard_board_nowActive
//uint8_t i;
////	uint8_t  top_board_count;
////	
////	top_board_count=GV_HardwareState.hard_1board_count+1
//	
//    for ( i = 0; i < GV_HardwareState.hard_board_count; i++) {

//        if ((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0 && i > (GV_HardwareState.hard_board_nowActive-1)) {//这里GV_HardwareState.hard_board_nowActive是从2开始，所以i必定从2开始
//					
////          GV_ComuBoardState.check_overtime_askdata_timesflag[GV_HardwareState.hard_board_nowActive-1]+=1;
//					
//					GV_HardwareState.hard_board_nowActive = i+1;//如果2没有，则跳到3
//					
//					break;
//        }

//				if(i==GV_HardwareState.hard_board_count-1){//最后1次没有置位激活
//				GV_HardwareState.hard_board_nowActive = GV_HardwareState.hard_board_count+1;
//				
//				}
//    }
//	
//	
//		
//		GV_CommSlaveAskState.A_DataType=0x00;
//		
//		
//		
//	if( GV_HardwareState.hard_board_nowActive > GV_HardwareState.hard_board_count ){
//	
//	GV_HardwareState.hard_board_nowActive =0;
//		GV_CommSlaveAskState.A_Addr=0xfd;
//		
//		c485_write_cmd(&GV_CommSlaveAskState.A_Addr, sizeof(GV_CommSlaveAskState), CTRL_CODE_ASK,1);
//		
//		
//	SuspendTaskHandler(&AskUploadDataovertimefun);
//		
//	}else{
//		GV_CommSlaveAskState.A_Addr=GV_HardwareState.hard_board_nowActive;
//		
//		c485_write_cmd(&GV_CommSlaveAskState.A_Addr, sizeof(GV_CommSlaveAskState), CTRL_CODE_ASK,1);
//	ResumeTaskHandler(&AskUploadDataovertimefun);//刷新计时
//	}
//	

//	
//}



//void clearScreenDataVol(void){
//	uint8_t i;
//for ( i = 0; i < GV_HardwareState.hard_board_count; i++) {
//if ((GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) == 0){


//memset(&GV_ComuBoardState.getSlaveUpdloadState[i].BS_Vol,0,106);
//}


//}



//}





/*********************************************END OF FILE**********************/
