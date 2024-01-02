/*
 * GSM_GPRS_Network.cpp
 *
 * Created: 25-08-2021 16:20:27
 *  Author: Tushar (SIO)
 */ 

#include <GSM_GPRS_Network.h>
#include <Hardware_Config.h>
#include <Initialize.h>



GSM_Query _sGSM_Query;
Sim_APNs _sSim_APNs;


void Store_All_APN(void)
{
// 	sprintf(_sSim_APNs.Vodafone, "www");
// 	
// 	sprintf(_sSim_APNs.Airtel, "airtelgprs.com");
// 	
// 	sprintf(_sSim_APNs.Idea, "imis/internet"); 
// 	
// 	sprintf(_sSim_APNs.BSNL, "bsnlnet");
// 	
// 	sprintf(_sSim_APNs.Jio,"jionet");
}


void Check_GSM_Signal(void)
{
	static unsigned char counter;
	int temp_sig_strenght;
	
	counter++;
	
	if(_kSERIAL_AT.available() != 0)
	{
		return;
	}
		
	if(_sRuble_Parameters.Sim_Check == _eSIM_INSERTED)
	{
		temp_sig_strenght = _kGSM_SIG_STRENGTH();   /* get the signal strength */		
		_kSERIAL_MON_PRINT_NUM(temp_sig_strenght);
	}
	else
	{
		temp_sig_strenght = 0;
	}
	
	if(temp_sig_strenght > 19)                 /* if there is weak signal */
	{
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_1 ;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_2;
		_sRuble_Parameters.Display_Update  |=    _kDISP_RANGE_3;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_NO_RANGE;
	}
	 
	if(temp_sig_strenght < 20 && temp_sig_strenght > 13)   /* if there is moderate signal */
	{
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_1 ;
		_sRuble_Parameters.Display_Update  |=    _kDISP_RANGE_2;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_3;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_NO_RANGE;
	}
	 
	 if(temp_sig_strenght < 14 && temp_sig_strenght > 5)   /* if there is strong signal */
	{
		_sRuble_Parameters.Display_Update  |=    _kDISP_RANGE_1 ;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_2;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_3;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_NO_RANGE;
	}
	 
	if(temp_sig_strenght < 5)                           /* if there is no signal */
	{
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_1 ;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_2;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_3;
		_sRuble_Parameters.Display_Update  |=    _kDISP_NO_RANGE;
	}
	
	if (temp_sig_strenght >= 99) 
	{
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_1 ;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_2;
		_sRuble_Parameters.Display_Update  &=   ~_kDISP_RANGE_3;
		_sRuble_Parameters.Display_Update  |=    _kDISP_NO_RANGE;
	}
	
}


