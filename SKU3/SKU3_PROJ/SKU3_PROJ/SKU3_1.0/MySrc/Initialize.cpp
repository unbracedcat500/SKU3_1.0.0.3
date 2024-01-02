/*
 * Initialize.cpp
 *
 * Created: 29-09-2021 18:37:56
 *  Author: Tushar (SIO)
 */ 
#include <Initialize.h>
#include <Lira_Sensor.h>
#include <Euro_Valve.h>
#include <MQTT_Handler.h>
#include <GSM_GPRS_Network.h>

EnergyMonitor RUBLE_PUMP_CURRENT;

unsigned int _gTime_To_Wait_Lora_Resp;
unsigned int _gWait_Blower_Time;
unsigned int _gfault_Occure;
unsigned char _gRunning_Flush_Num;
volatile unsigned int _gWdt_Counter;
volatile unsigned char _gStart_Wdt;
volatile unsigned char _gTank_Topic_check;
volatile unsigned char plot_schedule_topic;


void Param_Initialization(void)
{
	char temp = 0;
	unsigned int i;
	
	sprintf((char *)_sRuble_Parameters.Ruble_ID,"%s",_kRUBLE_ID);       /* Update the Ruble ID */
	_kEEPROM_WRITE(_kEEPROM_LOC_RUBLE_ID,_sRuble_Parameters.Ruble_ID);  /* save the ruble id */
	_kSERIAL_MON_CLEAR();
	
	_kSERIAL_MON_PRINT_NUM();
	for(i=0;i<6;i++)
	{
		_kSERIAL_MON_WRITE(_sRuble_Parameters.Ruble_ID[i]);    /* print Ruble ID on serial monitor */
	}
	_kSERIAL_MON_PRINT_NUM();
	
	Store_MQTT_Pub_Topics();                        /* store all publish topics */
	
	Store_MQTT_Sub_Topics();                        /* store all subscribe topics */
	
	Store_All_APN();                                /* store all operating network's APN */
	
	
	_kEEPROM_READ(_kEEPROM_LOC_READ_REGISTRATION,temp);                     /* read that ruble is registered or not */

	if(temp == _kCHECK_REGISTRATION)  /* if _kEEPROM_LOC_READ_REGISTRATION location filled with 0x09 it means that ruble controller is already register */
	{
		_sRuble_Parameters.Ruble_Registration_Status = _eRUBLE_REGISTERED; /* update that ruble is already registered */
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{	
			_kSERAIL_MON_WRITE("Ruble is already Registered To Sever");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
	}
	else
	{
		_sRuble_Parameters.Ruble_Registration_Status = _eRUBLE_NOT_REGISTERED;  /* set that ruble is not register to server */
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Ruble is not Registered To Sever");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		/****************************** Reset all the epprom location *************************************************/
		Dummy_Data_Store();
	}
	
	/********************************** Check gsm fault to init the gsm/gprs network and init broker **************************/
	_sRuble_Parameters.Sim_Check = _eSIM_INSERTED;   /* initially set sim is inserted, then we will check sim status using  AT command */
	
	
	Local_Time_Setting();
	
	
	if(_sRuble_Parameters.Ruble_Registration_Status != _eRUBLE_REGISTERED)
	{
		if(_sRuble_Parameters.Sim_Check == _eSIM_NOT_INSERTED)
		{
			_kLCD_CLEAR();
			_kLCD_BUFF_WRITE("FAULT: SIM",10,0,5);        /* print company name */
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("Please Insert Sim Card");  /* for debugging purpose */
				_kSERIAL_MON_CLEAR();
			}
			while(1);
		}
	}
	
	if(!(_sRuble_Parameters.Hardware_Faults & _eGSM_INIT_FAIL))
	{
		_sRuble_Parameters.Display_Update |= _eSEARCHING_FOR_NETWORK;
		Display_Update_1();
		Init_Broker();          /* if gsm initialize successfully then call the init_broker() function */
	}
	else
	{
		if(_sRuble_Parameters.Ruble_Registration_Status != _eRUBLE_REGISTERED)
		{
			_kLCD_CLEAR();
			_kLCD_BUFF_WRITE("FAULT: GSM",10,0,5);        /* print company name */
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("NOT REGISTER AND ALSO MODULE NOT WORK, PLEASE CHECK THE MODULE AND RESTART THE DEVICE");  /* for debugging purpose */
				_kSERIAL_MON_CLEAR();
			}
			while(1);
		}
		else
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("GSM AND MQTT WILL NOT WORK");                /* for debugging purpose */
				_kSERIAL_MON_CLEAR();
			}
		}
	}
	
	Read_EEPROM();                        /* read all the data which is stored in the EEPROM till now */
	
	/* MAKE PACKET ID INITAILLY 0 */
	
// 	for(i=0; i<_kMAX_LIRA_SENSOR_NODE; i++)
// 	{
// 		_sLira_Node_Param[i].Packet_ID = 0;
// 	}
	
	/******************************************************* Read the pump Nominal current **********************************/
	
	/****************************************************** Turn Off all valves and pumps fisrt *****************************/
	
	/********************************************************** copy the valve ids into the valve id ist **************************************/
	for(i=0;i<_kMAX_EURO_VALVE;i++)
	{
		sprintf((char*)_sRuble_Parameters.List_Of_Valve_ID[i],"%s",_sEuro_Valve_Param[i].Euro_Valve_ID);
		
		if(_sEuro_Valve_Param[i].Euro_Valve_Number)
		{
			/********************** make all valve ONLINE first *********************************/
			_sEuro_Valve_Param[i].Euro_Valve_Online = _kVALVE_ONLINE;
		}
	}
	
	
	/****************************************** set the operation mode if Read EEPROM function is not working *************************/
	switch(_sRuble_Parameters.Operation_Mode)
	{
		case _eOPER_MOD_AUTO:
		case _eOPER_MOD_TIMER:
		case _eOPER_MOD_MANUAL:
		case _eOPER_MOD_DATALOG:
		case _eOPER_MOD_SCHEDULER:
		{
			
		}break;
		default:
		{
			_sRuble_Parameters.Manual_Mode_Wait = 0;
			_sRuble_Parameters.Operation_Mode   = _eOPER_MOD_SCHEDULER;  // 5/5
			_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
			_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
		}break;
	}
	
	/****************************************** set the CT calibration ************************************************/
	RUBLE_PUMP_CURRENT.current(_kADC_PUMP_CUR, _kCT_CALIBRATION_FACTOR);
	
}


