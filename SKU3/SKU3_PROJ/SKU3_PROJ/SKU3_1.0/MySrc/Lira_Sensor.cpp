/*
 * Lira_Sensor.cpp
 *
 * Created: 25-08-2021 15:33:00
 *  Author: Tushar (SIO)
 */ 

#include <Lira_Sensor.h>
#include <Euro_Valve.h>
#include <Initialize.h>

Lira_Node_Param _sLira_Node_Param[_kMAX_LIRA_SENSOR_NODE];


bool Update_Lira_Sensor_Data(unsigned char *temp_id)
{
// 	unsigned char plot_number=0,count=0,i=0,id_len=0;
// 	unsigned int Polling_Time = 0;
// 	for(plot_number=0;plot_number<_kMAX_LIRA_SENSOR_NODE;plot_number++)        /* check all the structure's ID one by one */
// 	{
// 		/***************************** compare the id first ***********************************************/
// 		id_len = _kRESET;
// 		for(i=0;i<_kVALVE_ID_LEN;i++)
// 		{
// 			if(temp_id[i] != _sLira_Node_Param[plot_number].Lira_Node_ID[i])
// 			{
// 				i = _kVALVE_ID_LEN;
// 				id_len = _kRESET;
// 			}
// 			
// 			id_len++;
// 		}
// 		
// 		count = _kRESET;
// 		
// 		if(id_len == _kVALVE_ID_LEN)
// 		{
// 			_kSERAIL_MON_WRITE_NO_LN("Data recvd From Lira Sensor no : ");                /* for debugging purpose */
// 			_kSERIAL_MON_PRINT_NUM(plot_number+1);                /* for debugging purpose */
// 			
// 			switch(_sRuble_Parameters.Lora_Data_Rx[6])      // packet type
// 			{
// 				case _eBROADCASTING_PACKET:             /* reply to broadcasting packet */
// 				{
// 					memset(_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
// 					
// 					_kSERAIL_MON_WRITE("BROADCASTING PACKET");                /* for debugging purpose */
// 					
// 					for(i=0;i<_kVALVE_ID_LEN;i++)
// 					{
// 						_sRuble_Parameters.Lora_Data_TX[count++] = _sLira_Node_Param[plot_number].Lira_Node_ID[i];   /* copy the id */
// 					}
// 					
// 					_sRuble_Parameters.Lora_Data_TX[count++] = _eBROADCASTING_PACKET;
// 					
// 					for(i=0;i<_kVALVE_ID_LEN;i++)
// 					{
// 						_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Ruble_ID[i];   /* copy the ruble id */
// 						_kSERIAL_MON_WRITE(_sRuble_Parameters.Ruble_ID[i]);
// 					}
// 				}break;
// 				
// 				case _eLIRA_SENSOR_POLLING_DATA_PACKET:
// 				{
// 					_kSERAIL_MON_WRITE("lira sensor polling data packet");                /* for debugging purpose */
// 					
// 					_sLira_Node_Param[plot_number].Packet_ID = _sRuble_Parameters.Lora_Data_Rx[7];  /* copy the packet id(packet number) */
// 					
// 					_sLira_Node_Param[plot_number].Percent_Moisture[0]   = _sRuble_Parameters.Lora_Data_Rx[8] << 8;       /* copy the soil moisture sensor 1 data */
// 					_sLira_Node_Param[plot_number].Percent_Moisture[0]  |= _sRuble_Parameters.Lora_Data_Rx[9];
// 					
// 					_sLira_Node_Param[plot_number].Percent_Moisture[1]   = _sRuble_Parameters.Lora_Data_Rx[10] << 8;       /* copy the soil moisture sensor 2 data */
// 					_sLira_Node_Param[plot_number].Percent_Moisture[1]  |= _sRuble_Parameters.Lora_Data_Rx[11];
// 					
// 					_sLira_Node_Param[plot_number].Percent_Moisture[2]   = _sRuble_Parameters.Lora_Data_Rx[12] << 8;      /* copy the soil moisture sensor 3 data */
// 					_sLira_Node_Param[plot_number].Percent_Moisture[2]  |= _sRuble_Parameters.Lora_Data_Rx[13];
// 					
// 					_sLira_Node_Param[plot_number].Percent_Moisture[3]   = _sRuble_Parameters.Lora_Data_Rx[14] << 8;      /* copy the soil moisture sensor 4 data */
// 					_sLira_Node_Param[plot_number].Percent_Moisture[3] |= _sRuble_Parameters.Lora_Data_Rx[15];
// 					
// 					
// 					
// 					_sLira_Node_Param[plot_number].Soil_Temp= _sRuble_Parameters.Lora_Data_Rx[16]; /* copy the soil temperature */
// 					
// 					_sLira_Node_Param[plot_number].Air_Temperature= _sRuble_Parameters.Lora_Data_Rx[17]; /* copy air temperature */
// 					
// 					_sLira_Node_Param[plot_number].Humidity= _sRuble_Parameters.Lora_Data_Rx[18];  /* copy the humidity */
// 					
// 					_sLira_Node_Param[plot_number].Light_Intensity= (_sRuble_Parameters.Lora_Data_Rx[19]<<8) | (_sRuble_Parameters.Lora_Data_Rx[20]);    /* copy the light intensity */
// 					
// 					_sLira_Node_Param[plot_number].Lira_Battery_Level= (_sRuble_Parameters.Lora_Data_Rx[21]<<8) | (_sRuble_Parameters.Lora_Data_Rx[22]); /* copy batter voltage */
// 					
// 					
// 					
// 					/* ###################### calculate soil moisture and add into sm percentage data ############################# */
// 					_sLira_Node_Param[plot_number].Percent_Moisture[0] =  curveFit(_sLira_Node_Param[plot_number].Percent_Moisture[0]);
// 					_sLira_Node_Param[plot_number].Percent_Moisture[1] =  curveFit(_sLira_Node_Param[plot_number].Percent_Moisture[1]);
// 					_sLira_Node_Param[plot_number].Percent_Moisture[2] =  curveFit(_sLira_Node_Param[plot_number].Percent_Moisture[2]);
// 					_sLira_Node_Param[plot_number].Percent_Moisture[3] =  curveFit(_sLira_Node_Param[plot_number].Percent_Moisture[3]);
// 					
// 					/******************************* CHECK WEATHER ANY SENSOR IS OUT OF SOIL ********************************************/
// 					
// 					if(_sLira_Node_Param[plot_number].Percent_Moisture[0] == 91 || _sLira_Node_Param[plot_number].Percent_Moisture[1] == 91 || 
// 					_sLira_Node_Param[plot_number].Percent_Moisture[2]== 91 || _sLira_Node_Param[plot_number].Percent_Moisture[3] == 91)
// 					{
// 						_sGSM_Query.SMS_Alert |=  _eSENSOR_OUT_OF_SOIL;
// 					}
// 					
// 					//_sLira_Node_Param[plot_number].RSSI = _kLORA_LAST_RSSI();
// 					//_sLira_Node_Param[plot_number].SNR  = _kLORA_LAST_SNR();
// 					
// 					
// 					/******************************* UPDATE THE PRIORITY LIST********************************************/
// 					if(_sRuble_Parameters.Kill_Switch != _eKILL_SWITCH_SET)
// 					{
// 						if(_sLira_Node_Param[plot_number].Operation_Mode == _eOPER_MOD_AUTO && _sRuble_Parameters.Operation_Mode != _eOPER_MOD_TIMER &&
// 						_sRuble_Parameters.Temp_Operation_Mode != _eOPER_MOD_TIMER)
// 						{
// 							if(_sLira_Node_Param[plot_number].Percent_Moisture[(_sLira_Node_Param[plot_number].Selected_Depth)-1] <= _sLira_Node_Param[plot_number].WP_Value)
// 							{
// 								if(_sRuble_Parameters.Irrig_Priority[0])   /* check any plot is available in priority list */
// 								{
// 									if(_sRuble_Parameters.Irrigating_Plot != (plot_number+1))   /* if present plot is irrigating */
// 									{
// 										for(i=0;i<_kMAX_LIRA_SENSOR_NODE;i++)
// 										{
// 											if(_sRuble_Parameters.Irrig_Priority[i] != (plot_number+1) && _sRuble_Parameters.Irrig_Priority[i] == 0)
// 											{
// 												_sRuble_Parameters.Irrig_Priority[i] = plot_number+1;   /* if this plot already not exist in the list so add at the last */
// 												break;
// 											}
// 											else if(_sRuble_Parameters.Irrig_Priority[i] ==  plot_number+1)
// 											{
// 												break;   /* if the current plot already exist in the list */
// 											}
// 										}
// 									}
// 							
// 								}
// 								else
// 								{
// 									if(_sRuble_Parameters.Irrigating_Plot != (plot_number+1))   /* if present plot is irrigating */
// 									{
// 										_sRuble_Parameters.Irrig_Priority[0] = plot_number+1;   /* if not any plot is added yet in dry list then add to first location*/
// 									}
// 								}
// 							}
// 						}
// 					}
// 					
// 					if(_sRuble_Parameters.Irrigating_Plot == _kRESET && !_kGPIO_GET(_kPOWER_SOURCE_CHECK) &&
// 					
// 					_sLira_Node_Param[plot_number].Percent_Moisture[(_sLira_Node_Param[plot_number].Selected_Depth)-1] < _sLira_Node_Param[plot_number].FC_Value)
// 					{
// 						Polling_Time = _kLIRA_SLOW_DATA_PROCESS;   /* ask sensor to send its data frequently */
// 					}
// 					else
// 					{
// 						if(_sLira_Node_Param[plot_number].Percent_Moisture[(_sLira_Node_Param[plot_number].Selected_Depth)-1] > _sLira_Node_Param[plot_number].WP_Value + 2)
// 						{
// 							Polling_Time = _kLIRA_SLOW_DATA_PROCESS;   /* ask sensor to send its data frequently */
// 						}
// 						else
// 						{
// 							Polling_Time = _kLIRA_FAST_DATA_PROCESS;  /* lira node data should send in regular interval */
// 						}
// 					}
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("Data recvd From Lira Sensor no : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Lira_Sensor_Number);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("Battery level : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Lira_Battery_Level);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("SM1 : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Percent_Moisture[0]);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("SM2 : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Percent_Moisture[1]);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("SM3 : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Percent_Moisture[2]);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("SM4 : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Percent_Moisture[3]);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("Soil temp : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Soil_Temp);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("Air temp : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Air_Temperature);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("Humidity : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Humidity);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					_kSERAIL_MON_WRITE_NO_LN("Light : ");                /* for debugging purpose */
// 					_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[plot_number].Light_Intensity);
// 					_kSERIAL_MON_CLEAR();
// 					
// 					
// 					/*************************************** make the ack packet ***************************************************/
// 					count = _kRESET;
// 					for(i=0;i<_kVALVE_ID_LEN;i++)
// 					{
// 						_sRuble_Parameters.Lora_Data_TX[count++] = _sLira_Node_Param[plot_number].Lira_Node_ID[i];   /* copy the id */
// 					}
// 					
// 					_sRuble_Parameters.Lora_Data_TX[count++] = _eLIRA_SENSOR_POLLING_DATA_PACKET;
// 					
// 					_sRuble_Parameters.Lora_Data_TX[count++] = Polling_Time ;   /* copy polling time lower byte */
// 					
// 					_sRuble_Parameters.Lora_Data_TX[count++] = Polling_Time >> 8; /* copy polling time upper byte */
// 					
// 					
// 					_kSERAIL_MON_WRITE("Priority list :");
// 					
// 					for(i=0;i<_kMAX_LIRA_SENSOR_NODE;i++)
// 					{
// 						_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sRuble_Parameters.Irrig_Priority[i]);
// 					}
// 					
// 				}break;
// 			}
// 			
// 			switch(_sRuble_Parameters.Lora_Data_TX[6])
// 			{
// 				case _eBROADCASTING_PACKET:
// 				case _eLIRA_SENSOR_POLLING_DATA_PACKET:
// 				{
// 					_kLORA_SEND_DATA(count);
// 					_kSERAIL_MON_WRITE("DATA SENT TO LIRA");       /* for debugging purpose */
// 					_sRuble_Parameters.Last_Plot = plot_number;    /* indicate last plot which send the data because have to send this data to server */
// 					_sRuble_Parameters.Publish_Topic |= _ePUB_LIRA_NODE_DATA;    /* update the publish topic */
// 					
// 					Lira_Data_EEPROM(plot_number);   
// 					
// 					/**************************************** check if the battery level is low or not *************************************/
// 					
// 					if(_sLira_Node_Param[plot_number].Lira_Battery_Level <  _kLIRA_PLOT_LOW_BATTARY)
// 					{
// 						_sGSM_Query.SMS_Alert |= _eLIRA_NODE_BATT_LOW;
// 					}
// 					
// 				}break;
// 			}
// 			
// 			return 1;    /* if id get matched lira sensor node */
// 		}
// 	}
	
	return 0;           /*  id not matched with any lira sensor node */
}


void Lira_Data_EEPROM(unsigned char plot_number)
{
	_kEEPROM_WRITE(_kEEPROM_LOC_LIRA_SENSOR_DATA(plot_number), _sLira_Node_Param[plot_number]);
	return;
}

