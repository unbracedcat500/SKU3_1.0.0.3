/*
 * Running_Operation.cpp
 *
 * Created: 24-08-2021 10:46:23
 *  Author: Tushar (SIO)
 */ 


/*
 * Running_Operation.c
 *
 * Created: 23-08-2021 11:14:49
 *  Author: Tushar (SIO)
 */ 

#include <Initialize.h>
#include <Lira_Sensor.h>
#include <Euro_Valve.h>
#include <MQTT_Handler.h>
#include <GSM_GPRS_Network.h>

Ruble_Parameters _sRuble_Parameters;
Pump_Parameter   _sPump_Parameter;
volatile unsigned char _gPub_Buff[_kMAX_PUB_BUFF_SIZE];
volatile unsigned char _gRecvd_Data[110];
Extra_Plot   _sExtra_Plot[_kMAX_EXTRA_PLOT];


void Read_Switch_Status(void)
{
	static unsigned char key_pad,key_press=0,time_count=0,key_press_flag,time_count2;
	
	
	_kGPIO_SET(_kSWITCH_VCC_PIN, _kHIGH);         /* Give vcc to the switch */
	
	key_pad =  _kAUTO_MANUAL_SWITCH_STATUS;      /* update the status of auto manual switch */
	
	if(key_press)     /* if any key pressed */
	{
		if(time_count > 50)     /* if count will 50 (which is 50ms) then only this will execute */
		{
			switch(key_press)
			{
				case 0x01:         /* Switch is in manual mode */
				{
					_sRuble_Parameters.Mode_Switch_Pos	= _eMOD_SWITCH_MANUAL;
				}break;
				
			}
			
			time_count = _kRESET;     /* make count zero for read next when whenever it will press */
			key_press = _kRESET;
		}
		else
		{
			if(key_press == (0x01 ^ key_pad))  /*0x0f which that is button on that pins anding with port a and compare to key_press */
			{
				time_count++;      /* if key_press is still press then count++ */
			}
			else             /* if key not press */
			{
				time_count=_kRESET;
				key_press=_kRESET;
			}
		}
		
	}
	else
	{
		key_press = 0x01 ^ key_pad;   /* assign output operation of and if it will 1 then if statement will execute */
		
		if(!key_press)     /* if no any key pressed */
		{
			if(time_count2 > 50)
			{
				time_count2 = _kRESET;
				_sRuble_Parameters.Mode_Switch_Pos	= _eMOD_SWITCH_AUTO;
			}
			else
			{
				time_count2++;
			}
		}
	}
}

void Update_Indication(void)
{
	if((_sRuble_Parameters.lndications & _eRUBL_INDI_AUTO_MANUAL) !=0)  /* indicate status of the operation mode either manual or not */
	{
		if(!_kGPIO_GET(_kMODE_STATUS_LED))
		{
			_kGPIO_SET(_kMODE_STATUS_LED,_kON);      /* if selected operation is manual operation turn ON the led */
		}
		
	}
	else
	{
		if(_kGPIO_GET(_kMODE_STATUS_LED))
		{
			_kGPIO_SET(_kMODE_STATUS_LED,_kOFF);    /* if selected operation mode is other than manual mode turn off the led */
		}
		
	}
	
	if ((_sRuble_Parameters.lndications & _eRUBL_INDI_PUMP_ON_OFF)!=0)   /* check the pump indication */
	{
		if(!_kGPIO_GET(_kPUMP_STATUS_LED))
		{
			_kGPIO_SET(_kPUMP_STATUS_LED,_kON);                        /* if pump status is ON the turn ON LED */
		}
	}
	else
	{
		if(_kGPIO_GET(_kPUMP_STATUS_LED))
		{
			_kGPIO_SET(_kPUMP_STATUS_LED,_kOFF);                       /* if pump status is Off the turn off LED */
		}
	}
	
	if ((_sRuble_Parameters.lndications & _eRUBL_INDI_SERVER_CONN) != 0)  /* check the network indication */
	{
		if(!_kGPIO_GET(_kNETWORK_STATUS_LED))
		{
			_kGPIO_SET(_kNETWORK_STATUS_LED,_kON);                    /* if ruble is connected to server then turn ON the led */
		}
	}
	else
	{
		if(_kGPIO_GET(_kNETWORK_STATUS_LED))
		{
			_kGPIO_SET(_kNETWORK_STATUS_LED,_kOFF);                    /* if ruble is not connected to server then turn Off the led */
		}
	}
}

void Select_Operation_Mode(void)
{
	switch(_sRuble_Parameters.Temp_Operation_Mode)  /* user wants to change the mode using the app so server will send the desired mode */
	{
		case _eOPER_MOD_AUTO:   /* user decided switch to auto mode */
		{
			switch(_sRuble_Parameters.Operation_Mode)     /* check which was the earlier mode */
			{
				case _eOPER_MOD_TIMER:
				case _eOPER_MOD_DATALOG:
				{
					_sRuble_Parameters.Temp_Operation_Mode = _kRESET;              /* reset the temp mode for next time */
					
					if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_TIMER)
					{
						_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_CHANGED_MODE;   /* to switch to desired irrigation plot according to user */
					}
					else
					{
						_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;      /* to turn of the auto irrigation */
					}
					
					if(_sRuble_Parameters.Mode_Switch_Pos != _eMOD_SWITCH_MANUAL)  /* check the switch position to change the mode */
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Manual Mode selected");                /* for debugging purpose */
							_kSERIAL_MON_CLEAR();
						}
						_sRuble_Parameters.Manual_Mode_Wait = 0;
						_sRuble_Parameters.Operation_Mode =  _eOPER_MOD_SCHEDULER;   //5/5   /* if switch is at auto side then select the auto mode */
						_sGSM_Query.SMS_Alert             |= _eAUTO_MODE_SELECTED;  /* send SMS that auto mode is selected */
						_sRuble_Parameters.lndications   &= ~_eRUBL_INDI_AUTO_MANUAL;   /* indication is not selected mode is not manual mode */
						_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
						
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("schedule Auto Mode selected");                /* for debugging purpose */
							_kSERIAL_MON_CLEAR();
						}
						_sRuble_Parameters.Manual_Mode_Wait = 0;
						_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;   //5/5     /* if switch is at manual side then select the manual mode */
						_sGSM_Query.SMS_Alert            |= _eAUTO_MODE_SELECTED; /* send SMS that manual mode is selected */
						_sRuble_Parameters.lndications   |= _eRUBL_INDI_AUTO_MANUAL;   /* indication is manual mode is selected */
					}
					_sRuble_Parameters.Pump_Run_Timer =  _kRESET;                   /* reset the pump run timer */
					_sRuble_Parameters.Publish_Topic  |=  _ePUB_RUBLE_DATA;          /* publish the controller data to server */
					_sRuble_Parameters.Manual_Mode_Wait = _kRESET;
					/* update the EEPROM that mode is changed */
					_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
				}break;
				
			}
		}break;
		
		case _eOPER_MOD_DATALOG:    /* user decided switch to data log mode */
		{
			switch(_sRuble_Parameters.Operation_Mode)
			{
				case _eOPER_MOD_TIMER:
				case _eOPER_MOD_AUTO:
				case _eOPER_MOD_MANUAL:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Data Logger Mode selected");                /* for debugging purpose */
						_kSERIAL_MON_CLEAR();
					}
					
					_sRuble_Parameters.Temp_Operation_Mode = _kRESET;                /* reset the temp mode for next time */
					_sRuble_Parameters.Operation_Status    = _eOPERATION_STAT_SUSPENDED; /* to turn of the auto irrigation */
					_sRuble_Parameters.Operation_Mode =  _eOPER_MOD_DATALOG;         /* select data logger operation mode */
					_sRuble_Parameters.Pump_Run_Timer =  _kRESET;                    /* reset the pump timer */
					_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA;            /* publish the controller data to server */
					_sRuble_Parameters.lndications   |= _eRUBL_INDI_AUTO_MANUAL;     /* select the indication that manual mode is selected */
					_sGSM_Query.SMS_Alert             |= _eDATA_LOGGER_MODE_SELECTED; /* send SMS that data logger mode is selected */
					
					/* update the EEPROM that mode is changed */
					_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
				}break;
				
			}
		}break;
		
		case _eOPER_MOD_TIMER: /* user decided switch to timer mode */
		{
			switch(_sRuble_Parameters.Operation_Mode)
			{
				case _eOPER_MOD_AUTO:
				{
					_sRuble_Parameters.Temp_Operation_Mode = _kRESET;              /* reset the temp mode for next time */
					_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_CHANGED_MODE;   /* to switch to desired irrigation plot according to user */
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Timer Mode selected");                /* for debugging purpose */
						_kSERIAL_MON_CLEAR();
					}
					
					_sRuble_Parameters.Operation_Mode = _eOPER_MOD_TIMER;            /* select operation mode as timer mode */
					_sRuble_Parameters.Publish_Topic  |= _ePUB_TIMER_FEEDBACK;        /* published data to timer mode feedback */
					_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA; /* published data to timer mode feedback */
					_sRuble_Parameters.lndications   &= ~_eRUBL_INDI_AUTO_MANUAL;    /* select indication that manual mode is not selected */
					_sGSM_Query.SMS_Alert             |= _eTIMER_MODE_SELECTED;        /* send SMS that data auto mode is selected */
					
					/* update the EEPROM that mode is changed */
					_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
				}break;
				
				case _eOPER_MOD_SCHEDULER:
				{
					_sRuble_Parameters.Temp_Operation_Mode = _kRESET;              /* reset the temp mode for next time */
					_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_CHANGED_MODE;   /* to switch to desired irrigation plot according to user */
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Timer Mode selected");                /* for debugging purpose */
						_kSERIAL_MON_CLEAR();
					}
					_sRuble_Parameters.Operation_Mode = _eOPER_MOD_TIMER;            /* select operation mode as timer mode */
					_sRuble_Parameters.Publish_Topic  |= _ePUB_TIMER_FEEDBACK;        /* published data to timer mode feedback */
					_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA; /* published data to timer mode feedback */
					_sRuble_Parameters.lndications   &= ~_eRUBL_INDI_AUTO_MANUAL;    /* select indication that manual mode is not selected */
					_sGSM_Query.SMS_Alert             |= _eTIMER_MODE_SELECTED;        /* send SMS that data auto mode is selected */
					
					/* update the EEPROM that mode is changed */
					_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
					
				}break;
			}
			
			_sRuble_Parameters.Temp_Operation_Mode = _kRESET;              /* reset the temp mode for next time */
			
		}break;
	}
	
	
	
	/************************************************* Choose the operation mode from Running Operation *******************************************/
	
	switch(_sRuble_Parameters.Operation_Mode)
	{
		case _eOPER_MOD_TIMER:
		{
			if(!_sRuble_Parameters.Pump_Run_Timer)    /* if selected mode is timer and time gets over */
			{
				if(_sSchedules.Schedule_Enable)
				{
					if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
					{
						//_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
					}

					_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Scheduler selected");                /* for debugging purpose */
						_kSERIAL_MON_CLEAR();
					}
					_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;
				}
				else
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("schedule Auto Mode selected");                /* for debugging purpose */
						_kSERIAL_MON_CLEAR();
					}
					_sRuble_Parameters.Manual_Mode_Wait = 0;
					_sRuble_Parameters.Operation_Mode  = _eOPER_MOD_SCHEDULER;    // 5/5  /* if timer mode over then switch to auto mode */
					_sRuble_Parameters.Manual_Mode_Wait = _kRESET;
					_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
				}
				
				_sRuble_Parameters.Publish_Topic  |= _ePUB_TIMER_FEEDBACK; /* published data to timer mode feedback */
				_sRuble_Parameters.lndications    &= ~_eRUBL_INDI_AUTO_MANUAL;    /* select indication that manual mode is not selected */
				//_sGSM_Query.SMS_Alert             |= _eTIMER_OFF;                 /* send SMS that timer is OFF */
				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END; /* end of the operation after timer is over */
				_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA; /* published data to timer mode feedback */
				
				/* update the EEPROM that mode is changed */
				_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
				
				_kEEPROM_WRITE(_kTIMER_IRRIGATING_PLOT,_sRuble_Parameters.Irrigating_Plot);
				_kEEPROM_WRITE(_kPUMP_REMAINING_TIME,_sRuble_Parameters.Pump_Run_Timer);
			}
			
			//if(_sRuble_Parameters.Mode_Switch_Pos == _eMOD_SWITCH_MANUAL)   // now
			if(_sRuble_Parameters.Mode_Switch_Pos != _eMOD_SWITCH_MANUAL)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Manual Mode selected");                /* for debugging purpose */
					_kSERIAL_MON_CLEAR();
				}
				_sRuble_Parameters.Pump_Run_Timer = _kRESET;
				
				_sRuble_Parameters.Operation_Mode  = _eOPER_MOD_MANUAL;              /* select operation mode as manual mode */
				_sRuble_Parameters.Publish_Topic  |= _ePUB_TIMER_FEEDBACK;           /* published data to timer mode feedback */
				_sRuble_Parameters.lndications    |= _eRUBL_INDI_AUTO_MANUAL;        /* select the indication that manual mode is selected */
				_sGSM_Query.SMS_Alert             |= _eTIMER_STOPPED;                /* send SMS that timer is stopped */
				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;   /* operation status is suspended timer operation */
				
				
				_sRuble_Parameters.Kill_Switch = _eKILL_SWITCH_NOT_SET;
				_kEEPROM_WRITE(_kEEPROM_KILL_SWITCH,_sRuble_Parameters.Kill_Switch);
				
				/* update the EEPROM that mode is changed */
				_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
				
				
				_kEEPROM_WRITE(_kTIMER_IRRIGATING_PLOT,_sRuble_Parameters.Irrigating_Plot);
				_kEEPROM_WRITE(_kPUMP_REMAINING_TIME,_sRuble_Parameters.Pump_Run_Timer);
				_sRuble_Parameters.Manual_Mode_Wait = _kRESET;
				
			}
		}break;
		
		case _eOPER_MOD_AUTO:
		{
			//if(_sRuble_Parameters.Mode_Switch_Pos == _eMOD_SWITCH_MANUAL)    // now
			if(_sRuble_Parameters.Mode_Switch_Pos != _eMOD_SWITCH_MANUAL)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Manual Mode selected");                /* for debugging purpose */
					_kSERIAL_MON_CLEAR();
				}
				
				_sRuble_Parameters.Operation_Mode = _eOPER_MOD_MANUAL;              /* select operation mode as manual mode */
				_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA;               /* published ruble data */
				_sRuble_Parameters.lndications   |= _eRUBL_INDI_AUTO_MANUAL;        /* select the indication that manual mode is selected */
				_sGSM_Query.SMS_Alert             |= _eMANUAL_MODE_SELECTED;         /* send SMS that manual mode id selected */
				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;   /* operation status is suspended auto operation */
				
				_sRuble_Parameters.Kill_Switch = _eKILL_SWITCH_NOT_SET;
				_kEEPROM_WRITE(_kEEPROM_KILL_SWITCH,_sRuble_Parameters.Kill_Switch);
				
				_sRuble_Parameters.Manual_Mode_Wait = _kRESET;
				
				/* update the EEPROM that mode is changed */
				_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
			}
			
			if(_sSchedules.Schedule_Enable)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Scheduler selected");                /* for debugging purpose */
					_kSERIAL_MON_CLEAR();
				}
				_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;
				_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA;               /* published ruble data */
				_sRuble_Parameters.lndications   &= ~_eRUBL_INDI_AUTO_MANUAL;       /* select the indication that manual mode is not selected */
				_sGSM_Query.SMS_Alert             = _eAUTO_MODE_SELECTED;         /* send SMS that auto mode is selected */
				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;   /* operation status is suspended manual operation */
				
				/* update the EEPROM that mode is changed */
				_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
			}
		}break;
		
		case _eOPER_MOD_SCHEDULER:
		{
			if(_sRuble_Parameters.Mode_Switch_Pos != _eMOD_SWITCH_MANUAL)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Manual Mode selected");                /* for debugging purpose */
					_kSERIAL_MON_CLEAR();
				}
				
				_sRuble_Parameters.Operation_Mode = _eOPER_MOD_MANUAL;              /* select operation mode as manual mode */
				_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA;               /* published ruble data */
				_sRuble_Parameters.lndications   |= _eRUBL_INDI_AUTO_MANUAL;        /* select the indication that manual mode is selected */
				_sGSM_Query.SMS_Alert             |= _eMANUAL_MODE_SELECTED;         /* send SMS that manual mode id selected */
				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;   /* operation status is suspended auto operation */
				
				_sRuble_Parameters.Kill_Switch = _eKILL_SWITCH_NOT_SET;
				_kEEPROM_WRITE(_kEEPROM_KILL_SWITCH,_sRuble_Parameters.Kill_Switch);
				
				_sRuble_Parameters.Manual_Mode_Wait = _kRESET;
				
				/* update the EEPROM that mode is changed */
				_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
			}
			
// 			if(!_sSchedules.Schedule_Enable)    /* check if schedule status is disable */
// 			{
// 				_kSERAIL_MON_WRITE("Auto Mode selected");                /* for debugging purpose */
// 				_kSERIAL_MON_CLEAR();
// 				_sRuble_Parameters.Manual_Mode_Wait = 0;
// 				_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;        //5/5         /* select operation mode as auto mode */
// 				_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA;               /* published ruble data */
// 				_sRuble_Parameters.lndications   &= ~_eRUBL_INDI_AUTO_MANUAL;       /* select the indication that manual mode is not selected */
// 				_sGSM_Query.SMS_Alert             = _eAUTO_MODE_SELECTED;           /* send SMS that auto mode is selected */
// 				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;   /* operation status is suspended manual operation */
// 				
// 				_sRuble_Parameters.Manual_Mode_Wait = _kRESET;
// 				
// 				/* update the EEPROM that mode is changed */
// 				_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
// 			}
			
		}break;
		
		
		
		case _eOPER_MOD_MANUAL:
		{
			//if(_sRuble_Parameters.Mode_Switch_Pos != _eMOD_SWITCH_MANUAL)   // now
			if(_sRuble_Parameters.Mode_Switch_Pos == _eMOD_SWITCH_MANUAL)
			{
				if(_sSchedules.Schedule_Enable)
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Scheduler selected");                /* for debugging purpose */
						_kSERIAL_MON_CLEAR();
					}
					_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;
				}
				else
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Auto Mode selected");                /* for debugging purpose */
						_kSERIAL_MON_CLEAR();
					}
					_sRuble_Parameters.Manual_Mode_Wait = 0;
					_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;   //5/5             /* select operation mode as auto mode */
					_sRuble_Parameters.Manual_Mode_Wait = _kRESET;
				}
				
				_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_DATA;               /* published ruble data */
				_sRuble_Parameters.lndications   &= ~_eRUBL_INDI_AUTO_MANUAL;       /* select the indication that manual mode is not selected */
				_sGSM_Query.SMS_Alert             = _eAUTO_MODE_SELECTED;         /* send SMS that auto mode is selected */
				_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;   /* operation status is suspended manual operation */
				
				/* update the EEPROM that mode is changed */
				_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
			}
		}break;
	}
}