void ADC_Operation(void)
{
// 	char i=0,j=0;
// 	static unsigned char array_size,filter_len;
// 	static unsigned int mediun_battvol[_kMEDIUN_FILTER_LEN],mediun_pumpcur[_kMEDIUN_FILTER_LEN],filter_data,filter_data1,filter_data2,tickcount;
// 	static unsigned char temp_0[_kMOVING_AVG_FRAME_SIZE],temp_1[_kMOVING_AVG_FRAME_SIZE];
// 	
// 	if((g_uTick - tickcount) > 12)   /* update in every 12 msec */
// 	{
// 		tickcount = g_uTick;
// 		
// 		/********************************************* added the mediun filter ***********************************/
// 		
// 		if(array_size < _kMEDIUN_FILTER_LEN)
// 		{
// 			mediun_battvol[array_size] = _kADC_READ(_kADC_BATT_VOL);
// 			mediun_pumpcur[array_size] = _kADC_READ(_kADC_PUMP_CUR);
// 			array_size++;
// 		}
// 		else
// 		{
// 			array_size = 0;
// 			
// 			for (i = 0; i < _kMEDIUN_FILTER_LEN; i++)
// 			{
// 				for (j = i + 1; j < _kMEDIUN_FILTER_LEN; j++)
// 				{
// 					if (mediun_battvol[i] > mediun_battvol[j])
// 					{
// 						filter_data = mediun_battvol[i];
// 						mediun_battvol[i] = mediun_battvol[j];
// 						mediun_battvol[j] = filter_data;
// 					}
// 					
// 					if (mediun_pumpcur[i] > mediun_pumpcur[j])
// 					{
// 						filter_data = mediun_pumpcur[i];
// 						mediun_pumpcur[i] = mediun_pumpcur[j];
// 						mediun_pumpcur[j] = filter_data;
// 					}
// 					
// 				}
// 			}
// 			
// 			filter_data  = 0;
// 			filter_data1 = 0;
// 			filter_data2 = 0;
// 			
// 			//for(i = _kMEDIUM_AVG_LEN; i < (_kMEDIUN_FILTER_LEN - _kMEDIUM_AVG_LEN);i++)
// 			for(i = 11; i < 18; i++)
// 			{
// 				filter_data1 +=  mediun_battvol[i];
// 				filter_data2 +=  mediun_pumpcur[i];
// 			}
// 			//filter_data1 = 	filter_data1/_kMEDIUM_AVG_LEN;
// 			//filter_data2 = 	filter_data2/_kMEDIUM_AVG_LEN;
// 			filter_data1 = 	filter_data1/7;
// 			filter_data2 = 	filter_data2/7;
// 			filter_data = 0;
// 		}
// 		
// 		/******************************************************* added the moving average ******************************/
// 		
// 		if(filter_len < _kMOVING_AVG_FRAME_SIZE)
// 		{
// 			temp_0[filter_len] = 	filter_data1;
// 			temp_1[filter_len] = 	filter_data2;
// 			filter_len++;
// 		}
// 		else
// 		{
// 			temp_0[_kMOVING_AVG_FRAME_SIZE - 1] = filter_data1 ;
// 			temp_1[_kMOVING_AVG_FRAME_SIZE - 1] = filter_data2 ;
// 			
// 			for(j=0; j< (_kMOVING_AVG_FRAME_SIZE-1); j++) /* start from 0 to max size of the frame - 2 because its start from 0 and we just want to store data here less than max size */
// 			{
// 				temp_0[j] = temp_0[j+1];                   /* store the value of the next location of array into the current location */
// 				_sRuble_Parameters.ADC_Filter_Data[0] += temp_0[j];         /* add the data to the respected location for of the filter array */
// 				
// 				temp_1[j] = temp_1[j+1];                   /* store the value of the next location of array into the current location */
// 				_sRuble_Parameters.ADC_Filter_Data[1] += temp_1[j];         /* add the data to the respected location for of the filter array */
// 			}
// 
// 			_sRuble_Parameters.ADC_Filter_Data[0] += temp_0[_kMOVING_AVG_FRAME_SIZE - 1];
// 			_sRuble_Parameters.ADC_Filter_Data[1] += temp_1[_kMOVING_AVG_FRAME_SIZE - 1];
// 			
// 			
// 			_sRuble_Parameters.Ruble_Battery_Level = (_sRuble_Parameters.ADC_Filter_Data[0]/(_kMOVING_AVG_FRAME_SIZE));         /* battery voltage */
// 			_sPump_Parameter.Pump_Run_Current      = (_sRuble_Parameters.ADC_Filter_Data[1]/(_kMOVING_AVG_FRAME_SIZE));         /* pump current */
// 
// 			_sRuble_Parameters.ADC_Filter_Data[0]= _kRESET;      /* reset the filter data for next cycle */
// 			_sRuble_Parameters.ADC_Filter_Data[1]= _kRESET;      /* reset the filter data for next cycle */
// 		}
// 		
// 	}
	
}


