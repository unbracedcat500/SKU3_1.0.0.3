/*
 * Extra_Plot_Control.cpp
 *
 * Created: 01-07-2022 13:16:11
 *  Author: Tushar (SIO)
 */ 

#include <Initialize.h>


char Temperature_Control(void)
{
// 	/*****     check  humidity and temperature of selcted plot   ******/
// 	static unsigned char retry_fogger_time;
// 	static unsigned char operation_status = _eOPERATION_STAT_SUSPENDED;
// 	//static unsigned char check_user_input;
// 		
// 	switch(operation_status)
// 	{
// 		case _eOPERATION_STAT_SUSPENDED:
// 		{
// 			//if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_MANUAL)
// 			//{
// 				if(_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
// 				{
// 					if(_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot)
// 					{
// 						if(_sLira_Node_Param[_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Packet_ID)
// 						{
// 							if(_sLira_Node_Param[_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Lira_Sensor_Number)
// 							{
// 								if((_sLira_Node_Param[_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Humidity > _sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[1] && _sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[1] != 0 ) ||
// 								(_sLira_Node_Param[_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Air_Temperature > _sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[0] && _sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[0]))
// 								{
// 						
// 									//if(_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer == _kRESET)
// 									//{
// 										if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_AUTO)
// 										{
// 											operation_status = _eOPERATION_STAT_RUNNING;
// 											if(!Extra_Plot_Valve_Grouping_Handler(_kON, _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 											{
// 								
// 											}
// 											_kSERAIL_MON_WRITE("fan start: ");
// 											_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kSET;
// 									
// 											/////////////////////////////////////////////////////////////////
// 											 _sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 											_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 											_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 											_sRuble_Parameters.Extra_Plot_Feedback = _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 											_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 											/////////////////////////////////////////////////////////////////
// 										}
// 									//}
// 								}
// 							}
// 							else
// 							{
// 						
// 							}
// 						}
// 					}
// 				}
// 				else
// 				{
// 					//_kSERAIL_MON_WRITE("fan user input: ");
// 				}
// 			//}
// 		}break;
// 			
// 		case _eOPERATION_STAT_RUNNING:
// 		{
// 			if(_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
// 			{
// 				if(_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot)
// 				{
// 					if(_sLira_Node_Param[_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Lira_Sensor_Number)
// 					{
// 						if((_sLira_Node_Param[_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Humidity < _sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[1] && _sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[1] != 0 ) &&
// 						(_sLira_Node_Param[_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Air_Temperature < _sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[0] && _sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[0]))
// 						{
// 							operation_status = _eOPERATION_STAT_SUSPENDED;
// 							if(!Extra_Plot_Valve_Grouping_Handler(_kOFF, _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 							{
// 								
// 							}
// 							_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 							_kSERAIL_MON_WRITE("fan off 1: ");
// 							
// 							/////////////////////////////////////////////////////////////////
// 							_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 							_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 							_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 							_sRuble_Parameters.Extra_Plot_Feedback = _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 							_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 							/////////////////////////////////////////////////////////////////
// 						}
// 						else
// 						{
// 							//_kSERAIL_MON_WRITE("fan automated: ");
// 							
// 							if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_AUTO)
// 							{
// 								operation_status = _eOPERATION_STAT_SUSPENDED;
// 								if(!Extra_Plot_Valve_Grouping_Handler(_kOFF, _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 								{
// 									
// 								}
// 								_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 								_kSERAIL_MON_WRITE("fan off 3 manual mode selected: ");
// 								
// 								/////////////////////////////////////////////////////////////////
// 								_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 								_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 								_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 								_sRuble_Parameters.Extra_Plot_Feedback = _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 								_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 								/////////////////////////////////////////////////////////////////
// 							}
// 						}
// 					}
// 					
// 					
// 				}
// 				else
// 				{
// 					Extra_Plot_Valve_Grouping_Handler(_kOFF, _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM);
// 					operation_status = _eOPERATION_STAT_SUSPENDED;
// 					_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 					_kSERAIL_MON_WRITE("fan off 2: ");
// 					
// 					/////////////////////////////////////////////////////////////////
// 					_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 					_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 					_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 					_sRuble_Parameters.Extra_Plot_Feedback = _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 					_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 					/////////////////////////////////////////////////////////////////
// 				}
// 			}
// 			else
// 			{
// 				_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 				operation_status = _eOPERATION_STAT_SUSPENDED;
// 				_kSERAIL_MON_WRITE("fan suspend 1: ");
// 				
// 				/////////////////////////////////////////////////////////////////
// 				_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 				_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 				_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 				_sRuble_Parameters.Extra_Plot_Feedback = _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 				_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 				/////////////////////////////////////////////////////////////////
// 			}
// 		}break;
// 			
// 		default:
// 		{
// 			_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 			operation_status = _eOPERATION_STAT_SUSPENDED;
// 			_kSERAIL_MON_WRITE("fan suspend 2: ");
// 			
// 			/////////////////////////////////////////////////////////////////
// 			_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 			_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 			_sExtra_Plot[_kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 			_sRuble_Parameters.Extra_Plot_Feedback = _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 			_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 			/////////////////////////////////////////////////////////////////
// 		}break;
// 	}
}