void Run_Operation(void)
{
	unsigned char i;
	unsigned int temp_pump_timer;
	
	switch(_sRuble_Parameters.Operation_Mode)
	{
		case _eOPER_MOD_MANUAL:
		{
			switch(_sRuble_Parameters.Operation_Status)
			{
				case _eOPERATION_STAT_SUSPENDED:
				case _eOPERATION_STAT_OPRN_END:
				{
					Schedules_Monitoring();
					if(_sRuble_Parameters.Manual_Mode_Wait == 0)
					{
						if(Stop_Irrigation())
						{
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_RUNNING;   /* if old operation stops then start the new operation */
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Manual selected,  Operation Start");                /* for debugging purpose */
								_kSERIAL_MON_CLEAR();
							}
						}
						else
						{
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;                   /* something went wrong while old operation stops */
							_sRuble_Parameters.Manual_Mode_Wait = _kRETRY_TIMER_IN_TIMER_MODE;
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Manual , Irrigation fails to stop");                /* for debugging purpose */
								_kSERIAL_MON_CLEAR();
							}
						}
					}
				}break;
				
				case _eOPERATION_STAT_RUNNING:
				{
					/* do nothing */
					/* CHECK ANY UNNECCESSORY VALVE IS OPEN */
				}break;
				
			}
		}break;
		
		case _eOPER_MOD_AUTO:
		{
			switch(_sRuble_Parameters.Operation_Status)
			{
				case _eOPERATION_STAT_CHANGED_MODE:
				case _eOPERATION_STAT_SUSPENDED:
				case _eOPERATION_STAT_OPRN_END:
				{
					if(!_sRuble_Parameters.Kill_Switch_Through_App)
					{
						Schedules_Monitoring();
						if(_sRuble_Parameters.Irrigating_Plot)    /* if any plot is irrigating */
						{
							if(_sRuble_Parameters.Manual_Mode_Wait == 0)
							{
								if(Stop_Irrigation())                            /* if stopped successfully */
								{
									_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_RUNNING;   /* if old operation stops then start the new operation */
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("Mode: Auto Mode , Operation Start");                /* for debugging purpose */
										_kSERIAL_MON_CLEAR();
									}
								}
								else
								{
									_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;      /* something went wrong while old operation stops so again stops the operation */
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("Mode: Auto Mode , Irrigation fails to stop");                /* for debugging purpose */
										_kSERIAL_MON_CLEAR();
									}
									_sRuble_Parameters.Manual_Mode_Wait = _kRETRY_TIMER_IN_TIMER_MODE;
								}
							}
						}
						else   /* if not any plot is irrigating */
						{
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_RUNNING;
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Auto Mode , Going To Start");                /* for debugging purpose */
								_kSERIAL_MON_CLEAR();
							}
						}
					}
				}break;
				
				case _eOPERATION_STAT_RUNNING:
				{
					if(!_sRuble_Parameters.Irrigating_Plot && _sRuble_Parameters.Temp_Operation_Mode != _eOPER_MOD_TIMER
					&& _sRuble_Parameters.Operation_Mode != _eOPER_MOD_TIMER && !_sRuble_Parameters.Kill_Switch_Through_App
					&& !_sRuble_Parameters.Indication_Of_Faults & _eLINE_FAULT_STATUS)  /* if not any plot is irrigating */
					{
						if((_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET && _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kRESET))
						{
							if(1)  /* check if the priority list is not empty */
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Priority list is not empty");                /* for debugging purpose */
									_kSERIAL_MON_CLEAR();
								}
								//_sRuble_Parameters.Irrigating_Plot = _sRuble_Parameters.Irrig_Priority[0];   /* irrigate first plot from priority list */
								_sRuble_Parameters.Start_Monitoring_Pump_Faults_Timer = _kRESET;
							
								for(i=0;i<(_kMAX_LIRA_SENSOR_NODE-1);i++)    /* to remove the first plot from the priority list */
								{
									//_sRuble_Parameters.Irrig_Priority[i] = _sRuble_Parameters.Irrig_Priority[i+1];  /* delete first location from priority list */
								}
								//_sRuble_Parameters.Irrig_Priority[_kMAX_LIRA_SENSOR_NODE-1] = _kRESET;    /* reset last plot as zero */
							
								if(1) // 13/03/2023
								{
									if(!Start_Irrigation())    /* start the irrigation of plot */
									{
										_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
									}
								}
								else
								{
									_sRuble_Parameters.Irrigating_Plot = _kRESET;
								}
							}
						}
					}
					
					else    /* if any plot is irrigating */
					{
						if(_sPump_Parameter.Irrigation_Status)
						{
							/* if any fault occur then stop the irrigation */
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Auto Mode,Irrigation Related Fault occur");
								_kSERIAL_MON_CLEAR();
							}
							if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
							{
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
							}
						}
						
						if((_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status != _kRESET || _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("fogger on Irrigation stops");
								_kSERIAL_MON_CLEAR();
							}
							if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
							{
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
							}
						}
						
						if(_sRuble_Parameters.Kill_Switch_Through_App)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Auto Mode, kill switch through app");
								_kSERIAL_MON_CLEAR();
							}
							if(Stop_Irrigation())       /* stops the irrigation if any faults occur */
							{
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
							}
							else
							{
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
							}
						}
						
						#ifdef _kBATTTERY_IS_AVAILABLE
						if(_sRuble_Parameters.Switch_To_Battery == _eELECTRICITY_IS_NOT_THERE)
						{
							if(_sRuble_Parameters.Irrigating_Plot)
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Mode: Auto Mode, Electricity is not there");
									_kSERIAL_MON_CLEAR();
								}
								if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
								{
									_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
								}
							}
							
						}
						#endif
					}
				}break;
			}
		}break;
		
		case _eOPER_MOD_TIMER:
		{
			static unsigned char eeprom_timer;
			switch(_sRuble_Parameters.Operation_Status)
			{
				case _eOPERATION_STAT_CHANGED_MODE:    /* from server */
				case _eOPERATION_STAT_SUSPENDED:       /* from manual mode */
				case _eOPERATION_STAT_OPRN_END:        /* from timer mode */
				{
					if(_sRuble_Parameters.Timer_Mode_Wait_Timer == _kRESET)
					{
						if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER && _sRuble_Parameters.Line_Fault_Flag == _kSET)
						{
							_sRuble_Parameters.Timer_Mode_Wait_Timer = 4*60;
							_kEEPROM_WRITE(_kTIMER_IRRIGATING_PLOT,_sRuble_Parameters.Temp_Irrigating_Plot);
							return; 
						}
						
						if((_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status != _kRESET ||
							_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET))
						{
							_sRuble_Parameters.Timer_Mode_Wait_Timer = 60;
							return;
						}
						
						eeprom_timer = _kRESET;
						_sRuble_Parameters.Timer_Feedback_Send_Timer = _kRESET;
						Schedules_Monitoring();
							
						if(_sRuble_Parameters.Irrigating_Plot)   /* check if any plot is irrigating */
						{
							if(Stop_Irrigation())
							{
								_kSERAIL_MON_WRITE("Mode:Timer Mode , Operation Start");            /* for debugging purpose */
								_kSERIAL_MON_CLEAR();
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_RUNNING;
							}
							else
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Mode: Timer Mode , Irrigation fails to stop but run for next plot ");  /* for debugging purpose */
									_kSERIAL_MON_CLEAR();
								}
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_RUNNING;
								_sGSM_Query.SMS_Alert |= _eTIMER_STOPPED;
							}
						}
						else
						{
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_RUNNING;
						}
					}
					else
					{
						//_kSERAIL_MON_WRITE("Mode:Timer Mode , _sRuble_Parameters.Timer_Mode_Wait_Timer");	
					}
					
				}break;
				
				case _eOPERATION_STAT_RUNNING:
				{
					eeprom_timer++;
					if(eeprom_timer > 170)
					{
						eeprom_timer = _kRESET;
						_kEEPROM_WRITE(_kPUMP_REMAINING_TIME,_sRuble_Parameters.Pump_Run_Timer);
					}
					
					
					
					if(!_sRuble_Parameters.Irrigating_Plot)   /* if not any plot is irrigating already */
					{
						_sRuble_Parameters.Publish_Topic  |= _ePUB_TIMER_FEEDBACK;
						_kEEPROM_READ(_kPUMP_REMAINING_TIME,temp_pump_timer);
						_sRuble_Parameters.Pump_Run_Timer = temp_pump_timer;
						
						_sRuble_Parameters.Start_Monitoring_Pump_Faults_Timer = _kRESET;
						
						_sRuble_Parameters.Timer_Feedback_Send_Timer = _kTIMER_FEEDBACK_SEND_TIMER;
						
						if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET &&
						 _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kRESET)
						{
							_sRuble_Parameters.Irrigating_Plot = _sRuble_Parameters.Temp_Irrigating_Plot;   /* update the irrigating plot which was earlier set by server */
							_kEEPROM_WRITE(_kTIMER_IRRIGATING_PLOT,_sRuble_Parameters.Irrigating_Plot);
							
							if(!Start_Irrigation())     /* start the irrigation */
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Mode: Timer Mode , Timer mode has been failed due to not start stop 0.1");
									_kSERIAL_MON_CLEAR();
								}
								
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;
								_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
								
								if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER && _sRuble_Parameters.Line_Fault_Flag == _kSET)
								{
									_sRuble_Parameters.Timer_Mode_Wait_Timer = 4*60;
								}
								else
								{
									_sRuble_Parameters.Timer_Mode_Wait_Timer = _kRETRY_TIMER_IN_TIMER_MODE;
								}
							}
							else
							{
								if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("Mode: Timer Mode , Timer mode has been failed due to not start stop 0.1");
										_kSERIAL_MON_CLEAR();
									}
									_sRuble_Parameters.Timer_Mode_Wait_Timer = _kRETRY_TIMER_IN_TIMER_MODE;
									_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;
									_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
								}
							}
							
						}
						else
						{
							_sRuble_Parameters.Timer_Mode_Wait_Timer = _kRETRY_TIMER_IN_TIMER_MODE;
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;
							_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
						}
						
					}
					if(_sRuble_Parameters.Pump_Run_Timer)   /* if timer is not over */     /* 28/12/2021*/
					{
						_kSERAIL_MON_WRITE("Pump Run Timer in Timer : ");
						_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Pump_Run_Timer);
						_kSERIAL_MON_CLEAR();
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Mode: Timer Mode , Time Over");                /* for debugging purpose */
							_kSERIAL_MON_CLEAR();
						}
						Stop_Irrigation();
						_sRuble_Parameters.Pump_Run_Timer = _kRESET;
					}
					
					
					if(_sPump_Parameter.Irrigation_Status ||
					(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status != _kRESET 
					|| _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET))
					{
						if(_sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_DRY_RUN || _sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_LINE_FAULT ||
						_sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_OVER_LOAD  || _sPump_Parameter.Irrigation_Status == _eTHREE_PHASE_POWER_INT ||
						_sPump_Parameter.Irrigation_Status == _eWIRELESS_PUMP_FAIL_FAULT || _sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_EURO_FEEDBACK ||
						(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status != _kRESET ||
						 _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET))
						{
							#ifdef _kTIMER_RESUME_ENABLE
							Stop_Irrigation();
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Timer Mode, Irrigation Related Fault occur 0");
								_kSERIAL_MON_CLEAR();
							}
							if((_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status != _kRESET ||
							 _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Mode: Timer Mode, Irrigation Related Fault occur 1");
									_kSERIAL_MON_CLEAR();
								}
								_sRuble_Parameters.Timer_Mode_Wait_Timer = 60;
							}
							else
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("Mode: Timer Mode, Irrigation Related Fault occur 2");
									_kSERIAL_MON_CLEAR();
								}
								
								if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER && _sRuble_Parameters.Line_Fault_Flag == _kSET)
								{
									_sRuble_Parameters.Timer_Mode_Wait_Timer = 4*60;
								}
								else
								{
									_sRuble_Parameters.Timer_Mode_Wait_Timer = _kRETRY_TIMER_IN_TIMER_MODE;
								}
							}
							
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_SUSPENDED;
							
							#endif
							
							#ifndef _kTIMER_RESUME_ENABLE
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Timer Mode, Irrigation Related Fault occur");
								_kSERIAL_MON_CLEAR();
							}
							Stop_Irrigation();
							_sRuble_Parameters.Pump_Run_Timer = _kRESET;
							_sGSM_Query.SMS_Alert |= _eTIMER_STOPPED;
							#endif
							
						}
						else
						{
							/* if any fault occur then stop the irrigation */
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Timer Mode, Irrigation Related Fault occur 3");
								_kSERIAL_MON_CLEAR();
							}
							Stop_Irrigation();
							if((_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET || _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kRESET))
							{
								_sRuble_Parameters.Pump_Run_Timer = _kRESET;
							}	
							_sGSM_Query.SMS_Alert |= _eTIMER_STOPPED;
						}	
					}
					
					if(_sRuble_Parameters.Kill_Switch_Through_App)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Mode: Timer Mode, kill switch through app");
							_kSERIAL_MON_CLEAR();
						}
						if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
						{
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
						}
						else
						{
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
						}
					}
					
					
					#ifdef _kBATTTERY_IS_AVAILABLE
					if(_sRuble_Parameters.Switch_To_Battery == _eELECTRICITY_IS_NOT_THERE)
					{
						if(_sRuble_Parameters.Irrigating_Plot)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Auto Mode, Electricity is not there");
								_kSERIAL_MON_CLEAR();
							}
							if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
							{
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
							}
						}
						
					}
					#endif
					
				}break;
			}
		}break;
		
		case _eOPER_MOD_SCHEDULER:
		{
			switch(_sRuble_Parameters.Operation_Status)
			{
				case _eOPERATION_STAT_CHANGED_MODE:    /* from server */
				case _eOPERATION_STAT_SUSPENDED:       /* from manual mode */
				case _eOPERATION_STAT_OPRN_END:        /* from timer mode */
				{
					if(_sRuble_Parameters.Irrigating_Plot)   /* check if any plot is irrigating */
					{
						if(1)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Scheduler mode , Operation Start");            /* for debugging purpose */
								_kSERIAL_MON_CLEAR();
							}
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_RUNNING;
						}
						else
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Mode: Scheduler Mode , Irrigation fails to stop");  /* for debugging purpose */
								_kSERIAL_MON_CLEAR();
							}
							_sRuble_Parameters.Pump_Run_Timer = _kRESET;
						}
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Mode: Scheduler , Irrigation");                /* for debugging purpose */
							_kSERIAL_MON_CLEAR();
						}
						_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_RUNNING;
					}
				}break;
				
				case _eOPERATION_STAT_RUNNING:
				{
					Schedules_Monitoring();
				}break;
			}
			
		}break;
	}
}

bool Start_Irrigation(void)
{
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("Try to Start The Irrigation");                /* for debugging purpose */
	}
	_sPump_Parameter.Irrigation_Status = _kRESET;
	
	if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER && _sRuble_Parameters.Line_Fault_Flag == _kSET)
	{
		return 0;
	}
	
	#ifdef _kBATTTERY_IS_AVAILABLE
	if(_sRuble_Parameters.Switch_To_Battery == _eELECTRICITY_IS_NOT_THERE)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Fail to start electricity is not there");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		Extra_Plot_Valve_Grouping_Handler(_kOFF, (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM));
		return 0;                   /* if irrigation doesn't start successfully */
	}
	#endif
	
	Hard_Coded_Flush_Valve(_kCHECK_TRY_TO_ON);
	
	if(Valve_Grouping_Handler(_kON))   /* check either all group valves are ON or not */
	{
		_sRuble_Parameters.Start_Monitoring_Pump_Faults_Timer = _kRESET;
		
		if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET && _sRuble_Parameters.Pump_Selected_For_Irrigation == 1)
		{
			if((_kGPIO_GET(_kPUMP_1) != _kPUMP_ON))     /* check weather pump is already ON */
			{
				_kGPIO_SET(_kPUMP_1,_kPUMP_ON);      /* turn ON the pump */
			}
			
			if((_kGPIO_GET(_kPUMP_2) != _kPUMP_ON))     /* check weather pump is already ON */
			{
				_kGPIO_SET(_kPUMP_2,_kPUMP_ON);      /* turn ON the pump */
			}
			
			
			_sRuble_Parameters.Wireless_Pump_Handle_List[_sRuble_Parameters.Pump_Selected_For_Irrigation -1] = _kON;
			_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
		}
		else
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("wl 1 pump trigger 11111111111111111111");
				_kSERIAL_MON_CLEAR();
			}
			if(Wireless_Pump_On_Off(_ePUMP_ON,_sRuble_Parameters.Pump_Selected_For_Irrigation -2))
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("wl 1 pump trigger on");
					_kSERIAL_MON_CLEAR();
				}
				_sRuble_Parameters.Wireless_Pump_Handle_List[_sRuble_Parameters.Pump_Selected_For_Irrigation -1] = _kON;
				_sRuble_Parameters.Wirelss_Pump_Status = _kON;
				_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
				_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
				//return 1;
			}
			else
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("wl 1 pump trigger fail to on");
					_kSERIAL_MON_CLEAR();
				}
				if(_sRuble_Parameters.Irrigating_Plot == _kRESET && _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status !=_kSCHEDULER_ON
				&& _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation  !=_kSET
				&& _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status   !=_kSET
				/*&& _sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation !=_kSET
				&& _sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status   !=_kSET*/)
				{
					_sRuble_Parameters.Wirelss_Pump_Status = _kON;
				}
				else
				{
					_sRuble_Parameters.Wirelss_Pump_Status = _kON;
				}
				
				_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
				_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
				
				_sRuble_Parameters.Wireless_Pump_Handle_List[_sRuble_Parameters.Pump_Selected_For_Irrigation -1] = _kOFF;
				_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
				Stop_Irrigation();          /* turn off the valve whichever ON */
				Extra_Plot_Valve_Grouping_Handler(_kOFF, (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM));
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Irrigation fail Start");                /* for debugging purpose */
					_kSERIAL_MON_CLEAR();
				}
				
				return 0;
				
			}
		}
		
		
		_sRuble_Parameters.Wireless_Pump_Operate_Timer = _kMAX_TIME_TO_MONITOR_WIRELESS_PUMP;
		_sRuble_Parameters.lndications |=_eRUBL_INDI_PUMP_ON_OFF;
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Irrigation Start");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
		return 1;                    /* if irrigation start successfully then return 1 */
	}
	else
	{
		Stop_Irrigation();          /* turn off the valve whichever ON */
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Irrigation fail Start");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
		return 0;                   /* if irrigation doesn't start successfully */
	}
	
}

bool Stop_Irrigation(void)
{
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("Try to stop Irrigation");
		_kSERIAL_MON_CLEAR();
	}
	unsigned char pump_number;
	
	if(_sRuble_Parameters.Immediately_Turn_Off_Pump !=0 && _sRuble_Parameters.Immediately_Turn_Off_Pump <= _kMAX_WIRELESS_PUMP)
	{
		pump_number = _sRuble_Parameters.Immediately_Turn_Off_Pump;
		_sRuble_Parameters.Immediately_Turn_Off_Pump = 0;
	}
	else
	{
		pump_number = _sRuble_Parameters.Pump_Selected_For_Irrigation;
	}
	
	if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET && pump_number > 1)
	{
		if(_sRuble_Parameters.Wireless_Pump_Command == _eWIRELESS_PUMP_ON)
		{
			_sRuble_Parameters.Wireless_Pump_Command = _eWIRELESS_PUMP_OFF;
			_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
		}
	}
	
	
	if((/*_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kSET
	&&*/ _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation !=_kSET) || 
	_sRuble_Parameters.Line_Fault_Flag == _kSET)
	{
		if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET && pump_number == 1)
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
				_kSERAIL_MON_WRITE("wl 1 pump trigger 22222222222222222222222");
			}
			if(Wireless_Pump_On_Off(_ePUMP_OFF,pump_number-2))
			{
				_sRuble_Parameters.Wireless_Pump_Handle_List[pump_number -1] = _kOFF;
				_sRuble_Parameters.Pending_Plots[_sRuble_Parameters.Irrigating_Plot - 1] = _kRESET;
				_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
				_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
				_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
			}
			else
			{
				_sRuble_Parameters.Wireless_Pump_Handle_List[pump_number -1] = _kOFF;
				_sRuble_Parameters.Pending_Plots[_sRuble_Parameters.Irrigating_Plot - 1] = _kSET;
				
				
				if(_sRuble_Parameters.Irrigating_Plot == _kRESET
				&& _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation  !=_kSET
				&& _sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation !=_kSET)
				{
					_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
				}
				else
				{
					_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
				}
				_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
				
				_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
				
				_sRuble_Parameters.lndications   &=   ~_eRUBL_INDI_PUMP_ON_OFF;
				_sRuble_Parameters.Irrigating_Plot = _kRESET;     /* reset the irrigating plot */
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Irrigation Stops");
					_kSERIAL_MON_CLEAR();
				}
				_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
			}
		}
	}
	
	
	if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
	{
		return 1;       /* if not any plot is irrigating then return 1 */
	}
	
	_sRuble_Parameters.Last_Timer_Plot = _sRuble_Parameters.Irrigating_Plot;    // 13/06/2021
	
	Hard_Coded_Flush_Valve(_kCHECK_TRY_TO_OFF);
	if(Valve_Grouping_Handler(_kOFF))   /* check either all group valves are OFF or not */
	{
		_sRuble_Parameters.lndications   &=   ~_eRUBL_INDI_PUMP_ON_OFF;
		_sRuble_Parameters.Irrigating_Plot = _kRESET;     /* reset the irrigating plot */
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Irrigation Stops");
			_kSERIAL_MON_CLEAR();
		}
		_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
		return 1;                    /* if irrigation stops successfully then return 1 */
	}
	else
	{
		_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
		_sRuble_Parameters.Irrigating_Plot = _kRESET;        /* reset the irrigating plot */
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Irrigation fail to Stop");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		return 0;                   /* if irrigation doesn't stop successfully */
	}
}