void Read_EEPROM(void)
{
	unsigned char temp_data[50],count=0,i=0,j=0;
	
	/*********************************** Read the pump nominal current *******************************/
	_kEEPROM_READ(_kEEPROM_LOC_PUMP_NOMINAL_CURRENT,_sPump_Parameter.Pump_Nominal_Current);  /* read that ruble is registered or not */
	
	/************************** Read the RUBLE ID **************************/
	_kEEPROM_READ(_kEEPROM_LOC_RUBLE_ID,_sRuble_Parameters.Ruble_ID);
	for(i=0;i<6;i++)
	{
		_kSERIAL_MON_WRITE(_sRuble_Parameters.Ruble_ID[i]);
	}
	
	/************************** Read WP value *****************************/
	//_kEEPROM_READ(_kEEPROM_LOC_WP_VALUE,_sRuble_Parameters.WP_Value_Default);
	
	
	/************************* Read FC value ******************************/
	//_kEEPROM_READ(_kEEPROM_LOC_FC_VALUE,_sRuble_Parameters.FC_Value_Deafault);
	
	
	/************************* Upadte the all lira node data **************/
	for(i=0;i<_kMAX_LIRA_SENSOR_NODE;i++)
	{
		memset(temp_data,0,sizeof(temp_data));
		count = 0;
		_kEEPROM_READ(_kEEPROM_LOC_LIRA_SENSOR_DATA(i),_sLira_Node_Param[i]);
	}
	
	/*************************** Update all euro valve data ********************************/
	
	for(i=0;i<_kMAX_EURO_VALVE;i++)
	{
		_kEEPROM_READ(_kEEPROM_LOC_EURO_VALVE_DATA(i),_sEuro_Valve_Param[i]);
	}
	
	for(i=0;i<_kMAX_EURO_VALVE;i++)
	{
		_sEuro_Valve_Param[i].Euro_Valve_Status = _kUNDEFINED;
	}
	
	/******************************************* update firmware version **************************/
	//_kEEPROM_READ(_kEEPROM_LOC_FIRMWARE_VERSION,_sRuble_Parameters.Firmware_Version);
	
	
	/******************************************* update hardware version **************************/
	//_kEEPROM_READ(_kEEPROM_LOC_HARDWARE_VESRION,_sRuble_Parameters.Hardware_Version);
	
	
	/******************************************* update operation mode **************************/
	_kEEPROM_READ(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
	
	switch(_sRuble_Parameters.Operation_Mode)
	{
		case _eOPER_MOD_AUTO:
		case _eOPER_MOD_SCHEDULER:
		{
			_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;
			_sRuble_Parameters.lndications   &= ~_eRUBL_INDI_AUTO_MANUAL;
			_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
			_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
		}break;
		
		case _eOPER_MOD_TIMER:
		{
			_sRuble_Parameters.lndications   &= ~_eRUBL_INDI_AUTO_MANUAL;
			_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
			
			_kEEPROM_READ(_kTIMER_IRRIGATING_PLOT,_sRuble_Parameters.Temp_Irrigating_Plot);
			_kEEPROM_READ(_kPUMP_REMAINING_TIME,_sRuble_Parameters.Pump_Run_Timer);
			//_sRuble_Parameters.Temp_Irrigating_Plot = _sRuble_Parameters.Irrigating_Plot;
			_sRuble_Parameters.Publish_Topic |= _ePUB_TIMER_FEEDBACK;
		}break;
		
		case _eOPER_MOD_MANUAL:
		case _eOPER_MOD_DATALOG:
		{
			_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
			_sRuble_Parameters.lndications   |= _eRUBL_INDI_AUTO_MANUAL;
		}break;
		default:
		{
			_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;
			_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
			_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
		}break;
	}
	
	/******************************************* update ruble sim no. ************************************/
	_kEEPROM_READ(_kEEPROM_LOC_RUBLE_SIM_NUM,_sNetwork_and_Usr_Info.Ruble_SIM_No);
	
	
	/******************************************* update user sim no. ************************************/
	_kEEPROM_READ(_kEEPROM_LOC_USER_SIM_NUM,_sNetwork_and_Usr_Info.User_Sim_No);
	
	
	/***************************************** Update coefficient 1 *************************************/
	//_kEEPROM_READ(_kEEPROM_LOC_COEFF_1,_sRuble_Parameters.Coeff_1);
	
	
	/***************************************** Update coefficient 2 *************************************/
	//_kEEPROM_READ(_kEEPROM_LOC_COEFF_2,_sRuble_Parameters.Coeff_2);
	
	
	/***************************************** Update coefficient 3 *************************************/
	//_kEEPROM_READ(_kEEPROM_LOC_COEFF_3,_sRuble_Parameters.Coeff_3);
	
	
	/***************************************** update the sms alert ***********************************/
	_kEEPROM_READ(_kEEPROM_SMS_ALERT_START,_sRuble_Parameters.SMS_Alert_Start);
	
	
	/**************************************** update the kill switch **********************************/
	_kEEPROM_READ(_kEEPROM_KILL_SWITCH,_sRuble_Parameters.Kill_Switch);
	
	
	/*************************************** read schedule data ******************************************/
	_kEEPROM_READ(_kEEPROM_SCHEDULE_DATA, _sSchedules);
	
	
	_sSchedules.Scheduler_Retry_Timer = _kRESET;
	_sSchedules.Fertilizer_On_Scheduler_Status = _kRESET;
	_sSchedules.New_Schedules_Occur = _kRESET;
	_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
	_sSchedules.Pump_On_In_Scheduler = _kRESET;
	_sSchedules.Save_Schedule_Data_In_Eeprom = _kRESET;
	_sSchedules.Blower_On_Scheduler_Status = _kOFF;
	
	for(unsigned char tank_number = 0; tank_number < _kMAX_FERTILIZER_TANK; tank_number++)
	{
		_sSchedules.Running_Tank[tank_number] = _kRESET;
	}
	
	
	
	
	
	
	if(_sSchedules.Schedule_Enable)   /* schedules are enable then change the mode from auto to scheduler */
	{
		switch(_sRuble_Parameters.Operation_Mode)
		{
			case _eOPER_MOD_AUTO:
			case _eOPER_MOD_SCHEDULER:
			{
				_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;
				_sRuble_Parameters.lndications     &= ~_eRUBL_INDI_AUTO_MANUAL;
				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
				_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
			}break;
		}
	}
	else    /* schedules are not enable then change the mode from scheduler to auto */
	{
		switch(_sRuble_Parameters.Operation_Mode)
		{
			case _eOPER_MOD_AUTO:
			case _eOPER_MOD_SCHEDULER:
			{
				_sRuble_Parameters.Manual_Mode_Wait = 0;
				_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;   // 5/5
				_sRuble_Parameters.lndications     &= ~_eRUBL_INDI_AUTO_MANUAL;
				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
			}break;
		}
	}
	
	
	/*********************************** update extra plot ***************************/
	
	for(i=0;i<_kMAX_EXTRA_PLOT;i++)
	{
		_kEEPROM_READ(_kEEPROM_EXTRA_PLOT_DATA(i+1),_sExtra_Plot[i]);
		
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE_NO_LN("Extra Plot Number : ");
			_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[i].Extra_Plot_Number);
			_kSERIAL_MON_CLEAR();
		}
		
		if(_sExtra_Plot[i].Run_Timer)    /* check if timer was set for the extra plot */
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE_NO_LN("Extra Plot Run Timer : ");
				_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[i].Run_Timer);
				_kSERIAL_MON_CLEAR();
			}
			_sExtra_Plot[i].Set_Through_Timer = _eSET_THROUGH_TIMER;
		}
		
		if(!_sExtra_Plot[i].Run_Timer &&  !_sExtra_Plot[i].One_Touch)   /* if user not took any action earlier */
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE_NO_LN("Extra not Plot Run Timer : ");
				_kSERIAL_MON_CLEAR();
			}
			_sExtra_Plot[i].Status = _eSTOP_EXTRA_PLOT_OPERATION;
			_sExtra_Plot[i].Set_Through_Timer = _kRESET;
		}
		
		_sExtra_Plot[i].On_Off_Status = _kRESET;   /* initially off the status */
		
		switch(_sExtra_Plot[i].Status)
		{
			case _eSTART_EXTRA_PLOT_OPERATION:
			{
				
			}break;
			
			case _eRUN_EXTRA_PLOT_OPERATION:
			{
				_sExtra_Plot[i].Status = _eSTART_EXTRA_PLOT_OPERATION;
			}break;
			
			default:
			{
				if(i < 2 || i > 9)
				{
					_sExtra_Plot[i].Status = _kSCHEDULER_OFF;
					
				}
				else
				{
					_sExtra_Plot[i].Status = _eSTOP_EXTRA_PLOT_OPERATION;
				}
				//_sExtra_Plot[i].Status = _kSCHEDULER_OFF;
			}break;
		}
		
		for(int k=0;k<_kMAX_VALVE_FOR_1PLOT;k++)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				Serial.println();
				_kSERAIL_MON_WRITE_NO_LN("VALVE NO : ");
				_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[i].Euro_Valve_Group[k]);
				_kSERIAL_MON_CLEAR();
			}
		}
	}
	
	
	
	_kEEPROM_READ(_kCT_BY_PASS,_sRuble_Parameters.By_Pass_CT);
	
	_kEEPROM_READ(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
	
	_kEEPROM_READ(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
	
	_kEEPROM_READ(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
	
	_kEEPROM_READ(_kPUMP_SELECTED_FOR_IRRIGATION,_sRuble_Parameters.Pump_Selected_For_Irrigation);
	
	if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kRESET || _sRuble_Parameters.Pump_Selected_For_Irrigation > _kMAX_WIRELESS_PUMP+1)
	{
		_sRuble_Parameters.Pump_Selected_For_Irrigation = _kWIRED_PUMP_NUMBER;
		_kEEPROM_WRITE(_kPUMP_SELECTED_FOR_IRRIGATION,_sRuble_Parameters.Pump_Selected_For_Irrigation);
	}
	
	_kEEPROM_READ(_kFLUSH_RUN_BACK_UP, _sRuble_Parameters.Pump_Run_Timer_for_flush);
	
	for(unsigned char wl_pump_num=0; wl_pump_num < _kMAX_WIRELESS_PUMP; wl_pump_num++)
	{
		_kEEPROM_READ(_kWIRELESS_PUMP_DATA_SAVE(wl_pump_num), _sWireless_Pump_Param[wl_pump_num]);
	}
	
	_kEEPROM_READ(_kRUNNING_FLUSH_NUMBER, _gRunning_Flush_Num);
	
}


int curveFit(int x)
{
	
	//double y = (double)((double)((double)((double)(_sRuble_Parameters.Coeff_1))*(double)(pow(x,2))) + (double)((double)(_sRuble_Parameters.Coeff_2) * x) + (double)_sRuble_Parameters.Coeff_3);
// 	if(y < 0)
// 	{
// 		y = _kSM_SENSOR_ERROR;        //Error code for sensor in the air(removed from soil)
// 	}
// 	return y;
	
// 	_kSERAIL_MON_WRITE_NO_LN("ADC READING IS ");
// 	_kSERIAL_MON_PRINT_NUM(x);
// 	
// 	double y = (double)((double)((double)((double)(_sRuble_Parameters.Coeff_1))*(double)(pow(x,2))) + (double)((double)(_sRuble_Parameters.Coeff_2) * x) + (double)_sRuble_Parameters.Coeff_3);
// 	
// 	_kSERAIL_MON_WRITE_NO_LN("DATA AFTER COEEF SETTING ");
// 	_kSERIAL_MON_PRINT_NUM(y);
// 	
// 	if(y < 0)
// 	{
// 		y = _kSM_SENSOR_ERROR;        //Error code for sensor in the air(removed from soil)
// 		return y;
// 	}
// 	else
// 	{
// 		y = double((double)2.365 * (double)y);
// 		return y;
// 	}
	
}


int Calibration_Of_Pump(void)
{
	unsigned long previous,timeout;
	unsigned char answer =0;
	float filter_data[5],filter_data_1;
	char i,j;
	
	timeout = 10000;
	
	//_sRuble_Parameters.Do_Calibration = _kSET;
	
	
	
	
	
	if(_sRuble_Parameters.Do_Calibration == _kSET)
	{
		_sRuble_Parameters.Do_Calibration = _kRESET;
		
		if(_sRuble_Parameters.Pump_Selected_For_Irrigation > 1)
		{
			memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
			if(Wireless_Pump_On_Off(_eDO_CALIBRATION,_sRuble_Parameters.Pump_Selected_For_Irrigation - 2))
			{
				sprintf((char *)_gPub_Buff,(const char *)"{\"success\": \"true\"}");
			}
			else
			{
				sprintf((char *)_gPub_Buff,(const char *)"{\"success\": \"false\"}");
			}
			
			_kMQTT_PUB((unsigned char *)_sPublish_Topic.Calibration_FeedBack,(unsigned char *)_gPub_Buff);
			
			return 1;
		}
		
		
		previous = millis();
		do
		{
			for(i=0;i<5;i++)
			{
				filter_data[i] = RUBLE_PUMP_CURRENT.calcIrms(_kCT_AVERAGE_TURNS);
			}
			for (i = 0; i < 5; i++)
			{
				for (j = i + 1; j < 5; j++)
				{
					if (filter_data[i] > filter_data[j])
					{
						filter_data_1 = filter_data[i];
						filter_data[i] = filter_data[j];
						filter_data[j] = filter_data_1;
					}
				}
			}
			_sPump_Parameter.Pump_Run_Current =  filter_data[2];      // RUBLE_PUMP_CURRENT.calcIrms(_kCT_AVERAGE_TURNS);
			
			if(_sPump_Parameter.Pump_Run_Current > 2)
			{
				_kEEPROM_WRITE(_kEEPROM_LOC_PUMP_NOMINAL_CURRENT,_sPump_Parameter.Pump_Run_Current);     /* save the pump current */
				_kEEPROM_READ(_kEEPROM_LOC_PUMP_NOMINAL_CURRENT,_sPump_Parameter.Pump_Nominal_Current);  /* read pump nominal current */
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Pump Calibration is done");                /* for debugging purpose */
					_kSERIAL_MON_CLEAR();
				}
				answer = 1;
			}	
		}while ((answer == 0) && ((millis() - previous) < timeout));
		
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		if(answer)
		{
			_sRuble_Parameters.By_Pass_CT = _kSET;
			_kEEPROM_WRITE(_kCT_BY_PASS,_sRuble_Parameters.By_Pass_CT);
			sprintf((char *)_gPub_Buff,(const char *)"{\"success\": \"true\"}");	
		}
		else
		{
			sprintf((char *)_gPub_Buff,(const char *)"{\"success\": \"false\"}");
		}
		_kMQTT_PUB((unsigned char *)_sPublish_Topic.Calibration_FeedBack,(unsigned char *)_gPub_Buff);
	}
	else
	{
		if(_sRuble_Parameters.Do_Calibration > _kWIRED_PUMP_NUMBER)
		{
			//Wireless_Pump_On_Off(_kDO_CALIBRATION);
			Wireless_Pump_On_Off(_kDO_CALIBRATION,_sRuble_Parameters.Do_Calibration -2);
			
			_sRuble_Parameters.Do_Calibration = _kRESET;
			previous = millis();
			do
			{
				Lora_Operation();
				
				if(_sWireless_Pump_Param[_sRuble_Parameters.Do_Calibration -2].Response_To_Ruble & _eCALIBARATION_SUCCESS)
				{
					answer = 1;
				}
			}while ((answer == 0) && ((millis() - previous) < timeout));
			
			memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
			if(answer)
			{
				_sRuble_Parameters.By_Pass_CT = _kSET;
				_kEEPROM_WRITE(_kCT_BY_PASS,_sRuble_Parameters.By_Pass_CT);
				sprintf((char *)_gPub_Buff,(const char *)"{\"success\": \"true\"}");
			}
			else
			{
				sprintf((char *)_gPub_Buff,(const char *)"{\"success\": \"false\"}");
			}
			_kMQTT_PUB((unsigned char *)_sPublish_Topic.Calibration_FeedBack,(unsigned char *)_gPub_Buff);
			_sWireless_Pump_Param[_sRuble_Parameters.Do_Calibration -2].Response_To_Ruble &= ~_eCALIBARATION_SUCCESS;
			_sWireless_Pump_Param[_sRuble_Parameters.Do_Calibration -2].Response_To_Ruble &= ~_eCALIBRATION_FAIL;
		}
	}
}