char Humidity_Control(void)
{
// 	/*****     check  humidity and temperature of selcted plot   ******/
// 	static unsigned char operation_status = _eOPERATION_STAT_SUSPENDED;
// 	static unsigned int resume_operation_timer;
// 	static unsigned char previous_packet_number = 0;
// 	
// 	switch(operation_status)
// 	{
// 		case _eOPERATION_STAT_SUSPENDED:
// 		{
// 			//if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_AUTO)
// 			//{
// 				//_kSERAIL_MON_WRITE("fogger on 1: ");
// 				if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
// 				{
// 					//_kSERAIL_MON_WRITE("fogger on 2: ");
// 					if(resume_operation_timer == _kRESET)
// 					{
// 						//_kSERAIL_MON_WRITE("fogger on 3: ");
// 						if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot && _sRuble_Parameters.Irrigating_Plot == _kRESET)
// 						{
// 							//_kSERAIL_MON_WRITE("fogger on 4: ");
// 							if(_sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Packet_ID)
// 							{
// 								//_kSERAIL_MON_WRITE("fogger on 5: ");
// 								if(_sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Lira_Sensor_Number)
// 								{
// 									//_kSERAIL_MON_WRITE("fogger on 6: ");
// 									if(previous_packet_number != _sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Packet_ID)
// 									{
// 										//_kSERAIL_MON_WRITE("fogger on 7: ");
// 										previous_packet_number = _sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Packet_ID;
// 									
// 										if((_sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Humidity < _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[1] && _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[1] != 0 ) ||
// 										(_sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Air_Temperature > _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[0] && _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[0] !=0))
// 										{
// 											//_kSERAIL_MON_WRITE("fogger on 8: ");
// 											if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_AUTO)
// 											{	
// 												if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
// 												{
// 													//_kSERAIL_MON_WRITE("fogger on 8: ");
// 													if(Extra_Plot_Valve_Grouping_Handler(_kON, _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 													{
// 														//_kSERAIL_MON_WRITE("fogger on 9: ");
// 														operation_status = _eOPERATION_STAT_RUNNING;
// 														_sRuble_Parameters.Start_Monitoring_Pump_Faults_Timer = _kRESET;
// 														_sPump_Parameter.Irrigation_Status = _kRESET;
// 											
// 														if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
// 														{
// 															if((_kGPIO_GET(_kPUMP_1) != _kPUMP_ON))     /* check weather pump is already ON */
// 															{
// 																_kGPIO_SET(_kPUMP_1,_kPUMP_ON);      /* turn ON the pump */
// 															}
// 															
// 															_kGPIO_SET(_kPUMP_2,_kPUMP_ON);
// 															_kDELAY_MS(1000);
// 															_kGPIO_SET(_kPUMP_2,_kPUMP_OFF);
// 														}
// 														else
// 														{
// 															
// 															if(Wireless_Pump_On_Off(_kON))
// 															{
// 																_sRuble_Parameters.Wirelss_Pump_Status = _kON;
// 																_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 																_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kSET;
// 																_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Run_Timer = _kFOGGER_RUN_TIMER;
// 																
// 																_kSERAIL_MON_WRITE("fogger start : ");
// 																
// 																/////////////////////////////////////////////////////////////////
// 																_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 																_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 																_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 																_sRuble_Parameters.Extra_Plot_Feedback = _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 																_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 																/////////////////////////////////////////////////////////////////
// 																//return 1;
// 															}
// 															else
// 															{
// 																resume_operation_timer = _kRESUME_OPERATION_FOGGER;
// 															}
// 														}
// 									
// 														
// 													}
// 													else
// 													{
// 														resume_operation_timer = _kRESUME_OPERATION_FOGGER;
// 													}
// 												}
// 											}
// 										}
// 							
// 										}
// 										else
// 										{
// 											//_kSERAIL_MON_WRITE_NO_LN("fogger fail to start : ");
// 											//resume_operation_timer = _kRESUME_OPERATION_FOGGER;
// 										}
// 								}
// 							}
// 						}
// 					}
// 					else
// 					{
// 						resume_operation_timer--;
// 					}
// 				}
// 				else
// 				{
// 					//_kSERAIL_MON_WRITE("fogger user input: ");
// 				}
// 			//}
// 		}break;
// 		
// 		case _eOPERATION_STAT_RUNNING:
// 		{
// 			if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
// 			{
// 				/************************************** disabled the automation **********************************/
// 				if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot)
// 				{
// 					//_kSERAIL_MON_WRITE("fogger off 1: ");
// 					if(_sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Lira_Sensor_Number)
// 					{
// 						//_kSERAIL_MON_WRITE("fogger off 2: ");
// 						if((_sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Humidity > _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[1] && _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[1] != 0 ) &&
// 						(_sLira_Node_Param[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Air_Temperature < _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[0] && _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[0] !=0 )
// 						|| _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Run_Timer == _kRESET)
// 						{	
// 							//_kSERAIL_MON_WRITE("fogger off 3: ");
// 							operation_status = _eOPERATION_STAT_SUSPENDED;
// 						    
// 							if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
// 							{
// 								if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
// 								{
// 									if(_kGPIO_GET(_kPUMP_1) != _kPUMP_OFF)     /* check weather pump is already Off */
// 									{
// 										_kGPIO_SET(_kPUMP_1,_kPUMP_OFF);      /* turn Off the pump */
// 									}
// 								}
// 								else
// 								{
// 									if(Wireless_Pump_On_Off(_kOFF))
// 									{
// 										_sRuble_Parameters.Pending_Plots[13] = _kRESET;
// 										_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
// 										_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 										
// 										if(!Extra_Plot_Valve_Grouping_Handler(_kOFF, _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 										{
// 											
// 										}
// 									}
// 									else
// 									{
// 										_sRuble_Parameters.Pending_Plots[13] = _kSET;
// 										_sRuble_Parameters.Wirelss_Pump_Status = _kON;
// 										_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 										/* pump faults */
// 									}
// 								}
// 							}
// 							
// 							_kSERAIL_MON_WRITE("fogger off 4: ");
// 							_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 							
// 							  /////////////////////////////////////////////////////////////////
// 							  _sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 							  _sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 							  _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 							  _sRuble_Parameters.Extra_Plot_Feedback = _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 							  _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 							  /////////////////////////////////////////////////////////////////
// 						}
// 						else
// 						{
// 							//_kSERAIL_MON_WRITE("fogger off 5: ");
// 							//_kSERAIL_MON_WRITE("fogger automated: ");
// 							/************************** irrigation related fault occure **********************/
// 							if(_sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_DRY_RUN || _sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_LINE_FAULT ||
// 							_sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_OVER_LOAD  || _sPump_Parameter.Irrigation_Status == _eTHREE_PHASE_POWER_INT ||
// 							_sPump_Parameter.Irrigation_Status == _eEURO_BATTERY_LOW 
// 							|| _sRuble_Parameters.Irrigating_Plot != _kRESET)
// 							{
// 								_kSERAIL_MON_WRITE("fogger off 6: ");
// 								operation_status = _eOPERATION_STAT_SUSPENDED;
// 								
// 								if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
// 								{
// 									if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
// 									{
// 										if(_kGPIO_GET(_kPUMP_1) != _kPUMP_OFF)     /* check weather pump is already Off */
// 										{
// 											_kGPIO_SET(_kPUMP_1,_kPUMP_OFF);      /* turn Off the pump */
// 										}
// 									}
// 									else
// 									{
// 										if(Wireless_Pump_On_Off(_kOFF))
// 										{
// 											_sRuble_Parameters.Pending_Plots[13] = _kRESET;
// 											_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
// 											_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 											
// 											if(!Extra_Plot_Valve_Grouping_Handler(_kOFF, _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 											{
// 												
// 											}
// 										}
// 										else
// 										{
// 											_sRuble_Parameters.Pending_Plots[13] = _kSET;
// 											_sRuble_Parameters.Wirelss_Pump_Status = _kON;
// 											_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 											/* pump faults */
// 										}
// 									}
// 								}
// 								
// 								_kSERAIL_MON_WRITE("fogger off 7: ");
// 								_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 								
// 								if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
// 								{
// 									resume_operation_timer = _kRESUME_OPERATION_FOGGER;
// 								}
// 								
// 								  /////////////////////////////////////////////////////////////////
// 								  _sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 								  _sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 								  _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 								  _sRuble_Parameters.Extra_Plot_Feedback = _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 								  _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 								  /////////////////////////////////////////////////////////////////  
// 							 }
// 							 
// 							 else if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_AUTO ||
// 							 _sRuble_Parameters.Irrigating_Plot != _kRESET)
// 							 {
// 								 _kSERAIL_MON_WRITE("fogger off 6.1: ");
// 								 operation_status = _eOPERATION_STAT_SUSPENDED;
// 								 if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
// 								 {
// 									 
// 									 if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
// 									 {
// 										 if(_kGPIO_GET(_kPUMP_1) != _kPUMP_OFF)     /* check weather pump is already Off */
// 										 {
// 											 _kGPIO_SET(_kPUMP_1,_kPUMP_OFF);      /* turn Off the pump */
// 										 }
// 									 }
// 									 else
// 									 {
// 										 if(Wireless_Pump_On_Off(_kOFF))
// 										 {
// 											 _sRuble_Parameters.Pending_Plots[13] = _kRESET;
// 											 _sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
// 											 _kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 											 
// 											 if(!Extra_Plot_Valve_Grouping_Handler(_kOFF, _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 											 {
// 												 
// 											 }
// 										 }
// 										 else
// 										 {
// 											 _sRuble_Parameters.Pending_Plots[13] = _kSET;
// 											 _sRuble_Parameters.Wirelss_Pump_Status = _kON;
// 											 _kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 											 /* pump faults */
// 										 }
// 									 }
// 								 }
// 								 
// 								 _kSERAIL_MON_WRITE("fogger off 7.2: ");
// 								 _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 								 
// 								 /////////////////////////////////////////////////////////////////
// 								 _sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 								 _sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 								 _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 								 _sRuble_Parameters.Extra_Plot_Feedback = _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 								 _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 								 ///////////////////////////////////////////////////////////////// 
// 							 }
// 						}
// 					}
// 				}
// 				else
// 				{
// 					_kSERAIL_MON_WRITE("fogger off 8: ");
// 					if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
// 					{
// 						if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
// 						{
// 							if(_kGPIO_GET(_kPUMP_1) != _kPUMP_OFF)     /* check weather pump is already Off */
// 							{
// 								_kGPIO_SET(_kPUMP_1,_kPUMP_OFF);      /* turn Off the pump */
// 							}
// 						}
// 						else
// 						{
// 							if(Wireless_Pump_On_Off(_kOFF))
// 							{
// 								_sRuble_Parameters.Pending_Plots[13] = _kRESET;
// 								_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
// 								_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 								
// 								if(!Extra_Plot_Valve_Grouping_Handler(_kOFF, _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 								{
// 									
// 								}
// 							}
// 							else
// 							{
// 								_sRuble_Parameters.Pending_Plots[13] = _kSET;
// 								_sRuble_Parameters.Wirelss_Pump_Status = _kON;
// 								_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
// 								/* pump faults */
// 							}
// 						}
// 					}
// 					
// 					_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 					operation_status = _eOPERATION_STAT_SUSPENDED;	
// 					_kSERAIL_MON_WRITE("fogger off 3: ");
// 					
// 					/////////////////////////////////////////////////////////////////
// 					_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 					_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 					_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 					_sRuble_Parameters.Extra_Plot_Feedback = _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 					_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 					/////////////////////////////////////////////////////////////////
// 				}
// 			}
// 			else
// 			{
// 				_kSERAIL_MON_WRITE("fogger suspend 0: ");
// 				_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 				operation_status = _eOPERATION_STAT_SUSPENDED;
// 				
// 				/////////////////////////////////////////////////////////////////
// 				_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 				_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 				_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 				_sRuble_Parameters.Extra_Plot_Feedback = _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 				_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 				/////////////////////////////////////////////////////////////////
// 			}
// 		}break;
// 		
// 		default:
// 		{
// 			_kSERAIL_MON_WRITE("fogger suspend 1: ");
// 			operation_status = _eOPERATION_STAT_SUSPENDED;
// 			_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 			
// 			/////////////////////////////////////////////////////////////////
// 			_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 			_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 			_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 			_sRuble_Parameters.Extra_Plot_Feedback = _kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 			_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 			/////////////////////////////////////////////////////////////////
// 		}break;
// 	}
	
}