char Valve_Grouping_Handler(unsigned char on_off)
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
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		for(i=0;valve_number_list[i]!=0;i++)
		{
			_kSERAIL_MON_WRITE("valve list enter ");
			_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE((int )valve_number_list[i]);
			Serial.println();
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
		}
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			for(i=0;valve_number_list[i]!=0;i++)
			{
				_kSERAIL_MON_WRITE("valve list enter ");
				_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE((int )valve_number_list[i]);
				_kSERIAL_MON_CLEAR();
				Serial.println();
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
			
			if(_sRuble_Parameters.Irrigating_Plot <= _kMAX_LIRA_SENSOR_NODE )
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("valve list : ");
					_kSERIAL_MON_CLEAR();
				}
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
					{
						_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i]);
						_kSERAIL_MON_WRITE_NO_LN(" ");
					}
					
					_kSERAIL_MON_WRITE_NO_LN("Irrigating plot no : ");
					_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Irrigating_Plot-1);
					_kSERIAL_MON_CLEAR();
				}
				
				for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
				{
					if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && _sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] > 0)
					{
						unsigned char valve_number =0;
						unsigned char euro_valve_num=0,command_valve=0;
						
						valve_number =  _sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i];
						
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
									Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_eSUBSCRIBE_DATA);
									//mqttCallback((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff));
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
													if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
													{
														/********************************* turn on/off the wired valve *********************************/
														_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
											_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
											_sRuble_Parameters.Last_Valve = current_valve_number;
											_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
											
											for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
											{
												if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
												{
													/********************************* turn on/off the wired valve *********************************/
													_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
											if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
											{
												/********************************* turn on/off the wired valve *********************************/
												_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
								if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
								{
									_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
									_gfault_Occure = _kFAULT_RESET_TIMER;
									_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
									_sRuble_Parameters.Last_Valve = current_valve_number;
									_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
									
									for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
									{
										if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
										{
											/********************************* turn on/off the wired valve *********************************/
											_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
					
					else if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO
					&& _sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] < 100)
					{
						/********************************* turn on/off the wired valve *********************************/
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(!on_off));
						_kDELAY_MS(200);
						
						if(on_off == _kON)
						{
							
						}
					}
					else if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] > 100)
					{
						if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] == 101)
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
			else
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("valve list : ");
					_kSERIAL_MON_CLEAR();
				}
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
					{
						_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i]);
						_kSERAIL_MON_WRITE_NO_LN(" ");
					}
				}
				
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE_NO_LN("Irrigating plot no : ");
					_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Irrigating_Plot-1);
					_kSERIAL_MON_CLEAR();
				}
				
				for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
				{
					if(_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && _sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i] > 0)
					{
						unsigned char valve_number =0;
						unsigned char euro_valve_num=0,command_valve=0;
						
						valve_number =  _sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i];
						
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
									Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_eSUBSCRIBE_DATA);
									//mqttCallback((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff));
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
									/* check the id get matched to the valve or not */
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE_NO_LN("Euro valve rx data : ");
										_kSERIAL_MON_WRITE((char *)_sRuble_Parameters.Lora_Data_Rx,10);
										Serial.println();
										
										_kSERAIL_MON_WRITE_NO_LN("Euro valve ID which is Sent :");
										_kSERIAL_MON_WRITE(_sEuro_Valve_Param[current_valve_number].Euro_Valve_ID,7);
										Serial.println();
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
													if(_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
													{
														/********************************* turn on/off the wired valve *********************************/
														
														_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
												if(_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
												{
													/********************************* turn on/off the wired valve *********************************/
													_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
											if(_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
											{
												/********************************* turn on/off the wired valve *********************************/
												_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
								if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
								{
									_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
									_gfault_Occure = _kFAULT_RESET_TIMER;
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
					
					else if(_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
					{
						/********************************* turn on/off the wired valve *********************************/
						
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[(_sRuble_Parameters.Irrigating_Plot-1) - (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE)].Euro_Valve_Group[i]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(!on_off));
						_kDELAY_MS(200);
						if(on_off == _kON)
						{
						}
					}
					
					else if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] > 100)
					{
						if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] == 101)
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

void Lora_Operation(void)
{
	unsigned char temp_id[_kVALVE_ID_LEN],i;
	
	if(_kLORA_DATA_AVAIALABLE())     /* check weather data is available */
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Lora Data Recived");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		
		if(_kLORA_RECV_DATA())            /* read the data and check weather it received successfully or not */
		{
			/************************************ print the data recieved data ************************************/
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				for(i=0;_sRuble_Parameters.Lora_Data_Rx[i] !=0;i++)
				{
					_kSERIAL_MON_WRITE(_sRuble_Parameters.Lora_Data_Rx[i]);
				}
			}
			
			/* copy the id */
			for(i=0;i<_kVALVE_ID_LEN;i++)
			{
				temp_id[i] = _sRuble_Parameters.Lora_Data_Rx[i];   /* copy each byte one by one */
			}
			
			if(!Update_Lira_Sensor_Data(temp_id))    /* check weather is lira node has send the packet */
			{
				if(!Update_Euro_Valve_Data(temp_id))   /* check weather EURO valve have sent the packet */
				{
					if(!Update_Wireless_Pump_Data(temp_id))
					{
						memset(_sRuble_Parameters.Lora_Data_Rx,0,sizeof(_sRuble_Parameters.Lora_Data_Rx));  /* discard the package */
					}	
				}
			}
		}
	}
}

void Pub_Sub_Data(char* topic, unsigned char * payload, unsigned int len, unsigned char Pub_Sub)
{
	//static unsigned char plot_schedule_topic = _kRESET;
	StaticJsonBuffer<1580> jsondata;
	
	switch(Pub_Sub)
	{
		case _eSUBSCRIBE_DATA:
		{  
			unsigned char temp_data,send_feedback=0,temp_plot = 0, valve_num = 0,temp_array[_kMAX_VALVE_FOR_1PLOT][_kVALVE_ID_LEN+2],i,j,k,wired_valve,id_len,my_data;
			//unsigned char temp_data[10];
			JsonObject& temp_object = jsondata.parseObject(payload);
			
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("mqtt_callback_function 1");
			}
			
			if (temp_object.success())
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("mqtt_callback_function 2");
				}
			
				/************************************************************** ruble register feedback *****************************************************************/
			
				if(strcmp(_sSubscribe_Topic.Ruble_Register_Feedback, topic) == 0)
				{
					plot_schedule_topic = _kRESET;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Get Controller is registered to Sever Topic");
						_kSERIAL_MON_CLEAR();
					}
					
					_sRuble_Parameters.Ruble_Registration_Status =  _eRUBLE_REGISTERED;     /* update that ruble is registered to server */
					temp_data = _kCHECK_REGISTRATION;                                    /* update on the memory location */
					_kEEPROM_WRITE(_kEEPROM_LOC_READ_REGISTRATION, temp_data);              /* update the ruble is registered */
					send_feedback = _kSET;
				}
				/********************************************************* set timer topic ****************************************************************/
			
				if(strcmp(_sSubscribe_Topic.Ruble_Set_Timer, topic) == 0)
				{
					unsigned char stored_plot_num;
					plot_schedule_topic = _kRESET;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("get TIMER from server");
						_kSERIAL_MON_CLEAR();
					}
					if(_sRuble_Parameters.Kill_Switch != _eKILL_SWITCH_SET)
					{
						if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_MANUAL && _sRuble_Parameters.Operation_Mode != _eOPER_MOD_DATALOG)
						{
							stored_plot_num = temp_object[F("pNo")];
							if(stored_plot_num != 0)
							{
								if(stored_plot_num <= _kMAX_LIRA_SENSOR_NODE && _sLira_Node_Param[stored_plot_num-1].Lira_Sensor_Number)
								{
									_sRuble_Parameters.Temp_Irrigating_Plot = stored_plot_num;
									if(_sRuble_Parameters.Temp_Irrigating_Plot == 1)
									{
										return;
									}
									if(_sRuble_Parameters.Temp_Irrigating_Plot <= _kMAX_LIRA_SENSOR_NODE && _sLira_Node_Param[_sRuble_Parameters.Temp_Irrigating_Plot-1].Lira_Sensor_Number)
									{
										_sRuble_Parameters.Pump_Run_Timer       =  (unsigned int)temp_object[F("t")];
										_sRuble_Parameters.Pump_Run_Timer       *= 60;
										_sRuble_Parameters.Temp_Operation_Mode  = _eOPER_MOD_TIMER;
										
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE_NO_LN("PUMP RUN TIMER");
											_kSERIAL_MON_CLEAR();
											_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Pump_Run_Timer);
											_kSERAIL_MON_WRITE_NO_LN("Irrigating plot");
											_kSERIAL_MON_CLEAR();
											_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Temp_Irrigating_Plot);
										}
										
										_sRuble_Parameters.Timer_Mode_Wait_Timer = _kRESET;
										_sRuble_Parameters.Extra_Plot_Feedback = _kRESET;
									
										/* SELECT PUMP */
										_sRuble_Parameters.Immediately_Turn_Off_Pump = _sRuble_Parameters.Pump_Selected_For_Irrigation;
										if(_sRuble_Parameters.Immediately_Turn_Off_Pump == _sRuble_Parameters.Pump_Selected_For_Irrigation)
										{
											_sRuble_Parameters.Immediately_Turn_Off_Pump = _kRESET;
										}
										_kEEPROM_WRITE(_kPUMP_REMAINING_TIME,_sRuble_Parameters.Pump_Run_Timer);
										_kEEPROM_WRITE(_kPUMP_SELECTED_FOR_IRRIGATION,_sRuble_Parameters.Pump_Selected_For_Irrigation);	
									}
								}
								else
								{
									return;
								
									if(stored_plot_num > (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT) && _sExtra_Plot[(stored_plot_num-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Extra_Plot_Number)
									{
										if(stored_plot_num != _kFLUSH_PLOT_NUMBER)
										{
											stored_plot_num = (stored_plot_num-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT);
											_sExtra_Plot[stored_plot_num].Run_Timer = (unsigned int)temp_object[F("t")];
											_sExtra_Plot[stored_plot_num].Run_Timer *=  60;
											_sExtra_Plot[stored_plot_num].Status = _eSTART_EXTRA_PLOT_OPERATION;
											
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE_NO_LN("extra plot RUN TIMER");
												_kSERIAL_MON_CLEAR();
												_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[stored_plot_num].Run_Timer);
												_kSERAIL_MON_WRITE_NO_LN("extra plot run name");
												_kSERIAL_MON_CLEAR();
												_kSERIAL_MON_PRINT_NUM(stored_plot_num);
											}
											
											_sExtra_Plot[stored_plot_num].Set_Through_Timer = _eSET_THROUGH_TIMER;
											_sExtra_Plot[stored_plot_num].On_Off_Status = _kSET;
											Exra_Plot_Data_EEPROM((stored_plot_num+1));
										}
									}
								}
							}
							else
							{
								_sRuble_Parameters.Temp_Irrigating_Plot = _kRESET;
								stored_plot_num = _kRESET;
							}
						}
					}
					send_feedback = _kSET;
				}

				/************************************************************ ruble coefficient topic *********************************************************************/
			
				if(strcmp(_sSubscribe_Topic.Ruble_Soil_Coff, topic) == 0)
				{
					plot_schedule_topic = _kRESET;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Set coeffecient Topic from server");
						_kSERIAL_MON_CLEAR();
					}
					//_sRuble_Parameters.Coeff_1 = temp_object[F("coeff1")];
					//_sRuble_Parameters.Coeff_2 = temp_object[F("coeff2")];
					//_sRuble_Parameters.Coeff_3 = temp_object[F("coeff3")];
					//_kEEPROM_WRITE(_kEEPROM_LOC_COEFF_1, _sRuble_Parameters.Coeff_1);             /* update the coeff 1 */
					//_kEEPROM_WRITE(_kEEPROM_LOC_COEFF_2, _sRuble_Parameters.Coeff_2);             /* update the coeff 2 */
					//_kEEPROM_WRITE(_kEEPROM_LOC_COEFF_3, _sRuble_Parameters.Coeff_3);             /* update the coeff 3 */
					send_feedback = _kSET;
				}
				/*************************************************************** plot setup topic ***********************************************************/
			
				if(strcmp(_sSubscribe_Topic.Ruble_Plot_Setup, topic) == 0)
				{
					plot_schedule_topic = _kRESET;
					if(temp_plot == _sRuble_Parameters.Irrigating_Plot && _sRuble_Parameters.Irrigating_Plot != _kRESET)
					{
						return;
					}
				
					temp_plot  =  temp_object[F("pNo")];
				
					if(temp_plot == _kFOOGER_PLOT_NUMBER)
					{
						return;
					}
				
					if(temp_plot <= (_kMAX_LIRA_SENSOR_NODE + _kMAX_EXTRA_PLOT+_kMAX_PLOTS_LIMIT) && temp_plot)
					{
						if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
						{
							_sLira_Node_Param[temp_plot-1].Lira_Sensor_Number = temp_plot;
						}
						else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
						{
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Extra_Plot_Number = temp_plot;
						}
					
						memset(temp_array[0],0,sizeof(temp_array[0]));
						memset(temp_array[1],0,sizeof(temp_array[0]));
						memset(temp_array[2],0,sizeof(temp_array[0]));
						memset(temp_array[3],0,sizeof(temp_array[0]));
						memset(temp_array[4],0,sizeof(temp_array[0]));
					
						sprintf((char *)temp_array[0],"%s",(const char *)temp_object[F("valve1")]);
						sprintf((char *)temp_array[1],"%s",(const char *)temp_object[F("valve2")]);
						sprintf((char *)temp_array[2],"%s",(const char *)temp_object[F("valve3")]);
						sprintf((char *)temp_array[3],"%s",(const char *)temp_object[F("valve4")]);
						sprintf((char *)temp_array[4],"%s",(const char *)temp_object[F("valve5")]);
					
						if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
						{
							memset(_sLira_Node_Param[temp_plot-1].Euro_Valve_Group,0,_kMAX_VALVE_FOR_1PLOT);
						}
						else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
						{
							memset(_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group,0,_kMAX_VALVE_FOR_1PLOT);
						}
					
						for(i=0;i<_kMAX_VALVE_FOR_1PLOT ;i++)
						{
							if(temp_array[i][0] != 'V' && temp_array[i][0] != 'v' && temp_array[i][0] != 'P' &&
							temp_array[i][0] != 'p')   /* if first character is not 'V', it means that this is not wired valve */
							{
								if((temp_array[i][0]) != '0' && (temp_array[i][0] != 0))                /* ############# yet to decide ############## */
								{
									wired_valve = temp_array[i][0] - 0x30;
									if(temp_array[i][1] != 0)
									{
										wired_valve = (wired_valve *10 + temp_array[i][1] - 0x30);
									}
								
									if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
									{
										_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = wired_valve + (_kMAX_EURO_VALVE * _kNUMBER_OF_VALVE_CONNECT_TO_EURO);
										if(_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] > ((_kMAX_EURO_VALVE * _kNUMBER_OF_VALVE_CONNECT_TO_EURO) + _kMAX_WIRED_VALVE))
										{
											_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = 0;
										}
									}
									else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
									{
										_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = wired_valve + (_kMAX_EURO_VALVE * _kNUMBER_OF_VALVE_CONNECT_TO_EURO);
										if(_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] > ((_kMAX_EURO_VALVE * _kNUMBER_OF_VALVE_CONNECT_TO_EURO) + _kMAX_WIRED_VALVE))
										{
											_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = 0;
										}
									}
								}
								else
								{
									if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
									{
										_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = 0;
									}
									else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
									{
										_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = 0;
									}
								}
							}
							else
							{
								if(temp_array[i][0] == 'V' || temp_array[i][0] == 'v')   /* if first character is not 'V', it means that this is not wired valve */
								{
									for(j=0;j<_kMAX_EURO_VALVE-1;j++)
									{
										id_len = _kRESET;
										for(k=0;k<_kVALVE_ID_LEN;k++)
										{
											if(_sRuble_Parameters.List_Of_Valve_ID[j][k] == temp_array[i][k])
											{
												id_len++;
											}
										}
									
										if(id_len == _kVALVE_ID_LEN)
										{
											if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
											{
												switch(temp_array[i][7])
												{
													case 0x31:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = (j*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1);
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 1 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x32:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = (j*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+2);
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 2 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x33:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = (j*_kNUMBER_OF_VALVE_CONNECT_TO_EURO) + 3;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 3 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												}
											}
											else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
											{
												switch(temp_array[i][7])
												{
													case 0x31:
													{
														_sExtra_Plot[(temp_plot-1)- (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = (j*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1);
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 1 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x32:
													{
														_sExtra_Plot[(temp_plot-1)-(_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = (j*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+2);
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 2 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x33:
													{
														_sExtra_Plot[(temp_plot-1)-(_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = (j*_kNUMBER_OF_VALVE_CONNECT_TO_EURO) + 3;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 3 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												}
											}
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE_NO_LN("select from list");
												_kSERIAL_MON_CLEAR();
											}
											break;
										}
									}
								
									/********************** if the valve id is not in the list *******************************/
									if((_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] == 0 && temp_plot<= _kMAX_LIRA_SENSOR_NODE) ||
									(_sExtra_Plot[(temp_plot-1)-(_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i]==0 && temp_plot > _kMAX_LIRA_SENSOR_NODE))
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE_NO_LN("valve is not in the list");
											_kSERIAL_MON_CLEAR();
										}
										/******************** check which is the empty array *********************/
										for(valve_num=0;valve_num<_kMAX_EURO_VALVE-1; valve_num++)
										{
											if(_sEuro_Valve_Param[valve_num].Euro_Valve_Number == 0)
											{
												break;
											}
										}
									
										if(valve_num < _kMAX_EURO_VALVE)
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												Serial.println();
												_kSERAIL_MON_WRITE_NO_LN("new valve position ");
												Serial.println();
												_kSERIAL_MON_PRINT_NUM(valve_num);
												_kSERIAL_MON_CLEAR();
											}
										
											for(my_data=0;my_data<_kVALVE_ID_LEN;my_data++)
											{
												_sEuro_Valve_Param[valve_num].Euro_Valve_ID[my_data] = temp_array[i][my_data];
											}
										
											strcpy((char *)_sRuble_Parameters.List_Of_Valve_ID[valve_num],(const char *)_sEuro_Valve_Param[valve_num].Euro_Valve_ID);
										
											_sEuro_Valve_Param[valve_num].Euro_Valve_Number  = valve_num + 1;
											_sEuro_Valve_Param[valve_num].Euro_Battery_Level = 0xff;                 /* default value */
											_sEuro_Valve_Param[valve_num].Euro_Valve_Online = _kVALVE_ONLINE;        /* default ONLINE */
											_sEuro_Valve_Param[valve_num].Euro_Valve_Status = _kOFF;                 /* default ON  */
											_sEuro_Valve_Param[valve_num].Faults = 0;                                /* default ON  */
										
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												Serial.println();
												_kSERAIL_MON_WRITE_NO_LN("VALVE DATA GET FROM SERVER");
												Serial.println();
												_kSERIAL_MON_CLEAR();
												_kSERAIL_MON_WRITE_NO_LN("valve NO: ");
												_kSERIAL_MON_PRINT_NUM(_sEuro_Valve_Param[valve_num].Euro_Valve_Number);
												Serial.println();
												_kSERIAL_MON_CLEAR();
												_kSERAIL_MON_WRITE_NO_LN("valve ID: ");
												_kSERIAL_MON_WRITE((char *)_sEuro_Valve_Param[valve_num].Euro_Valve_ID);
												Serial.println();
												_kSERIAL_MON_CLEAR();
											}
										
											if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
											{
												switch(temp_array[i][7])
												{
													case 0x31:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = (valve_num*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1);
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 1 of euro controller new");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x32:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = (valve_num*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+2);
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 2 of euro controller new");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x33:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = (valve_num*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+3);
														
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 3 of euro controller new");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												}
											}
											else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
											{
												switch(temp_array[i][7])
												{
													case 0x31:
													{
														_sExtra_Plot[(temp_plot-1)- (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = (valve_num*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1);
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 1 of euro controller");
															Serial.println();
														}
													}break;
												
													case 0x32:
													{
														_sExtra_Plot[(temp_plot-1)-(_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = (valve_num*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+2);
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 2 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x33:
													{
														_sExtra_Plot[(temp_plot-1)-(_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = (valve_num*_kNUMBER_OF_VALVE_CONNECT_TO_EURO) + 3;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 3 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												}
											}
											Euro_Data_EEPROM(valve_num);
										}
									}
								}
							
								else if(temp_array[i][0] == 'P' || temp_array[i][0] == 'p')
								{
								
									for(j=0;j<_kMAX_EURO_VALVE;j++)
									{
										id_len = _kRESET;
										for(k=0;k<_kVALVE_ID_LEN;k++)
										{
											if(_sRuble_Parameters.List_Of_Valve_ID[j][k] == temp_array[i][k])
											{
												id_len++;
											}
										}
									
										if(id_len == _kVALVE_ID_LEN)
										{
											if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
											{
												switch(temp_array[i][7])
												{
													case 0x31:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = 101;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 1 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x32:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = 102;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 2 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x33:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = 103;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 3 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												}
											}
											else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
											{
												switch(temp_array[i][7])
												{
													case 0x31:
													{
														_sExtra_Plot[(temp_plot-1)- (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = 101;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 1 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x32:
													{
														_sExtra_Plot[(temp_plot-1)- (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = 102;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 2 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x33:
													{
														_sExtra_Plot[(temp_plot-1)- (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = 103;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 3 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												}
											}
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE_NO_LN("select from list");
												_kSERIAL_MON_CLEAR();
											}
											break;
										}
									}
								
									/********************** if the valve id is not in the list *******************************/
									if((_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] == 0 && temp_plot<= _kMAX_LIRA_SENSOR_NODE) ||
									(_sExtra_Plot[(temp_plot-1)-(_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i]==0 && temp_plot > _kMAX_LIRA_SENSOR_NODE))
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE_NO_LN("valve is not in the list");
											_kSERIAL_MON_CLEAR();
										}
										/******************** check which is the empty array *********************/

										valve_num = _kMAX_EURO_VALVE-1;
										if(valve_num < _kMAX_EURO_VALVE)
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												Serial.println();
												_kSERAIL_MON_WRITE_NO_LN("new valve position ");
												Serial.println();
												_kSERIAL_MON_PRINT_NUM(valve_num);
												_kSERIAL_MON_CLEAR();
											}
										
											for(my_data=0;my_data<_kVALVE_ID_LEN;my_data++)
											{
												for(unsigned char wl_pump_num = 0; wl_pump_num < _kMAX_WIRELESS_PUMP; wl_pump_num++)
												{
													_sWireless_Pump_Param[wl_pump_num].Wireless_Pump_ID[my_data] = temp_array[i][my_data];
												}
											}
										
											for(unsigned char wl_pump_num = 0; wl_pump_num < _kMAX_WIRELESS_PUMP; wl_pump_num++)
											{
												_sWireless_Pump_Param[wl_pump_num].Wireless_Pump_Number = wl_pump_num+1;
											}
										
											for(unsigned char wl_pump_num = 0; wl_pump_num < _kMAX_WIRELESS_PUMP; wl_pump_num++)
											{
												_sWireless_Pump_Param[wl_pump_num].Response_To_Ruble = 0;
											}
										
											for(unsigned char wl_pump_num = 0; wl_pump_num < _kMAX_WIRELESS_PUMP; wl_pump_num++)
											{
												_sWireless_Pump_Param[wl_pump_num].Indication_Of_Faults = 0;
											}
										
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												Serial.println();
												_kSERAIL_MON_WRITE_NO_LN("wireless pump DATA GET FROM SERVER");
												Serial.println();
												_kSERIAL_MON_CLEAR();
											}
										
											if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
											{
												switch(temp_array[i][7])
												{
													case 0x31:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = 101;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 1 of euro controller new");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x32:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = 102;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 2 of euro controller new");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x33:
													{
														_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i] = 103;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 3 of euro controller new");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												}
											}
											else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
											{
												switch(temp_array[i][7])
												{
													case 0x31:
													{
														_sExtra_Plot[(temp_plot-1)- (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = 101;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 1 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x32:
													{
														_sExtra_Plot[(temp_plot-1)- (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = 102;
														
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 2 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												
													case 0x33:
													{
														_sExtra_Plot[(temp_plot-1)- (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i] = 103;
														if(_sRuble_Parameters.Debug_Mode == _kSET)
														{
															_kSERAIL_MON_WRITE_NO_LN("valve 3 of euro controller");
															Serial.println();
															_kSERIAL_MON_CLEAR();
														}
													}break;
												}
											}
										
											for(unsigned char wl_pump_num = 0; wl_pump_num < _kMAX_WIRELESS_PUMP; wl_pump_num++)
											{
												Wireless_Pump_EEPROM(wl_pump_num);
												delay(100);
											}
										}
									}
								}
							}
						}
					
						if(temp_plot <= _kMAX_LIRA_SENSOR_NODE)
						{
							/* check which mode is selected (either data log or auto mode)*/
							temp_data =  temp_object[F("opMode")];
							
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("sensor data get from server");
								_kSERAIL_MON_WRITE_NO_LN("Sensor No.: ");
								_kSERIAL_MON_CLEAR();
								_kSERIAL_MON_PRINT_NUM(_sLira_Node_Param[temp_plot-1].Lira_Sensor_Number);
								Serial.println();
								_kSERIAL_MON_CLEAR();
								_kSERAIL_MON_WRITE_NO_LN("valve 1: ");
								_kSERIAL_MON_WRITE((char *)temp_array[0]);
								Serial.println();
								_kSERIAL_MON_CLEAR();
								_kSERAIL_MON_WRITE_NO_LN("valve 2: ");
								_kSERIAL_MON_WRITE((char *)temp_array[1]);
								Serial.println();
								_kSERIAL_MON_CLEAR();
								_kSERAIL_MON_WRITE_NO_LN("valve 3: ");
								_kSERIAL_MON_WRITE((char *)temp_array[2]);
								Serial.println();
								_kSERIAL_MON_CLEAR();
								_kSERAIL_MON_WRITE_NO_LN("valve 4: ");
								_kSERIAL_MON_WRITE((char *)temp_array[3]);
								Serial.println();
								_kSERIAL_MON_CLEAR();
								_kSERAIL_MON_WRITE_NO_LN("valve 5: ");
								_kSERIAL_MON_WRITE((char *)temp_array[4]);
								Serial.println();
								_kSERIAL_MON_CLEAR();
							}

							/******************** set valve in ascending order ***********************/
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
								{
									Serial.println();
									_kSERAIL_MON_WRITE_NO_LN("VALVE NO : ");
									_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sLira_Node_Param[temp_plot-1].Euro_Valve_Group[i]);
									_kSERIAL_MON_CLEAR();
								}
						
								for(valve_num=0;valve_num<_kMAX_EURO_VALVE;valve_num++)
								{
									Serial.println();
									_kSERAIL_MON_WRITE_NO_LN("valve ID: ");
									_kSERIAL_MON_WRITE((char *)_sEuro_Valve_Param[valve_num].Euro_Valve_ID);
									_kSERIAL_MON_CLEAR();
								}
							}
							Lira_Data_EEPROM(temp_plot-1);
							send_feedback = _kSET;
						}
						else if(temp_plot > (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE))
						{
							send_feedback = _kSET;
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("sensor data get from server");
								_kSERAIL_MON_WRITE_NO_LN("Extra Plot Number.: ");
								_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Extra_Plot_Number);
								_kSERIAL_MON_CLEAR();
							
						
								for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
								{
									Serial.println();
									_kSERAIL_MON_WRITE_NO_LN("VALVE NO : ");
									_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Euro_Valve_Group[i]);
									_kSERIAL_MON_CLEAR();
								}
							}
						
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Min_Valve[0] = temp_object[F("min")][0];
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Min_Valve[1] = temp_object[F("min")][1];
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Max_Value[0] = temp_object[F("max")][0];
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Max_Value[1] = temp_object[F("max")][1];
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Selected_Plot = temp_object[F("sel_plot")];
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].One_Touch = _kRESET;
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Set_Through_Timer = _kRESET;
							_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Run_Timer = _kRESET;
						
							if((temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT) == (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM))
							{
								unsigned int temp;
								_sRuble_Parameters.Pump_Run_Timer_for_flush = _kRESET;
								if(_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Min_Valve[0])
								{
									temp =  _sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Max_Value[0];
									_sSchedules.Flush_Schedule_In_Every = temp *3600;
									temp = _sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Min_Valve[0];
									_sSchedules.Flush_Schedule_For = temp*60;
								}
								else
								{
									_sSchedules.Flush_Schedule_For = 0;
									_sSchedules.Flush_Schedule_In_Every = 0;	
								}
								
							}
							
						    if(_sRuble_Parameters.Debug_Mode == _kSET)
						    {
								for(i=0;i<2;i++)
								{
									Serial.println();
									_kSERAIL_MON_WRITE_NO_LN("min : ");
									_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Min_Valve[i]);
									Serial.println();
									_kSERAIL_MON_WRITE_NO_LN("max : ");
									_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Max_Value[i]);
									_kSERIAL_MON_CLEAR();
								}
								Serial.println();
								_kSERAIL_MON_WRITE_NO_LN("plot sel : ");
								_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[(temp_plot-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Selected_Plot);
								_kSERIAL_MON_CLEAR();
							}
							
							Exra_Plot_Data_EEPROM((temp_plot) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT));
						}
					}
				}
			
				/***************************************************************** euro valve topic ************************************************************/
				/***************************************************************** euro valve topic ************************************************************/
				if(strcmp(_sSubscribe_Topic.Ruble_Euro_Valve_Set, topic) == 0)
				{
					unsigned temp  =  temp_object[F("code")];
					plot_schedule_topic = _kRESET;
				
					switch(temp)
					{
						case _eVALVE_DELETE:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("valve delete topic : ");
								_kSERIAL_MON_CLEAR();
							}
							
							sprintf((char *)temp_array[0],"%s",(const char *)temp_object[F("vId")]);
							for(valve_num=0;valve_num<_kMAX_EURO_VALVE;valve_num++)
							{
								id_len = _kRESET;
								for(k=0;k<_kVALVE_ID_LEN;k++)
								{
									if(_sEuro_Valve_Param[valve_num].Euro_Valve_ID[k] == temp_array[0][k])
									{
										id_len++;
									}
								}
							
								if(id_len == _kVALVE_ID_LEN)
								{
									strcpy((char *)_sEuro_Valve_Param[valve_num].Euro_Valve_ID,(const char *)"000000");
									strcpy((char *)_sRuble_Parameters.List_Of_Valve_ID[valve_num],(const char *)_sEuro_Valve_Param[valve_num].Euro_Valve_ID);
									_sEuro_Valve_Param[valve_num].Euro_Valve_Number = 0;
									_sEuro_Valve_Param[valve_num].Euro_Battery_Level = 0xff;                 /* default value */
									_sEuro_Valve_Param[valve_num].Euro_Valve_Online = _kVALVE_ONLINE;        /* default ONLINE */
									_sEuro_Valve_Param[valve_num].Euro_Valve_Status = _kOFF;                 /* default ON  */
									_sEuro_Valve_Param[valve_num].Faults = 0; 
									
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{                               /* default ON  */
										_kSERAIL_MON_WRITE_NO_LN("VALVE DATA GET FROM SERVER");
										_kSERIAL_MON_CLEAR();
										_kSERAIL_MON_WRITE_NO_LN("valve NO: ");
										_kSERIAL_MON_CLEAR();
										_kSERIAL_MON_PRINT_NUM(_sEuro_Valve_Param[valve_num].Euro_Valve_Number);
										_kSERAIL_MON_WRITE_NO_LN("valve ID: ");
										_kSERIAL_MON_CLEAR();
										_kSERIAL_MON_WRITE((char *)_sEuro_Valve_Param[valve_num].Euro_Valve_ID);
										_kSERIAL_MON_CLEAR();
									}
									Euro_Data_EEPROM(valve_num);
									/********************** check if any plot contain this valve **********************************/
									unsigned char check_all_valves;
								
									for(check_all_valves = 0; check_all_valves < _kNUMBER_OF_VALVE_CONNECT_TO_EURO+1; check_all_valves++)
									{
										for(i=0;i<_kMAX_LIRA_SENSOR_NODE;i++)
										{
											for(j=0;j<_kMAX_VALVE_FOR_1PLOT;j++)
											{
												if(_sLira_Node_Param[i].Euro_Valve_Group[j]!=0 && _sLira_Node_Param[i].Euro_Valve_Group[j] == (valve_num+1) + check_all_valves)
												{
													_sLira_Node_Param[i].Euro_Valve_Group[j] = 0;
													Lira_Data_EEPROM(i);
													if(_sRuble_Parameters.Debug_Mode == _kSET)
													{
														_kSERAIL_MON_WRITE_NO_LN("VALVE delete from Plot no.: ");
														_kSERIAL_MON_CLEAR();
														_kSERIAL_MON_PRINT_NUM(i+1);
														_kSERIAL_MON_CLEAR();
													}
												}
											}
										}
									}
									break;
								}
							}
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								for(valve_num=0;valve_num<_kMAX_EURO_VALVE;valve_num++)
								{
									Serial.println();
									_kSERAIL_MON_WRITE_NO_LN("valve ID: ");
									_kSERIAL_MON_CLEAR();
									_kSERIAL_MON_WRITE((char *)_sEuro_Valve_Param[valve_num].Euro_Valve_ID);
									_kSERIAL_MON_CLEAR();
								}
							}
						
						}break;
					
						case _ePLOT_DALETE:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("plot delete topic : ");
								_kSERIAL_MON_CLEAR();
							}
							unsigned char temp_plot;
							temp_plot = temp_object[F("PLOT_NUM")];
							_sLira_Node_Param[temp_plot-1].Lira_Sensor_Number = _kRESET;
						}break;
					
						case _eSCHEDULE_PLOT_DELETE:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("schedule plot delete topic : ");
								_kSERIAL_MON_CLEAR();
							}
							unsigned char temp_plot;
							temp_plot = temp_object[F("SCHED_NUM")];
							Delete_Schedule(temp_plot);
						}break;
					
						case _eEXTRA_PLOT_DELETE:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("extra plot delete topic : ");
								_kSERIAL_MON_CLEAR();
							}
							unsigned char temp_plot;
							temp_plot = temp_object[F("PLOT_NUM")];
							temp_plot = temp_plot-_kEXTRA_PLOT_START_FROM;
							_sExtra_Plot[temp_plot].Auto_Operation = _kRESET;
							_sExtra_Plot[temp_plot].Extra_Plot_Number = _kRESET;
							_sExtra_Plot[temp_plot].Max_Value[0] = _kRESET;
							_sExtra_Plot[temp_plot].Max_Value[1] = _kRESET;
							_sExtra_Plot[temp_plot].Max_Value[2] = _kRESET;
							_sExtra_Plot[temp_plot].Min_Valve[0] = _kRESET;
							_sExtra_Plot[temp_plot].Min_Valve[1] = _kRESET;
							_sExtra_Plot[temp_plot].Min_Valve[1] = _kRESET;
							_sExtra_Plot[temp_plot].On_Off_Status = _kRESET;
							_sExtra_Plot[temp_plot].One_Touch = _kRESET;
							_sExtra_Plot[temp_plot].Publish_status = _kRESET;
							_sExtra_Plot[temp_plot].Run_Timer = _kRESET;
							_sExtra_Plot[temp_plot].Selected_Plot = _kRESET;
							_sExtra_Plot[temp_plot].Set_Through_Timer = _kRESET;
							_sExtra_Plot[temp_plot].Status = _kRESET;
						}break;
					
						case _ePAUSE_SCHEDULE:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("pause schedule topic : ");
								_kSERIAL_MON_CLEAR();
							}
							unsigned char temp_plot;
							_sSchedules.Schedule_Pause_Resume = _kSET;
							_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
						}break;
					
						case _eRESUME_SCHEDULE:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("resume schedule topic : ");
								_kSERIAL_MON_CLEAR();
							}
							_sSchedules.Schedule_Pause_Resume = _kRESET;
							_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
						}break;
					
						case _eFORCED_STOP:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("force stop topic : ");
								_kSERIAL_MON_CLEAR();
							}
							_sRuble_Parameters.Force_Stop = _kSET;
						}break;
					
						case _eFORCED_STOP_RESUME:
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("force stop resume topic : ");
								_kSERIAL_MON_CLEAR();
							}
							_sRuble_Parameters.Force_Stop = _kRESET;
						
						}break;
					
						case _eRECOVER_DATA:
						{
						}break;
					
					}
				
					send_feedback = _kSET;
				}
			
				/*************************************************************** ruble controller set up *******************************************************/
				if(strcmp(_sSubscribe_Topic.Ruble_Controller_Setup, topic) == 0)
				{
					plot_schedule_topic = _kRESET;
					memset((char *)_sNetwork_and_Usr_Info.User_Sim_No,0, sizeof(_sNetwork_and_Usr_Info.User_Sim_No));
					memset((char *)_sNetwork_and_Usr_Info.Ruble_SIM_No,0,sizeof(_sNetwork_and_Usr_Info.Ruble_SIM_No));
					strcpy((char *) _sNetwork_and_Usr_Info.User_Sim_No,(const char *)temp_object[F("farmerNumber")]);
					strcpy((char *) _sNetwork_and_Usr_Info.Ruble_SIM_No,(const char *)temp_object[F("deviceNumber")]);
					valve_num =  temp_object[F("sms")];
					if(valve_num == 1)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sms alert on");
							_kSERIAL_MON_CLEAR();
						}
						_sGSM_Query.SMS_Alert |=  _eSMS_ALERT_ON;
						_sRuble_Parameters.SMS_Alert_Start = 0x08;
						_kEEPROM_WRITE(_kEEPROM_SMS_ALERT_START,_sRuble_Parameters.SMS_Alert_Start);
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sms alert off");
							_kSERIAL_MON_CLEAR();
						}
						_sRuble_Parameters.SMS_Alert_Start = 0x09;
						_kEEPROM_WRITE(_kEEPROM_SMS_ALERT_START,_sRuble_Parameters.SMS_Alert_Start);
					}
					_kEEPROM_WRITE(_kEEPROM_LOC_RUBLE_SIM_NUM,_sNetwork_and_Usr_Info.Ruble_SIM_No);
					_kEEPROM_WRITE(_kEEPROM_LOC_USER_SIM_NUM, _sNetwork_and_Usr_Info.User_Sim_No);
					send_feedback = _kSET;
				}
				
				
				

				/*********************************************** schedule Topic 1 *************************************/
				if(strcmp(_sSubscribe_Topic.Schedule_Topic,topic) == 0)
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE_NO_LN("Schedule topic get from server : ");
						_kSERIAL_MON_CLEAR();
					}
					_sSchedules.Dummy_Schedule_Enable = temp_object[F("Sen")];
					_sSchedules.Dummy_Based_On_Trigger_Time = _eBASED_ON_TRIGGER_TIME;       //temp_object[F("Tim_Bas")];       /* 1, 0*/
					_sSchedules.Dummy_Cyclic_Schedule_Flag  = 0;       //temp_object[F("Cyclic_Sc")];     /* 1 , 0 */
					_sSchedules.Dummy_Days_In_Between_Schedules   = 1; //temp_object[F("Sc_Days")]; /* 0-7*/
					//_sSchedules.Flush_Schedule_For = _kDEFAULT_FLUSH_TIME;//temp_object[F("Flush_Filt_For")];
					//_sSchedules.Flush_Schedule_For = _sSchedules.Flush_Schedule_For * 60;
					//_sSchedules.Flush_Schedule_In_Every = _kDEFAULT_FlUSH_REPEAT; // temp_object[F("Flush_Filt_Ever")];
					//_sSchedules.Flush_Schedule_In_Every = _sSchedules.Flush_Schedule_In_Every * 60;
					_sSchedules.Dummy_Running_Schedule_At = 0;       //temp_object[F("Running_Sc_Num")];
					_sSchedules.Dummy_Trigger_Time = temp_object[F("ScOn")][0];         //temp_object[F("Trg_Tim")];
					_sSchedules.Dummy_Trigger_Time = (_sSchedules.Dummy_Trigger_Time/100)*3600+(_sSchedules.Dummy_Trigger_Time%100)*60;
					_sSchedules.Dummy_Pump_Selected_For_Schedule =  _sRuble_Parameters.Pump_Selected_For_Irrigation;//temp_object[F("pUMPNUM")];
					_sSchedules.Dummy_Start_From_Today = temp_object[F("ScOff")][0];
					
					
					for(int i=0;i<_kNUMBER_OF_SCHEDULES-1;i++)
					{
						_sSchedules.Dummy_Schedule_Num_List[i] = temp_object[F("ScNum")][i+1];
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("SC NUM : ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Dummy_Schedule_Num_List[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
				
					for(int i=0; i<_kNUMBER_OF_SCHEDULES-1; i++)
					{
						_sSchedules.Dummy_Running_Hr_Of_Each_Plot[i] = temp_object[F("ScOn")][i+1]; //temp_object[F("ScDur")][i+1];
						_sSchedules.Dummy_Running_Hr_Of_Each_Plot[i] = 	(_sSchedules.Dummy_Running_Hr_Of_Each_Plot[i]/100) * 3600 + (_sSchedules.Dummy_Running_Hr_Of_Each_Plot[i]%100) *60;
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("SC on off Time : ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Dummy_Running_Hr_Of_Each_Plot[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
					for(int i=0;i<_kNUMBER_OF_SCHEDULES-1;i++)
					{	
						_sSchedules.Dummy_Schedule_Status[i] = temp_object[F("ScStat")][i+1];
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("SC STAT : ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Dummy_Schedule_Status[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
					send_feedback = _kSET;
					plot_schedule_topic = _kSET;
					_gTank_Topic_check = _kRESET;
				}
			
				/************************************************* tank topic *****************************************/
				if(strcmp(_sSubscribe_Topic.Tank_Schedule, topic) == 0)
				{
					if(plot_schedule_topic == _kRESET || _gTank_Topic_check > 50)
					{
						//plot_schedule_topic = _kRESET;
						return;
					}
					
					plot_schedule_topic = _kRESET;
					
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Get Tank_Schedule from Server");
						_kSERIAL_MON_CLEAR();
					}
					
					_sSchedules.New_Schedules_Occur = _kSET;
					_sSchedules.Schedule_Enable = _sSchedules.Dummy_Schedule_Enable;
				
				
					for(int i=0; i<(_kNUMBER_OF_SCHEDULES * _kMAX_FERTILIZER_TANK)-3 ; i++)
					{
						_sSchedules.Fertilizer_Tank_Status[i] = temp_object[F("TnSt")][i+3];
// 						if(_sRuble_Parameters.Debug_Mode == _kSET)
// 						{
// 							_kSERAIL_MON_WRITE_NO_LN("TANK STAT: ");
// 							_kSERIAL_MON_PRINT_NUM(_sSchedules.Fertilizer_Tank_Status[i]);
// 							_kSERIAL_MON_CLEAR();
// 						}
						
					}
				
					for(int i=0; i<(_kNUMBER_OF_SCHEDULES * _kMAX_FERTILIZER_TANK)-3 ; i++)
					{
						_sSchedules.Fertilizer_Tank_Number[i] = temp_object[F("TnNo")][i+3];
// 						if(_sRuble_Parameters.Debug_Mode == _kSET)
// 						{
// 							_kSERAIL_MON_WRITE_NO_LN("TANK NO: ");
// 							_kSERIAL_MON_PRINT_NUM(_sSchedules.Fertilizer_Tank_Number[i]);
// 							_kSERIAL_MON_CLEAR();
// 						}
					}
					
					for(int i=0; i<(_kNUMBER_OF_SCHEDULES * _kMAX_FERTILIZER_TANK)-3 ; i++)
					{
						
						_sSchedules.Fertilizer_Tank_On_After[i] = temp_object[F("TnOn")][i+3];
						_sSchedules.Fertilizer_Tank_On_After[i] = ((_sSchedules.Fertilizer_Tank_On_After[i]/100)*3600) + ((_sSchedules.Fertilizer_Tank_On_After[i]%100)*60);
						
// 						if(_sRuble_Parameters.Debug_Mode == _kSET)
// 						{
// 							_kSERAIL_MON_WRITE_NO_LN("TANK on after: ");
// 							_kSERIAL_MON_PRINT_NUM(_sSchedules.Fertilizer_Tank_On_After[i]);
// 							_kSERIAL_MON_CLEAR();
// 						}
					}
					
					for(int i=0; i<(_kNUMBER_OF_SCHEDULES * _kMAX_FERTILIZER_TANK)-3 ; i++)
					{	
						_sSchedules.Tank_Off_Time_Duration[i] = temp_object[F("TnOf")][i+3];//temp_object[F("TnDur")][i];
						_sSchedules.Tank_Off_Time_Duration[i] = ((_sSchedules.Tank_Off_Time_Duration[i]/100)*3600) + ((_sSchedules.Tank_Off_Time_Duration[i]%100)*60);
// 						if(_sRuble_Parameters.Debug_Mode == _kSET)
// 						{
// 							_kSERAIL_MON_WRITE_NO_LN("TANK Duration: ");
// 							_kSERIAL_MON_PRINT_NUM(_sSchedules.Tank_Off_Time_Duration[i]);
// 							_kSERIAL_MON_CLEAR();
// 						}
					}
					
					
					
					
					
					
					
					
				
					for(int i=0;i<_kNUMBER_OF_SCHEDULES;i++)
					{
						_sSchedules.Schedule_Status[i] =  _sSchedules.Dummy_Schedule_Status[i];
// 						if(_sRuble_Parameters.Debug_Mode == _kSET)
// 						{
// 							_kSERAIL_MON_WRITE_NO_LN("SC STAT : ");
// 							_kSERIAL_MON_PRINT_NUM(_sSchedules.Schedule_Status[i]);
// 							_kSERIAL_MON_CLEAR();
// 						}
					}
				
					for(int i=0; i<_kNUMBER_OF_SCHEDULES; i++)
					{
						_sSchedules.Running_Hr_Of_Each_Plot[i] = _sSchedules.Dummy_Running_Hr_Of_Each_Plot[i];
// 						if(_sRuble_Parameters.Debug_Mode == _kSET)
// 						{
// 							_kSERAIL_MON_WRITE_NO_LN("SC on off Time : ");
// 							_kSERIAL_MON_PRINT_NUM(_sSchedules.Running_Hr_Of_Each_Plot[i]);
// 							_kSERIAL_MON_CLEAR();
// 						}
					}
				
					for(int i=0;i<_kNUMBER_OF_SCHEDULES;i++)
					{
						_sSchedules.Schedule_Num_List[i] = _sSchedules.Dummy_Schedule_Num_List[i];
// 						if(_sRuble_Parameters.Debug_Mode == _kSET)
// 						{
// 							_kSERAIL_MON_WRITE_NO_LN("SC NUM : ");
// 							_kSERIAL_MON_PRINT_NUM(_sSchedules.Schedule_Num_List[i]);
// 							_kSERIAL_MON_CLEAR();
// 						}
					}
					
					//real_index = 0;
					
					
					unsigned char my_index = 0;
					for(unsigned int real_index=0; real_index<_kNUMBER_OF_SCHEDULES; real_index++)
					{
						if(_sSchedules.Schedule_Status[real_index] == _kSET)
						{
							_sSchedules.Schedule_Status[my_index] = _sSchedules.Schedule_Status[real_index];
							_sSchedules.Running_Hr_Of_Each_Plot[my_index] = _sSchedules.Running_Hr_Of_Each_Plot[real_index];
							_sSchedules.Schedule_Num_List[my_index] = _sSchedules.Schedule_Num_List[real_index];
							
							for(unsigned char tank_number=0;tank_number < _kMAX_FERTILIZER_TANK; tank_number++)
							{
								_sSchedules.Fertilizer_Tank_Status[(my_index*3) + tank_number] = _sSchedules.Fertilizer_Tank_Status[(real_index*3) + tank_number];
								
								_sSchedules.Fertilizer_Tank_Number[(my_index*3) + tank_number] = _sSchedules.Fertilizer_Tank_Number[(real_index*3) + tank_number];
								
								_sSchedules.Fertilizer_Tank_On_After[(my_index*3) + tank_number] = _sSchedules.Fertilizer_Tank_On_After[(real_index*3) + tank_number];
								
								_sSchedules.Tank_Off_Time_Duration[(my_index*3) + tank_number] = _sSchedules.Tank_Off_Time_Duration[(real_index*3) + tank_number];
							}
							
							my_index++;
						}
					}
					
					if(my_index < _kNUMBER_OF_SCHEDULES)
					{
						for(unsigned char check_index=my_index; check_index < _kNUMBER_OF_SCHEDULES; check_index++)
						{
							_sSchedules.Schedule_Status[check_index] = _kRESET;
						}
						
					}
					
					for(int i=0;i<_kNUMBER_OF_SCHEDULES;i++)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("SC STAT : ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Schedule_Status[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
					
					for(int i=0;i<_kNUMBER_OF_SCHEDULES;i++)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("SC on off Time : ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Running_Hr_Of_Each_Plot[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
					
					for(int i=0;i<_kNUMBER_OF_SCHEDULES;i++)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("SC NUM : ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Schedule_Num_List[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
					
					
					
					for(int i=0; i<(_kNUMBER_OF_SCHEDULES * _kMAX_FERTILIZER_TANK)-3 ; i++)
					{
						//_sSchedules.Fertilizer_Tank_Status[i] = temp_object[F("TnSt")][i+3];
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("TANK STAT: ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Fertilizer_Tank_Status[i]);
							_kSERIAL_MON_CLEAR();
						}
						
					}
					
					for(int i=0; i<(_kNUMBER_OF_SCHEDULES * _kMAX_FERTILIZER_TANK)-3 ; i++)
					{
						//_sSchedules.Fertilizer_Tank_Number[i] = temp_object[F("TnNo")][i+3];
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("TANK NO: ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Fertilizer_Tank_Number[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
					
					for(int i=0; i<(_kNUMBER_OF_SCHEDULES * _kMAX_FERTILIZER_TANK)-3 ; i++)
					{
						
						//_sSchedules.Fertilizer_Tank_On_After[i] = temp_object[F("TnOn")][i+3];
						//_sSchedules.Fertilizer_Tank_On_After[i] = ((_sSchedules.Fertilizer_Tank_On_After[i]/100)*3600) + ((_sSchedules.Fertilizer_Tank_On_After[i]%100)*60);
						
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("TANK on after: ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Fertilizer_Tank_On_After[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
					
					for(int i=0; i<(_kNUMBER_OF_SCHEDULES * _kMAX_FERTILIZER_TANK)-3 ; i++)
					{
						//_sSchedules.Tank_Off_Time_Duration[i] = temp_object[F("TnOf")][i+3];//temp_object[F("TnDur")][i];
						//_sSchedules.Tank_Off_Time_Duration[i] = ((_sSchedules.Tank_Off_Time_Duration[i]/100)*3600) + ((_sSchedules.Tank_Off_Time_Duration[i]%100)*60);
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("TANK Duration: ");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.Tank_Off_Time_Duration[i]);
							_kSERIAL_MON_CLEAR();
						}
					}
					
					
					
				
					_sSchedules.Based_On_Trigger_Time = _sSchedules.Dummy_Based_On_Trigger_Time ;
					_sSchedules.Cyclic_Schedule_Flag = _sSchedules.Dummy_Cyclic_Schedule_Flag;  /* 1 , 0 */
					_sSchedules.Days_In_Between_Schedules = _sSchedules.Dummy_Days_In_Between_Schedules; /* 0-7*/
					_sSchedules.Trigger_Time = _sSchedules.Dummy_Trigger_Time;
					
					if(_sSchedules.Based_On_Trigger_Time < _kMAX_BUFFER_TIME)
					{
						_sSchedules.Based_On_Trigger_Time = _kMAX_BUFFER_TIME +60;
					}
					
					_sSchedules.New_Schedul_Date = _sRuble_Parameters.Update_Local_Date[0];
					_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
					_sSchedules.Pump_On_In_Scheduler = _kRESET;
					_sSchedules.Save_Schedule_Data_In_Eeprom = _kRESET;
					_sSchedules.Running_Schedule_At = _sSchedules.Dummy_Running_Schedule_At;
					_sSchedules.Pump_Selected_For_Schedule = _sSchedules.Dummy_Pump_Selected_For_Schedule;
					_sSchedules.Schedule_Start_From_Today = _sSchedules.Dummy_Start_From_Today;
					
					_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE_NO_LN("SC START FROM : ");
						_kSERIAL_MON_PRINT_NUM(_sSchedules.Schedule_Start_From_Today);
					}
					
					send_feedback = _kSET;
					
				}
			
				/**************************************************************** Calibration topic*************************************************************/
				if(strcmp(_sSubscribe_Topic.Calibration, topic) == 0)
				{
					plot_schedule_topic = _kRESET;
					unsigned char cal;
					_sRuble_Parameters.Calibration_Over_Ruled = _kRESET;
					cal = temp_object[F("Calibration")];
					if(cal)
					{
						if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_MANUAL)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Calibration topic get from server");
								_kSERIAL_MON_CLEAR();
							}
							_sRuble_Parameters.Do_Calibration = _kSET;
							send_feedback = _kSET;
						}
					}
					else
					{
						if(_sRuble_Parameters.Pump_Selected_For_Irrigation > _kWIRED_PUMP_NUMBER)	
						{
							_kSERAIL_MON_WRITE("Calibration over ruled");
							_sRuble_Parameters.Do_Calibration = _kSET;
							_sRuble_Parameters.Calibration_Over_Ruled = _kSET;
						}
						else
						{
							_sRuble_Parameters.Calibration_Over_Ruled = 2;
							_sRuble_Parameters.By_Pass_CT = _kRESET;
							_kEEPROM_WRITE(_kCT_BY_PASS,_sRuble_Parameters.By_Pass_CT);
						}
					}
				}
			
				/**************************************************************** Calibration topic*************************************************************/
				if(strcmp(_sSubscribe_Topic.One_Touch_Feedback, topic) == 0)
				{
					plot_schedule_topic = _kRESET;
					unsigned char stored_plot_num = 0;
					unsigned char check_status;
					if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_MANUAL)
					{
						stored_plot_num = temp_object[F("pNo")];
						check_status    = temp_object[F("status")];
					
						if(stored_plot_num == _kFAN_PLOT_NUMBER || stored_plot_num == _kWIRELESS_PUMP_ENABLE_DISABLE_PLOT)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("plot match......................................................");
								_kSERIAL_MON_CLEAR();
							}
						}
						else
						{
							//jsondata.clear();
							return;
						}
						
						if(_sRuble_Parameters.Irrigating_Plot !=0 && stored_plot_num == _kWIRELESS_PUMP_ENABLE_DISABLE_PLOT)
						{
							return;
						}
					
						if(stored_plot_num > (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT) && _sExtra_Plot[(stored_plot_num-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT)].Extra_Plot_Number)
						{
							stored_plot_num = (stored_plot_num-1) - (_kMAX_LIRA_SENSOR_NODE+_kMAX_PLOTS_LIMIT);
							if(_sExtra_Plot[stored_plot_num].Status != _kSCHEDULER_ON
							&& stored_plot_num != (_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM))
							{
								if(!_sExtra_Plot[stored_plot_num].Run_Timer)
								{
									if(check_status)
									{
										_sExtra_Plot[stored_plot_num].One_Touch = _kSET;
										_sExtra_Plot[stored_plot_num].Status = _eSTART_EXTRA_PLOT_OPERATION;
										_sExtra_Plot[stored_plot_num].Set_Through_Timer = _eSET_THROUGH_ONE_TOUCH;
									}
									else
									{
										if(_sExtra_Plot[stored_plot_num].Status == _eSTART_EXTRA_PLOT_OPERATION)
										{
											_sExtra_Plot[stored_plot_num].Status = _eRUN_EXTRA_PLOT_OPERATION;
											_sExtra_Plot[stored_plot_num].Set_Through_Timer = _eSET_THROUGH_ONE_TOUCH;
										}
										else
										{
											_sExtra_Plot[stored_plot_num].Status = _eSTART_EXTRA_PLOT_OPERATION;
											_sExtra_Plot[stored_plot_num].Set_Through_Timer = _eSET_THROUGH_ONE_TOUCH;
										}
										_sExtra_Plot[stored_plot_num].One_Touch = _kRESET;
									}
								
									Exra_Plot_Data_EEPROM((stored_plot_num+1));
									send_feedback = _kSET;
								
								}
							}
						}
					}
				}
			
				/**************************************************************** topic not matched *************************************************************/
			
			}
		
			else
			{
				//_JSON_CLEAR_BUFF();
				//jsondata.clear();
			}
		
			switch(send_feedback)
			{
				case _kSET:
				{
					//jsondata.clear();
					send_feedback = _kRESET;
					memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
					sprintf((char *)_gPub_Buff,"{\"success\": \"true\"}");
					_kMQTT_PUB((unsigned char *)_sPublish_Topic.General_Feedback,(unsigned char *)_gPub_Buff);
					return;
				}break;
			}
		}break;
		
	}

}