void Faults_Handler(void)
{
	static unsigned char temp_back_up;
	static unsigned char faults_check_timer = 0,fault_occure = 0;
	
	//_kSERAIL_MON_WRITE("Faults_Handler 1 ");
	
	
	
	if(_sRuble_Parameters.Pump_Selected_For_Irrigation > 1)
	{
		if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_AUTO || _sRuble_Parameters.Operation_Mode == _eOPER_MOD_TIMER ||
		_sRuble_Parameters.Operation_Mode == _eOPER_MOD_SCHEDULER)
		{
			if(_sRuble_Parameters.Wirelss_Pump_Status == _kON)
			{
				/* check faults */
				//_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults;
				if((_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eR_PHASE_STATUS) !=  _eR_PHASE_STATUS||
				   (_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eY_PHASE_STATUS) != _eY_PHASE_STATUS    ||
				   (_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eB_PHASE_STATUS) != _eB_PHASE_STATUS ||
				   (_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eDRY_RUN_STATUS) == _eDRY_RUN_STATUS ||
				   (_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eLINE_FAULT_STATUS) ==  _eLINE_FAULT_STATUS ||
				   (_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _ePUMP_OVERLOAD) ==  _ePUMP_OVERLOAD||
				   (_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults  & _eMANUAL_MODE) == _eMANUAL_MODE /*||
				   (_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults  & _ePUMP_STATUS) != _ePUMP_STATUS*/ ||
				   (_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _ePUMP_NOT_WORKING)  == _ePUMP_NOT_WORKING /*||
				   _sRuble_Parameters.Update_Local_Date[2] == 70*/)
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Wireless Pump Fault ocuure............ ");
						_kSERIAL_MON_PRINT_NUM(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults);
					}
					_sPump_Parameter.Irrigation_Status = _eWIRELESS_PUMP_FAIL_FAULT;
					_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_LINE_FAULT;
					_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_FAULTS; 
				}
			}
		}
		return ;
	}
	
	
	
	if(_sPump_Parameter.Irrigation_Status != 0)
	{
		if(_gfault_Occure == 0)
		{
			_sPump_Parameter.Irrigation_Status = 0;
		}	
	}
	
	
	
	if(_sRuble_Parameters.Read_Ct_Value_Timer)
	{
		if(fault_occure)
		{
			fault_occure = _kRESET;
			faults_check_timer++;
		}
		
		_sRuble_Parameters.Read_Ct_Value_Timer = _kRESET;
		_sPump_Parameter.Pump_Run_Current = RUBLE_PUMP_CURRENT.calcIrms(_kCT_AVERAGE_TURNS);
	}
	
	
	
	if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER)
	{
		if((Check_Phase()) == 1)
		{
			if(_kGPIO_GET(_kPUMP_1) == _kPUMP_ON)
			{
				if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_AUTO || _sRuble_Parameters.Operation_Mode == _eOPER_MOD_TIMER ||
				_sRuble_Parameters.Operation_Mode == _eOPER_MOD_SCHEDULER)
				{
					_sPump_Parameter.Irrigation_Status = _eTHREE_PHASE_POWER_INT;
				}
			}
			if(!_sRuble_Parameters.Line_Fault_Flag)
			{
				_sRuble_Parameters.Line_Fault_Flag = _kSET;
				_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_LINE_FAULT;
				_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_FAULTS;
			}
		}
		else
		{
			if(_sRuble_Parameters.Line_Fault_Flag)
			{
				_sRuble_Parameters.Line_Fault_Flag = _kRESET;
			}
		}
	}
	else
	{
		_sRuble_Parameters.Line_Fault_Flag = _kRESET;
	}
	
	
	
	if(!_sRuble_Parameters.By_Pass_CT)
	{
		return;
	}
	
	if(_sPump_Parameter.Pump_Run_Current < ((_sPump_Parameter.Pump_Nominal_Current/100) * 15))
	{
		if(_sRuble_Parameters.Pump_On_Flag == _ePUMP_ON)
		{
			fault_occure = _kRESET;
			faults_check_timer = _kRESET;
			_sRuble_Parameters.Pump_On_Flag = _ePUMP_OFF;
			_sGSM_Query.SMS_Alert |= _ePUMP_OFF;
			_sRuble_Parameters.Publish_Topic |=  _ePUB_RUBLE_DATA;
		}
	}
	
	if(_sPump_Parameter.Pump_Run_Current > ((_sPump_Parameter.Pump_Nominal_Current/100) * 18))
	{
		if(_sRuble_Parameters.Pump_On_Flag == _ePUMP_OFF)
		{
			fault_occure = _kRESET;
			faults_check_timer = _kRESET;
			_sGSM_Query.SMS_Alert |=  _ePUMP_ON;
			_sRuble_Parameters.Pump_On_Flag = _ePUMP_ON;
			_sRuble_Parameters.Publish_Topic |=  _ePUB_RUBLE_DATA;
		}
		
	}
	
	//_kSERAIL_MON_WRITE("Faults_Handler 9 ");
	
	
	if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_MANUAL || ( (_sRuble_Parameters.Start_Monitoring_Pump_Faults_Timer > _kSTART_MONITORING_PUMP_FAULTS_TIMER) && ((_sRuble_Parameters.Irrigating_Plot !=0) ||( _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kSET) || (_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kSET))))
	// || _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kSET || _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kSET))
	{
		_sRuble_Parameters.Start_Monitoring_Pump_Faults_Timer = _kSTART_MONITORING_PUMP_FAULTS_TIMER+1;   /* don't want to overflow */
		
		if(_sPump_Parameter.Pump_Run_Current > (_sPump_Parameter.Pump_Nominal_Current) + ((_sPump_Parameter.Pump_Nominal_Current/100) * 32))
		{
			if((_sPump_Parameter.Irrigation_Status != _eIRRIG_STAT_OVER_LOAD))
			{
				fault_occure = _eIRRIG_STAT_OVER_LOAD;
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Pump Overlaoded ");
					_kSERIAL_MON_PRINT_NUM(faults_check_timer);
				}
				if(faults_check_timer > 3)
				{
					faults_check_timer = 0;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Pump Overlaoded");
						_kSERIAL_MON_CLEAR();
					}
					_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_OVER_LOAD;
					_gfault_Occure = _kFAULT_RESET_TIMER;
					_sGSM_Query.SMS_Alert |=  _ePUMP_OVERLOAD;
					_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
				}
			}
		}
		else
		{
			if(fault_occure == _eIRRIG_STAT_OVER_LOAD)
			{
				fault_occure = _kRESET;
			}
		}
		
		//_kSERAIL_MON_WRITE("Faults_Handler 10 ");
		
		/*********if pump consume less current which is 60% less than nominal current then there is dry run fault *******************/
		
		if(_sPump_Parameter.Pump_Run_Current < ((_sPump_Parameter.Pump_Nominal_Current/100) * 58))
		{
			if(_sPump_Parameter.Pump_Run_Current > ((_sPump_Parameter.Pump_Nominal_Current/100) * 20))
			{
				if((_sPump_Parameter.Irrigation_Status != _eIRRIG_STAT_DRY_RUN))
				{
					fault_occure = _eIRRIG_STAT_DRY_RUN;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Pump Dry Run ");
						_kSERIAL_MON_PRINT_NUM(faults_check_timer);
					}
					if(faults_check_timer > 3)
					{
						faults_check_timer = 0;
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Pump Dry Run");
							_kSERIAL_MON_CLEAR();
						}
						_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_DRY_RUN;
						_gfault_Occure = _kFAULT_RESET_TIMER;
						_sGSM_Query.SMS_Alert |=  _eDRY_RUN;
						_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
					}
				}
			}
		}
		else
		{
			if(fault_occure == _eIRRIG_STAT_DRY_RUN)
			{
				fault_occure = _kRESET;
			}
		}
		
		//_kSERAIL_MON_WRITE("Faults_Handler 11 ");
		
		/*********if pump consume less current which is 90% less than nominal current then there is line fault *******************/
		if(_sPump_Parameter.Pump_Run_Current < ((_sPump_Parameter.Pump_Nominal_Current/100) * 15))
		{
			if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_AUTO || _sRuble_Parameters.Operation_Mode == _eOPER_MOD_TIMER ||
			_sRuble_Parameters.Operation_Mode == _eOPER_MOD_SCHEDULER)
			{
				if(_kGPIO_GET(_kPUMP_1) == _kPUMP_ON)
				{
					if((_sPump_Parameter.Irrigation_Status != _eIRRIG_STAT_LINE_FAULT))
					{
						fault_occure = _eIRRIG_STAT_LINE_FAULT;
						fault_occure = _eIRRIG_STAT_DRY_RUN;
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Pump Line Fault ");
						}
						if(faults_check_timer > 3)
						{
							faults_check_timer = _kRESET;
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Pump Line Fault");
								_kSERIAL_MON_CLEAR();
							}
							
							_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_LINE_FAULT;
							_gfault_Occure = _kFAULT_RESET_TIMER;
							
							/*if(!_sRuble_Parameters.Line_Fault_Flag)
							{
								_sRuble_Parameters.Line_Fault_Flag = _kSET;
								_sGSM_Query.SMS_Alert |=  _eLINE_FAULT_SMS;
							}*/
							
							_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
						}
					}
				}
			}
		}
		else
		{
			if(fault_occure == _eIRRIG_STAT_LINE_FAULT)
			{
				fault_occure = _kRESET;
			}
			/*if(_sRuble_Parameters.Line_Fault_Flag)
			{
				_sRuble_Parameters.Line_Fault_Flag = _kRESET;
				//_sGSM_Query.SMS_Alert |=  _e3PHASE_POWER_INTERRUPTED;
			}*/
		}
		
		if(fault_occure == _kRESET)
		{
			faults_check_timer = _kRESET;
		}
	}
	
	//_kSERAIL_MON_WRITE("Faults_Handler 12 ");
}


