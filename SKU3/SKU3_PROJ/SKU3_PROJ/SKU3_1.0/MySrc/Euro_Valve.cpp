
/*
 * Euro_Valve.cpp
 *
 * Created: 25-08-2021 15:31:22
 *  Author: Tushar (SIO)
 */ 

#include <Euro_Valve.h>
#include <Initialize.h>

Euro_Valve_Param _sEuro_Valve_Param[_kMAX_EURO_VALVE];
Wireless_Pump_Param _sWireless_Pump_Param[_kMAX_WIRELESS_PUMP];


bool Update_Euro_Valve_Data(unsigned char *temp_id)
{
	unsigned char valve_number=0,i=0,count =0,id_len;
	_sWireless_Pump_Param[0].Wireless_Pump_Number = 1;
	
	for(valve_number=0; valve_number < _kMAX_EURO_VALVE; valve_number++)
	{
		/***************************** compare the id first ***********************************************/
		id_len = _kRESET;
		for(i=0;i<_kVALVE_ID_LEN;i++)
		{
			if(temp_id[i] != _sEuro_Valve_Param[valve_number].Euro_Valve_ID[i])
			{
				i = _kVALVE_ID_LEN;
				id_len = _kRESET;
			}
			
			id_len++;
		}
		count = 0;
		
		if(id_len == _kVALVE_ID_LEN)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("EURO DATA RECIEVED");                /* for debugging purpose */
				_kSERIAL_MON_CLEAR();
			}
			
			switch(_sRuble_Parameters.Lora_Data_Rx[6])
			{
				case _eEURO_VALVE_FAULT_DATA:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("_eEURO_VALVE_FAULT_DATA");                /* for debugging purpose */
						_kSERIAL_MON_CLEAR();
					}
					
					switch(_sRuble_Parameters.Lora_Data_Rx[7])
					{
						case _eNO_FAULT:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("_eNO_FAULT ");
								_kSERIAL_MON_CLEAR();
							}
							
							_sEuro_Valve_Param[valve_number].Faults = _sRuble_Parameters.Lora_Data_Rx[7];    /* update the battery low faults */
							_sEuro_Valve_Param[valve_number].Euro_Valve_Online = _kVALVE_ONLINE;
							_sEuro_Valve_Param[valve_number].Euro_Valve_Status = _sRuble_Parameters.Lora_Data_Rx[9];    /* update valve status data  */
						}break;
						
						case _eLOW_BAT_FAULT:
						{
							_sEuro_Valve_Param[valve_number].Faults = _sRuble_Parameters.Lora_Data_Rx[7];    /* update the battery low faults */
							_sEuro_Valve_Param[valve_number].Euro_Valve_Online = _kVALVE_OFFLINE;
							if(_sRuble_Parameters.Irrigating_Plot == 0) /* if not any plot is irrigating and valve status is still on then off the valve */
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE_NO_LN("not any plot is irrigating");
									_kSERIAL_MON_CLEAR();
								}
								
								_sEuro_Valve_Param[valve_number].Euro_Valve_Status = _kVALVE_OFF;
							}
							else
							{
								_sEuro_Valve_Param[valve_number].Euro_Valve_Status   = _sRuble_Parameters.Lora_Data_Rx[9];
							}
							
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("low battery fault");
								_kSERIAL_MON_CLEAR(); 
							}
							
							_sGSM_Query.SMS_Alert |= _eEURO_BATT_LOW;
						}break;
						
						default:
						{
							/* Fault not matched */
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("default command ");
								_kSERIAL_MON_CLEAR();
							}
							
							return 0;
						}break;
					}
					
					_sEuro_Valve_Param[valve_number].Euro_Battery_Level  = (_sRuble_Parameters.Lora_Data_Rx[10]<<8) | (_sRuble_Parameters.Lora_Data_Rx[11]);      /* update battery level */
					
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE_NO_LN("Data recvd From Euro : "); 
						_kSERIAL_MON_CLEAR();                         /* for debugging purpose */
						_kSERIAL_MON_PRINT_NUM(_sEuro_Valve_Param[valve_number].Euro_Valve_Number);
						_kSERIAL_MON_CLEAR();
						_kSERAIL_MON_WRITE_NO_LN("Battery level : ");                                 /* for debugging purpose */
						_kSERIAL_MON_PRINT_NUM(_sEuro_Valve_Param[valve_number].Euro_Battery_Level);
						_kSERIAL_MON_CLEAR();
						_kSERAIL_MON_WRITE_NO_LN("Euro online/offline : ");      
						_kSERIAL_MON_CLEAR();                     /* for debugging purpose */
						_kSERIAL_MON_PRINT_NUM(_sEuro_Valve_Param[valve_number].Euro_Valve_Online);
						_kSERAIL_MON_WRITE_NO_LN("Euro status : "); 
						_kSERIAL_MON_CLEAR();                                  /* for debugging purpose */
						_kSERIAL_MON_PRINT_NUM(_sEuro_Valve_Param[valve_number].Euro_Valve_Status);
						_kSERIAL_MON_CLEAR();
					}
					/*********************************** make the ack packet ***************************************/
					memset(_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
					
					for(i=0;i<_kVALVE_ID_LEN;i++)
					{
						_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[valve_number].Euro_Valve_ID[i];   /* copy the id */
					}
					_sRuble_Parameters.Lora_Data_TX[count++] = _eEURO_VALVE_FAULT_DATA;
					_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Lora_Data_Rx[7];
					_sRuble_Parameters.Lora_Data_TX[count++] = _sEuro_Valve_Param[valve_number].Euro_Valve_Online;
				}break;
				
				default:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("EURO PACKET IS NOT APPLICABLE");
						_kSERIAL_MON_CLEAR();
					}
					return 0;
				}break;
			}
			
			switch(_sRuble_Parameters.Lora_Data_TX[6])
			{
				case _eEURO_VALVE_FAULT_DATA:
				case _eVALVE_OFFLINE_FAULT:
				{
					_sRuble_Parameters.Last_Valve = valve_number;   /* update last valve number to publish the data */
					_sRuble_Parameters.Publish_Topic |= _ePUB_EURO_VALVE_DATA;  /* update the publish data for server */
					Euro_Data_EEPROM(valve_number);
					
					if(_sRuble_Parameters.Irrigating_Plot)   /* if plot is irrigating then stop the irrigation */
					{
						for(unsigned char port_number =0 ; port_number < _kNUMBER_OF_VALVE_CONNECT_TO_EURO; port_number++)
						{
							for(unsigned int wireless_valve_num=0; wireless_valve_num<_kMAX_VALVE_FOR_1PLOT; wireless_valve_num++)
							{
								if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wireless_valve_num] == (valve_number*3) + (port_number+1))
								{
									/********* if valve is on and there is some fault then stop the irrigation process **********/
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("valve fault occure please turn off the pump");
										_kSERIAL_MON_CLEAR();
									}
									_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;   // suspend the operation if fault occur in running valve
									// turn off the pump immediately
									if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
									{
										if(_kGPIO_GET(_kPUMP_1) != _kPUMP_OFF)     /* check weather pump is already Off */
										{
											_kGPIO_SET(_kPUMP_1,_kPUMP_OFF);      /* turn Off the pump */
										}
										
										if(_kGPIO_GET(_kPUMP_2) != _kPUMP_OFF)     /* check weather pump is already Off */
										{
											_kGPIO_SET(_kPUMP_2,_kPUMP_OFF);      /* turn Off the pump */
										}
									}
									else
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("wl pump trigger 11111111111111111111");
											_kSERIAL_MON_CLEAR();
										}
										if(Wireless_Pump_On_Off(_ePUMP_OFF,_sRuble_Parameters.Pump_Selected_For_Irrigation -2))
										{
											_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
											_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
										}
										else
										{
											if(_sRuble_Parameters.Irrigating_Plot == _kRESET 
											&& _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation  !=_kSET
											&& _sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation !=_kSET)
											{
												_sRuble_Parameters.Wirelss_Pump_Status = _kON;
											}
											else
											{
												_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;	
											}
											_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
										}
									}				
									_sEuro_Valve_Param[valve_number].Euro_Valve_Status = _kVALVE_OFF;
									_sPump_Parameter.Irrigation_Status = _eEURO_BATTERY_LOW;
									_gfault_Occure = _kFAULT_RESET_TIMER;
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("_eEURO_BATTERY_LOW");
									}
									_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_FAULTS;
									_kLORA_SEND_DATA(count);   // Send ack immediately
									return 1;
								}
							}
						}
					}
				
				for(unsigned int check_plot=0; check_plot<_kMAX_EXTRA_PLOT; check_plot++)
				{
					if(_sExtra_Plot[check_plot].Extra_Plot_Number)
					{
						for(unsigned char port_number =0 ; port_number < _kNUMBER_OF_VALVE_CONNECT_TO_EURO; port_number++)
						{
							for(unsigned int wireless_valve_num=0; wireless_valve_num<_kMAX_VALVE_FOR_1PLOT; wireless_valve_num++)
							{
								if(_sExtra_Plot[check_plot].Euro_Valve_Group[wireless_valve_num] == (valve_number*3) + (port_number+1))
								{
									if(_sExtra_Plot[check_plot].On_Off_Status == _kSET || _sExtra_Plot[check_plot].Auto_Operation == _kSET)
									{
										if((check_plot+_kEXTRA_PLOT_START_FROM) == _kFOOGER_PLOT_NUMBER)
										{
											/********* if valve is on and there is some fault then stop the irrigation process **********/
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("fogger: valve fault occure please turn off the pump");
												_kSERIAL_MON_CLEAR();	
											}
											if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
											{
												if(_kGPIO_GET(_kPUMP_1) != _kPUMP_OFF)     /* check weather pump is already Off */
												{
													_kGPIO_SET(_kPUMP_1,_kPUMP_OFF);      /* turn Off the pump */
												}
												
												if(_kGPIO_GET(_kPUMP_2) != _kPUMP_OFF)     /* check weather pump is already Off */
												{
													_kGPIO_SET(_kPUMP_2,_kPUMP_OFF);      /* turn Off the pump */
												}
											}
											else
											{
												if(_sRuble_Parameters.Debug_Mode == _kSET)
												{
													_kSERAIL_MON_WRITE("wl pump trigger 000000000000000000");
													_kSERIAL_MON_CLEAR();
												}
												if(Wireless_Pump_On_Off(_ePUMP_OFF,_sRuble_Parameters.Pump_Selected_For_Irrigation -2))
												{
													_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
													_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
												}
												else
												{
													if(_sRuble_Parameters.Irrigating_Plot == _kRESET
													&& _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation  !=_kSET
													&& _sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation !=_kSET)
													{
														_sRuble_Parameters.Wirelss_Pump_Status = _kON;
													}
													else
													{
														_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
													}
													_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
												}
											}
											_sExtra_Plot[check_plot].Run_Timer = _kRESET;
											_sExtra_Plot[check_plot].One_Touch = _kRESET;					
											_sEuro_Valve_Param[valve_number].Euro_Valve_Status = _kVALVE_OFF;
											_sPump_Parameter.Irrigation_Status = _eEURO_BATTERY_LOW;
											_gfault_Occure = _kFAULT_RESET_TIMER;
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("_eEURO_BATTERY_LOW 1 ");
											}
											_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_FAULTS;
											_kLORA_SEND_DATA(count);   // Send ack immediately
											return 1;
										}
									}
								}
							}
						}						
					}
				}
				
				 if(_sRuble_Parameters.Debug_Mode == _kSET)
				 {
					_kSERAIL_MON_WRITE("ACK0 SEND TO VALVE ");
					_kSERIAL_MON_CLEAR();
					for(int k =0; k<count; k++)
					{
						_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Lora_Data_TX[k]);
					}
				 }
					_kSERIAL_MON_PRINT_NUM();
					_kLORA_SEND_DATA(count);
				}break;
				
				default:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("ACK1 SEND TO VALVE");
						_kSERIAL_MON_CLEAR();
					}
					_kLORA_SEND_DATA(count);
					_sEuro_Valve_Param[valve_number].Euro_Valve_Online = _kVALVE_ONLINE;
				}break;
			}
			return 1;
		}
	}
	return 0;
}