void Publish_Data(void)
{
	char temp_data[40],i=0;
	static unsigned char retry = 0,ruble_data_success_counter = 0;
	static unsigned int extra_plot_data_resend_timer = 0;
	retry = 2;
	
	//JsonObject& Transmit_Buff = jsondata.createObject();
	
	StaticJsonBuffer<270> jsonSensorData;
	JsonObject& Transmit_Buff = jsonSensorData.createObject();
				
	if(_sRuble_Parameters.Publish_Topic & _ePUB_RUBLE_FAULTS)          /* if the topic is ruble faults */
	{
		plot_schedule_topic = _kRESET;
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		Transmit_Buff[F("codes")] = _sPump_Parameter.Irrigation_Status;    /* store the irrigation status */
		if(_sRuble_Parameters.Pump_Selected_For_Irrigation > _kWIRED_PUMP_NUMBER)
		{
			Transmit_Buff[F("test")]  =  _sWireless_Pump_Param[_sRuble_Parameters.Pump_Selected_For_Irrigation-2].Indication_Of_Faults;
		}
		else
		{
			Transmit_Buff[F("test")]  =  0;
		}
		
		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                              /* copy the JSON formatted data into the publish buffer */
		while(retry > 0)
		{
			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Ruble_Faults,(unsigned char *)_gPub_Buff))   /* publish data to the server */
			{
				break;
			}
			retry--;
		}
		_sRuble_Parameters.Publish_Topic &= ~_ePUB_RUBLE_FAULTS;        /* reset the topic */
		switch(_sPump_Parameter.Irrigation_Status)                   /* check which fault has been sent to server */
		{
			case _eIRRIG_STAT_EURO_FEEDBACK:
			case _eIRRIG_STAT_EURO_CURRENT_SENSE:
			{
				//_sPump_Parameter.Irrigation_Status = _kRESET;         /* reset the irrigation status if fault was related to the valve */
			}break;
		}
		return;
	}
	
	if(_sRuble_Parameters.Publish_Topic & _ePUB_TIMER_FEEDBACK)   /* if the topic Timer feedback */
	{
		plot_schedule_topic = _kRESET;
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("_ePUB_TIMER_FEEDBACK");
			_kSERIAL_MON_CLEAR();
		}
		Transmit_Buff[F("pStatus")] = !_kGPIO_GET(_kPUMP_1);            /* store the pump status */
		Transmit_Buff[F("medium")] = 0;                                    /* store medium of timer selection */
		
		if(_sRuble_Parameters.Pump_Run_Timer)
		{
			Transmit_Buff[F("time")] = (_sRuble_Parameters.Pump_Run_Timer/60) + 1;
		}
		else
		{
			Transmit_Buff[F("time")] = _sRuble_Parameters.Pump_Run_Timer;
		}
		
		if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_TIMER)
		{
			Transmit_Buff[F("plot")] = _sRuble_Parameters.Temp_Irrigating_Plot;     /* send the irrigating plot number */
		}
		else
		{
			Transmit_Buff[F("plot")] = _sRuble_Parameters.Last_Timer_Plot;    //13/12/2021
			_sRuble_Parameters.Last_Timer_Plot = 0;
		}
		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                           /* copy the JSON formatted data into the publish buffer */

		while(retry > 0)
		{
			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Timer_FeedBack,(unsigned char *)_gPub_Buff))   /* publish data to the server */
			{
				break;
			}
			retry--;
		}
		_sRuble_Parameters.Publish_Topic &= ~_ePUB_TIMER_FEEDBACK;        /* reset the topic */
		return;
	}
	
	if(_sRuble_Parameters.Publish_Topic & _eWIRELESS_PUMP_FAIL)          /* if the topic is ruble faults */
	{
		plot_schedule_topic = _kRESET;
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		Transmit_Buff[F("codes")] = 6;    /* store the irrigation status */
		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                              /* copy the JSON formatted data into the publish buffer */
		while(retry > 0)
		{
			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Ruble_Faults,(unsigned char *)_gPub_Buff))   /* publish data to the server */
			{
				break;
			}
			retry--;
		}
		_sRuble_Parameters.Publish_Topic &= ~_eWIRELESS_PUMP_FAIL;        /* reset the topic */
		
		return;
	}
	
	if(_sRuble_Parameters.Publish_Topic & _ePUB_LIRA_NODE_DATA)     /* if the topic lira node data */
	{
		plot_schedule_topic = _kRESET;
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("_ePUB_LIRA_NODE_DATA");
			_kSERIAL_MON_CLEAR();
		}
		Transmit_Buff[F("packet_id")] = 0;   /* store packet id(packet Number) */
		Transmit_Buff[F("sm1")] = 0;              /* store soil moisture sensor 1 data */
		Transmit_Buff[F("sm2")] = 0;              /* store soil moisture sensor 2 data */
		Transmit_Buff[F("sm3")] = 0;              /* store soil moisture sensor 3 data */
		Transmit_Buff[F("sm4")] = 0;              /* store soil moisture sensor 4 data */
		Transmit_Buff[F("soil_temp")] = 0;   /* store soil temperature */
		Transmit_Buff[F("temperature")] = 0;  /* store the air temperature */
		Transmit_Buff[F("humidity")] = 0;            /* store the humidity */
		Transmit_Buff[F("light")] = 0;          /* store the light intensity */
		Transmit_Buff[F("vBat")] = 0; /* store the battery level */
		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));
		
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERIAL_MON_WRITE((char *)_gPub_Buff);
		}
		/* copy the JSON formatted data into the publish buffer */

		memset(temp_data,0,sizeof(temp_data));
		sprintf(temp_data,"%s/%s",_sPublish_Topic.Lira_Node_Data, "000000"); /* copy the topic including id */
		while(retry > 0)
		{
			if(_kMQTT_PUB((unsigned char *)temp_data,(unsigned char *)_gPub_Buff))   /* publish data to the server */
			{
				break;
			}
			retry--;
		}
		_sRuble_Parameters.Publish_Topic &= ~_ePUB_LIRA_NODE_DATA;             /* reset the topic */
		return;
	}
	
	if(_sRuble_Parameters.Publish_Topic & _ePUB_EURO_VALVE_DATA)         /* if the topic is EURO valve data */
	{
		plot_schedule_topic = _kRESET;
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		if(_sRuble_Parameters.Last_Valve < _kMAX_EURO_VALVE)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("_ePUB_EURO_VALVE_DATA");
				_kSERIAL_MON_CLEAR();
			}
			Transmit_Buff[F("valveStatus")] = _sEuro_Valve_Param[_sRuble_Parameters.Last_Valve].Euro_Valve_Online;  /* store EURO valve ONLINE/OFFLINE */
			Transmit_Buff[F("vBat")] = _sEuro_Valve_Param[_sRuble_Parameters.Last_Valve].Euro_Battery_Level;        /* store the battery level */
			Transmit_Buff[F("valveOpen")] = _sEuro_Valve_Param[_sRuble_Parameters.Last_Valve].Euro_Valve_Status;    /* store valve status ON/OFF */
			Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));           /* copy the JSON formatted data into the publish buffer */
			
			memset(temp_data,0,sizeof(temp_data));
			sprintf(temp_data,"%s/%s",_sPublish_Topic.Euro_Valve_Data, _sEuro_Valve_Param[_sRuble_Parameters.Last_Valve].Euro_Valve_ID);  /* copy the topic including id */
			while(retry > 0)
			{
				if(_kMQTT_PUB((unsigned char *)temp_data,(unsigned char *)_gPub_Buff))   /* publish data to the server */
				{
					break;
				}
				retry--;
			}
		}
		_sRuble_Parameters.Publish_Topic &= ~_ePUB_EURO_VALVE_DATA;       /* reset the topic */
		return;
	}
	
	if(_sRuble_Parameters.Publish_Topic & _ePUB_HARDWARE_FAULTS)     /* if the topic is hardware fault */
	{
		plot_schedule_topic = _kRESET;
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("_ePUB_EURO_VALVE_DATA");
			_kSERIAL_MON_CLEAR();
		}
		Transmit_Buff[F("codes")] = _sRuble_Parameters.Hardware_Faults;       /* store the hardware faults */
		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                               /* copy the JSON formatted data into the publish buffer */
		
		while(retry > 0)
		{
			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Hardware_Faults,(unsigned char *)_gPub_Buff))   /* publish data to the server */
			{
				break;
			}
			retry--;
		}
		_sRuble_Parameters.Publish_Topic &= ~_ePUB_HARDWARE_FAULTS;       /* reset the topic */
		return;
	}
	
	if(_sRuble_Parameters.Publish_Topic & _eELECTRICITY_STATUS)
	{
		
	}
	
	
	if(_sRuble_Parameters.Publish_Topic & _eEXTRA_PLOT_FEEDBACK)
	{	
		plot_schedule_topic = _kRESET;
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Extra Plot Timer Fb");
			_kSERIAL_MON_CLEAR();
		}
		Transmit_Buff[F("pStatus")] = !_kGPIO_GET(_kPUMP_1);            /* store the pump status */
		if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
		{
			if(_sRuble_Parameters.By_Pass_CT)
			{
						
				switch(_sRuble_Parameters.Pump_On_Flag)
				{
					case _ePUMP_OFF:
					{
						Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
					}break;
							
					case _ePUMP_ON:
					{
						Transmit_Buff[F("pStatus")] = 1;                          /* store the pump status */
					}break;
					default:
					{
						Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
					}break;
				}
			}
			else
			{
				if(!_kGPIO_GET(_kPUMP_1))
				{
					Transmit_Buff["pStatus"] = 1;
				}
				else
				{
					Transmit_Buff["pStatus"] = 0;
				}
			}
		}
		else
		{
			if(_sRuble_Parameters.Wirelss_Pump_Status == _kON)
			{
				Transmit_Buff["pStatus"] = 1;
			}
			else
			{
				Transmit_Buff["pStatus"] = 0;
			}
		}
		Transmit_Buff[F("medium")] = 0;                                    /* store medium of timer selection */
		
		if(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback1].Run_Timer)
		{
			Transmit_Buff[F("time")] = (_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback1].Run_Timer/60) + 1;
			Transmit_Buff[F("plot")] =  _sRuble_Parameters.Extra_Plot_Feedback1+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);
		}
		else
		{
			Transmit_Buff[F("time")] = _sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback1].Run_Timer;
			Transmit_Buff[F("plot")] =  _sRuble_Parameters.Extra_Plot_Feedback1+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);
		}
		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff)); 
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{                                          /* copy the JSON formatted data into the publish buffer */
			_kSERIAL_MON_WRITE((char *)_gPub_Buff);
		}
		
		while(retry > 0)
		{
			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Timer_FeedBack,(unsigned char *)_gPub_Buff))   /* publish data to the server */
			{
				_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback1].Publish_status = _kRESET;
				Exra_Plot_Data_EEPROM(_sRuble_Parameters.Extra_Plot_Feedback1+1);
				break;
			}
			retry--;
		}
		_sRuble_Parameters.Publish_Topic &= ~_eEXTRA_PLOT_FEEDBACK;            /* reset the topic */
		return;
	}
	
	if(_sRuble_Parameters.Publish_Topic & _eONE_TOUCH_FEEDBACK)
	{
		plot_schedule_topic = _kRESET;
		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("one touch feedback");
		}
		
		Transmit_Buff[F("pNo")] = _sRuble_Parameters.Extra_Plot_Feedback+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);       /* store the hardware faults */
		
		if(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status == _eSTOP_EXTRA_PLOT_OPERATION && 
		_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Auto_Operation == _kRESET)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("one touch off...................");
				_kSERIAL_MON_CLEAR();
				_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status);
				_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Auto_Operation);
			}
			Transmit_Buff[F("status")]  = 0;
		}
		else
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("one touch on.....................");
				_kSERIAL_MON_CLEAR();
				_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status);
				_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Auto_Operation);
			}
			Transmit_Buff[F("status")]  = 1;
		}
		if(_sRuble_Parameters.Extra_Plot_Feedback < 2 || _sRuble_Parameters.Extra_Plot_Feedback > 9)
		{
			if(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status == _kSCHEDULER_OFF)
			{
				Transmit_Buff[F("status")]  = 0;
			}
			if(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status == _kSCHEDULER_ON)
			{
				Transmit_Buff[F("status")]  = 1;
			}
						
			if(_sRuble_Parameters.Extra_Plot_Feedback == 1)
			{
				if(_kGPIO_GET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Euro_Valve_Group[0]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)]))
				{
					Transmit_Buff[F("status")]  = 0;
				}
				else
				{
					Transmit_Buff[F("status")]  = 1;
				}
			}
		}
					
		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));
		
		/* copy the JSON formatted data into the publish buffer */
		while(retry > 0)
		{
			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.One_Touch_Monitor,(unsigned char *)_gPub_Buff))   /* publish data to the server */
			{
				_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Publish_status = _kRESET;
				Exra_Plot_Data_EEPROM(_sRuble_Parameters.Extra_Plot_Feedback+1);
				break;
			}
			retry--;
		}
		_sRuble_Parameters.Publish_Topic &= ~_eONE_TOUCH_FEEDBACK;       /* reset the topic */
		return;
	}
	
	
	if(_sRuble_Parameters.Publish_Topic & _ePUB_RUBLE_DATA)     /* if the topic is ruble data */
	{
		plot_schedule_topic = _kRESET;
		unsigned int remaining_time_in_min;		
		if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
		{
			if(_sRuble_Parameters.By_Pass_CT)
			{
				switch(_sRuble_Parameters.Pump_On_Flag)
				{
					case _ePUMP_OFF:
					{
						Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
					}break;
					
					case _ePUMP_ON:
					{
						Transmit_Buff[F("pStatus")] = 1;                          /* store the pump status */
					}break;
					default:
					{
						Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
					}break;
				}
			}
			else
			{
				if(!_kGPIO_GET(_kPUMP_1))
				{
					Transmit_Buff["pStatus"] = 1;
				}
				else
				{
					Transmit_Buff["pStatus"] = 0;
				}
			}
		}
		else
		{
			if(_sRuble_Parameters.Wirelss_Pump_Status == _kON)
			{
				Transmit_Buff["pStatus"] = 1;
			}
			else
			{
				Transmit_Buff["pStatus"] = 0;
			}
		}
		

		Transmit_Buff[F("mode")] = (char)_sRuble_Parameters.Operation_Mode;             /* store the operation mode */
		Transmit_Buff[F("currentTransformer")] = _sPump_Parameter.Pump_Run_Current;     /* store the current consumption by pump */
		
		if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_MANUAL)
		{
			Transmit_Buff[F("plot")] = _sRuble_Parameters.Irrigating_Plot;
		}
		else
		{
			Transmit_Buff[F("plot")] = 0;
		}
		
		if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_SCHEDULER)
		{
			if(_sSchedules.Which_Sheduler_Is_Running)
			{
				Transmit_Buff[F("sc_index")] = _sSchedules.Which_Sheduler_Is_Running;
				remaining_time_in_min = _sSchedules.Running_Hr_Of_Each_Plot[_sSchedules.Which_Sheduler_Is_Running-1] - _sSchedules.Keep_Scheduler_Run_Timer;
				Transmit_Buff[F("sc_remain_time")] = remaining_time_in_min/60;
			}
			else
			{
				Transmit_Buff[F("sc_index")] = 0;
				Transmit_Buff[F("sc_remain_time")] = 0;
			}
		}
		else
		{
			Transmit_Buff[F("sc_index")] = 0;
			Transmit_Buff[F("sc_remain_time")] = 0;
		}
		
		Transmit_Buff[F("pump_num")] = _sRuble_Parameters.Pump_Selected_For_Irrigation;
		Transmit_Buff[F("three_phase")] = _sRuble_Parameters.Line_Fault_Flag;
		
		Transmit_Buff[F("date")] = _sRuble_Parameters.Update_Local_Date[0];
		Transmit_Buff[F("month")] = _sRuble_Parameters.Update_Local_Date[1];
		Transmit_Buff[F("year")] = _sRuble_Parameters.Update_Local_Date[2];
		Transmit_Buff[F("current_time")] = _sRuble_Parameters.Current_Time_In_Sec;
		
		Transmit_Buff[F("r_phase")] = _kGPIO_GET(_kR_PHASE_INPUT);
		Transmit_Buff[F("y_phase")] = _kGPIO_GET(_kY_PHASE_INPUT);
		Transmit_Buff[F("b_phase")] = _kGPIO_GET(_kB_PHASE_INPUT);
		
		
		
		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff)); 
		
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{                                                /* copy the JSON formatted data into the publish buffer */
			_kSERIAL_MON_WRITE((char *)_gPub_Buff);
		}
		
		ruble_data_success_counter++;
		
		while(retry > 0)
		{
			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Ruble_Data,(unsigned char *)_gPub_Buff))   /* publish data to the server */
			{
				ruble_data_success_counter = _kRESET;
				break;
			}
			retry--;
		}
		
		if(ruble_data_success_counter > 3)
		{
			_sRuble_Parameters.Subscribe_Topic_Status = _eALL_TOPIC_NOT_SUBCRIBED;
			Ping_to_Server();
		}
		_sRuble_Parameters.Publish_Topic &= ~_ePUB_RUBLE_DATA;            /* reset the topic */
		return;
	}
	            	
	/************************************** check extra plot data ************************************/
	if(extra_plot_data_resend_timer == 0)
	{
		for(unsigned char i=0; i<_kMAX_EXTRA_PLOT; i++)
		{
			if(_sExtra_Plot[i].Extra_Plot_Number >= _kEXTRA_PLOT_START_FROM)
			{
				if(_sExtra_Plot[i].Publish_status)
				{
					_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[i].Publish_status);
					switch(_sExtra_Plot[i].Publish_status)
					{
						case _eEXTRA_PLOT_ONETOUCH_RESEND:
						{
							memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("one touch feedback resend ");
								_kSERIAL_MON_CLEAR();
							}
							Transmit_Buff[F("pNo")] = i+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);       /* store the hardware faults */
							if(_sExtra_Plot[i].Status == _eSTOP_EXTRA_PLOT_OPERATION &&
							_sExtra_Plot[i].Auto_Operation == _kRESET)
							{
								Transmit_Buff[F("status")]  = 0;
							}
							else
							{
								Transmit_Buff[F("status")]  = 1;
							}
							
							if(i < 2 || i > 9)
							{
								if(_sExtra_Plot[i].Status == _kSCHEDULER_OFF)
								{
									Transmit_Buff[F("status")]  = 0;
								}
								if(_sExtra_Plot[i].Status == _kSCHEDULER_ON)
								{
									Transmit_Buff[F("status")]  = 1;
								}
							}
							
							if(i==1)
							{
								if(_kGPIO_GET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[i].Euro_Valve_Group[0]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)]))
								{
									Transmit_Buff[F("status")]  = 0;
								}
								else
								{
									Transmit_Buff[F("status")]  = 1;
								}	
							}
				
							Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));
							
							/* copy the JSON formatted data into the publish buffer */
							while(retry > 0)
							{
								plot_schedule_topic = _kRESET;
								if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.One_Touch_Monitor,(unsigned char *)_gPub_Buff))   /* publish data to the server */
								{
									_sExtra_Plot[i].Publish_status = _kRESET;
									Exra_Plot_Data_EEPROM(i+1);
									return;
								}
								retry--;
							}
										
							extra_plot_data_resend_timer = _kEXTRA_PLOT_RESEND_TIMER;
							return;
						}break;
						
						case _eEXTRA_PLOT_TIMER_RESEND:
						{
							memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("Extra Plot Timer Fb resend ");
								_kSERIAL_MON_CLEAR();
							}
							if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
							{
								if(_sRuble_Parameters.By_Pass_CT)
								{
									switch(_sRuble_Parameters.Pump_On_Flag)
									{
										case _ePUMP_OFF:
										{
											Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
										}break;
										
										case _ePUMP_ON:
										{
											Transmit_Buff[F("pStatus")] = 1;                          /* store the pump status */
										}break;
										default:
										{
											Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
										}break;
									}
								}
								else
								{
									if(!_kGPIO_GET(_kPUMP_1))
									{
										Transmit_Buff["pStatus"] = 1;
									}
									else
									{
										Transmit_Buff["pStatus"] = 0;
									}
								}
							}
							else
							{
								if(_sRuble_Parameters.Wirelss_Pump_Status == _kON)
								{
									Transmit_Buff["pStatus"] = 1;
								}
								else
								{
									Transmit_Buff["pStatus"] = 0;
								}
							}
							/*Transmit_Buff[F("pStatus")] = !_kGPIO_GET(_kPUMP_1);            / * store the pump status * /*/
							Transmit_Buff[F("medium")] = 0;                                    /* store medium of timer selection */
							if(_sExtra_Plot[i].Run_Timer)
							{
								Transmit_Buff[F("time")] = (_sExtra_Plot[i].Run_Timer/60) + 1;
								Transmit_Buff[F("plot")] =  i+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);
							}
							else
							{
								Transmit_Buff[F("time")] = _sExtra_Plot[i].Run_Timer;
								Transmit_Buff[F("plot")] =  i+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);
							}	
							Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                           /* copy the JSON formatted data into the publish buffer */
							_kSERIAL_MON_WRITE((char *)_gPub_Buff);
							while(retry > 0)
							{
								plot_schedule_topic = _kRESET;
								if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Timer_FeedBack,(unsigned char *)_gPub_Buff))   /* publish data to the server */
								{
									_sExtra_Plot[i].Publish_status = _kRESET;
									Exra_Plot_Data_EEPROM(i+1);
									return;
								}
								extra_plot_data_resend_timer = _kEXTRA_PLOT_RESEND_TIMER;
								retry--;
							}
							return;
						}break;
						
						case _eEXTRA_PLOT_AUTO_FEEDBACK:
						{
							
						}break;
						
					}
				}
			}
		}
	}
	else
	{
		extra_plot_data_resend_timer--;
	}

		
	
	
	
	
	
	
	
	
	
	