unsigned char Check_Phase(void)
{
	
// 	if(_kGPIO_GET(_kPHASE_DETECTION_SWITCH))  /* if there is signal phase configuration */
// 	{
// 		return 0;
// 	}
	
	//if(0)
	//if(_kGPIO_GET(_kDEBUG_PIN) /*|| _sRuble_Parameters.Update_Local_Date[2] == 70*/) 
	 if(_kGPIO_GET(_kPHASE_DETECTION) /*|| _sRuble_Parameters.Update_Local_Date[2] == 70*/)   /* If any phase is not there */
// 	if(_kGPIO_GET(_kR_PHASE_INPUT) == _kHIGH ||
// 	_kGPIO_GET(_kY_PHASE_INPUT) == _kHIGH  ||
// 	_kGPIO_GET(_kB_PHASE_INPUT) == _kHIGH)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("phase is not there ");
		}
		return 1;
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("phase is there ");
	}
	return 0;
}


bool Check_Sim(char check_status)
{
	static char counter;
	char temp_data[2];
	
	counter++;
	
	if(_kSERIAL_AT.available() != 0)
	{
		if(_sRuble_Parameters.Sim_Check == _eSIM_INSERTED)
		{
			return 1;
		}
		
		return 0;
	}
	
	switch(check_status)
	{
		case _eDIRESCT_CHECK:
		{
			if(_sRuble_Parameters.Sim_Check == _eSIM_NOT_INSERTED)
			{
				_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_1 ;
				_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_2;
				_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_3;
				_sRuble_Parameters.Display_Update  &=   ~_kDISP_NO_RANGE;
				
				memset(temp_data,0,sizeof(temp_data));
				temp_data[0] = 0x1A;
				_kSERIAL_AT.println((char *)temp_data);

	             _kSERIAL_AT.println(F("AT+CFUN=1,1"));
				 Send_AT_Command(NULL, "OK",  NULL,10000,1);
				 _kSERIAL_AT.println(F("ATE0"));
				 Send_AT_Command(NULL, "OK",  NULL,3000,2);
				_kGSM_INIT();
			}
			_kSERIAL_AT.println(F("AT+CPIN?"));
			if(!Send_AT_Command(NULL, "OK",  NULL,2000,2))
			{
				_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_1 ;
				_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_2;
				_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_3;
				_sRuble_Parameters.Display_Update  &=   ~_kDISP_NO_RANGE;
				_sRuble_Parameters.Sim_Check = _eSIM_NOT_INSERTED;
				return 0;
			}
			_sRuble_Parameters.Sim_Check = _eSIM_INSERTED;
			return 1;
		}break;
	}	
}


