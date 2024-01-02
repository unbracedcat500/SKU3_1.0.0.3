/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include <Initialize.h>
#include <Lira_Sensor.h>
#include <Euro_Valve.h>
#include <MQTT_Handler.h>
#include <GSM_GPRS_Network.h>


//#include <RHReliableDatagram.h>
//#include <TimedAction.h>
//#include <SPI.h>

// Declared weak in Arduino.h to allow user redefinitions.
//int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

int main(void)
{
	init();

	initVariant();

#if defined(USBCON)
	USBDevice.attach();
#endif


   /****************************************** started user program *********************************************/
 
	static uint32_t tickcount;
	static unsigned char timer_count,i;
	i = 0;
	
	Initialise_Hardware();
	
	Param_Initialization();
	
	if(_sRuble_Parameters.SMS_Alert_Start == 0x08)
	{
		//_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"RUBLE RESTART");
	}
	
	while (_kSERAIL_AT_CHECK())
	{
		_kSERIAL_AT_READ();
	}
	
	Get_Local_Time();
	Check_GSM_Signal();
	
	Start_Display_In_Interrupt = 1;
   _sRuble_Parameters.Display_Refresh = _kSET;
   
	//Display_Update_1();    
	tickcount = g_uTick;
	_sRuble_Parameters.Line_Fault_Flag = _kRESET;
	_sRuble_Parameters.Pump_On_Flag =_ePUMP_OFF;
	
	Valve_Grouping_Handler(_kFORCEFULYY_OFF);
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("NOMINAL CURRENT : ");
		_kSERIAL_MON_PRINT_NUM(_sPump_Parameter.Pump_Nominal_Current);
		_kSERIAL_MON_CLEAR();
	}
	
	_sSchedules.Scheduler_Retry_Timer = _kRESET;
	_sRuble_Parameters.Wireless_Pump_Operate_Timer = _kMAX_TIME_TO_MONITOR_WIRELESS_PUMP;
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("Days in between : ");
		_kSERIAL_MON_PRINT_NUM(_sSchedules.Days_In_Between_Schedules);
		_kSERIAL_MON_CLEAR();
	}
	//Turn_Off_All_Pump_For_Safety();
	
	_sSchedules.Shedule_Run_Status_Is_Triggered  = _kRESET;
	_sRuble_Parameters.Immediately_Turn_Off_Pump = _kRESET;
	_sRuble_Parameters.Wireless_Pump_Command     = _kRESET;
	_sRuble_Parameters.Electricity_Status        = _kON;
	
	Select_Operation_Mode();
	
	//_sRuble_Parameters.By_Pass_CT = _kRESET;
	//_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status         = _kRESET;
	_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation = _kRESET;
	_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status  = _kRESET;
	_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status = _eSTOP_EXTRA_PLOT_OPERATION;
	
	_sExtra_Plot[(_kBLOWER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status = _kRESET;
	_sExtra_Plot[(_kBLOWER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status = _kSCHEDULER_OFF;
	
	_sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status = _kRESET;
	_sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation = _kRESET;
	_sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status = _kRESET;
	_sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Publish_status = _kRESET;
	
	
	
	delay(5000);
	Get_Local_Time();
	
// 	wdt_disable();  /* Disable the watchdog and wait for more than 2 seconds */
// 	delay(3000);  /* Done so that the Arduino doesn't keep resetting infinitely in case of wrong configuration */
// 	wdt_enable(WDTO_8S);
	//wdt_reset();
	
	unsigned int temp;
	if(_sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[0] !=0 &&
	(_sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Extra_Plot_Number == _kFLUSH_PLOT_NUMBER))
	{
		//_sRuble_Parameters.Pump_Run_Timer_for_flush = _kRESET;
		temp =  _sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[0];
		_sSchedules.Flush_Schedule_In_Every = temp *3600;
		temp = _sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[0];
		_sSchedules.Flush_Schedule_For = temp * 60;
	}
	else
	{
		_sSchedules.Flush_Schedule_In_Every = 0;
		_sSchedules.Flush_Schedule_For = 0;
	}
	
	_gStart_Wdt = _kSET;
	_sSchedules.Pump_On_In_Scheduler = _kRESET;
	
	while (1)
	{
		if(_g_1Sec_Time)
		{
			_g_1Sec_Time = _kRESET;
			tickcount = g_uTick;
			timer_count++;
			
			_gWdt_Counter = 0;
			
			Publish_Data();
			//Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_ePUBLSIH_DATA);
			
			Lora_Operation();
			
			Run_Extra_Plot_Operation();	
			
			Run_Operation();             /* Run the operation as per the selected mode */
			
		   /* read strength of GSM signal and update the display update variable */
			
			Display_Update_1();
			
			Keeps_Monitor_Server_Connection();	
			
			if(timer_count > 60)           /* update in every 1 minute */
			{
				timer_count = 0;
				Get_Local_Time();
				Hard_Coded_Flush_Valve(_kON);	
			}
			
			Wireless_Motor_Enable_Disable_Check();  /* changed */
			
			Handle_Wireless_Pump();
			
			Plot_Status_List_Monitor();
			
			Pump_Status_List_Monitor();
			
			Pump_Run_Monitoring();
			
			Flush_Controlling();
			
			Debug_Check();
			
		}	
		
		if(Subscribed_data_check())
		{
			if(Seperate_Out_Sub_Data())
			{
				Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_eSUBSCRIBE_DATA);
				//mqttCallback((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff));
			}
		}
		
		Calibration_Of_Pump();
		
		Select_Operation_Mode();
		
		Update_Indication();
		
		Faults_Handler();
			
	}
        
	return 0;
	
}


void Testing_Function(void)
{
	/********************************* Test GSM Nodule ******************************/
	
	/********************************* Test local Time ******************************/
	
	/********************************* Test Pump Relay ******************************/
	
	/********************************* Test valve Relays ****************************/
	
	/********************************* Test lora module *****************************/
	
	/********************************* Add 2 plots **********************************/
	
	/********************************* Check Timer **********************************/
}