void Get_Local_Time(void)
{
	char i;
	
	//_kSERIAL_AT.println(F("AT+CNTP=\"pool.ntp.org\",22"));
	//Send_AT_Command(NULL, "OK",  NULL,5000,2);
	//AT+CNTP="202.120.2.101",32
	
	//_kSERIAL_AT.println(F("AT+CTZU=0"));
	//Send_AT_Command(NULL, "OK",  NULL,3000,2);
	
	//_kSERIAL_AT.println(F("AT+CNTP=\"202.120.2.101\",22"));
	
	
	_kSERIAL_AT.println(F("AT+CNTP=\"in.pool.ntp.org\",22"));
	Send_AT_Command(NULL, "OK",  NULL,5000,2);
// 	
// 	
	_kSERIAL_AT.println(F("AT+CNTP"));
	Send_AT_Command(NULL, "+CNTP: 0",  NULL,3000,2);

	//AT+CHTPSERV=”ADD”,”www.google.com”,80,1
	
// 	_kSERIAL_AT.println(F("AT+CHTPSERV=\"ADD\",\"www.google.com\",80,1"));
// 	Send_AT_Command(NULL, "OK",  NULL,5000,2);
	
// 	_kSERIAL_AT.println(F("AT+CHTPUPDATE"));
// 	Send_AT_Command(NULL, "OK",  NULL,5000,2);
	
// 	_kSERIAL_AT.println(F("AT+CTZU=1"));
// 	Send_AT_Command(NULL, "OK",  NULL,3000,2);
	
	_kSERIAL_AT.println(F("AT+CCLK?"));
	Send_AT_Command(NULL, "OK", NULL,2000,2);
	
   if(strstr((char *)_gRecvd_Data,"+CCLK") != 0)
   {
	   for(i=0;i<20;i++)
	   {
		   if(_gRecvd_Data[i] == '+' && _gRecvd_Data[i+5] == ':')
		   {
			   break;
		   }
	   }
   }
   else
   {
	   return;
   }
   
	/*************************************************** UPDATE TIME AND DATE *********************************************/
	
	_sRuble_Parameters.Update_Local_Time[0] = ((_gRecvd_Data[i+17]-0x30)*10) + (_gRecvd_Data[i+18]-0x30);    /* hold hour */
	_sRuble_Parameters.Update_Local_Time[1] = ((_gRecvd_Data[i+20]-0X30)*10) + (_gRecvd_Data[i+21]-0X30);    /* hold minute */
	_sRuble_Parameters.Update_Local_Time[2] = ((_gRecvd_Data[i+23]-0X30)*10) + (_gRecvd_Data[i+24]-0X30);    /* hold second */
	
	_sRuble_Parameters.Current_Time_In_Sec = (unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[0]*3600)+(unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[1]*60) +(unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[2]) ;
	//_sRuble_Parameters.Current_Time_In_Sec = 36000;
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		Serial.println();
		_kSERIAL_MON_PRINT_NUM((unsigned long int)_sRuble_Parameters.Current_Time_In_Sec);
	}
	
	_sRuble_Parameters.Update_Local_Date[2] = ((_gRecvd_Data[i+8]-0X30)*10) +  (_gRecvd_Data[i+9]-0X30);      /* hold date */
	_sRuble_Parameters.Update_Local_Date[1] = ((_gRecvd_Data[i+11]-0X30)*10) + (_gRecvd_Data[i+12]-0X30);     /* hold month */
	_sRuble_Parameters.Update_Local_Date[0] = ((_gRecvd_Data[i+14]-0X30)*10) + (_gRecvd_Data[i+15]-0X30); 
	    /* hold year */
		_sRuble_Parameters.Update_Local_Date[0] = _sRuble_Parameters.Update_Local_Date[0];
		_sRuble_Parameters.Update_Local_Date[1] = _sRuble_Parameters.Update_Local_Date[1];
		
		//_sRuble_Parameters.Update_Local_Date[0] = 31;
		//_sRuble_Parameters.Update_Local_Date[1] = _eDEC;
		//_sRuble_Parameters.Current_Time_In_Sec = 0;
		
		
		_kSERAIL_MON_WRITE("my date : ");
		_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Update_Local_Date[0]);
		_kSERIAL_MON_CLEAR();
		_kSERAIL_MON_WRITE("my month : ");
		_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Update_Local_Date[1]);
		_kSERIAL_MON_CLEAR();
		_kSERAIL_MON_WRITE("my year : ");
		_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Update_Local_Date[2]);
		_kSERIAL_MON_CLEAR();
		
	if(_sRuble_Parameters.Update_Local_Time[0] > 23 || _sRuble_Parameters.Update_Local_Time[1] > 59 ||  _sRuble_Parameters.Update_Local_Time[2] > 59)
	{
		return;
	}
	_sRuble_Parameters.Display_Update  |= _kTIME_UPDATE;
}


bool Gsm_Gprs_Network(void)
{
	char temp_data[100];
	unsigned char check_sim;
	/***************************************** wait for the netwrok *******************************************/ 
	check_sim = 0;
	if (_kIS_GSM_CONNECTED())
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Connected to gsm/GPRS network 0");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		memset(temp_data,0,sizeof(temp_data));
		Get_Operator_Name(temp_data);
		
		
		if(strstr(temp_data,"Vod") != 0 || strstr(temp_data,"Vi In") != 0)
		{
			strcpy(_sNetwork_and_Usr_Info.Ruble_Sim_APN,"www"/*_sSim_APNs.Vodafone*/);
			_kSERAIL_MON_WRITE("Vodafone");               
			_kSERIAL_MON_CLEAR();
			check_sim = 1;
		}
		
		/*else if (strstr(temp_data,"Airtel") != 0 || strstr(temp_data,"airtel") != 0)
		{
			strcpy(_sNetwork_and_Usr_Info.Ruble_Sim_APN,_sSim_APNs.Airtel);
			_kSERAIL_MON_WRITE("Airtel");               
			_kSERIAL_MON_CLEAR();
		}*/
		
		/*else if (strstr(temp_data,"idea") != 0 || strstr(temp_data,"Idea") != 0)
		{
			/*strcpy(_sNetwork_and_Usr_Info.Ruble_Sim_APN,_sSim_APNs.Idea);
			_kSERAIL_MON_WRITE("Idea");               
			_kSERIAL_MON_CLEAR();
		}*/
		
		/*else if (strstr(temp_data, "BSNL") != 0)                            //need to record response of BSNL network
		{
			/*strcpy(_sNetwork_and_Usr_Info.Ruble_Sim_APN,_sSim_APNs.BSNL);
			_kSERAIL_MON_WRITE("BSNL");               
			_kSERIAL_MON_CLEAR();
		}*/
		
		else if(strstr(temp_data,"jio") != 0  || strstr(temp_data,"Jio")!=0)
		{
			strcpy(_sNetwork_and_Usr_Info.Ruble_Sim_APN,"jionet"/*_sSim_APNs.Jio*/);
			_kSERAIL_MON_WRITE("jio");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
			check_sim = 1;
		}
		
		else
		{
			_kSERAIL_MON_WRITE("Can't get opeartor Name");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
			//return 0;
		}		
		
		if(check_sim == 1)
		{		
			if (!_kGPRS_CONNECT(_sNetwork_and_Usr_Info.Ruble_Sim_APN))
			{
				_kSERAIL_MON_WRITE("Fail to Connect GPRS");                /* for debugging purpose */
				_kSERIAL_MON_CLEAR();
				_kSERIAL_AT.println(F("AT+CFUN=1,1"));
				Send_AT_Command(NULL, "Call Ready",  NULL,10000,1);
				_kDELAY_MS(10000);
				if(_sRuble_Parameters.Ruble_Registration_Status != _eRUBLE_REGISTERED)
				{
					_kDELAY_MS(120000);
				}
				_kSERIAL_AT.println(F("ATE0"));
				Send_AT_Command(NULL, "OK",  NULL,3000,2);
				//return 0;
			}
		}


		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{			
			_kSERAIL_MON_WRITE("Succs Connected to GPRS network");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		return 1;
		
	}
	else
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("fail to connect gsm/GPRS network");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		
		_kSERIAL_AT.println(F("AT+CFUN=1,1"));
		Send_AT_Command(NULL, "OK",  NULL,10000,1);
		_kDELAY_MS(10000);
		if(_sRuble_Parameters.Ruble_Registration_Status != _eRUBLE_REGISTERED)
		{
			_kDELAY_MS(120000);
		}
		_kSERIAL_AT.println(F("ATE0"));
		Send_AT_Command(NULL, "OK",  NULL,3000,2);
		return 0;
		
	}
	
}