void Clear_Serial_Data(void)
{
	unsigned long previous;
	previous = millis();
	while (_kSERIAL_AT.available() > 0 && ((millis() - previous) < 100))
	{
		_kSERIAL_AT.read(); // clears the buffer
	}
}


void Kill_Switch_Through_SMS(void)
{
// 	static unsigned char kill_switch_timer;
// 	if(kill_switch_timer > 240)
// 	{
// 		kill_switch_timer = _kRESET;
// 		Read_SMS();
// 	}
// 	else
// 	{
// 		kill_switch_timer++;
// 	}
}


void Keeps_Monitor_Server_Connection(void)
{
	static uint32_t server_wait_timer;
	static unsigned char timer_count,connect_count,ping_timer,ping_req_timer;
	if(_sRuble_Parameters.Subscribe_Topic_Status == _eALL_TOPIC_NOT_SUBCRIBED)
	{
		ping_req_timer = 100;
	}
	else
	{
		ping_req_timer = 100;
	}
				
	if(!_sRuble_Parameters.Wait_Server_Not_Reachable)
	{
		if(ping_timer > ping_req_timer)
		{
			ping_timer = 0;
			if(!_kMQTT_LOOP())
			{
				_sRuble_Parameters.Subscribe_Topic_Status = _eALL_TOPIC_NOT_SUBCRIBED;
				_sRuble_Parameters.lndications &= ~_eRUBL_INDI_SERVER_CONN;
				if(connect_count < _kRETRY_TO_CONNECT_SERVER)
				{
					_sRuble_Parameters.Wait_Server_Not_Reachable = _kRESET;
					if(!Ping_to_Server())
					{
						connect_count++;
					}
					else
					{
						connect_count = _kRESET;
					}
				}
				else
				{
					if(_sRuble_Parameters.Wait_Server_Not_Reachable != _kSET)
					{
						_sRuble_Parameters.Wait_Server_Not_Reachable = _kSET;
						server_wait_timer = _kWAIT_DUE_TO_SERVER_NOT_RECHABLE;
					}
				}
			}
		}
		else
		{
			ping_timer++;
		}
	}
						
	if(_sRuble_Parameters.Wait_Server_Not_Reachable)  /* don't try again and again to connect the server have to wait for some time */
	{
		if(server_wait_timer)
		{
			server_wait_timer--;
		}
		else
		{
			_sRuble_Parameters.Wait_Server_Not_Reachable = _kRESET;
			connect_count = _kRESET;
		}
	}
}