// 	char temp_data[40],i=0;
// 	static unsigned char retry = 0,ruble_data_success_counter = 0;
// 	static unsigned int extra_plot_data_resend_timer = 0;
// 	retry = 2;
// 	
// 	
// 	StaticJsonBuffer<270> jsonSensorData;
// 	JsonObject& Transmit_Buff = jsonSensorData.createObject();
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _ePUB_RUBLE_FAULTS)          /* if the topic is ruble faults */
// 	{
// 		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 		Transmit_Buff[F("codes")] = _sPump_Parameter.Irrigation_Status;    /* store the irrigation status */
// 		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                              /* copy the JSON formatted data into the publish buffer */
// 		while(retry > 0)
// 		{
// 			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Ruble_Faults,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 			{
// 				break;
// 			}
// 			retry--;
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_ePUB_RUBLE_FAULTS;        /* reset the topic */
// 		switch(_sPump_Parameter.Irrigation_Status)                   /* check which fault has been sent to server */
// 		{
// 			case _eIRRIG_STAT_EURO_FEEDBACK:
// 			case _eIRRIG_STAT_EURO_CURRENT_SENSE:
// 			{
// 				_sPump_Parameter.Irrigation_Status = _kRESET;         /* reset the irrigation status if fault was related to the valve */
// 			}break;
// 		}
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _ePUB_TIMER_FEEDBACK)   /* if the topic Timer feedback */
// 	{
// 		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 		_kSERAIL_MON_WRITE("_ePUB_TIMER_FEEDBACK");
// 		_kSERIAL_MON_CLEAR();
// 		Transmit_Buff[F("pStatus")] = !_kGPIO_GET(_kPUMP_1);            /* store the pump status */
// 		Transmit_Buff[F("medium")] = 0;                                    /* store medium of timer selection */
// 		
// 		if(_sRuble_Parameters.Pump_Run_Timer)
// 		{
// 			Transmit_Buff[F("time")] = (_sRuble_Parameters.Pump_Run_Timer/60) + 1;
// 		}
// 		else
// 		{
// 			Transmit_Buff[F("time")] = _sRuble_Parameters.Pump_Run_Timer;
// 		}
// 		
// 		if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_TIMER)
// 		{
// 			Transmit_Buff[F("plot")] = _sRuble_Parameters.Temp_Irrigating_Plot;     /* send the irrigating plot number */
// 		}
// 		else
// 		{
// 			Transmit_Buff[F("plot")] = _sRuble_Parameters.Last_Timer_Plot;    //13/12/2021
// 			_sRuble_Parameters.Last_Timer_Plot = 0;
// 		}
// 		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                           /* copy the JSON formatted data into the publish buffer */
// 
// 		while(retry > 0)
// 		{
// 			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Timer_FeedBack,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 			{
// 				break;
// 			}
// 			retry--;
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_ePUB_TIMER_FEEDBACK;        /* reset the topic */
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _eWIRELESS_PUMP_FAIL)          /* if the topic is ruble faults */
// 	{
// 		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 		Transmit_Buff[F("codes")] = 6;    /* store the irrigation status */
// 		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                              /* copy the JSON formatted data into the publish buffer */
// 		while(retry > 0)
// 		{
// 			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Ruble_Faults,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 			{
// 				break;
// 			}
// 			retry--;
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_eWIRELESS_PUMP_FAIL;        /* reset the topic */
// 		
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _ePUB_LIRA_NODE_DATA)     /* if the topic lira node data */
// 	{
// 		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 		_kSERAIL_MON_WRITE("_ePUB_LIRA_NODE_DATA");
// 		_kSERIAL_MON_CLEAR();
// 		Transmit_Buff[F("packet_id")] = 0;   /* store packet id(packet Number) */
// 		Transmit_Buff[F("sm1")] = 0;              /* store soil moisture sensor 1 data */
// 		Transmit_Buff[F("sm2")] = 0;              /* store soil moisture sensor 2 data */
// 		Transmit_Buff[F("sm3")] = 0;              /* store soil moisture sensor 3 data */
// 		Transmit_Buff[F("sm4")] = 0;              /* store soil moisture sensor 4 data */
// 		Transmit_Buff[F("soil_temp")] = 0;   /* store soil temperature */
// 		Transmit_Buff[F("temperature")] = 0;  /* store the air temperature */
// 		Transmit_Buff[F("humidity")] = 0;            /* store the humidity */
// 		Transmit_Buff[F("light")] = 0;          /* store the light intensity */
// 		Transmit_Buff[F("vBat")] = 0; /* store the battery level */
// 		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));
// 		_kSERIAL_MON_WRITE((char *)_gPub_Buff);
// 		/* copy the JSON formatted data into the publish buffer */
// 
// 		memset(temp_data,0,sizeof(temp_data));
// 		sprintf(temp_data,"%s/%s",_sPublish_Topic.Lira_Node_Data, "000000"); /* copy the topic including id */
// 		while(retry > 0)
// 		{
// 			if(_kMQTT_PUB((unsigned char *)temp_data,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 			{
// 				break;
// 			}
// 			retry--;
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_ePUB_LIRA_NODE_DATA;             /* reset the topic */
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _ePUB_EURO_VALVE_DATA)         /* if the topic is EURO valve data */
// 	{
// 		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 		if(_sRuble_Parameters.Last_Valve < _kMAX_EURO_VALVE)
// 		{
// 			_kSERAIL_MON_WRITE("_ePUB_EURO_VALVE_DATA");
// 			_kSERIAL_MON_CLEAR();
// 			Transmit_Buff[F("valveStatus")] = _sEuro_Valve_Param[_sRuble_Parameters.Last_Valve].Euro_Valve_Online;  /* store EURO valve ONLINE/OFFLINE */
// 			Transmit_Buff[F("vBat")] = _sEuro_Valve_Param[_sRuble_Parameters.Last_Valve].Euro_Battery_Level;        /* store the battery level */
// 			Transmit_Buff[F("valveOpen")] = _sEuro_Valve_Param[_sRuble_Parameters.Last_Valve].Euro_Valve_Status;    /* store valve status ON/OFF */
// 			Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));           /* copy the JSON formatted data into the publish buffer */
// 			
// 			memset(temp_data,0,sizeof(temp_data));
// 			sprintf(temp_data,"%s/%s",_sPublish_Topic.Euro_Valve_Data, _sEuro_Valve_Param[_sRuble_Parameters.Last_Valve].Euro_Valve_ID);  /* copy the topic including id */
// 			while(retry > 0)
// 			{
// 				if(_kMQTT_PUB((unsigned char *)temp_data,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 				{
// 					break;
// 				}
// 				retry--;
// 			}
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_ePUB_EURO_VALVE_DATA;       /* reset the topic */
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _ePUB_HARDWARE_FAULTS)     /* if the topic is hardware fault */
// 	{
// 		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 		_kSERAIL_MON_WRITE("_ePUB_EURO_VALVE_DATA");
// 		_kSERIAL_MON_CLEAR();
// 		Transmit_Buff[F("codes")] = _sRuble_Parameters.Hardware_Faults;       /* store the hardware faults */
// 		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                               /* copy the JSON formatted data into the publish buffer */
// 		
// 		while(retry > 0)
// 		{
// 			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Hardware_Faults,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 			{
// 				break;
// 			}
// 			retry--;
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_ePUB_HARDWARE_FAULTS;       /* reset the topic */
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _eELECTRICITY_STATUS)
// 	{
// 		
// 	}
// 	
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _eEXTRA_PLOT_FEEDBACK)
// 	{	
// 		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 		_kSERAIL_MON_WRITE("Extra Plot Timer Fb");
// 		_kSERIAL_MON_CLEAR();
// 		Transmit_Buff[F("pStatus")] = !_kGPIO_GET(_kPUMP_1);            /* store the pump status */
// 		if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
// 		{
// 			if(_sRuble_Parameters.By_Pass_CT)
// 			{
// 						
// 				switch(_sRuble_Parameters.Pump_On_Flag)
// 				{
// 					case _ePUMP_OFF:
// 					{
// 						Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
// 					}break;
// 							
// 					case _ePUMP_ON:
// 					{
// 						Transmit_Buff[F("pStatus")] = 1;                          /* store the pump status */
// 					}break;
// 					default:
// 					{
// 						Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
// 					}break;
// 				}
// 			}
// 			else
// 			{
// 				if(!_kGPIO_GET(_kPUMP_1))
// 				{
// 					Transmit_Buff["pStatus"] = 1;
// 				}
// 				else
// 				{
// 					Transmit_Buff["pStatus"] = 0;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if(_sRuble_Parameters.Wirelss_Pump_Status == _kON)
// 			{
// 				Transmit_Buff["pStatus"] = 1;
// 			}
// 			else
// 			{
// 				Transmit_Buff["pStatus"] = 0;
// 			}
// 		}
// 		Transmit_Buff[F("medium")] = 0;                                    /* store medium of timer selection */
// 		
// 		if(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback1].Run_Timer)
// 		{
// 			Transmit_Buff[F("time")] = (_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback1].Run_Timer/60) + 1;
// 			Transmit_Buff[F("plot")] =  _sRuble_Parameters.Extra_Plot_Feedback1+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);
// 		}
// 		else
// 		{
// 			Transmit_Buff[F("time")] = _sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback1].Run_Timer;
// 			Transmit_Buff[F("plot")] =  _sRuble_Parameters.Extra_Plot_Feedback1+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);
// 		}
// 		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                           /* copy the JSON formatted data into the publish buffer */
// 		_kSERIAL_MON_WRITE((char *)_gPub_Buff);
// 		
// 		while(retry > 0)
// 		{
// 			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Timer_FeedBack,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 			{
// 				_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback1].Publish_status = _kRESET;
// 				Exra_Plot_Data_EEPROM(_sRuble_Parameters.Extra_Plot_Feedback1+1);
// 				break;
// 			}
// 			retry--;
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_eEXTRA_PLOT_FEEDBACK;            /* reset the topic */
// 		return;
// 	}
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _eONE_TOUCH_FEEDBACK)
// 	{
// 		memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 		_kSERAIL_MON_WRITE("one touch feedback");
// 		
// 		Transmit_Buff[F("pNo")] = _sRuble_Parameters.Extra_Plot_Feedback+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);       /* store the hardware faults */
// 		
// 		if(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status == _eSTOP_EXTRA_PLOT_OPERATION && 
// 		_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Auto_Operation == _kRESET)
// 		{
// 			_kSERAIL_MON_WRITE("one touch off...................");
// 			_kSERIAL_MON_CLEAR();
// 			_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status);
// 			_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Auto_Operation);
// 			Transmit_Buff[F("status")]  = 0;
// 		}
// 		else
// 		{
// 			_kSERAIL_MON_WRITE("one touch on.....................");
// 			_kSERIAL_MON_CLEAR();
// 			_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status);
// 			_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Auto_Operation);
// 			Transmit_Buff[F("status")]  = 1;
// 		}
// 		if(_sRuble_Parameters.Extra_Plot_Feedback < 2 || _sRuble_Parameters.Extra_Plot_Feedback > 9)
// 		{
// 			if(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status == _kSCHEDULER_OFF)
// 			{
// 				Transmit_Buff[F("status")]  = 0;
// 			}
// 			if(_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Status == _kSCHEDULER_ON)
// 			{
// 				Transmit_Buff[F("status")]  = 1;
// 			}
// 		}
// 		
// 		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));
// 		
// 		/* copy the JSON formatted data into the publish buffer */
// 		while(retry > 0)
// 		{
// 			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.One_Touch_Monitor,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 			{
// 				_sExtra_Plot[_sRuble_Parameters.Extra_Plot_Feedback].Publish_status = _kRESET;
// 				Exra_Plot_Data_EEPROM(_sRuble_Parameters.Extra_Plot_Feedback+1);
// 				break;
// 			}
// 			retry--;
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_eONE_TOUCH_FEEDBACK;       /* reset the topic */
// 		return;
// 	}
// 	
// 	
// 	if(_sRuble_Parameters.Publish_Topic & _ePUB_RUBLE_DATA)     /* if the topic is ruble data */
// 	{
// 		unsigned int remaining_time_in_min;		
// 		if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
// 		{
// 			if(_sRuble_Parameters.By_Pass_CT)
// 			{
// 				switch(_sRuble_Parameters.Pump_On_Flag)
// 				{
// 					case _ePUMP_OFF:
// 					{
// 						Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
// 					}break;
// 					
// 					case _ePUMP_ON:
// 					{
// 						Transmit_Buff[F("pStatus")] = 1;                          /* store the pump status */
// 					}break;
// 					default:
// 					{
// 						Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
// 					}break;
// 				}
// 			}
// 			else
// 			{
// 				if(!_kGPIO_GET(_kPUMP_1))
// 				{
// 					Transmit_Buff["pStatus"] = 1;
// 				}
// 				else
// 				{
// 					Transmit_Buff["pStatus"] = 0;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if(_sRuble_Parameters.Wirelss_Pump_Status == _kON)
// 			{
// 				Transmit_Buff["pStatus"] = 1;
// 			}
// 			else
// 			{
// 				Transmit_Buff["pStatus"] = 0;
// 			}
// 		}
// 		
// 
// 		Transmit_Buff[F("mode")] = (char)_sRuble_Parameters.Operation_Mode;             /* store the operation mode */
// 		Transmit_Buff[F("currentTransformer")] = _sPump_Parameter.Pump_Run_Current;     /* store the current consumption by pump */
// 		
// 		if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_MANUAL)
// 		{
// 			Transmit_Buff[F("plot")] = _sRuble_Parameters.Irrigating_Plot;
// 		}
// 		else
// 		{
// 			Transmit_Buff[F("plot")] = 0;
// 		}
// 		
// 		if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_SCHEDULER)
// 		{
// 			if(_sSchedules.Which_Sheduler_Is_Running)
// 			{
// 				Transmit_Buff[F("sc_index")] = _sSchedules.Which_Sheduler_Is_Running;
// 				remaining_time_in_min = _sSchedules.Running_Hr_Of_Each_Plot[_sSchedules.Which_Sheduler_Is_Running-1] - _sSchedules.Keep_Scheduler_Run_Timer;
// 				Transmit_Buff[F("sc_remain_time")] = remaining_time_in_min/60;
// 			}
// 			else
// 			{
// 				Transmit_Buff[F("sc_index")] = 0;
// 				Transmit_Buff[F("sc_remain_time")] = 0;
// 			}
// 		}
// 		else
// 		{
// 			Transmit_Buff[F("sc_index")] = 0;
// 			Transmit_Buff[F("sc_remain_time")] = 0;
// 		}
// 		
// 		Transmit_Buff[F("pump_num")] = _sRuble_Parameters.Pump_Selected_For_Irrigation;
// 		Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                                 /* copy the JSON formatted data into the publish buffer */
// 		_kSERIAL_MON_WRITE((char *)_gPub_Buff);
// 		ruble_data_success_counter++;
// 		
// 		while(retry > 0)
// 		{
// 			if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Ruble_Data,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 			{
// 				ruble_data_success_counter = _kRESET;
// 				break;
// 			}
// 			retry--;
// 		}
// 		
// 		if(ruble_data_success_counter > 3)
// 		{
// 			_sRuble_Parameters.Subscribe_Topic_Status = _eALL_TOPIC_NOT_SUBCRIBED;
// 			Ping_to_Server();
// 		}
// 		_sRuble_Parameters.Publish_Topic &= ~_ePUB_RUBLE_DATA;            /* reset the topic */
// 		return;
// 	}
// 	
// 	
// 	/************************************** check extra plot data ************************************/
// 	if(extra_plot_data_resend_timer == 0)
// 	{
// 		for(unsigned char i=0; i<_kMAX_EXTRA_PLOT; i++)
// 		{
// 			if(_sExtra_Plot[i].Extra_Plot_Number >= _kEXTRA_PLOT_START_FROM)
// 			{
// 				if(_sExtra_Plot[i].Publish_status)
// 				{
// 					_kSERIAL_MON_PRINT_NUM(_sExtra_Plot[i].Publish_status);
// 					switch(_sExtra_Plot[i].Publish_status)
// 					{
// 						case _eEXTRA_PLOT_ONETOUCH_RESEND:
// 						{
// 							memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 							_kSERAIL_MON_WRITE("one touch feedback resend ");
// 							_kSERIAL_MON_CLEAR();
// 							Transmit_Buff[F("pNo")] = i+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);       /* store the hardware faults */
// 							if(_sExtra_Plot[i].Status == _eSTOP_EXTRA_PLOT_OPERATION &&
// 							_sExtra_Plot[i].Auto_Operation == _kRESET)
// 							{
// 								Transmit_Buff[F("status")]  = 0;
// 							}
// 							else
// 							{
// 								Transmit_Buff[F("status")]  = 1;
// 							}
// 							
// 							if(i < 2 || i > 9)
// 							{
// 								if(_sExtra_Plot[i].Status == _kSCHEDULER_OFF)
// 								{
// 									Transmit_Buff[F("status")]  = 0;
// 								}
// 								if(_sExtra_Plot[i].Status == _kSCHEDULER_ON)
// 								{
// 									Transmit_Buff[F("status")]  = 1;
// 								}
// 							}
// 				
// 							Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));
// 							
// 							/* copy the JSON formatted data into the publish buffer */
// 							while(retry > 0)
// 							{
// 								if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.One_Touch_Monitor,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 								{
// 									_sExtra_Plot[i].Publish_status = _kRESET;
// 									Exra_Plot_Data_EEPROM(i+1);
// 									return;
// 								}
// 								retry--;
// 							}
// 							extra_plot_data_resend_timer = _kEXTRA_PLOT_RESEND_TIMER;
// 							return;
// 						}break;
// 						
// 						case _eEXTRA_PLOT_TIMER_RESEND:
// 						{
// 							memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
// 							_kSERAIL_MON_WRITE("Extra Plot Timer Fb resend ");
// 							_kSERIAL_MON_CLEAR();
// 							if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
// 							{
// 								if(_sRuble_Parameters.By_Pass_CT)
// 								{
// 									switch(_sRuble_Parameters.Pump_On_Flag)
// 									{
// 										case _ePUMP_OFF:
// 										{
// 											Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
// 										}break;
// 										
// 										case _ePUMP_ON:
// 										{
// 											Transmit_Buff[F("pStatus")] = 1;                          /* store the pump status */
// 										}break;
// 										default:
// 										{
// 											Transmit_Buff[F("pStatus")] = 0;                          /* store the pump status */
// 										}break;
// 									}
// 								}
// 								else
// 								{
// 									if(!_kGPIO_GET(_kPUMP_1))
// 									{
// 										Transmit_Buff["pStatus"] = 1;
// 									}
// 									else
// 									{
// 										Transmit_Buff["pStatus"] = 0;
// 									}
// 								}
// 							}
// 							else
// 							{
// 								if(_sRuble_Parameters.Wirelss_Pump_Status == _kON)
// 								{
// 									Transmit_Buff["pStatus"] = 1;
// 								}
// 								else
// 								{
// 									Transmit_Buff["pStatus"] = 0;
// 								}
// 							}
// 							/*Transmit_Buff[F("pStatus")] = !_kGPIO_GET(_kPUMP_1);            / * store the pump status * /*/
// 							Transmit_Buff[F("medium")] = 0;                                    /* store medium of timer selection */
// 							if(_sExtra_Plot[i].Run_Timer)
// 							{
// 								Transmit_Buff[F("time")] = (_sExtra_Plot[i].Run_Timer/60) + 1;
// 								Transmit_Buff[F("plot")] =  i+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);
// 							}
// 							else
// 							{
// 								Transmit_Buff[F("time")] = _sExtra_Plot[i].Run_Timer;
// 								Transmit_Buff[F("plot")] =  i+(_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+1);
// 							}	
// 							Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff));                                           /* copy the JSON formatted data into the publish buffer */
// 							_kSERIAL_MON_WRITE((char *)_gPub_Buff);
// 							while(retry > 0)
// 							{
// 								if(_kMQTT_PUB((unsigned char *)_sPublish_Topic.Timer_FeedBack,(unsigned char *)_gPub_Buff))   /* publish data to the server */
// 								{
// 									_sExtra_Plot[i].Publish_status = _kRESET;
// 									Exra_Plot_Data_EEPROM(i+1);
// 									return;
// 								}
// 								extra_plot_data_resend_timer = _kEXTRA_PLOT_RESEND_TIMER;
// 								retry--;
// 							}
// 							return;
// 						}break;
// 						
// 						case _eEXTRA_PLOT_AUTO_FEEDBACK:
// 						{
// 							
// 						}break;
// 						
// 					}
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		extra_plot_data_resend_timer--;
// 	}
}