void Euro_Data_EEPROM(unsigned char valve_number)
{
	_kEEPROM_WRITE(_kEEPROM_LOC_EURO_VALVE_DATA(valve_number), _sEuro_Valve_Param[valve_number]);
	return;
}

bool Update_Wireless_Pump_Data(unsigned char *temp_id)
{
	unsigned char pump_number=0,i=0,count = 0,id_len;
	
	for(pump_number=0; pump_number < _kMAX_WIRELESS_PUMP; pump_number++)
	{
		/***************************** compare the id first ***********************************************/
		id_len = _kRESET;
		for(i=0;i<_kVALVE_ID_LEN;i++)
		{
			if(temp_id[i] != _sWireless_Pump_Param[pump_number].Wireless_Pump_ID[i])
			{
				i = _kVALVE_ID_LEN;
				id_len = _kRESET;
			}
			
			id_len++;
		}
		count = 0;
		if(id_len == _kVALVE_ID_LEN)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("WIRELESS PUMP DATA RECIEVED");
				_kSERIAL_MON_CLEAR();
			}
			
			pump_number =  _sRuble_Parameters.Lora_Data_Rx[10];
			pump_number = pump_number-1;
			
			
			switch(_sRuble_Parameters.Lora_Data_Rx[6])
			{
				case _eCALIBRATION_RESPONSE:
				{
					_sWireless_Pump_Param[pump_number].Response_To_Ruble |= _sRuble_Parameters.Lora_Data_Rx[7];
				}break;
				
				case _eWIRELESS_FAULT:
				{
					_sWireless_Pump_Param[pump_number].Response_To_Ruble   |= _sRuble_Parameters.Lora_Data_Rx[7];
					_sWireless_Pump_Param[pump_number].Indication_Of_Faults = (_sRuble_Parameters.Lora_Data_Rx[8]<<8) | (_sRuble_Parameters.Lora_Data_Rx[9]);
				}break;
			}
				
			memset(_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
			
			for(i=0;i<_kVALVE_ID_LEN;i++)
			{
				_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[pump_number].Wireless_Pump_ID[i];   /* copy the id */
			}
			
			_sRuble_Parameters.Lora_Data_TX[count++] = _eGOT_DATA;
			
			_kLORA_SEND_DATA(count);
			
			Wireless_Pump_EEPROM(pump_number);
			
			return 1;
			
		}
	}
	
	return 0;
}

void Wireless_Pump_EEPROM(unsigned char Pump_Number)
{
	_kEEPROM_WRITE(_kWIRELESS_PUMP_DATA_SAVE(Pump_Number), _sWireless_Pump_Param[Pump_Number]);
	return;
}