void Local_Time_Setting(void)
{
	_kSERIAL_AT.println(F("AT"));
	if(Send_AT_Command(NULL, "OK",  NULL,3000,2))
	{			
		_kSERIAL_AT.println(F("ATE0"));
		Send_AT_Command(NULL, "OK",  NULL,3000,2);
		_kSERIAL_AT.println(F("AT+CPIN?"));
		
		if(!Send_AT_Command(NULL, "OK",  NULL,10000,2))
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("check sim me");
				_kSERIAL_MON_CLEAR();
			}
			_sRuble_Parameters.Sim_Check = _eSIM_NOT_INSERTED;
		}
	}
	else
	{
		_sGSM_Query.Gsm_Module_Test = _kSET;
	}
}


void Exra_Plot_Data_EEPROM(unsigned char plot_number)
{
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		Serial.println();
		_kSERAIL_MON_WRITE_NO_LN("extra plot : ");
		_kSERIAL_MON_PRINT_NUM((int )plot_number);
		Serial.println();
		_kSERIAL_MON_CLEAR();
	
		_kSERAIL_MON_WRITE("data saving in eeprom ");
		_kSERAIL_MON_WRITE_NO_LN("Extra Plot Number.: ");
		_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[plot_number-1].Extra_Plot_Number);
		_kSERIAL_MON_CLEAR();
	
		for(int i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
		{
			Serial.println();
			_kSERAIL_MON_WRITE_NO_LN("VALVE NO : ");
			_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[plot_number-1].Euro_Valve_Group[i]);
			_kSERIAL_MON_CLEAR();
		}
	}
	
	_kEEPROM_WRITE(_kEEPROM_EXTRA_PLOT_DATA(plot_number), _sExtra_Plot[plot_number-1]);
	return;
}


