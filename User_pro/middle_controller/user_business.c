#include "./middle_controller/user_business.h"

#include "./sys/bsp_systime.h"   




#include "./io/bsp_io_output.h"   

#include "./DataConvert/data_verification.h"
//#include "./converter/data_convert.h"
#include <stdlib.h>

 #include "./sys/sysio.h"
#include "string.h"

#include "./usart/bsp_usart_COM485.h" 
#include "./sys/systick.h"
#include "./usart/bsp_usart.h"

#include "./pro_com/usart_485_232_send.h"

//#include "./middle_business/usart_485_address.h"
//#include "./middle_controller/user_controler.h"


#if 0

void vol_to_standard_auto_stop(void){

if(GV_BatteryState.charge_begin==EM_Charge){
if(GV_BatteryState.d_current_bat_vol>=GV_BatteryState.set_bat_upperLimit_vol){

	SYSTEM_INFO("EM_Charge  stop\r\n");
	
	StopMachine();
		StopBeep_bySeconds(4000);
}

}else if(GV_BatteryState.charge_begin==EM_Discharge){
if(GV_BatteryState.d_current_bat_vol<=GV_BatteryState.set_bat_lowerLimit_vol){

	SYSTEM_INFO("EM_Discharge  stop\r\n");
	
	StopMachine();
	StopBeep_bySeconds(4000);
}


}


}


#endif






