void Dummy_Data_Store(void)
{
	unsigned int i,j;
	for(i=0;i<_kMAX_LIRA_SENSOR_NODE;i++)
	{
		_sLira_Node_Param[i].Lira_Sensor_Number = 0;
		Lira_Data_EEPROM(i);
	}

	for(i=0;i<_kMAX_EURO_VALVE;i++)
	{
		_sEuro_Valve_Param[i].Euro_Valve_Number = 0;
		Euro_Data_EEPROM(i);
	}	 
		 
	/***************************************** Update coefficient 1 *************************************/
	//_sRuble_Parameters.Coeff_1 = -0.0001;
	//_kEEPROM_WRITE(_kEEPROM_LOC_COEFF_1,_sRuble_Parameters.Coeff_1);	 
		 
	/***************************************** Update coefficient 2 *************************************/
	//_sRuble_Parameters.Coeff_2 = 0.2004;
	//_kEEPROM_WRITE(_kEEPROM_LOC_COEFF_2,_sRuble_Parameters.Coeff_2);	 
		 
	/***************************************** Update coefficient 3 *************************************/
	//_sRuble_Parameters.Coeff_3 = -57.891;
	//_kEEPROM_WRITE(_kEEPROM_LOC_COEFF_3,_sRuble_Parameters.Coeff_3);
		 	 
	/******************************************* update ruble sim no. ************************************/
	_kEEPROM_WRITE(_kEEPROM_LOC_RUBLE_SIM_NUM,_sNetwork_and_Usr_Info.Ruble_SIM_No);	 	
		 	
	/******************************************* update user sim no. ************************************/
	_kEEPROM_WRITE(_kEEPROM_LOC_USER_SIM_NUM,_sNetwork_and_Usr_Info.User_Sim_No);	 	
		 	
	/***************************************** update the sms alert ***********************************/
	_sRuble_Parameters.SMS_Alert_Start = 0X09;
	_kEEPROM_WRITE(_kEEPROM_SMS_ALERT_START,_sRuble_Parameters.SMS_Alert_Start);
			 
	_sRuble_Parameters.Kill_Switch = _eKILL_SWITCH_NOT_SET;
	_kEEPROM_WRITE(_kEEPROM_KILL_SWITCH,_sRuble_Parameters.Kill_Switch);
	
	_sSchedules.Schedule_Enable     = _kRESET; /* initially schedules are not set */
	_sSchedules.New_Schedules_Occur = _kRESET;
	_sSchedules.Shedule_Run_Status  = _kRESET;
	_sSchedules.Scheduler_Retry_Timer = _kRESET;
	_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
	_sSchedules.All_Schedule_Run_Flag    = _kRESET;
	_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
	_sSchedules.Irrigating_Date  = 1;
	_sSchedules.Irrigating_Month = 1;
	_sSchedules.Started_Date = 1;
	_sSchedules.Started_Month = 1;
	_sSchedules.Schedule_Are_Started = 1;
	_sSchedules.Trigger_Time = 1;
	_sSchedules.Trigger_Reset = _kRESET;
	_sSchedules.Based_On_Trigger_Time     = _kSET;
	_sSchedules.Days_In_Between_Schedules = _kRESET;
	_sSchedules.Cyclic_Schedule_Flag      = _kRESET;
	_sSchedules.all_schedule_done_flag    = _kRESET;

	for(i=0; i< _kMAX_LIRA_SENSOR_NODE+1; i++)
	{
		_sSchedules.Running_Hr_Of_Each_Plot[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE+1; i++)
	{
		_sSchedules.Schedule_Status[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE+1; i++)
	{
		_sSchedules.Schedule_Num_List[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE+1; i++)
	{
		_sSchedules.Scheduler_Already_Run[i]       = _kRESET;
		_sSchedules.Scheduler_Already_Run_Month[i] = _kRESET;
	}
	
	_sSchedules.Pump_On_In_Scheduler = _kRESET;
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1; i++)
	{
		_sSchedules.Fertilizer_Tank_Number[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1; i++)
	{
		_sSchedules.Fertilizer_Tank_Number[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1; i++)
	{
		_sSchedules.Fertilizer_Tank_On_After[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1; i++)
	{
		_sSchedules.Tank_Off_Time_Duration[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1; i++)
	{
		_sSchedules.Fertilizer_Tank_Status[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1; i++)
	{
		_sSchedules.Tank_Run_Time[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1; i++)
	{
		_sSchedules.Tank_Running_Date[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_FERTILIZER_TANK+1; i++)
	{
		_sSchedules.Tank_On_Retry[i] = _kRESET;
	}
	
	_sSchedules.Fertilizer_On_Scheduler_Status = _kRESET;
	
	for(i=0; i< _kMAX_FERTILIZER_TANK+1; i++)
	{
		_sSchedules.Fertilizer_Tank_Run_Timer[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_FERTILIZER_TANK+1; i++)
	{
		_sSchedules.Running_Tank[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_FERTILIZER_TANK+1; i++)
	{
		_sSchedules.Tank_Running_Timer[i] = _kRESET;
	}
	
	_sSchedules.Blower_On_Time = _kRESET;
	_sSchedules.Blower_On_Scheduler_Status = _kRESET;
	_sSchedules.Blower_On_Retry = _kRESET;
	_sSchedules.Dummy_Schedule_Enable = _kRESET;
	_sSchedules.Dummy_Cyclic_Schedule_Flag = _kRESET;
	_sSchedules.Run_Program_In_How_Much_Day = _kRESET;
	_sSchedules.Dummy_Days_In_Between_Schedules = _kRESET;
	_sSchedules.Dummy_Trigger_Time = _kRESET;
	_sSchedules.Dummy_Based_On_Trigger_Time = _kRESET;
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE+1; i++)
	{
		_sSchedules.Dummy_Running_Hr_Of_Each_Plot[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE+1; i++)
	{
		_sSchedules.Dummy_Schedule_Status[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE+1; i++)
	{
		_sSchedules.Dummy_Schedule_Num_List[i] = _kRESET;
	}
	
	for(i=0; i< _kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK+1; i++)
	{
		_sSchedules.Dummy_Schedule_Num_List[i] = _kRESET;
	}
	
	_sSchedules.Save_Schedule_Data_In_Eeprom = _kRESET;
	
	for(i=0;i<_kMAX_FERTILIZER_TANK+1;i++)
	{
		_sSchedules.blower_run_status[i] = _kRESET;
	}
	
	_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
	/********************** Extra plot data *****************************/
	for(i=0;i<_kMAX_EXTRA_PLOT;i++)
	{
		_sExtra_Plot[i].Extra_Plot_Number = 0;	  
		for(int i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
		{
			_sExtra_Plot[i].Euro_Valve_Group[i] = 0;
		}
		_kEEPROM_WRITE(_kEEPROM_EXTRA_PLOT_DATA(i+1),_sExtra_Plot[i]);
	}
	
	_sRuble_Parameters.Pump_Run_Timer = _kRESET;   /* reset the pump run timer */
	_kEEPROM_WRITE(_kTIMER_IRRIGATING_PLOT,_sRuble_Parameters.Pump_Run_Timer);
	_sRuble_Parameters.Temp_Irrigating_Plot = _kRESET;  /* reset the temp irrigating plot */
	_kEEPROM_WRITE(_kTIMER_IRRIGATING_PLOT,_sRuble_Parameters.Temp_Irrigating_Plot);
	_sRuble_Parameters.Manual_Mode_Wait = 0;
	_sRuble_Parameters.Operation_Mode = _eOPER_MOD_SCHEDULER;  // 5/5 /* set the operation mode as auto */
	_kEEPROM_WRITE(_kEEPROM_LOC_OPERATION_MODE,_sRuble_Parameters.Operation_Mode);
	_sRuble_Parameters.By_Pass_CT = _kRESET;
	_kEEPROM_WRITE(_kCT_BY_PASS,_sRuble_Parameters.By_Pass_CT);
	_sRuble_Parameters.Wireless_Pump_Command = _kRESET;
	_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_COMMAND,_sRuble_Parameters.Wireless_Pump_Command);
	_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
	_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
	
	for(i=0; i<_kMAX_WIRELESS_PUMP+2; i++)
	{
		_sRuble_Parameters.Wireless_Pump_Handle_List[i] = _kOFF;
		_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
	}
	_sRuble_Parameters.Pump_Selected_For_Irrigation = _kWIRED_PUMP_NUMBER;
	_kEEPROM_WRITE(_kPUMP_SELECTED_FOR_IRRIGATION,_sRuble_Parameters.Pump_Selected_For_Irrigation);	
	
	_sRuble_Parameters.Pump_Run_Timer_for_flush = 0;
	_kEEPROM_WRITE(_kFLUSH_RUN_BACK_UP, _sRuble_Parameters.Pump_Run_Timer_for_flush);
	
	
	
	for(unsigned char wl_pump_num = 0; wl_pump_num < _kMAX_WIRELESS_PUMP; wl_pump_num++)
	{
		_sWireless_Pump_Param[wl_pump_num].Indication_Of_Faults = 0;
		_sWireless_Pump_Param[wl_pump_num].Wireless_Pump_Number = 0;
		_sWireless_Pump_Param[wl_pump_num].Response_To_Ruble = 0;
		
		Wireless_Pump_EEPROM(wl_pump_num);
	}
}

void Run_Extra_Plot_Operation(void)
{
	unsigned char check_plot;
	static unsigned char timer_backup[_kMAX_EXTRA_PLOT];
	
	for(check_plot=0; check_plot<_kMAX_EXTRA_PLOT; check_plot++)
	{
		if(_sExtra_Plot[check_plot].Extra_Plot_Number && _sExtra_Plot[check_plot].Extra_Plot_Number <= (_kMAX_PLOTS_LIMIT+_kMAX_LIRA_SENSOR_NODE+_kMAX_EXTRA_PLOT))
		{
			switch(_sExtra_Plot[check_plot].Status)
			{
				case _eSTOP_EXTRA_PLOT_OPERATION:
				{
					
				}break;
				
				case _eSTART_EXTRA_PLOT_OPERATION:
				{
					if(_sExtra_Plot[check_plot].Run_Timer==0 && _sExtra_Plot[check_plot].One_Touch == 0)
					{
						_sExtra_Plot[check_plot].Status = _eSTOP_EXTRA_PLOT_OPERATION;
						_sExtra_Plot[check_plot].Run_Timer = _kRESET;
						Stop_Extra_Plot_Operation(check_plot);
						Exra_Plot_Data_EEPROM(check_plot+1);
						return;
					}
					if(Start_Extra_Plot_Operation(check_plot))
					{
						_sExtra_Plot[check_plot].Status = _eRUN_EXTRA_PLOT_OPERATION;
						/************************** operation started *********************/
						//Exra_Plot_Data_EEPROM(check_plot+1);
						return;   // 14/7/2022
					}
					else
					{
						_sExtra_Plot[check_plot].Status = _eSTOP_EXTRA_PLOT_OPERATION;
						_sExtra_Plot[check_plot].Run_Timer = _kRESET;
						Stop_Extra_Plot_Operation(check_plot);
						Exra_Plot_Data_EEPROM(check_plot+1);
						return;
						/************************** not able to start the operation *********************/
					}
				}break;
				
				case _eRUN_EXTRA_PLOT_OPERATION:
				{
					/******************* save the running time in eeprom *****************************/
					
					if(!_sExtra_Plot[check_plot].Run_Timer &&  !_sExtra_Plot[check_plot].One_Touch)
					{
						_sExtra_Plot[check_plot].Status = _eSTOP_EXTRA_PLOT_OPERATION;
						Stop_Extra_Plot_Operation(check_plot);
						Exra_Plot_Data_EEPROM(check_plot+1);
						return;
					}
					
					if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_MANUAL)
					{
						if(check_plot != (_kWIRELESS_PUMP_ENABLE_DISABLE_PLOT-_kEXTRA_PLOT_START_FROM)
						&& check_plot != (_kFLUSH_PLOT_NUMBER -_kEXTRA_PLOT_START_FROM))
						{
							
							_sExtra_Plot[check_plot].Status = _eSTOP_EXTRA_PLOT_OPERATION;
							_sExtra_Plot[check_plot].Run_Timer = _kRESET;
							Stop_Extra_Plot_Operation(check_plot);
							Exra_Plot_Data_EEPROM(check_plot+1);
						}
						
						return;
					}
					
					if(_sExtra_Plot[check_plot].Run_Timer)
					{
						timer_backup[check_plot]++;
						if(timer_backup[check_plot] > 50)
						{
							timer_backup[check_plot] = _kRESET;
							Exra_Plot_Data_EEPROM(check_plot+1);
						}
					}
					/********************** check weather any fault occure related to pump ******************/
					if(check_plot == (_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
					{
						if(_sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_DRY_RUN || _sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_LINE_FAULT ||
						_sPump_Parameter.Irrigation_Status == _eIRRIG_STAT_OVER_LOAD  || _sPump_Parameter.Irrigation_Status == _eTHREE_PHASE_POWER_INT ||
						_sPump_Parameter.Irrigation_Status == _eEURO_BATTERY_LOW)
						{
							_kSERAIL_MON_WRITE("fogger: fault occure");
							_kSERIAL_MON_CLEAR();
							Stop_Extra_Plot_Operation(check_plot);
							return;
						}
					}
					/* check any other operation which will cause stop operation */
				}break;
// 				default:
// 				{
// 					
// 				}break;
			}
		}
	}
}

bool Start_Extra_Plot_Operation(unsigned char check_plot)
{
	_kSERAIL_MON_WRITE("Try to Start extra plot");                /* for debugging purpose */
	
	_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(check_plot);
	
	_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
	
	if(check_plot == (_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM) ||
	check_plot == (_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
	{
		_sPump_Parameter.Irrigation_Status = _kRESET;
	}
	
	#ifdef _kBATTTERY_IS_AVAILABLE
	if(_sRuble_Parameters.Switch_To_Battery == _eELECTRICITY_IS_NOT_THERE)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Fail to start electricity is not there");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		return 0;                   /* if irrigation doesn't start successfully */
	}
	#endif
	
	
	if(Extra_Plot_Valve_Grouping_Handler(_kON, check_plot))   /* check either all group valves are ON or not */
	{
		
		/************************* turn on the pump if fogger is there ******************************/
		
		if(check_plot == (_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM) ||
		check_plot == (_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
		{
			if(check_plot != (_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
			{
				Hard_Coded_Flush_Valve(_kCHECK_TRY_TO_ON);
			}
			
			_sRuble_Parameters.Start_Monitoring_Pump_Faults_Timer = _kRESET;
			_sPump_Parameter.Irrigation_Status = _kRESET;
			
			if(_sRuble_Parameters.Wireless_Pump_Command == _kRESET)
			{
				if((_kGPIO_GET(_kPUMP_1) != _kPUMP_ON))     /* check weather pump is already ON */
				{
					_kGPIO_SET(_kPUMP_1,_kPUMP_ON);      /* turn ON the pump */
				}
				
				if((_kGPIO_GET(_kPUMP_2) != _kPUMP_ON))     /* check weather pump is already ON */
				{
					_kGPIO_SET(_kPUMP_2,_kPUMP_ON);
				}
				
			}
			else
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("wl pump trigger 33333333333333333333");
					_kSERIAL_MON_CLEAR();
				}
				if(Wireless_Pump_On_Off(_ePUMP_ON,_sRuble_Parameters.Pump_Selected_For_Irrigation-2))
				{
					_sRuble_Parameters.Wirelss_Pump_Status = _kON;
					_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
				}
				else
				{
					_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
					_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
					return 0;  // 21/08/2023
				}
			}
		}
		
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Extra Plot ON");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		
		_sExtra_Plot[check_plot].On_Off_Status = _kSET;
		
		if(_sExtra_Plot[check_plot].Set_Through_Timer == _eSET_THROUGH_TIMER)
		{
			_sRuble_Parameters.Publish_Topic |= _eEXTRA_PLOT_FEEDBACK;
			_sExtra_Plot[check_plot].One_Touch = _kRESET;
			_sRuble_Parameters.Extra_Plot_Feedback1 = check_plot;
			_sExtra_Plot[check_plot].Publish_status = _eEXTRA_PLOT_TIMER_RESEND;
		}
		else
		{
			_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
			_sExtra_Plot[check_plot].Set_Through_Timer = _kRESET;
			_sRuble_Parameters.Extra_Plot_Feedback = check_plot;
			_sExtra_Plot[check_plot].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
		}
		
		return 1;                    /* if irrigation start successfully then return 1 */
	}
	else
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("extra plot fails to on");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		return 0;                   /* if irrigation doesn't start successfully */
	}
}

bool Stop_Extra_Plot_Operation(unsigned char check_plot)
{
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("Try to stop Extra");
		_kSERIAL_MON_CLEAR();
	}
	
	_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
	
	_sExtra_Plot[check_plot].On_Off_Status = _kRESET;
	
	
	if(((check_plot == (_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM)) || (check_plot == (_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))) && (_sRuble_Parameters.Operation_Mode != _eOPER_MOD_TIMER) ||
	(_sPump_Parameter.Irrigation_Status != _kRESET && ((check_plot == (_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM) || (check_plot == (_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM)))))
	|| (_sRuble_Parameters.Operation_Mode == _eOPER_MOD_TIMER && _sRuble_Parameters.Irrigating_Plot == 0 && ((check_plot == (_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM) || (check_plot == (_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM)))))
	|| _sRuble_Parameters.Line_Fault_Flag == _kSET)
	{
		Hard_Coded_Flush_Valve(_kCHECK_TRY_TO_OFF);
		if(_sRuble_Parameters.Irrigating_Plot == _kRESET)
		{
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
					_kSERAIL_MON_WRITE("wl pump trigger 444444444444444444444");
					_kSERIAL_MON_CLEAR();
				}
				if(Wireless_Pump_On_Off(_ePUMP_OFF,_sRuble_Parameters.Pump_Selected_For_Irrigation-2))
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Extra plot Stops fogger back up");
						_kSERIAL_MON_CLEAR();
					}
					_sRuble_Parameters.Pending_Plots[13] = _kRESET;
					_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
					_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
				}
				else
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Extra plot fail to Stops fogger back up");
						_kSERIAL_MON_CLEAR();
					}
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
					_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
					
					_sRuble_Parameters.Pending_Plots[13] = _kSET;
					
					if(_sExtra_Plot[check_plot].Set_Through_Timer == _eSET_THROUGH_TIMER)
					{
						_sRuble_Parameters.Publish_Topic |= _eEXTRA_PLOT_FEEDBACK;
						_sExtra_Plot[check_plot].Set_Through_Timer = _kRESET;
						_sRuble_Parameters.Extra_Plot_Feedback1 = check_plot;
						_sExtra_Plot[check_plot].Publish_status = _eEXTRA_PLOT_TIMER_RESEND;
					}
					else
					{
						_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
						_sExtra_Plot[check_plot].One_Touch = _kRESET;
						_sRuble_Parameters.Extra_Plot_Feedback = check_plot;
						_sExtra_Plot[check_plot].Set_Through_Timer = _kRESET;
						_sExtra_Plot[check_plot].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
					}
					_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
					return 1;                    /* if irrigation stops successfully then return 1 */
				}
			}
		}
	}
	
	_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
	
	// 	if(_sExtra_Plot[check_plot].Set_Through_Timer == _kRESET && check_plot == (_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM))
	// 	{
	// 		_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
	// 		_sRuble_Parameters.Extra_Plot_Feedback = check_plot;
	// 		_sExtra_Plot[check_plot].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
	// 		return 1;
	// 	}
	
	if(Extra_Plot_Valve_Grouping_Handler(_kOFF, check_plot))   /* check either all group valves are OFF or not */
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Extra plot Stops");
			_kSERIAL_MON_CLEAR();
		}
		_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
		
		
		if(_sExtra_Plot[check_plot].Set_Through_Timer == _eSET_THROUGH_TIMER)
		{
			_sRuble_Parameters.Publish_Topic |= _eEXTRA_PLOT_FEEDBACK;
			_sExtra_Plot[check_plot].Set_Through_Timer = _kRESET;
			_sRuble_Parameters.Extra_Plot_Feedback1 = check_plot;
			_sExtra_Plot[check_plot].Publish_status = _eEXTRA_PLOT_TIMER_RESEND;
		}
		else
		{
			_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
			_sExtra_Plot[check_plot].One_Touch = _kRESET;
			_sRuble_Parameters.Extra_Plot_Feedback = check_plot;
			_sExtra_Plot[check_plot].Set_Through_Timer = _kRESET;
			_sExtra_Plot[check_plot].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
		}
		
		return 1;                    /* if irrigation stops successfully then return 1 */
	}
	else
	{
		//_sRuble_Parameters.Irrigating_Plot = _kRESET;
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Extra plot fails to Stop");                /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
		return 0;                   /* if irrigation doesn't stop successfully */
	}
}

bool Extra_Plot_Valve_Grouping_Handler(unsigned char on_off, unsigned char check_plot)
{
	unsigned char i,j,k,count=0,current_valve_number,compare_id,id_len,data_send_counter = 0,temp_data[30];
	unsigned char valve_number_list[_kMAX_LIRA_SENSOR_NODE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO], total_valve_number,valve_number_not_match_flag=0;
	unsigned char hex_array[] = {0x01,0x02,0x04, 0x08, 0x10, 0x20, 0x40, 0x80},total_valve;
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("valve list : ");
	}
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
		{
			_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[check_plot].Euro_Valve_Group[i]);
			_kSERAIL_MON_WRITE_NO_LN(" ");
		}
		
		_kSERAIL_MON_WRITE_NO_LN("Irrigating Extra plot no : ");
		_kSERIAL_MON_PRINT_NUM(check_plot);
		_kSERIAL_MON_CLEAR();
	}
	
	
	if(check_plot == _kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM && on_off == _kON)
	{
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
		
		if(_gRunning_Flush_Num == _kRESET || _gRunning_Flush_Num > total_valve)
		{
			_gRunning_Flush_Num = _kSET;
			_kEEPROM_WRITE(_kRUNNING_FLUSH_NUMBER, _gRunning_Flush_Num);
		}
		if(_sExtra_Plot[check_plot].Euro_Valve_Group[_gRunning_Flush_Num-1] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && _sExtra_Plot[check_plot].Euro_Valve_Group[i] > 0)
		{
			
		}
		else
		{
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[check_plot].Euro_Valve_Group[_gRunning_Flush_Num-1]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(!on_off));
			_kDELAY_MS(200);
			return 1;
		}
		return 1;
	}
	
	for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
	{
		if(_sExtra_Plot[check_plot].Euro_Valve_Group[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && _sExtra_Plot[check_plot].Euro_Valve_Group[i] > 0)
		{
			unsigned char valve_number =0;
			unsigned char euro_valve_num=0,command_valve=0;
			
			valve_number =  _sExtra_Plot[check_plot].Euro_Valve_Group[i];
			
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
					_kSERIAL_MON_CLEAR();
					_kSERIAL_MON_PRINT_NUM(euro_valve_num);
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
						Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_eSUBSCRIBE_DATA);
						//mqttCallback((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff));
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
									
									for(unsigned char wired_valve =0; wired_valve < _kMAX_VALVE_FOR_1PLOT; wired_valve++)
									{
										if(_sExtra_Plot[check_plot].Euro_Valve_Group[wired_valve] > (_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO))
										{
											/********************************* turn on/off the wired valve *********************************/
											_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[check_plot].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
								_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
								_sRuble_Parameters.Last_Valve = current_valve_number;
								_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
								
								for(unsigned char wired_valve =0; wired_valve < _kMAX_VALVE_FOR_1PLOT; wired_valve++)
								{
									if(_sExtra_Plot[check_plot].Euro_Valve_Group[wired_valve] > (_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO))
									{
										/********************************* turn on/off the wired valve *********************************/
										
										_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[check_plot].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
							
							for(unsigned char wired_valve =0; wired_valve < _kMAX_VALVE_FOR_1PLOT; wired_valve++)
							{
								if(_sExtra_Plot[check_plot].Euro_Valve_Group[wired_valve] > (_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO))
								{
									/********************************* turn on/off the wired valve *********************************/
									_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[check_plot].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
						
						for(unsigned char wired_valve =0; wired_valve < _kMAX_VALVE_FOR_1PLOT; wired_valve++)
						{
							if(_sExtra_Plot[check_plot].Euro_Valve_Group[wired_valve] > (_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO))
							{
								/********************************* turn on/off the wired valve *********************************/
								
								_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[check_plot].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
		
		else if(_sExtra_Plot[check_plot].Euro_Valve_Group[i] > (_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO))
		{
			/********************************* turn on/off the wired valve *********************************/
			
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[check_plot].Euro_Valve_Group[i]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(!on_off));
			_kDELAY_MS(200);
			if(on_off == _kON)
			{
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

unsigned char Handle_Wireless_Pump(void)
{
	unsigned char wireless_pump_number;
	if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
	{
		if(_sRuble_Parameters.Irrigating_Plot !=0
		|| _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET
		|| _sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET)
		{
			if(_sRuble_Parameters.Wireless_Pump_Operate_Timer == 0)
			{
				/* send pump on command */
				
				_sRuble_Parameters.Wireless_Pump_Operate_Timer = _kMAX_TIME_TO_MONITOR_WIRELESS_PUMP;
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("wl 1 pump trigger 333333333333333333333333");
					_kSERIAL_MON_CLEAR();
				}
				if(Wireless_Pump_On_Off(_ePUMP_ON,_sRuble_Parameters.Pump_Selected_For_Irrigation-2))
				{
					_sRuble_Parameters.Wirelss_Pump_Status = _kON;
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
						_sRuble_Parameters.Wirelss_Pump_Status = _kON;
					}
					
					_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_EURO_FEEDBACK;
					_sRuble_Parameters.Publish_Topic |= _eWIRELESS_PUMP_FAIL;
					_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
					_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
				}
				
				_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
				
				/************************ MAKE THE PACKET *****************************/
			}
		}
		else
		{
			if(_sRuble_Parameters.Wireless_Pump_Operate_Timer == 0)
			{
				/* send pump off command */
				
				_sRuble_Parameters.Wireless_Pump_Operate_Timer = _kMAX_TIME_TO_MONITOR_WIRELESS_PUMP*4;
				
				/************************ MAKE THE PACKET *****************************/
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("wl 1 pump trigger 4444444444444444444444444444");
					_kSERIAL_MON_CLEAR();
				}
				if(Wireless_Pump_On_Off(_ePUMP_OFF,_sRuble_Parameters.Pump_Selected_For_Irrigation-2))
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
						_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
					}
					else
					{
						_sRuble_Parameters.Wirelss_Pump_Status = _kOFF;
					}
					_kEEPROM_WRITE(_kEEPROM_LOC_WIRELESS_PUMP_STATUS,_sRuble_Parameters.Wirelss_Pump_Status);
				}
				
				_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
				
			}
		}
	}
}

unsigned char Wireless_Pump_On_Off(unsigned char On_Off, unsigned char wireless_pump_number)
{
	//unsigned char wireless_pump_number = 0;
	unsigned char count =0,command_valve=0,j=0,id_len=0,compare_id =0,current_valve_number =0,data_send_counter =0,id_checked = 0;
	static unsigned char fault_check_counter = _kRESET;
	unsigned char answer = 0;
	fault_check_counter++;
	
	switch(On_Off)
	{
		case _ePUMP_ON:
		case _ePUMP_OFF:
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE_NO_LN("Wireless pump no. ");
				_kSERIAL_MON_PRINT_NUM(wireless_pump_number);
				_kSERIAL_MON_CLEAR();
			}
			if(_kLORA_DATA_AVAIALABLE())    /* read if there is already any data received(garbage data) */
			{
				_kLORA_RECV_DATA();         /*  lora rx_tx buffer will update*/
			}
			
			memset((char *)_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
			count = _kRESET;
			command_valve = 1;
			/********************************** wireless valve command (on/off) **********************************/
			/* valve id, on/off command, pump run timer */
			
			/************************************* copy the euro valve ID *********************************************/
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[0];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[1];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[2];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[3];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[4];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[5];
			
			/*********************************** packet_id ***********************************************/
			_sRuble_Parameters.Lora_Data_TX[count++] = _eCOMMAND_FROM_WIRELESS_PUMP;
			
			/*********************************** copy on/off command ***********************************************/
			_sRuble_Parameters.Lora_Data_TX[count++] = On_Off;
			
			/********************************** copy operation mode ************************************************/
			_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Operation_Mode;
			
			/******************************** copy pump timer (used if selected mode is timer) *********************/
			_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Pump_Run_Timer << 8;
			_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Pump_Run_Timer;
			_sRuble_Parameters.Lora_Data_TX[count++] = wireless_pump_number+1;
			_sRuble_Parameters.Lora_Data_TX[count++] = 'S';
			//_sRuble_Parameters.Lora_Data_TX[count++] = wireless_pump_number+1;
			
			
			/* check which structure to which EURO valve we are going to update */
			unsigned char euro_valve_num = _kMAX_EURO_VALVE;
			
			data_send_counter = _kRESET;
			answer = _kRESET;
			
			while(data_send_counter < _kRETRY_TO_SEND_VALVE)
			{
				data_send_counter++;
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Send On/off Command to wireless Pump 1");
					_kSERIAL_MON_CLEAR();
				}
				Display_Update_1();    // 20/1/2022
				answer = 0;
				_gTime_To_Wait_Lora_Resp = 0;
				
				id_checked = _kRESET;
				_kLORA_SEND_DATA(count);
				
				do
				{
					if(_kLORA_DATA_AVAIALABLE())
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("wireless pump available 1: ");
							Serial.println();
							_kSERIAL_MON_CLEAR();
						}
						if(_kLORA_RECV_DATA())
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE_NO_LN("Wireless Pump rx data : ");
								_kSERIAL_MON_WRITE((char *)_sRuble_Parameters.Lora_Data_Rx,10);
								Serial.println();
								_kSERIAL_MON_CLEAR();
								_kSERAIL_MON_WRITE_NO_LN("wireless pump valve ID which is Sent :");
								_kSERIAL_MON_WRITE(_sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID,7);
								Serial.println();
								_kSERIAL_MON_CLEAR();
							}
							
							for(j=0;j<_kVALVE_ID_LEN;j++)
							{
								if(_sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[j] != _sRuble_Parameters.Lora_Data_Rx[j])
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("ID Not matched with pump which is replied 4");
										_kSERIAL_MON_CLEAR();
									}
									
									delay(1000);
									
									if(_gTime_To_Wait_Lora_Resp > 10)
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE_NO_LN("wireless pump available 4.1: ");
											Serial.println();
											_kSERIAL_MON_CLEAR();
										}
										return 0;
									}
									
									if(data_send_counter >= _kRETRY_TO_SEND_VALVE)
									{
										if(fault_check_counter > 2)
										{
											fault_check_counter = _kRESET;
											_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
											_sRuble_Parameters.Last_Pump = wireless_pump_number;
											_sRuble_Parameters.Publish_Topic |=   _eWIRELESS_PUMP_FAIL;
										}
									}
									id_checked = 1;
								}
							}
							
							/* if id get matched */
							if(id_checked == _kRESET)
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE_NO_LN("wireless pump available 4: ");
									Serial.println();
									_kSERIAL_MON_CLEAR();
								}
								answer = 1;
								data_send_counter = 3;
								delay(1000);
								
								if(On_Off == _ePUMP_ON)
								{
									if(_sRuble_Parameters.Lora_Data_Rx[7] == _ePUMP_ON)
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("_ePUMP_ON");
										}
										fault_check_counter = _kRESET;
										data_send_counter = 3;
										_sWireless_Pump_Param[wireless_pump_number].Indication_Of_Faults  = (_sRuble_Parameters.Lora_Data_Rx[11]<<8) | (_sRuble_Parameters.Lora_Data_Rx[12]);
										answer = 1;
										return 1;
									}
									else if (_sRuble_Parameters.Lora_Data_Rx[7] == _ePUMP_OR_ELECTRICITY_RELATED)
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("_ePUMP_OR_ELECTRICITY_RELATED");
										}
										fault_check_counter = _kRESET;
										data_send_counter = 3;
										
										_sRuble_Parameters.Last_Pump = wireless_pump_number;
										_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_LINE_FAULT;
										_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_FAULTS;
										
										//_sRuble_Parameters.Publish_Topic |=   _eWIRELESS_PUMP_FAIL;
										_sWireless_Pump_Param[wireless_pump_number].Indication_Of_Faults  = (_sRuble_Parameters.Lora_Data_Rx[11]<<8) | (_sRuble_Parameters.Lora_Data_Rx[12]);
										
										return 0;
									}
									else
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("Wireless Pump Feedback fail");
											_kSERIAL_MON_CLEAR();
										}
									}
								}
								else if(On_Off == _ePUMP_OFF)
								{
									if(_sRuble_Parameters.Lora_Data_Rx[7] == _ePUMP_OFF)
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("_ePUMP_OFF");
										}
										fault_check_counter = _kRESET;
										data_send_counter = 3;
										_sWireless_Pump_Param[wireless_pump_number].Indication_Of_Faults  = (_sRuble_Parameters.Lora_Data_Rx[11]<<8) | (_sRuble_Parameters.Lora_Data_Rx[12]);
										answer = 1;
										return 1;
									}
									else if (_sRuble_Parameters.Lora_Data_Rx[7] == _ePUMP_OR_ELECTRICITY_RELATED)
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("_ePUMP_OR_ELECTRICITY_RELATED");
										}
										fault_check_counter = _kRESET;
										data_send_counter = 3;
										_sWireless_Pump_Param[wireless_pump_number].Indication_Of_Faults  = (_sRuble_Parameters.Lora_Data_Rx[11]<<8) | (_sRuble_Parameters.Lora_Data_Rx[12]);
										
										_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_LINE_FAULT;
										_sRuble_Parameters.Publish_Topic  |= _ePUB_RUBLE_FAULTS;
										
										return 0;
									}
									else
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("Wireless Pump Feedback fail");
											_kSERIAL_MON_CLEAR();
										}
										/*return 0;*/
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
						
					}
				}while(_gTime_To_Wait_Lora_Resp < 8 && answer == _kRESET);
				
				if(answer == 1)
				{
					fault_check_counter = _kRESET;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE_NO_LN("wireless pump available 6: ");
						Serial.println();
						_kSERIAL_MON_CLEAR();
					}
					return 1;
				}
				
				//return 0;
				
				if(fault_check_counter > 2)
				{
					fault_check_counter = _kRESET;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE_NO_LN("wireless pump available 7: ");
						Serial.println();
						_kSERIAL_MON_CLEAR();
					}
					_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
					_sRuble_Parameters.Last_Pump = wireless_pump_number;
					_sRuble_Parameters.Publish_Topic |=   _eWIRELESS_PUMP_FAIL;
					return 0;
				}
				delay(1000);
			}
			
			_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
			_sRuble_Parameters.Last_Pump = wireless_pump_number;
			_sRuble_Parameters.Publish_Topic |=   _eWIRELESS_PUMP_FAIL;
			
			return 0;
			
			
			if(answer == 1)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE_NO_LN("wireless pump available 8: ");
					Serial.println();
					_kSERIAL_MON_CLEAR();
				}
				fault_check_counter = _kRESET;
				return 1;
			}
			if(data_send_counter > 3)
			{
				if(fault_check_counter > 2)
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE_NO_LN("wireless pump available 9: ");
						Serial.println();
						_kSERIAL_MON_CLEAR();
					}
					fault_check_counter = _kRESET;
					//_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
					_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
					_sRuble_Parameters.Last_Pump = wireless_pump_number;
					//_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
					_sRuble_Parameters.Publish_Topic |=   _eWIRELESS_PUMP_FAIL;
					return 0;
				}
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE_NO_LN("wireless pump available 4.2: ");
					Serial.println();
					_kSERIAL_MON_CLEAR();
				}
				return 0;
			}
			
			return 0;
			
		}break;
		
		case _eDO_CALIBRATION:
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE_NO_LN("Wireless pump no. ");
				_kSERIAL_MON_PRINT_NUM(wireless_pump_number);
				_kSERIAL_MON_CLEAR();
			}
			if(_kLORA_DATA_AVAIALABLE())    /* read if there is already any data received(garbage data) */
			{
				_kLORA_RECV_DATA();         /*  lora rx_tx buffer will update*/
			}
			
			memset((char *)_sRuble_Parameters.Lora_Data_TX,0,sizeof(_sRuble_Parameters.Lora_Data_TX));
			count = _kRESET;
			
			/************************************* copy the euro valve ID *********************************************/
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[0];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[1];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[2];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[3];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[4];
			_sRuble_Parameters.Lora_Data_TX[count++] = _sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[5];
			
			/*********************************** packet_id ***********************************************/
			_sRuble_Parameters.Lora_Data_TX[count++] = _eCOMMAND_FROM_WIRELESS_PUMP;
			
			_sRuble_Parameters.Lora_Data_TX[count++] = _eDO_CALIBRATION;
			
			/********************************** copy operation mode ************************************************/
			_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Operation_Mode;
			
			/******************************** copy pump timer (used if selected mode is timer) *********************/
			_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Pump_Run_Timer << 8;
			_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Pump_Run_Timer;
			_sRuble_Parameters.Lora_Data_TX[count++] = wireless_pump_number+1;
			_sRuble_Parameters.Lora_Data_TX[count++] = 'S';
			_sRuble_Parameters.Lora_Data_TX[count++] = _sRuble_Parameters.Calibration_Over_Ruled;
			
			data_send_counter = _kRESET;
			
			while(data_send_counter < _kRETRY_TO_SEND_VALVE)
			{
				data_send_counter++;
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Send On/off Command to wireless Pump 1");
					_kSERIAL_MON_CLEAR();
				}
				Display_Update_1();    // 20/1/2022
				_kLORA_SEND_DATA(count);
				if(_kCHEK_DATA_AVAILABLE_TIMEOUT(12000))    /* wait for 3 sec to wait the data from the EURO valve */
				{
					if(_kLORA_RECV_DATA())            /* check the received data */
					{
						/* check the id get matched to the valve or not */
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE_NO_LN("Wireless Pump rx data : ");
							_kSERIAL_MON_WRITE((char *)_sRuble_Parameters.Lora_Data_Rx,10);
							Serial.println();
							_kSERIAL_MON_CLEAR();
							_kSERAIL_MON_WRITE_NO_LN("Euro valve ID which is Sent :");
							_kSERIAL_MON_WRITE(_sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID,7);
							Serial.println();
							_kSERIAL_MON_CLEAR();
						}
							
						for(j=0;j<_kVALVE_ID_LEN;j++)
						{
							if(_sWireless_Pump_Param[wireless_pump_number].Wireless_Pump_ID[j] != _sRuble_Parameters.Lora_Data_Rx[j])
							{
								if(data_send_counter >= _kRETRY_TO_SEND_VALVE-1)
								{
									return 0;
								}
							}
						}
						
						if(_sRuble_Parameters.Lora_Data_Rx[7] != _eDO_CALIBRATION)     /* check weather valve is ON/off or Not */
						{
							if(data_send_counter >= _kRETRY_TO_SEND_VALVE-1)
							{
								return 0;
							}
						}
					}
					else
					{
						if(data_send_counter >= _kRETRY_TO_SEND_VALVE-1)
						{
							return 0;
						}
					}
				}
				else
				{
					if(data_send_counter >= _kRETRY_TO_SEND_VALVE-1)
					{
						return 0;
					}
				}
			}
			
			if(_sRuble_Parameters.Lora_Data_Rx[13] == _eCALIBARATION_SUCCESS)
			{
				return 1;
			}
			
			return 0;
				
		}break;
		
	}
}