char Pump_Run_Monitoring(void)
{
	if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
	{
		if(_sRuble_Parameters.By_Pass_CT == _kSET)
		{
			if(_sPump_Parameter.Pump_Run_Current > ((_sPump_Parameter.Pump_Nominal_Current) - ((_sPump_Parameter.Pump_Nominal_Current/100) * 90)))   // DEMO ONLY
			{
				_sRuble_Parameters.Pump_Is_Running_Or_Not = _kSET;
				_sRuble_Parameters.Pump_On_Flag = _ePUMP_ON;	
			}
			else
			{
				_sRuble_Parameters.Pump_Is_Running_Or_Not = _kRESET;
				_sRuble_Parameters.Pump_On_Flag = _ePUMP_OFF;
			}
		}
		else
		{
			if(!_kGPIO_GET(_kPUMP_1))
			{
				_sRuble_Parameters.Pump_Is_Running_Or_Not = _kSET;
			}
			else
			{
				_sRuble_Parameters.Pump_Is_Running_Or_Not = _kRESET;
			}
		}
	}
	else
	{
		if(_sRuble_Parameters.Wirelss_Pump_Status == _kON &&
		(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _ePUMP_STATUS) == _ePUMP_STATUS)
		{
			_sRuble_Parameters.Pump_Is_Running_Or_Not = _kSET;
		}
		else
		{
			_sRuble_Parameters.Pump_Is_Running_Or_Not = _kRESET;
		}
		
	}
}


unsigned char Debug_Check(void)
{
	if(_kGPIO_GET(_kDEBUG_PIN))
	{
		_sRuble_Parameters.Debug_Mode = _kRESET;
		return	1;	
	}
	
	_sRuble_Parameters.Debug_Mode = _kSET;
	return 0;
}	