char Light_Control(void)
{
// 		/*****     check  humidity and temperature of selcted plot   ******/
// 		static unsigned char operation_status = _eOPERATION_STAT_SUSPENDED;
// 		
// 		switch(operation_status)
// 		{
// 			case _eOPERATION_STAT_SUSPENDED:
// 			{
// 				//if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_MANUAL)
// 				//{
// 					if(_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
// 					{
// 						if(_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot)
// 						{
// 							if(_sLira_Node_Param[_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Packet_ID)
// 							{
// 								if(_sLira_Node_Param[_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Lira_Sensor_Number)
// 								{
// 									if((_sLira_Node_Param[_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Light_Intensity < _sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Min_Valve[0]) )
// 									{
// 										if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_AUTO)
// 										{
// 							
// 											//if(_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer == _kRESET)
// 											//{
// 												operation_status = _eOPERATION_STAT_RUNNING;
// 												if(!Extra_Plot_Valve_Grouping_Handler(_kON, _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 												{
// 										
// 												}
// 												_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kSET;
// 											//}
// 											_kSERAIL_MON_WRITE("light start: ");
// 									
// 											/////////////////////////////////////////////////////////////////
// 											_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 											_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 											_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 											_sRuble_Parameters.Extra_Plot_Feedback = _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 											_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 											/////////////////////////////////////////////////////////////////
// 										}
// 									}
// 									else
// 									{
// 							
// 									}
// 								}
// 							}
// 						}
// 					}
// 					else
// 					{
// 						//_kSERAIL_MON_WRITE("light user input: ");
// 					}
// 				//}
// 			}break;
// 			
// 			case _eOPERATION_STAT_RUNNING:
// 			{
// 				if(_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
// 				{
// 					if(_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot)
// 					{
// 						if(_sLira_Node_Param[_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Lira_Sensor_Number)
// 						{
// 							if(_sLira_Node_Param[_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Selected_Plot-1].Light_Intensity > _sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Max_Value[0])
// 							{
// 								operation_status = _eOPERATION_STAT_SUSPENDED;
// 								if(!Extra_Plot_Valve_Grouping_Handler(_kOFF, _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 								{
// 							
// 								}
// 								_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 								_kSERAIL_MON_WRITE("light off 1: ");
// 								
// 								/////////////////////////////////////////////////////////////////
// 								_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 								_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 								_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 								_sRuble_Parameters.Extra_Plot_Feedback = _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 								_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 								/////////////////////////////////////////////////////////////////
// 							}
// 							else
// 							{
// 									//_kSERAIL_MON_WRITE("light is automated : ");
// 									
// 									if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_AUTO)
// 									{
// 										operation_status = _eOPERATION_STAT_SUSPENDED;
// 										if(!Extra_Plot_Valve_Grouping_Handler(_kOFF, _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
// 										{
// 											
// 										}
// 										_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 										_kSERAIL_MON_WRITE("light off 3 manual mode: ");
// 										
// 										/////////////////////////////////////////////////////////////////
// 										_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 										_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 										_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 										_sRuble_Parameters.Extra_Plot_Feedback = _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 										_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 										/////////////////////////////////////////////////////////////////
// 									}
// 							}
// 						}
// 						else
// 						{
// 							//if(_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer)
// 							//{
// 							//	operation_status = _eOPERATION_STAT_SUSPENDED;
// 							//}
// 						}
// 					}
// 					else
// 					{
// 						Extra_Plot_Valve_Grouping_Handler(_kOFF, _kFAN_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM);
// 						operation_status = _eOPERATION_STAT_SUSPENDED;
// 						_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 						_kSERAIL_MON_WRITE("light off 2: ");
// 						
// 						/////////////////////////////////////////////////////////////////
// 						_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 						_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 						_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 						_sRuble_Parameters.Extra_Plot_Feedback = _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 						_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 						/////////////////////////////////////////////////////////////////
// 					}
// 				}
// 				else
// 				{
// 					_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 					operation_status = _eOPERATION_STAT_SUSPENDED;
// 					_kSERAIL_MON_WRITE("light suspend 1: ");
// 					
// 					/////////////////////////////////////////////////////////////////
// 					_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 					_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 					_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 					_sRuble_Parameters.Extra_Plot_Feedback = _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 					_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 					/////////////////////////////////////////////////////////////////
// 				}
// 				
// 			}break;
// 			
// 			default:
// 			{
// 				operation_status = _eOPERATION_STAT_SUSPENDED;
// 				_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation = _kRESET;
// 				_kSERAIL_MON_WRITE("light suspend 2: ");
// 				
// 				/////////////////////////////////////////////////////////////////
// 				_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
// 				_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 				_sExtra_Plot[_kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Set_Through_Timer = _kRESET;
// 				_sRuble_Parameters.Extra_Plot_Feedback = _kLIGHT_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 				_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 				/////////////////////////////////////////////////////////////////
// 			}break;
// 		}
 }
 
unsigned char Update_Pump_List(void)
{
	if(_sRuble_Parameters.Pump_Selected_For_Irrigation)
	{
		if(_sRuble_Parameters.Wirelss_Pump_Status == _kON)
		{
			/* update list */
			_sRuble_Parameters.Wireless_Pump_Handle_List[_sRuble_Parameters.Pump_Selected_For_Irrigation -1] = _kON;
			_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
		}
		else
		{
			if(_sRuble_Parameters.Wirelss_Pump_Status == _kOFF)
			{
				_sRuble_Parameters.Wireless_Pump_Handle_List[_sRuble_Parameters.Pump_Selected_For_Irrigation -1] = _kOFF;
				_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
			}
		}
	}
}

unsigned char Turn_Off_All_Pump_For_Safety(void)
{
	for(unsigned char i=0; i<_kMAX_WIRELESS_PUMP;i++)
	{
		if(_sWireless_Pump_Param[i].Wireless_Pump_Number > 0 && _sWireless_Pump_Param[i].Wireless_Pump_Number <= _kMAX_WIRELESS_PUMP)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("wl pump trigger 222222222222222222");
				_kSERIAL_MON_CLEAR();
			}
			Wireless_Pump_On_Off(_ePUMP_OFF,i);
			//Wireless_Pump_On_Off(i,_kOFF);
		}
	}
}

unsigned char Wireless_Motor_Enable_Disable_Check(void)
{
	if(_sExtra_Plot[_kWIRELESS_PUMP_ENABLE_DISABLE_PLOT - _kEXTRA_PLOT_START_FROM].Extra_Plot_Number == _kWIRELESS_PUMP_ENABLE_DISABLE_PLOT)
	{
		if(_sExtra_Plot[_kWIRELESS_PUMP_ENABLE_DISABLE_PLOT - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kSET &&
		_sExtra_Plot[_kWIRELESS_PUMP_ENABLE_DISABLE_PLOT - _kEXTRA_PLOT_START_FROM].Extra_Plot_Number != 0)
		{
			switch(_sExtra_Plot[_kWIRELESS_PUMP_ENABLE_DISABLE_PLOT - _kEXTRA_PLOT_START_FROM].Euro_Valve_Group[0])
			{
				case 101:
				{
					
					_sRuble_Parameters.Pump_Selected_For_Irrigation = 2;
				}break;
				
				case 102:
				{
					_sRuble_Parameters.Pump_Selected_For_Irrigation = 3;

				}break;
				
				case 103:
				{
					_sRuble_Parameters.Pump_Selected_For_Irrigation = 4;
				}break;
				
				default:
				{
					_sRuble_Parameters.Pump_Selected_For_Irrigation = 2;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Selected valve number wireless pump > ");
						_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_kWIRELESS_PUMP_ENABLE_DISABLE_PLOT - _kEXTRA_PLOT_START_FROM].Euro_Valve_Group[0]);
						_kSERIAL_MON_CLEAR();
						_kSERAIL_MON_WRITE("Selected wireless pump > ");
						_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Pump_Selected_For_Irrigation);
						_kSERIAL_MON_CLEAR();
					}
				}break;
			}
			
			if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("wireless pump 3....................,,,,,,, ");
					_kSERIAL_MON_CLEAR();
				}
				_sRuble_Parameters.Wireless_Pump_Command = _eWIRELESS_PUMP_WAIT;
				_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
			}
		}
		else
		{
			_sRuble_Parameters.Pump_Selected_For_Irrigation = 1;
			if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("wireless pump 4 ##########################");
					_kSERIAL_MON_CLEAR();
				}
				_sRuble_Parameters.Wireless_Pump_Command = _kRESET;
				_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
				Valve_Grouping_Handler(_kOFF_FOR_WIRELESS_SAFETY);
			}
		}
	}
	else
	{
		_sRuble_Parameters.Pump_Selected_For_Irrigation = 1;
		
		if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
		{
			_sRuble_Parameters.Pump_Selected_For_Irrigation = 1;
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("wireless pump 5 ");
				_kSERIAL_MON_CLEAR();
			}
			_sRuble_Parameters.Wireless_Pump_Command = _kRESET;
			_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
			Valve_Grouping_Handler(_kOFF_FOR_WIRELESS_SAFETY);
		}
	}
	
	
	
	
	
	if(_sRuble_Parameters.Pump_Selected_For_Irrigation > 1)
	{
		if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("wireless pump 3 ");
				_kSERIAL_MON_CLEAR();
			}
			_sRuble_Parameters.Wireless_Pump_Command = _eWIRELESS_PUMP_WAIT;
			_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
		}
	}
	else
	{
		/* Turn on wired motor */
		if(_sRuble_Parameters.Pump_Selected_For_Irrigation == 1)
		{
			if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("wireless pump 5 ");
					_kSERIAL_MON_CLEAR();
				}
				_sRuble_Parameters.Wireless_Pump_Command = _kRESET;
				_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
				// ######################### what should to to switch the pump ###################// 
				//Valve_Grouping_Handler(_kOFF_FOR_WIRELESS_SAFETY);
			}
		}
	}
}