unsigned char Plot_Status_List_Monitor(void)
{
	unsigned char set_flag;
	unsigned char List_Off_Valves[_kMAX_VALVE_FOR_1PLOT];
	unsigned char counter = 0,i=0,count=0,j=0,id_len=0,compare_id=0,current_valve_number=0,data_send_counter=0;
	
	if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
	{
		if(_sRuble_Parameters.Wirelss_Pump_Status == _kOFF || (_sRuble_Parameters.Irrigating_Plot != 0) ||
		_sRuble_Parameters.Operation_Mode == _eOPER_MOD_MANUAL)
		{
			for(unsigned char plot_check=0; plot_check<_kMAX_LIRA_SENSOR_NODE;plot_check++)
			{
				if(_sRuble_Parameters.Pending_Plots[plot_check] == _kSET)
				{
					/* turn off the valve */
				
					if((_sRuble_Parameters.Irrigating_Plot-1) == plot_check)// _sRuble_Parameters.Pending_Plots[plot_check])
					{
						return 1;
					}
					if(_sRuble_Parameters.Irrigating_Plot)
					{
						memset(List_Off_Valves,0,_kMAX_VALVE_FOR_1PLOT);
						for(unsigned char check_valve=0; check_valve < _kMAX_VALVE_FOR_1PLOT; check_valve++)
						{
							set_flag = 0;
							for(unsigned char valves_number=0; valves_number < _kMAX_VALVE_FOR_1PLOT; valves_number++)
							{
								if(_sLira_Node_Param[plot_check].Euro_Valve_Group[check_valve] == 
								_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot- 1].Euro_Valve_Group[valves_number])
								{
									set_flag = _kSET;
								}
							}
						
							if(set_flag == _kRESET)
							{
								List_Off_Valves[counter++] = _sLira_Node_Param[plot_check].Euro_Valve_Group[check_valve];
							}
						}
					
						/**************************** turn off the valves in the list *************************/	
					}
					else
					{
						for(unsigned char valves_number=0; valves_number<_kMAX_VALVE_FOR_1PLOT; valves_number++)
						{
							List_Off_Valves[valves_number] = _sLira_Node_Param[plot_check].Euro_Valve_Group[valves_number];
						}
					}
				
				
				
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
				
						_kSERAIL_MON_WRITE("last plot vvalve list : ");
						_kSERIAL_MON_CLEAR();
						for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
						{
							_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(List_Off_Valves[i]);
							_kSERAIL_MON_WRITE_NO_LN(" ");
						}
				
						_kSERAIL_MON_WRITE_NO_LN("last plot no : ");
						_kSERIAL_MON_PRINT_NUM(plot_check);
						_kSERIAL_MON_CLEAR();
					}
					
					for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
					{
						if(List_Off_Valves[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && List_Off_Valves[i] > 0)
						{
							unsigned char valve_number =0;
							unsigned char euro_valve_num=0,command_valve=0;
						
							valve_number =  List_Off_Valves[i];
						
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
							_sRuble_Parameters.Lora_Data_TX[count++] = _kOFF;
						
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
										Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_eSUBSCRIBE_DATA);
										//mqttCallback((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff));
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
													_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
													_sRuble_Parameters.Last_Valve = current_valve_number;
													_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
												
													for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
													{
														if(List_Off_Valves[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
														{
															/********************************* turn on/off the wired valve *********************************/
															_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
									
										if(_sRuble_Parameters.Lora_Data_Rx[8] != _kOFF)     /* check weather valve is ON/off or Not */
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
													if(List_Off_Valves[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
													{
														/********************************* turn on/off the wired valve *********************************/
														_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
												if(List_Off_Valves[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
												{
													/********************************* turn on/off the wired valve *********************************/
													_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
							
									if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
									{
										_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
										_gfault_Occure = _kFAULT_RESET_TIMER;
										_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
										_sRuble_Parameters.Last_Valve = current_valve_number;
										_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
									
										for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
										{
											if(List_Off_Valves[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
											{
												/********************************* turn on/off the wired valve *********************************/
												_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
					
						else if(List_Off_Valves[i] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO
						&& List_Off_Valves[i] < 100)
						{
							/********************************* turn on/off the wired valve *********************************/
							_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[i]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(!_kOFF));
							_kDELAY_MS(200);
						}
					
						else if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] > 100)
						{
							if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] == 101)
							{
							
							}
						}
					}
				
					/* if all valve gets on */
					_sRuble_Parameters.Pending_Plots[plot_check] = _kRESET;
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("All valves successfully ON/OFF");
						_kSERIAL_MON_CLEAR();
					}
					return 1;
				}
			}
		}
	}
		
}

unsigned char Fogger_Status_List_Monitor(void)
{
	unsigned char set_flag;
	unsigned char List_Off_Valves[_kMAX_VALVE_FOR_1PLOT];
	unsigned char counter = 0,i=0,count=0,j=0,id_len=0,compare_id=0,current_valve_number=0,data_send_counter=0;
	
	unsigned char plot_check = 13;
	
	if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
	{
		if(_sRuble_Parameters.Wirelss_Pump_Status == _kOFF || (_sRuble_Parameters.Irrigating_Plot != 0)
		|| _sRuble_Parameters.Operation_Mode == _eOPER_MOD_MANUAL)
		{
			if(_sRuble_Parameters.Pending_Plots[plot_check] == _kSET)
			{
				/* turn off the valve */
			
				if((_sRuble_Parameters.Irrigating_Plot-1) == plot_check)// _sRuble_Parameters.Pending_Plots[plot_check])
				{
					return 1;
				}
				if(_sRuble_Parameters.Irrigating_Plot)
				{
					memset(List_Off_Valves,0,_kMAX_VALVE_FOR_1PLOT);
					for(unsigned char check_valve=0; check_valve < _kMAX_VALVE_FOR_1PLOT; check_valve++)
					{
						set_flag = 0;
						for(unsigned char valves_number=0; valves_number < _kMAX_VALVE_FOR_1PLOT; valves_number++)
						{
							if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET && _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kRESET)
							{
								List_Off_Valves[counter++] = _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Euro_Valve_Group[check_valve];
							}
						}
					}
				
					/**************************** turn off the valves in the list *************************/
				}
				else
				{
					for(unsigned char valves_number=0; valves_number<_kMAX_VALVE_FOR_1PLOT; valves_number++)
					{
						if(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET && _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kRESET)
						{
							List_Off_Valves[counter++] = _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Euro_Valve_Group[valves_number];
						}
					}
				}
			
			
			
			
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("last plot vvalve list : ");
					_kSERIAL_MON_CLEAR();
				}
				
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
					{
						_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(List_Off_Valves[i]);
						_kSERAIL_MON_WRITE_NO_LN(" ");
					}
				}
			    if(_sRuble_Parameters.Debug_Mode == _kSET)
			    {
					_kSERAIL_MON_WRITE_NO_LN("last plot no : ");
					_kSERIAL_MON_PRINT_NUM(plot_check);
					_kSERIAL_MON_CLEAR();
				}
				
				for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
				{
					if(List_Off_Valves[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && List_Off_Valves[i] > 0)
					{
						unsigned char valve_number =0;
						unsigned char euro_valve_num=0,command_valve=0;
					
						valve_number =  List_Off_Valves[i];
					
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
						}
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
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
						_sRuble_Parameters.Lora_Data_TX[count++] = _kOFF;
					
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
									Pub_Sub_Data((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff),_eSUBSCRIBE_DATA);
									//mqttCallback((char *)_gRecvd_Data, (unsigned char *)_gPub_Buff, strlen((const char*)_gPub_Buff));
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
												_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
												_sRuble_Parameters.Last_Valve = current_valve_number;
												_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
											
												for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
												{
													if(List_Off_Valves[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
													{
														/********************************* turn on/off the wired valve *********************************/
														_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
								
									if(_sRuble_Parameters.Lora_Data_Rx[8] != _kOFF)     /* check weather valve is ON/off or Not */
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
												if(List_Off_Valves[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
												{
													/********************************* turn on/off the wired valve *********************************/
													_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
											if(List_Off_Valves[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
											{
												/********************************* turn on/off the wired valve *********************************/
												_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
							
								if(data_send_counter == _kRETRY_TO_SEND_VALVE-1)
								{
									_sPump_Parameter.Irrigation_Status =  _eIRRIG_STAT_EURO_FEEDBACK;
									_gfault_Occure = _kFAULT_RESET_TIMER;
									_sGSM_Query.SMS_Alert |= _eVALVE_SENSE_FAIL;
									_sRuble_Parameters.Last_Valve = current_valve_number;
									_sRuble_Parameters.Publish_Topic |=   _ePUB_RUBLE_FAULTS;
								
									for(unsigned char wired_valve=0; wired_valve <_kMAX_VALVE_FOR_1PLOT;  wired_valve++)
									{
										if(List_Off_Valves[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
										{
											/********************************* turn on/off the wired valve *********************************/
											_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[wired_valve]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(1));
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
				
					else if(List_Off_Valves[i] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO
					&& List_Off_Valves[i] < 100)
					{
						/********************************* turn on/off the wired valve *********************************/
						_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[List_Off_Valves[i]-(_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO+1)],(!_kOFF));
						_kDELAY_MS(200);
					}
				
					else if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] > 100)
					{
						if(_sLira_Node_Param[_sRuble_Parameters.Irrigating_Plot-1].Euro_Valve_Group[i] == 101)
						{
						
						}
					}
				}
				_sRuble_Parameters.Pending_Plots[plot_check] = _kRESET;
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					
					_kSERAIL_MON_WRITE("All valves successfully ON/OFF");
					_kSERIAL_MON_CLEAR();
				}
				return 1;
			
				/* if all valve gets on */
			
			}
		}
	}
}

unsigned char Pump_Status_List_Monitor(void)
{
	static unsigned char counter = 0;
	if(counter > 120)
	{
		for(unsigned char pump_number=0; pump_number<_kMAX_WIRELESS_PUMP+1; pump_number++)
		{
			if(_sRuble_Parameters.Wireless_Pump_Handle_List[pump_number] == _kON)
			{
				if((pump_number+1) != _sRuble_Parameters.Pump_Selected_For_Irrigation)
				{
					if(pump_number < 1)
					{
						counter = _kRESET;
						if(_kGPIO_GET(_kPUMP_1) != _kPUMP_OFF)     /* check weather pump is already Off */
						{
							_kGPIO_SET(_kPUMP_1,_kPUMP_OFF);      /* turn Off the pump */
						}
						
						if(_kGPIO_GET(_kPUMP_2) != _kPUMP_OFF)     /* check weather pump is already Off */
						{
							_kGPIO_SET(_kPUMP_2,_kPUMP_OFF);      /* turn Off the pump */
						}
						_sRuble_Parameters.Wireless_Pump_Handle_List[pump_number] = _kOFF;
						_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
						return 1;
					}
					else
					{
						counter = _kRESET;
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("wl 1 pump trigger 5555555555555555555555");
							_kSERIAL_MON_CLEAR();
						}
						//if(Wireless_Pump_On_Off(pump_number-1,_kOFF))
						if(Wireless_Pump_On_Off(_ePUMP_OFF,pump_number-1))
						{
							_sRuble_Parameters.Wireless_Pump_Handle_List[pump_number] = _kOFF;
							_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
							return 1;
						}	
					}
					_kEEPROM_WRITE(_kHANDLE_WIRELESS_STATUS_LIST,_sRuble_Parameters.Wireless_Pump_Handle_List);
					return 0;
				}
			}
		
		}
	}
	else
	{
		counter++;
	}
}