void Send_SMS_Alert(void)
{
// 	unsigned char temp_data[60];
// 	if(_kSERIAL_AT.available() != 0)
// 	{
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.Sim_Check != _eSIM_INSERTED)
// 	{
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.SMS_Alert_Start == 0x08)
// 	{
// 		if(_sGSM_Query.SMS_Alert & _eRUBLE_START)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"RESTART");
// 			_sGSM_Query.SMS_Alert &=  ~_eRUBLE_START;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert &  _eMODE_CHANGED)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"MODE CHANGE");
// 			_sGSM_Query.SMS_Alert &=  ~_eMODE_CHANGED;
// 		}
// 	
// // 		if (_sGSM_Query.SMS_Alert & _eDATA_LOGGER_MODE_SELECTED)
// // 		{
// // 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"DATA LOGGER MODE IS SELECTED");
// // 			_sGSM_Query.SMS_Alert &= ~_eDATA_LOGGER_MODE_SELECTED;
// // 		}
// 	
// // 		if (_sGSM_Query.SMS_Alert &  _eMANUAL_MODE_SELECTED)
// // 		{
// // 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"MANUAL MODE SELECTED");
// // 			_sGSM_Query.SMS_Alert &=  ~_eMANUAL_MODE_SELECTED;
// // 		}
// 	
// // 		if (_sGSM_Query.SMS_Alert & _eAUTO_MODE_SELECTED)
// // 		{
// // 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"AUTO MODE SELECTED");
// // 			_sGSM_Query.SMS_Alert &=  ~_eAUTO_MODE_SELECTED;
// // 		}
// 	
// // 		if (_sGSM_Query.SMS_Alert & _eTIMER_MODE_SELECTED)
// // 		{
// // 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"TIMER MODE SELECTED");
// // 			_sGSM_Query.SMS_Alert &= ~ _eTIMER_MODE_SELECTED;
// // 		}
// 	
// 		if (_sGSM_Query.SMS_Alert & _eTIMER_STOP_MANUAL_MODE_SELECTED)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"MAN MODE,TIMER STOP");
// 			_sGSM_Query.SMS_Alert &= ~_eTIMER_STOP_MANUAL_MODE_SELECTED;
// 		}
// 	
// // 		if (_sGSM_Query.SMS_Alert &  _eTIMER_SET)
// // 		{
// // 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"TIMER SET");
// // 			_sGSM_Query.SMS_Alert &= ~_eTIMER_SET;
// // 		}
// 	
// // 		if (_sGSM_Query.SMS_Alert & _eTIMER_OFF)
// // 		{
// // 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"TIMER OFF");
// // 			_sGSM_Query.SMS_Alert &= ~_eTIMER_OFF;
// // 		}
// 	
// 		if (_sGSM_Query.SMS_Alert & _eTIMER_STOPPED)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"TIMER STOP");
// 			_sGSM_Query.SMS_Alert &= ~_eTIMER_STOPPED;
// 		}
// 	
// 		if (_sGSM_Query.SMS_Alert & _ePUMP_ON)
// 		{
// 			//if(_sPump_Parameter.Pump_Run_Current > (_sPump_Parameter.Pump_Nominal_Current/100) * 18)
// 			//{
// 			   _kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"PUMP ON");
// 			   _sGSM_Query.SMS_Alert &= ~_ePUMP_ON;
// 			//}
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert & _ePUMP_OFF)
// 		{
// 			//if(_sPump_Parameter.Pump_Run_Current < (_sPump_Parameter.Pump_Nominal_Current/100) * 16)
// 			//{
// 				_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"PUMP OFF");
// 				_sGSM_Query.SMS_Alert &= ~_ePUMP_OFF;
// 			//}	
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert & _eTIMER_OFF)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"TIMER OFF");
// 			_sGSM_Query.SMS_Alert &= ~_eTIMER_OFF;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert &  _eNO_ELECTRICITY )
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"NO ELECTRICITY");
// 			_sGSM_Query.SMS_Alert &= ~_eNO_ELECTRICITY;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert & _eELECTRICITY_RESUMED)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"ELECTRICITY RESUME");
// 			_sGSM_Query.SMS_Alert &= ~_eELECTRICITY_RESUMED;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert & _e3PHASE_POWER_INTERRUPTED)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"3 PHASE INTERRUPT");
// 			_sGSM_Query.SMS_Alert &= ~_e3PHASE_POWER_INTERRUPTED;
// 		}
// 		
// 		if(_sGSM_Query.SMS_Alert & _eLINE_FAULT_SMS)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"LINE FAULT");
// 			_sGSM_Query.SMS_Alert &= ~_eLINE_FAULT_SMS;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert &  _eSENSOR_OUT_OF_SOIL)    /* yet to implement */
// 		{
// 			memset(temp_data,0,sizeof(temp_data));
// 			sprintf((char *)temp_data,"SM REMOVAL Plot NO.:%d",_sRuble_Parameters.Last_Plot+1);
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,(char *)temp_data);
// 			_sGSM_Query.SMS_Alert &= ~_eSENSOR_OUT_OF_SOIL;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert & _eLIRA_NODE_BATT_LOW)   /* yet to implement */
// 		{
// 			_sGSM_Query.SMS_Alert |= _eLIRA_NODE_BATT_LOW;
// 		
// 			memset(temp_data,0,sizeof(temp_data));
// 			sprintf((char *)temp_data,"Plot NO.:%d LIRA BAT LOW",_sRuble_Parameters.Last_Plot+1);
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,(char *)temp_data);
// 			_sGSM_Query.SMS_Alert &= ~_eLIRA_NODE_BATT_LOW;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert & _eEURO_BATT_LOW)   /* yet to implement */
// 		{
// 			memset(temp_data,0,sizeof(temp_data));
// 			sprintf((char *)temp_data,"EURO NO.:%d  BAT LOW",_sRuble_Parameters.Last_Valve+1);
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,(char *)temp_data);
// 			_sGSM_Query.SMS_Alert &= ~_eEURO_BATT_LOW;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert &  _ePUMP_OVERLOAD)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"OVERLOAD");
// 			_sGSM_Query.SMS_Alert &=  ~_ePUMP_OVERLOAD;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert &  _ePUMP_DRY_RUN)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"DRY RUN");
// 			_sGSM_Query.SMS_Alert &=  ~_ePUMP_DRY_RUN;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert &   _eVALVE_SENSE_FAIL)
// 		{
// 			memset(temp_data,0,sizeof(temp_data));
// 			sprintf((char *)temp_data,"EURO NO.:%d Fail",_sRuble_Parameters.Last_Valve+1);
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,(char *)temp_data);
// 			_sGSM_Query.SMS_Alert &=   ~_eVALVE_SENSE_FAIL;
// 		}
// 	
// 		if(_sGSM_Query.SMS_Alert &  _eVALVE_FEEDBACK_FAIL)
// 		{
// 			/*_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"Wired valve fail");*/
// 			_sGSM_Query.SMS_Alert &=  ~_eVALVE_FEEDBACK_FAIL;
// 		}
// 		
// 		if(_sGSM_Query.SMS_Alert &  _eSMS_ALERT_ON)
// 		{
// 			_kSEND_SMS(_sNetwork_and_Usr_Info.User_Sim_No,"SMS ACTIVE");
// 			_sGSM_Query.SMS_Alert &=  ~_eSMS_ALERT_ON;
// 		}
// 	}
// 	else
// 	{
// 		_sGSM_Query.SMS_Alert = 0;
// 	}
	
}