char Valve_Grouping_Handler_For_wireless_Pump(unsigned char Target_Plot_Num, unsigned char on_off)
{
	unsigned char i,j,k,count=0,current_valve_number,compare_id,id_len,data_send_counter = 0,temp_data[30];
	unsigned char valve_number_list[_kMAX_LIRA_SENSOR_NODE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO], total_valve_number,valve_number_not_match_flag=0;
	unsigned char hex_array[] = {0x01,0x02,0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	
	/******************************************* check weather all valves status is off before starts the irrigation *******************************/
	
	memset((char*)valve_number_list,0,sizeof(valve_number_list));
	total_valve_number = _kRESET;
	valve_number_not_match_flag = _kRESET;
	
	for(i=0;i<_kMAX_LIRA_SENSOR_NODE;i++)
	{
		if(_sLira_Node_Param[i].Lira_Sensor_Number > 0 && _sLira_Node_Param[i].Lira_Sensor_Number < _kMAX_LIRA_SENSOR_NODE+1)
		{
			for(j=0;j<_kMAX_VALVE_FOR_1PLOT;j++)
			{
				if(valve_number_list[0] != 0)    // if list is empty
				{
					for(k=0;valve_number_list[k]!=0;k++)
					{
						if(_sLira_Node_Param[i].Euro_Valve_Group[j])
						{
							if(valve_number_list[k] == _sLira_Node_Param[i].Euro_Valve_Group[j])
							{
								break;
							}
							
							if(valve_number_list[k] != _sLira_Node_Param[i].Euro_Valve_Group[j] && valve_number_list[k+1] == 0)
							{
								valve_number_not_match_flag = _kSET;
							}
						}
					}
				}
				else
				{
					if(_sLira_Node_Param[i].Euro_Valve_Group[j])   // check weather valve is there or not
					{
						valve_number_not_match_flag = _kSET;
					}
				}
				if(valve_number_not_match_flag)
				{
					if(_sLira_Node_Param[i].Euro_Valve_Group[j] <= _kMAX_EURO_VALVE * _kNUMBER_OF_VALVE_CONNECT_TO_EURO)
					{
						valve_number_list[total_valve_number] = _sLira_Node_Param[i].Euro_Valve_Group[j];
						total_valve_number++;
					}
					
				}
				valve_number_not_match_flag = _kRESET;
			}
			
		}
	}
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("valve list is : ");
		_kSERIAL_MON_CLEAR();
	
		for(i=0;valve_number_list[i]!=0;i++)
		{
			_kSERAIL_MON_WRITE("valve list enter ");
			_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE((int )valve_number_list[i]);
			Serial.println();
			_kSERIAL_MON_CLEAR();
		}
	}
	
	
	
	
	if(on_off == _kFORCEFULYY_OFF)
	{
		valve_number_not_match_flag = _kRESET;
		for(i=0;i<_kMAX_EXTRA_PLOT;i++)
		{
			if(_sExtra_Plot[i].Extra_Plot_Number > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE) && _sExtra_Plot[i].Extra_Plot_Number < (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+_kMAX_EXTRA_PLOT+1))
			{
				for(j=0;j<_kMAX_VALVE_FOR_1PLOT;j++)
				{
					if(valve_number_list[0] != 0)    // if list is empty
					{
						for(k=0;valve_number_list[k]!=0;k++)
						{
							if(_sExtra_Plot[i].Euro_Valve_Group[j])
							{
								if(valve_number_list[k] == _sExtra_Plot[i].Euro_Valve_Group[j])
								{
									break;
								}
								
								if(valve_number_list[k] != _sExtra_Plot[i].Euro_Valve_Group[j] && valve_number_list[k+1] == 0)
								{
									valve_number_not_match_flag = _kSET;
								}
							}
						}
					}
					else
					{
						if(_sExtra_Plot[i].Euro_Valve_Group[j])   // check weather valve is there or not
						{
							valve_number_not_match_flag = _kSET;
						}
					}
					if(valve_number_not_match_flag)
					{
						if(_sExtra_Plot[i].Euro_Valve_Group[j] <= _kMAX_EURO_VALVE * _kNUMBER_OF_VALVE_CONNECT_TO_EURO)
						{
							valve_number_list[total_valve_number] = _sExtra_Plot[i].Euro_Valve_Group[j];
							total_valve_number++;
						}
						
					}
					valve_number_not_match_flag = _kRESET;
				}
				
			}
		}
		
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("valve list is : ");
			_kSERIAL_MON_CLEAR();
		
			for(i=0;valve_number_list[i]!=0;i++)
			{
				_kSERAIL_MON_WRITE("valve list enter ");
				_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE((int )valve_number_list[i]);
				Serial.println();
				_kSERIAL_MON_CLEAR();
			}
		}
	}
	
	
	if(on_off == _kFORCEFULYY_OFF || on_off == _kOFF_FOR_WIRELESS_SAFETY)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Check valve that all valves are OFF or not");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		for(i=0;i<_kMAX_EURO_VALVE-1;i++)
		{
			total_valve_number = _kRESET;
			if(_sEuro_Valve_Param[i].Euro_Valve_Number)
			{
				for(k=0;valve_number_list[k]!=0;k++)
				{
					for(j=0;j < _kNUMBER_OF_VALVE_CONNECT_TO_EURO;j++)
					{
						if(valve_number_list[k] == (i*_kNUMBER_OF_VALVE_CONNECT_TO_EURO) + (j+1))
						{
							total_valve_number |= hex_array[j];
						}
					}
				}
				
				
				for(k=0;k<_kNUMBER_OF_VALVE_CONNECT_TO_EURO;k++)
				{
					if(total_valve_number & hex_array[k])
					{
						data_send_counter = 0;
						while(data_send_counter < _kRETRY_TO_SEND_VALVE)
						{
							data_send_counter++;
							/****************** if any valve on then turn off that valve first *******************************/
							memset((char *)_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
							count = 0;
							
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[0];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[1];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[2];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[3];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[4];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[5];
							
							_sRuble_Parameters.Lora_Data_TX[count++] = _eEURO_VALVE_COMMAND;
							
							
							_sRuble_Parameters.Lora_Data_TX[count++] = _kOFF;
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Operation_Mode;
							_sRuble_Parameters.Lora_Data_TX[count++] = 0;
							_sRuble_Parameters.Lora_Data_TX[count++] = 0;
							_sRuble_Parameters.Lora_Data_TX[count++] = k+1;
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("Send On/off Command to EURO valve ");
								_kSERIAL_MON_PRINT_NUM(i+1);
								_kSERIAL_MON_CLEAR();
								_kSERAIL_MON_WRITE_NO_LN("Test only TX data : ");
								_kSERIAL_MON_WRITE((char *)_sRuble_Parameters.Lora_Data_TX,count);
								Serial.println();
								_kSERIAL_MON_CLEAR();
							}
							
							Display_Update_1();   // 20/1/2022
							_kLORA_SEND_DATA(count);
							
							
							
							if(_kCHEK_DATA_AVAILABLE_TIMEOUT(3000))    /* wait for 3 sec to wait the data from the EURO valve */
							{
								_kLORA_RECV_DATA();            /* check the received data */
								
								//RF95_setModeRx_Continuous();
								
								id_len = _kRESET;
								for(compare_id=0;compare_id<_kVALVE_ID_LEN;compare_id++)
								{
									if(_sEuro_Valve_Param[i].Euro_Valve_ID[compare_id] != _sRuble_Parameters.Lora_Data_Rx[compare_id])
									{
										compare_id = _kVALVE_ID_LEN;
										id_len = _kRESET;
									}
									id_len++;
								}
								
								if(id_len == _kVALVE_ID_LEN)
								{
									_sEuro_Valve_Param[i].Euro_Valve_Online = _kVALVE_ONLINE;    /* valve online if got the data from that valve */
									_sEuro_Valve_Param[i].Euro_Valve_Status =  _sRuble_Parameters.Lora_Data_Rx[8];
									_sEuro_Valve_Param[i].Euro_Battery_Level = _sRuble_Parameters.Lora_Data_Rx[10];    /* copy the batter voltage */
									_sEuro_Valve_Param[i].Euro_Battery_Level |= _sRuble_Parameters.Lora_Data_Rx[9] << 8;    /* copy the batter voltage */
									
									if(_sEuro_Valve_Param[i].Euro_Valve_Status != _kOFF)
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("fail to off the valve(was ealier on)");
											_kSERIAL_MON_CLEAR();
										}
									}
									else
									{
										data_send_counter = _kRETRY_TO_SEND_VALVE;
									}
								}
								else
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("valve id not match(before start)");
										_kSERIAL_MON_CLEAR();
									}
								}
							}
							else
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("valve fail to off which was already on");
									_kSERIAL_MON_CLEAR();
								}
							}
							_kDELAY_MS(1000);
						}
					}
				}
			}
			
		}
	}
	
	if(on_off == _kFORCEFULYY_OFF || on_off == _kOFF_FOR_WIRELESS_SAFETY)
	{
		if(on_off == _kOFF_FOR_WIRELESS_SAFETY)
		{
			for(unsigned char plot_numbers=0; plot_numbers<_kMAX_LIRA_SENSOR_NODE; plot_numbers++)
			{
				for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
				{
					if(_sLira_Node_Param[plot_numbers].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
					{
						/********************************* turn on/off the wired valve *********************************/
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[plot_numbers].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
						_kDELAY_MS(200);
					}
				}
			}
			
			if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Extra_Plot_Number == _kFOOGER_PLOT_NUMBER)
			{
				for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
				{
					if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
					{
						/********************************* turn on/off the wired valve *********************************/
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
						_kDELAY_MS(200);
					}
				}
			}
		}
		return 1;
	}
	
	
	
	
	
	if(on_off == _kON)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Check valve that all valves are OFF or not");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		for(i=0;i<_kMAX_EURO_VALVE;i++)
		{
			total_valve_number = _kRESET;
			if(_sEuro_Valve_Param[i].Euro_Valve_Status != _kOFF)
			{
				for(k=0;valve_number_list[k]!=0;k++)
				{
					for(j=0;j < _kNUMBER_OF_VALVE_CONNECT_TO_EURO;j++)
					{
						if(valve_number_list[k] == (i*_kNUMBER_OF_VALVE_CONNECT_TO_EURO) + (j+1))
						{
							total_valve_number |= hex_array[j];
						}
					}
				}
				
				
				for(k=0;k<_kNUMBER_OF_VALVE_CONNECT_TO_EURO;k++)
				{
					if(total_valve_number & hex_array[k])
					{
						data_send_counter = 0;
						while(data_send_counter < _kRETRY_TO_SEND_VALVE)
						{
							data_send_counter++;
							
							/****************** if any valve on then turn off that valve first *******************************/
							
							memset((char *)_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
							count = 0;
							
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[0];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[1];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[2];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[3];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[4];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[i].Euro_Valve_ID[5];
							
							_sRuble_Parameters.Lora_Data_TX[count++] = _eEURO_VALVE_COMMAND;
							
							_sRuble_Parameters.Lora_Data_TX[count++] = _kOFF;
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Operation_Mode;
							_sRuble_Parameters.Lora_Data_TX[count++] = 0;
							_sRuble_Parameters.Lora_Data_TX[count++] = 0;
							_sRuble_Parameters.Lora_Data_TX[count++] = k+1;
							
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("Send On/off Command to EURO valve ;");
								_kSERIAL_MON_PRINT_NUM(i+1);
								_kSERIAL_MON_CLEAR();
								_kSERAIL_MON_WRITE_NO_LN("Test only TX data : ");
								_kSERIAL_MON_WRITE((char *)_sRuble_Parameters.Lora_Data_TX,6);
								Serial.println();
								_kSERIAL_MON_CLEAR();
							}
							
							Display_Update_1();
							_kLORA_SEND_DATA(count);
							
							
							if(_kCHEK_DATA_AVAILABLE_TIMEOUT(3000))    /* wait for 3 sec to wait the data from the EURO valve */
							{
								_kLORA_RECV_DATA();            /* check the received data */
								//RF95_setModeRx_Continuous();
								
								id_len = _kRESET;
								for(compare_id=0;compare_id<_kVALVE_ID_LEN;compare_id++)
								{
									if(_sEuro_Valve_Param[i].Euro_Valve_ID[compare_id] != _sRuble_Parameters.Lora_Data_Rx[compare_id])
									{
										compare_id = _kVALVE_ID_LEN;
										id_len = _kRESET;
									}
									id_len++;
								}
								
								if(id_len == _kVALVE_ID_LEN)
								{
									_sEuro_Valve_Param[i].Euro_Valve_Online = _kVALVE_ONLINE;    /* valve online if got the data from that valve */
									_sEuro_Valve_Param[i].Euro_Valve_Status =  _sRuble_Parameters.Lora_Data_Rx[8];
									_sEuro_Valve_Param[i].Euro_Battery_Level = _sRuble_Parameters.Lora_Data_Rx[10];    /* copy the batter voltage */
									_sEuro_Valve_Param[i].Euro_Battery_Level |= _sRuble_Parameters.Lora_Data_Rx[9] << 8;    /* copy the batter voltage */
									
									if(_sEuro_Valve_Param[i].Euro_Valve_Status != _kOFF)
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("fail to off the valve(was ealier onn)");
											_kSERIAL_MON_CLEAR();
										}
									}
									else
									{
										data_send_counter = _kRETRY_TO_SEND_VALVE;
									}
								}
								else
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("valve id not match(before startt)");
										_kSERIAL_MON_CLEAR();
									}
								}
							}
							else
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("valve fail to give feedback(Not listening)...");
									_kSERIAL_MON_CLEAR();
								}
							}
							_kDELAY_MS(1000);
						}
					}
				}
			}
		}
	}
	
	
	/************************************************* if everything ok then turn on or off the valves *********************************************/
	
	switch(on_off)
	{
		case _kON:
		case _kOFF:
		{
			/* check weather the plot is already irrigating or not  to stop the irrigation */
			
			if(Target_Plot_Num <= _kMAX_LIRA_SENSOR_NODE )
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("valve list : ");
					_kSERIAL_MON_CLEAR();
				
					for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
					{
						_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i]);
						_kSERAIL_MON_WRITE_NO_LN(" ");
					}
				
					_kSERAIL_MON_WRITE_NO_LN("Target_Plot_Num plot no : ");
					_kSERIAL_MON_PRINT_NUM(Target_Plot_Num-1);
					_kSERIAL_MON_CLEAR();
				}
				
				unsigned char same_valve = _kRESET;
				
				for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
				{
					if(on_off == _kOFF)
					{
						if(Target_Plot_Num != _sRuble_Parameters.Irrigating_Plot)
						{
							same_valve = _kRESET;
							if(_sRuble_Parameters.Irrigating_Plot != 0)
							{
								for(unsigned char check_with_current_plot =0; check_with_current_plot < _kMAX_VALVE_FOR_1PLOT; check_with_current_plot++)
								{
									if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i]
									== _sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[check_with_current_plot])
									{
										same_valve = _kSET;
									}
								}
							}
						}
					}
					else
					{
						same_valve = _kRESET;
					}
					
					if(same_valve == _kRESET)
					{
						
					
					
						if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && _sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i] > 0)
						{
							unsigned char valve_number =0;
							unsigned char euro_valve_num=0,command_valve=0;
						
							valve_number =  _sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i];
						
							if(valve_number <= _kNUMBER_OF_VALVE_CONNECT_TO_EURO)
							{
								euro_valve_num = 1;
								command_valve = valve_number;
							}
							else
							{
								if(valve_number%_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
								{
									euro_valve_num = valve_number/_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1;
									command_valve  = valve_number%_kNUMBER_OF_VALVE_CONNECT_TO_EURO;
								}
								else
								{
									euro_valve_num = valve_number/_kNUMBER_OF_VALVE_CONNECT_TO_EURO;
									command_valve = _kNUMBER_OF_VALVE_CONNECT_TO_EURO;
								}
							
							}
						
							/** ???????????????????????????????????? irrigating plot ????????????????????????????????????????? **/
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("Wireless Valve no. ");
								_kSERIAL_MON_PRINT_NUM(euro_valve_num);
								_kSERIAL_MON_CLEAR();
							}
						
							if(_kLORA_DATA_AVAIALABLE())    /* read if there is already any data received(garbage data) */
							{
								_kLORA_RECV_DATA();         /*  lora rx_tx buffer will update*/
							}
						
							memset((char *)_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
							count = _kRESET;
							/********************************** wireless valve command (on/off) **********************************/
							/* valve id, on/off command, pump run timer */
						
							/************************************* copy the euro valve ID *********************************************/
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][0];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][1];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][2];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][3];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][4];
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][5];
						
							/*********************************** packet_id ***********************************************/
							_sRuble_Parameters.Lora_Data_TX[count++] = _eEURO_VALVE_COMMAND;
						
							/*********************************** copy on/off command ***********************************************/
							_sRuble_Parameters.Lora_Data_TX[count++] = on_off;
						
							/********************************** copy operation mode ************************************************/
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Operation_Mode;
						
							/******************************** copy pump timer (used if selected mode is timer) *********************/
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Pump_Run_Timer << 8;
							_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Pump_Run_Timer;
							_sRuble_Parameters.Lora_Data_TX[count++] = command_valve;
						
							/* check which structure to which EURO valve we are going to update */
							for(j=0;j<_kMAX_EURO_VALVE;j++)
							{
								/************************ compare each valve id one by one with the list of euro valves **************************/
								id_len = _kRESET;
								for(compare_id=0;compare_id<_kVALVE_ID_LEN;compare_id++)
								{
									if(_sEuro_Valve_Param[j].Euro_Valve_ID[compare_id] != _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][compare_id])
									{
										compare_id = _kVALVE_ID_LEN;
										id_len = _kRESET;
									}
									id_len++;
								}
							
								if(id_len == _kVALVE_ID_LEN)
								{
									current_valve_number = j;    /* data send will update of this valve because valve get match */
									break;
								}
							}
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("Current valve number ");
								_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(current_valve_number);
								_kSERIAL_MON_CLEAR();
							}
							
							data_send_counter = _kRESET;
						
							while(data_send_counter < _kRETRY_TO_SEND_VALVE)
							{
								if(Subscribed_data_check())
								{
									if(Seperate_Out_Sub_Data())
									{
										//mqttCallback((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff));
										Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_eSUBSCRIBE_DATA);
									}
								}
								data_send_counter++;
							
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Send On/off Command to EURO valve 1");
									_kSERIAL_MON_CLEAR();
								}
								
								Display_Update_1();    // 20/1/2022
								_kLORA_SEND_DATA(count);
							
								if(_kCHEK_DATA_AVAILABLE_TIMEOUT(3000))    /* wait for 3 sec to wait the data from the EURO valve */
								{
									/**************************** update the received data in buff lora rx_tx **********************/
									if(_kLORA_RECV_DATA())            /* check the received data */
									{
										//RF95_setModeRx_Continuous();
										/* check the id get matched to the valve or not */
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE_NO_LN("Euro valve rx data : ");
											_kSERIAL_MON_WRITE((char *)_sRuble_Parameters.Lora_Data_Rx,10);
											Serial.println();
											_kSERIAL_MON_CLEAR();
									
											_kSERAIL_MON_WRITE_NO_LN("Euro valve ID which is Sent :");
											_kSERIAL_MON_WRITE(_sEuro_Valve_Param[current_valve_number].Euro_Valve_ID,7);
											Serial.println();
											_kSERIAL_MON_CLEAR();
										}
									
										for(j=0;j<_kVALVE_ID_LEN;j++)
										{
											if(_sEuro_Valve_Param[current_valve_number].Euro_Valve_ID[j] != _sRuble_Parameters.Lora_Data_Rx[j])
											{
												if(_sRuble_Parameters.Debug_Mode == _kSET)
												{
													_kSERAIL_MON_WRITE("ID Not matched with valve which is replied");
													_kSERIAL_MON_CLEAR();
												}
											
												if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
												{
													_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
													_gfault_Occure = _kFAULT_RESET_TIMER;
													if(_sRuble_Parameters.Debug_Mode == _kSET)
													{
														_kSERAIL_MON_WRITE("_eIRRIG_STAT_EURO_FEEDBACK 0");
													}
													_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
													_sRuble_Parameters.Last_Valve = current_valve_number;
													_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
												
													for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
													{
														if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
														{
															/********************************* turn on/off the wired valve *********************************/
															_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
															_kDELAY_MS(200);
														}
													}
												
													return 0;      /* id not get matched */
												}
												break;
											}
										}
									
										/* if id get matched */
									
										_sEuro_Valve_Param[current_valve_number].Euro_Valve_Online = _kVALVE_ONLINE;    /* valve online if got the data from that valve */
										_sEuro_Valve_Param[current_valve_number].Euro_Battery_Level = _sRuble_Parameters.Lora_Data_Rx[10];    /* copy the batter voltage */
										_sEuro_Valve_Param[current_valve_number].Euro_Battery_Level |= _sRuble_Parameters.Lora_Data_Rx[9] << 8;    /* copy the batter voltage */
									
										if(_sRuble_Parameters.Lora_Data_Rx[8] != on_off)     /* check weather valve is ON/off or Not */
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("Euro Feedback fail");
												_kSERIAL_MON_CLEAR();
											}
											if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
											{
												_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
												_gfault_Occure = _kFAULT_RESET_TIMER;
												if(_sRuble_Parameters.Debug_Mode == _kSET)
												{
													_kSERAIL_MON_WRITE("_eIRRIG_STAT_EURO_FEEDBACK 1");
												}
												_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
												_sRuble_Parameters.Last_Valve = current_valve_number;
												_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
											
												for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
												{
													if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
													{
														/********************************* turn on/off the wired valve *********************************/
														_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
														_kDELAY_MS(200);
													}
												}
											
												return 0;
											}
										}
										else
										{
											data_send_counter = 3;
										}
									}
									else
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("Euro Feedback fail/Not received any data from Euro valve 1");
											_kSERIAL_MON_CLEAR();
										}
										if(data_send_counter == _kRETRY_TO_SEND_VALVE -1)
										{
											_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
											_gfault_Occure = _kFAULT_RESET_TIMER;
											_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
											_sRuble_Parameters.Last_Valve = current_valve_number;
											_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
											for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
											{
												if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
												{
													/********************************* turn on/off the wired valve *********************************/
													_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
													_kDELAY_MS(200);
												}
											}
											return 0;       /* fail to received the data */
										}
									}
								}
								else
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("Euro Feedback fail/Not received any data from Euro valve 0");
										_kSERIAL_MON_CLEAR();
									}
									//RF95_setModeRx_Continuous();
								
									if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
									{
										_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
										_gfault_Occure = _kFAULT_RESET_TIMER;
										_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
										_sRuble_Parameters.Last_Valve = current_valve_number;
										_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
									
										for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
										{
											if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
											{
												/********************************* turn on/off the wired valve *********************************/
												_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
												_kDELAY_MS(200);
											}
										}
										return 0;          /* no data available */
									}
								}
								_kDELAY_MS(1000);
							}
						
						}
					
						/******************************************************** handle wired valve *******************************************************************/
					
						else if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO
						&& _sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i] < 100)
						{
							/********************************* turn on/off the wired valve *********************************/
							_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(!on_off));
							_kDELAY_MS(200);
						
						
							if(on_off == _kON)
							{
							
							}
						}
						else if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i] > 100)
						{
							if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i] == 101)
							{
								if(on_off == _kON)
								{
									if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
									{
										_sRuble_Parameters.Wireless_Pump_Command = _eWIRELESS_PUMP_ON;
										_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
									}
								}
								else
								{
									if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
									{
										_sRuble_Parameters.Wireless_Pump_Command = _eWIRELESS_PUMP_OFF;
										_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
									}
								}
							
							}
						}
					}
				}
				
				/* if all valve gets on */
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("All valves successfully ON/OFF");
					_kSERIAL_MON_CLEAR();
				}
				return 1;
			}
			else
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("valve list : ");
					_kSERIAL_MON_CLEAR();
				}
				for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
				{
					_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i]);
					_kSERAIL_MON_WRITE_NO_LN(" ");
					_kSERIAL_MON_CLEAR();
				}
				
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE_NO_LN("Irrigating plot no : ");
					_kSERIAL_MON_PRINT_NUM(Target_Plot_Num-1);
					_kSERIAL_MON_CLEAR();
				}
				
				for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
				{
					if(_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && _sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i] > 0)
					{
						unsigned char valve_number =0;
						unsigned char euro_valve_num=0,command_valve=0;
						
						valve_number =  _sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i];
						
						if(valve_number <= _kNUMBER_OF_VALVE_CONNECT_TO_EURO)
						{
							euro_valve_num = 1;
							command_valve = valve_number;
						}
						else
						{
							if(valve_number%_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
							{
								euro_valve_num = valve_number/_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1;
								command_valve  = valve_number%_kNUMBER_OF_VALVE_CONNECT_TO_EURO;
							}
							else
							{
								euro_valve_num = valve_number/_kNUMBER_OF_VALVE_CONNECT_TO_EURO;
								command_valve = _kNUMBER_OF_VALVE_CONNECT_TO_EURO;
							}
							
						}
						/** ???????????????????????????????????? irrigating plot ????????????????????????????????????????? **/
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("Wireless Valve no. ");
							_kSERIAL_MON_PRINT_NUM(euro_valve_num);
							_kSERIAL_MON_CLEAR();
						}
						
						
						if(_kLORA_DATA_AVAIALABLE())    /* read if there is already any data received(garbage data) */
						{
							_kLORA_RECV_DATA();         /*  lora rx_tx buffer will update*/
						}
						
						memset((char *)_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
						count = _kRESET;
						/********************************** wireless valve command (on/off) **********************************/
						/* valve id, on/off command, pump run timer */
						
						/************************************* copy the euro valve ID *********************************************/
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][0];
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][1];
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][2];
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][3];
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][4];
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][5];
						
						/*********************************** packet_id ***********************************************/
						_sRuble_Parameters.Lora_Data_TX[count++] = _eEURO_VALVE_COMMAND;
						
						/*********************************** copy on/off command ***********************************************/
						_sRuble_Parameters.Lora_Data_TX[count++] = on_off;
						
						/********************************** copy operation mode ************************************************/
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Operation_Mode;
						
						/******************************** copy pump timer (used if selected mode is timer) *********************/
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Pump_Run_Timer << 8;
						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Pump_Run_Timer;
						_sRuble_Parameters.Lora_Data_TX[count++] = command_valve;
						
						/* check which structure to which EURO valve we are going to update */
						for(j=0;j<_kMAX_EURO_VALVE;j++)
						{
							/************************ compare each valve id one by one with the list of euro valves **************************/
							id_len = _kRESET;
							for(compare_id=0;compare_id<_kVALVE_ID_LEN;compare_id++)
							{
								if(_sEuro_Valve_Param[j].Euro_Valve_ID[compare_id] != _sRuble_Parameters.List_Of_Valve_ID[euro_valve_num-1][compare_id])
								{
									compare_id = _kVALVE_ID_LEN;
									id_len = _kRESET;
								}
								id_len++;
							}
							
							if(id_len == _kVALVE_ID_LEN)
							{
								current_valve_number = j;    /* data send will update of this valve because valve get match */
								break;
							}
						}
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("Current valve number ");
							_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(current_valve_number);
							_kSERIAL_MON_CLEAR();
						}
						
						data_send_counter = _kRESET;
						
						while(data_send_counter < _kRETRY_TO_SEND_VALVE)
						{
							if(Subscribed_data_check())
							{
								if(Seperate_Out_Sub_Data())
								{
									//mqttCallback((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff));
									Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_eSUBSCRIBE_DATA);
								}
							}
							data_send_counter++;
							
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Send On/off Command to EURO valve 1");
								_kSERIAL_MON_CLEAR();
							}
							
							Display_Update_1();
							_kLORA_SEND_DATA(count);
							
							if(_kCHEK_DATA_AVAILABLE_TIMEOUT(3000))    /* wait for 3 sec to wait the data from the EURO valve */
							{
								/**************************** update the received data in buff lora rx_tx **********************/
								if(_kLORA_RECV_DATA())            /* check the received data */
								{
									
									//RF95_setModeRx_Continuous();
									/* check the id get matched to the valve or not */
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE_NO_LN("Euro valve rx data : ");
										_kSERIAL_MON_WRITE((char *)_sRuble_Parameters.Lora_Data_Rx,10);
										Serial.println();
										_kSERIAL_MON_CLEAR();
									
										_kSERAIL_MON_WRITE_NO_LN("Euro valve ID which is Sent :");
										_kSERIAL_MON_WRITE(_sEuro_Valve_Param[current_valve_number].Euro_Valve_ID,7);
										Serial.println();
										_kSERIAL_MON_CLEAR();
									}
									
									for(j=0;j<_kVALVE_ID_LEN;j++)
									{
										if(_sEuro_Valve_Param[current_valve_number].Euro_Valve_ID[j] != _sRuble_Parameters.Lora_Data_Rx[j])
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("ID Not matched with valve which is replied");
												_kSERIAL_MON_CLEAR();
											}
											
											if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
											{
												_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
												_gfault_Occure = _kFAULT_RESET_TIMER;
												_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
												_sRuble_Parameters.Last_Valve = current_valve_number;
												_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
												
												for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
												{
													if(_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
													{
														/********************************* turn on/off the wired valve *********************************/
														
														_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
														_kDELAY_MS(200);
														if(on_off == _kON)
														{
														}
													}
												}
												return 0;      /* id not get matched */
											}
											break;
										}
									}
									
									/* if id get matched */
									
									_sEuro_Valve_Param[current_valve_number].Euro_Valve_Online = _kVALVE_ONLINE;    /* valve online if got the data from that valve */
									_sEuro_Valve_Param[current_valve_number].Euro_Battery_Level = _sRuble_Parameters.Lora_Data_Rx[10];    /* copy the batter voltage */
									_sEuro_Valve_Param[current_valve_number].Euro_Battery_Level |= _sRuble_Parameters.Lora_Data_Rx[9] << 8;    /* copy the batter voltage */
									
									if(_sRuble_Parameters.Lora_Data_Rx[8] != on_off)     /* check weather valve is ON/off or Not */
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("Euro Feedback fail");
											_kSERIAL_MON_CLEAR();
										}
										if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
										{
											_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
											_gfault_Occure = _kFAULT_RESET_TIMER;
											_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
											_sRuble_Parameters.Last_Valve = current_valve_number;
											_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
											
											
											for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
											{
												if(_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
												{
													/********************************* turn on/off the wired valve *********************************/
													
													_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
													_kDELAY_MS(200);
												}
											}
											
											return 0;
										}
									}
									else
									{
										data_send_counter = 3;
									}
								}
								else
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("Euro Feedback fail/Not received any data from Euro valve 1");
										_kSERIAL_MON_CLEAR();
									}
									if(data_send_counter == _kRETRY_TO_SEND_VALVE -1)
									{
										_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
										_gfault_Occure = _kFAULT_RESET_TIMER;
										_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
										_sRuble_Parameters.Last_Valve = current_valve_number;
										_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
										
										
										for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
										{
											if(_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
											{
												/********************************* turn on/off the wired valve *********************************/
												
												_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
												_kDELAY_MS(200);
											}
										}
										return 0;       /* fail to received the data */
									}
								}
							}
							else
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Euro Feedback fail/Not received any data from Euro valve 0");
									_kSERIAL_MON_CLEAR();
								}
								//RF95_setModeRx_Continuous();
								
								if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
								{
									_gfault_Occure = _kFAULT_RESET_TIMER;
									_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
									_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
									_sRuble_Parameters.Last_Valve = current_valve_number;
									_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
									return 0;          /* no data available */
								}
							}
							_kDELAY_MS(1000);
						}
					}
					
					/******************************************************** handle wired valve *******************************************************************/
					
					else if(_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
					{
						/********************************* turn on/off the wired valve *********************************/
						
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[(Target_Plot_Num-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(!on_off));
						_kDELAY_MS(200);
						if(on_off == _kON)
						{
						}
					}
					
					else if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i] > 100)
					{
						if(_sLira_Node_Param[Target_Plot_Num-1].Euro_Valve_Group[i] == 101)
						{
							if(on_off == _kON)
							{
								if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
								{
									_sRuble_Parameters.Wireless_Pump_Command = _eWIRELESS_PUMP_ON;
									_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
								}
								
							}
							else
							{
								if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
								{
									_sRuble_Parameters.Wireless_Pump_Command = _eWIRELESS_PUMP_OFF;
									_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
								}
							}
							
						}
					}
				}
				
				/* if all valve gets on */
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("All valves successfully ON/OFF");
					_kSERIAL_MON_CLEAR();
				}
				return 1;
			}
			
		}break;
	}
}

unsigned char Flush_Controlling(void)
{	
	static unsigned int flush_monitor_timer = _kRESET;
	static unsigned char flush_monitoring_trigger = _kRESET;
	static unsigned char flush_time_save = 0,total_valve;
	
	total_valve = 0;
	for(unsigned valve_num=0; valve_num<5;valve_num++)
	{
		if(_sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Euro_Valve_Group[valve_num] != _kRESET)
		{
			total_valve++;
		}
	}
	
	if(total_valve < 2)
	{
		_gRunning_Flush_Num = _kSET;
	}
	
	
	
	
	if(_sSchedules.Flush_Schedule_In_Every > 14400)
	{
		_sSchedules.Flush_Schedule_In_Every = 14400;
		
	}
	
	if(_sSchedules.Flush_Schedule_For > 900)
	{
		_sSchedules.Flush_Schedule_For = 900;
	}
	
	
	if(flush_time_save > 40)
	{
		flush_time_save = 0;
		_kEEPROM_WRITE(_kFLUSH_RUN_BACK_UP, _sRuble_Parameters.Pump_Run_Timer_for_flush);
	}
	else
	{
		flush_time_save++;
	}
	
	
	
	
	if(_sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Extra_Plot_Number == _kFLUSH_PLOT_NUMBER)
	{
		if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_MANUAL)
		{
			if(!_sRuble_Parameters.Flush_Retry)
			{
				_kSERAIL_MON_WRITE("Flush_Running 0: ");
				_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sRuble_Parameters.Pump_Run_Timer_for_flush);
				Serial.println();
				_kSERIAL_MON_CLEAR();
				
				if(_sRuble_Parameters.Pump_Run_Timer_for_flush > _sSchedules.Flush_Schedule_In_Every)
				{
					//_kSERAIL_MON_WRITE("Flush_Running 1: ");
					//_kSERIAL_MON_CLEAR();
					if(_sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kSET)
					{
						if(_sRuble_Parameters.Flush_Running)
						{
							Hard_Coded_Flush_Valve(_kON);
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Flush_Running 2: ");
								_kSERIAL_MON_CLEAR();
							}
							
							if(_sPump_Parameter.Irrigation_Status || 
							_sSchedules.Flush_Schedule_In_Every == 0 || _sRuble_Parameters.Line_Fault_Flag == _kSET)
							{
								_kEEPROM_WRITE(_kRUNNING_FLUSH_NUMBER, _gRunning_Flush_Num);
								
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Flush_Running 3: ");
									_kSERIAL_MON_CLEAR();
								}
								
								if(_sPump_Parameter.Irrigation_Status || _sPump_Parameter.Irrigation_Status == _kSET)
								{
									_sRuble_Parameters.Flush_Retry = _kSCHEDULE_RETRY_TIMER;
								}
								
								if(Stop_Extra_Plot_Operation(_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
								{
									
								}
								
								//if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status != _kSCHEDULER_OFF)
								if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kRESET)
								{
									_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
									_sRuble_Parameters.Extra_Plot_Feedback = (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM);
									_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
									
									//_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status = _kSCHEDULER_OFF;
									_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation = _kRESET;
									flush_monitor_timer = _kRESET;
									flush_monitoring_trigger = _kSET;
									
									//_sRuble_Parameters.Pump_Run_Timer_for_flush = _kRESET;
									
									_kEEPROM_WRITE(_kFLUSH_RUN_BACK_UP, _sRuble_Parameters.Pump_Run_Timer_for_flush);
								}
								
								Hard_Coded_Flush_Valve(_kON);
							}
						}
						else
						{
								//_kSERAIL_MON_WRITE("Flush_Running 4: ");
								//_kSERIAL_MON_CLEAR();
								
								//_sRuble_Parameters.Pump_Run_Timer_for_flush = _kRESET;
							
								if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_TIMER)
								{
									_sRuble_Parameters.Publish_Topic |= _ePUB_TIMER_FEEDBACK;
								}
							
								if(Stop_Extra_Plot_Operation(_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
								{
								
								}
							
								//if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status != _kSCHEDULER_OFF)
								if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kRESET)
								{
									
									_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
									_sRuble_Parameters.Extra_Plot_Feedback = (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM);
									_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
								
									//_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status = _kSCHEDULER_OFF;
									_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation = _kRESET;
									flush_monitor_timer = _kRESET;
									flush_monitoring_trigger = _kSET;
									
									if(_gRunning_Flush_Num >= total_valve)
									{
										_gRunning_Flush_Num = _kSET;
										_sRuble_Parameters.Pump_Run_Timer_for_flush = _kRESET;
									}
									else
									{
										_gRunning_Flush_Num++;
									}
									
									_kEEPROM_WRITE(_kFLUSH_RUN_BACK_UP, _sRuble_Parameters.Pump_Run_Timer_for_flush);
								}
								
								_kEEPROM_WRITE(_kRUNNING_FLUSH_NUMBER, _gRunning_Flush_Num);
							
								Hard_Coded_Flush_Valve(_kON);   /* turn off the valve */
						}
					}
					else
					{
						if(_sExtra_Plot[_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM].On_Off_Status == _kSET ||
						_sSchedules.Flush_Schedule_In_Every == 0)
						{
							//_kSERAIL_MON_WRITE("Flush_Running 5: ");
							//_kSERIAL_MON_CLEAR();
							return 1;
						}
						
						
						
						if(_sRuble_Parameters.Irrigating_Plot != _kRESET || _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kRESET)
						{
							if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER && _sRuble_Parameters.Line_Fault_Flag == _kSET)
							{
								return 0;
							}
							
							if(_sRuble_Parameters.Pump_Selected_For_Irrigation > 1)
							{
								if((_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eR_PHASE_STATUS) !=  _eR_PHASE_STATUS||
								(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eY_PHASE_STATUS) != _eY_PHASE_STATUS    ||
								(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eB_PHASE_STATUS) != _eB_PHASE_STATUS ||
								(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eDRY_RUN_STATUS) == _eDRY_RUN_STATUS ||
								(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _eLINE_FAULT_STATUS) ==  _eLINE_FAULT_STATUS ||
								(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _ePUMP_OVERLOAD) ==  _ePUMP_OVERLOAD||
								(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults  & _eMANUAL_MODE) == _eMANUAL_MODE /*||
								(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults  & _ePUMP_STATUS) != _ePUMP_STATUS*/ ||
								(_sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults & _ePUMP_NOT_WORKING)  == _ePUMP_NOT_WORKING)
								{
									return 0;
								}
							}
							if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_SCHEDULER)
							{
								if(_sSchedules.Scheduler_Retry_Timer != 0)
								{
									_sRuble_Parameters.Flush_Retry = _sSchedules.Scheduler_Retry_Timer+20;
									return 0;
								}
							}
							
							if(Start_Extra_Plot_Operation(_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
							{
							     if(_sRuble_Parameters.Debug_Mode == _kSET)
							     {
									_kSERAIL_MON_WRITE("Flush_Running 6: ");
									_kSERIAL_MON_CLEAR();
								 }
								
								_sRuble_Parameters.Flush_Running =  _sSchedules.Flush_Schedule_For;
								_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
								_sRuble_Parameters.Extra_Plot_Feedback = (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM);
								_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
							
								//_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status = _kSCHEDULER_ON;
							
								_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation = _kSET;
							
								Hard_Coded_Flush_Valve(_kOFF);
								_kEEPROM_WRITE(_kRUNNING_FLUSH_NUMBER, _gRunning_Flush_Num);
							}
							else
							{
								//if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status != _kSCHEDULER_OFF)
								//if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kRESET || 
								//_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kSET)
								if(1)
								{
									//_kSERAIL_MON_WRITE("Flush_Running 6.1: ");
									//_kSERIAL_MON_CLEAR();
									_sRuble_Parameters.Flush_Retry = _kSCHEDULE_RETRY_TIMER;
								
									if(Stop_Extra_Plot_Operation(_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
									{
									
									}
								
									if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kRESET)
									{
										_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
										_sRuble_Parameters.Extra_Plot_Feedback = (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM);
										_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
									
										//_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status = _kSCHEDULER_OFF;
										_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation = _kRESET;
										flush_monitor_timer = _kRESET;
										flush_monitoring_trigger = _kSET;
									
										_sRuble_Parameters.Pump_Run_Timer_for_flush = _kRESET;
										_kEEPROM_WRITE(_kFLUSH_RUN_BACK_UP, _sRuble_Parameters.Pump_Run_Timer_for_flush);
									}
								
									Hard_Coded_Flush_Valve(_kON);
								}
							}	
					   }
					}
				}
			}
			else
			{
				
			}
		}
		else
		{
			if(_sRuble_Parameters.Flush_Running || _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kRESET)
			{
				//_kSERAIL_MON_WRITE("Flush_Running 7: ");
				//_kSERIAL_MON_CLEAR();
				_sRuble_Parameters.Flush_Running = _kRESET;
				
				
				if(Stop_Extra_Plot_Operation(_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
				{
					
				}
				
				//if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status != _kSCHEDULER_OFF)
				if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kRESET)
				{
					//_kSERAIL_MON_WRITE("Flush_Running 8: ");
					//_kSERIAL_MON_CLEAR();
					_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
					_sRuble_Parameters.Extra_Plot_Feedback = (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM);
					_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
					
					//_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status = _kSCHEDULER_OFF;
					_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation = _kRESET;
					flush_monitor_timer = _kRESET;
					flush_monitoring_trigger = _kSET;
					_sRuble_Parameters.Pump_Run_Timer_for_flush = _kRESET;
					_kEEPROM_WRITE(_kFLUSH_RUN_BACK_UP, _sRuble_Parameters.Pump_Run_Timer_for_flush);
				}
				_sRuble_Parameters.Flush_Valve_Off_Timer = _kRESET;
				
				Hard_Coded_Flush_Valve(_kON);
			}
		}
	}
	
	
	
	if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation == _kRESET)
	{
		if(flush_monitoring_trigger)
		{
			if(flush_monitor_timer > 60)
			{
				flush_monitor_timer = _kRESET;
				if(flush_monitoring_trigger)
				{
					Hard_Coded_Flush_Valve(_kON);
					if(Extra_Plot_Valve_Grouping_Handler(_kOFF, (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)))
					{
						flush_monitoring_trigger = _kRESET;
					}
				}
			
			}
			else
			{
				flush_monitor_timer++;
			}
		}
	}
	
	
	if(_gRunning_Flush_Num == _kRESET || _gRunning_Flush_Num > total_valve)
	{
		_gRunning_Flush_Num = _kSET;
		_kEEPROM_WRITE(_kRUNNING_FLUSH_NUMBER, _gRunning_Flush_Num);
	}
	
}

unsigned char Hard_Coded_Flush_Valve(unsigned char on_off)
{
	return 1;
	
	switch(on_off)
	{
		case _kON:     /* ready for irrigation by flush control function*/
		{ 
			if(_sRuble_Parameters.Irrigating_Plot != _kRESET ||
			_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status != _kRESET || 
			_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET)
			{	
				if(_sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kSET)
				{
					if(_kGPIO_GET(_sRuble_Parameters.Wired_valve_List[22]))
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 1.0: ");
							_kSERIAL_MON_CLEAR();
						}
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[22],(0));
					}
					if(_kGPIO_GET(_sRuble_Parameters.Wired_valve_List[23]))
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 1.0: ");
							_kSERIAL_MON_CLEAR();
						}
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[23],(0));
					}
				}
				else
				{
					if(!_kGPIO_GET(_sRuble_Parameters.Wired_valve_List[22]))
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 1: ");
							_kSERIAL_MON_CLEAR();
						}
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[22],(1));
					}
					if(!_kGPIO_GET(_sRuble_Parameters.Wired_valve_List[23]))
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 1: ");
							_kSERIAL_MON_CLEAR();
						}
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[23],(1));
					}
				}
			}
			else
			{
				if(!_kGPIO_GET(_sRuble_Parameters.Wired_valve_List[22]))
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 1: ");
						_kSERIAL_MON_CLEAR();
					}
					_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[22],(1));
				}
				if(!_kGPIO_GET(_sRuble_Parameters.Wired_valve_List[23]))
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 1: ");
						_kSERIAL_MON_CLEAR();
					}
					_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[23],(1));
				}
			}
			
		}break;
		
		case _kOFF:  /* ready for flush by flush control function*/
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 2: ");
				_kSERIAL_MON_CLEAR();
			}
			
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[22],(1));
			_kDELAY_MS(20);
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[23],(1));
			_kDELAY_MS(20);
		}break;
		
		case _kCHECK_TRY_TO_ON:   /* ready for irrigation by start control function */
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 3.0: ");
				_kSERIAL_MON_CLEAR();
			}
			if( _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation  !=_kSET)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 3: ");
					_kSERIAL_MON_CLEAR();
				}
				_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[22],(0));
				_kDELAY_MS(20);
				_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[23],(0));
				_kDELAY_MS(20);
			}
		}break;
		
		case _kCHECK_TRY_TO_OFF:   /* ready for to stop irrigation by stop control function*/
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("Hard_Coded_Flush_Valve 4: ");
				_kSERIAL_MON_CLEAR();
			}
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[22],(1));
			_kDELAY_MS(20);
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[23],(1));
			_kDELAY_MS(20);
		}break;
	}
}

