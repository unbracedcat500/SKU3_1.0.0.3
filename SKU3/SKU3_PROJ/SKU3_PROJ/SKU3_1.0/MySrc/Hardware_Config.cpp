/*
 * Hardware_Config.cpp
 *
 * Created: 24-08-2021 10:45:23
 *  Author: Tushar (SIO)
 */ 



#include <Hardware_Config.h>
#include <Initialize.h>
//#include "My_Lora.h"
//#include <RH_RF95.h>
#include <RH_RF95.h>
#include <avr/wdt.h>

volatile uint32_t g_uTick,_g_1Sec_Time;       /**< increment the value of this tick variable in every 1 millisecond(in timer 1 interrupt) */
LiquidCrystal_I2C lcd(0x27, 20, 4);     /**< make the instance for the LCD */
volatile unsigned char Start_Display_In_Interrupt;

RH_RF95 lora(_kLORA_CS, _kLORA_INT); 




void Initialise_Hardware(void)
{
	unsigned char i;
	char temp_data[3];
	
	wdt_enable(WDTO_8S);
	_gStart_Wdt = 0;
	
	Timer1_Interrupt_Int();           /* used to initialize for 1 MSEC */ // now
	Timer0_Interrupt_Int();           /* used for 1 sec */

	/***************************** GPIO OUTPUT INITIALIZE ********************************/
	_kLCD_INIT();                             /* it will return nothing because */
	
	/***************************** serial **************************************/
	_KSERIAL_MON_INIT(9600);         /* serial monitor for debugging purpose */
	_KSERIAL_AT_INIT(115200);          /* serial for the AT commands */
	
	_kGPIO_MODE(19,_kINPUT_PULLUP);
	
	
	lcd.setCursor(0,1);
	lcd.print(F("  FV : SKU3_1.0.0.3 "));
	
	lcd.setCursor(0,2);
	lcd.print(F("                    "));
	
	delay(3000);
	
	lcd.clear();
	lcd.setCursor(5,1);
	lcd.print(F("SENSE IT OUT"));
	_kSERIAL_MON_CLEAR();

	_kGPIO_MODE(LED_BUILTIN, _kOUPTPUT_MODE);           /* testing led */
	_kGPIO_MODE(_kPUMP_1, _kOUPTPUT_MODE);              /* pump 1 on off */
	_kGPIO_MODE(_kPUMP_2, _kOUPTPUT_MODE);              /* pump 2 on off */
	
	_kGPIO_MODE(_kVALVE_1, _kOUPTPUT_MODE);             /* valve 1 on off */
	_kGPIO_MODE(_kVALVE_2, _kOUPTPUT_MODE);             /* valve 2 on off */
	_kGPIO_MODE(_kVALVE_3, _kOUPTPUT_MODE);             /* valve 3 on off */
	_kGPIO_MODE(_kVALVE_4, _kOUPTPUT_MODE);             /* valve 4 on off */
	_kGPIO_MODE(_kVALVE_5, _kOUPTPUT_MODE);             /* valve 5 on off */
	_kGPIO_MODE(_kVALVE_6, _kOUPTPUT_MODE);             /* valve 6 on off */
	_kGPIO_MODE(_kVALVE_7, _kOUPTPUT_MODE);             /* valve 7 on off */
	_kGPIO_MODE(_kVALVE_8, _kOUPTPUT_MODE);             /* valve 8 on off */
	_kGPIO_MODE(_kVALVE_9, _kOUPTPUT_MODE);             /* valve 9 on off */
	_kGPIO_MODE(_kVALVE_10, _kOUPTPUT_MODE);            /* valve 10 on off */
	_kGPIO_MODE(_kVALVE_11, _kOUPTPUT_MODE);            /* valve 11 on off */
	_kGPIO_MODE(_kVALVE_12, _kOUPTPUT_MODE);            /* valve 12 on off */
	_kGPIO_MODE(_kVALVE_13, _kOUPTPUT_MODE);             /* valve 9 on off */
	_kGPIO_MODE(_kVALVE_14, _kOUPTPUT_MODE);            /* valve 10 on off */
	_kGPIO_MODE(_kVALVE_15, _kOUPTPUT_MODE);            /* valve 11 on off */
	_kGPIO_MODE(_kVALVE_16, _kOUPTPUT_MODE);            /* valve 12 on off */
	
	_kGPIO_MODE(_kVALVE_17, _kOUPTPUT_MODE);             /* valve 1 on off */
	_kGPIO_MODE(_kVALVE_18, _kOUPTPUT_MODE);             /* valve 2 on off */
	_kGPIO_MODE(_kVALVE_19, _kOUPTPUT_MODE);             /* valve 3 on off */
	_kGPIO_MODE(_kVALVE_20, _kOUPTPUT_MODE);             /* valve 4 on off */
	_kGPIO_MODE(_kVALVE_21, _kOUPTPUT_MODE);             /* valve 5 on off */
	_kGPIO_MODE(_kVALVE_22, _kOUPTPUT_MODE);             /* valve 6 on off */
	_kGPIO_MODE(_kVALVE_23, _kOUPTPUT_MODE);             /* valve 7 on off */
	_kGPIO_MODE(_kVALVE_24, _kOUPTPUT_MODE);             /* valve 8 on off */
// 	_kGPIO_MODE(_kVALVE_25, _kOUPTPUT_MODE);             /* valve 9 on off */
// 	_kGPIO_MODE(_kVALVE_26, _kOUPTPUT_MODE);            /* valve 10 on off */
// 	_kGPIO_MODE(_kVALVE_27, _kOUPTPUT_MODE);            /* valve 11 on off */
// 	_kGPIO_MODE(_kVALVE_28, _kOUPTPUT_MODE);            /* valve 12 on off */
// 	_kGPIO_MODE(_kVALVE_29, _kOUPTPUT_MODE);             /* valve 9 on off */
// 	_kGPIO_MODE(_kVALVE_30, _kOUPTPUT_MODE);            /* valve 10 on off */
// 	_kGPIO_MODE(_kVALVE_31, _kOUPTPUT_MODE);            /* valve 11 on off */
// 	_kGPIO_MODE(_kVALVE_32, _kOUPTPUT_MODE);            /* valve 12 on off */

		
	
	_kGPIO_MODE(_kLORA_CS, _kOUPTPUT_MODE);              /* lora chip select*/
	_kGPIO_MODE(_kSWITCH_VCC_PIN,  _kOUPTPUT_MODE);      /* VCC pin for switch */
	_kGPIO_MODE(_kPUMP_STATUS_LED, _kOUPTPUT_MODE);      /* used to indicate weather pump is on or off */
	_kGPIO_MODE(_kMODE_STATUS_LED, _kOUPTPUT_MODE);      /* indicate that either manual or other mode selected */
	_kGPIO_MODE(_kNETWORK_STATUS_LED, _kOUPTPUT_MODE);   /* indicate weather controller is connected to server or not */
	
	_kGPIO_MODE(_kGSM_MODULE_ENABLE, _kOUPTPUT_MODE);            /* GSM enable pin */
	_kGPIO_SET(_kGSM_MODULE_ENABLE,_kHIGH);   // now
	
	_kGPIO_MODE(_kPHASE_DETECTION, _kINPUT_MODE);
	_kGPIO_MODE(_kDEBUG_PIN, _kINPUT_MODE);
	//_kGPIO_MODE(_kPHASE_DETECTION_SWITCH, _kINPUT_PULLUP);
	
	/****************************** turn off the pump ***********************************/
	_kGPIO_SET(_kPUMP_1,_kHIGH);
	_kGPIO_SET(_kPUMP_2,_kHIGH);
	
	/*************************** update all valves **************************************/
	_sRuble_Parameters.Wired_valve_List[0]   = _kVALVE_1;
	_sRuble_Parameters.Wired_valve_List[1]   = _kVALVE_2;
	_sRuble_Parameters.Wired_valve_List[2]   = _kVALVE_3;
	_sRuble_Parameters.Wired_valve_List[3]   = _kVALVE_4;
	_sRuble_Parameters.Wired_valve_List[4]   = _kVALVE_5;
	_sRuble_Parameters.Wired_valve_List[5]   = _kVALVE_6;
	_sRuble_Parameters.Wired_valve_List[6]   = _kVALVE_7;
	_sRuble_Parameters.Wired_valve_List[7]   = _kVALVE_8;
	_sRuble_Parameters.Wired_valve_List[8]   = _kVALVE_9;
	_sRuble_Parameters.Wired_valve_List[9]   = _kVALVE_10;
	_sRuble_Parameters.Wired_valve_List[10]  = _kVALVE_11;
	_sRuble_Parameters.Wired_valve_List[11]  = _kVALVE_12;
	_sRuble_Parameters.Wired_valve_List[12]  = _kVALVE_13;
	_sRuble_Parameters.Wired_valve_List[13]  = _kVALVE_14;
	_sRuble_Parameters.Wired_valve_List[14]  =  _kVALVE_15;
	_sRuble_Parameters.Wired_valve_List[15]  =  _kVALVE_16;	
	_sRuble_Parameters.Wired_valve_List[16]  = _kVALVE_17;
	_sRuble_Parameters.Wired_valve_List[17]  = _kVALVE_18;
	_sRuble_Parameters.Wired_valve_List[18]  = _kVALVE_19;
	_sRuble_Parameters.Wired_valve_List[19]  = _kVALVE_20;
	_sRuble_Parameters.Wired_valve_List[20]  = _kVALVE_21;
	_sRuble_Parameters.Wired_valve_List[21]  = _kVALVE_22;
	_sRuble_Parameters.Wired_valve_List[22]  = _kVALVE_23;
	_sRuble_Parameters.Wired_valve_List[23]  = _kVALVE_24;
	
// 	_sRuble_Parameters.Wired_valve_List[24]  = _kVALVE_9;
// 	_sRuble_Parameters.Wired_valve_List[25]  = _kVALVE_10;
// 	_sRuble_Parameters.Wired_valve_List[26]  = _kVALVE_11;
// 	_sRuble_Parameters.Wired_valve_List[27]  = _kVALVE_12;
// 	_sRuble_Parameters.Wired_valve_List[28]  = _kVALVE_13;
// 	_sRuble_Parameters.Wired_valve_List[29]  = _kVALVE_14;
// 	_sRuble_Parameters.Wired_valve_List[30]  =  _kVALVE_15;
// 	_sRuble_Parameters.Wired_valve_List[31]  =  _kVALVE_16;
	
	for(i=0;i<_kMAX_WIRED_VALVE;i++)  
	{
		_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[i],_kHIGH);
	}
	
	/************************** RYB input ***************************/
	_kGPIO_MODE(_kR_PHASE_INPUT, _kINPUT_PULLUP);
	_kGPIO_MODE(_kY_PHASE_INPUT, _kINPUT_PULLUP);
	_kGPIO_MODE(_kB_PHASE_INPUT, _kINPUT_PULLUP);
	
	
	
/*while(1)       // this is for the relay testing(also for both pump relay)
	{
		_kGPIO_SET(_kPUMP_1,_kLOW);
		delay(1000);
		_kGPIO_SET(_kPUMP_1,_kHIGH);
		delay(1000);
		_kGPIO_SET(_kPUMP_2,_kLOW);
		delay(1000);
		_kGPIO_SET(_kPUMP_2,_kHIGH);
		delay(1000);
		
		for(i=0;i<_kMAX_WIRED_VALVE;i++)     
		{
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[i],_kLOW);
			delay(1000);
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[i],_kHIGH);
			delay(1000);
		}
		
	} */
	
	/************???????????????????????????????????????????????******************/
			
	 DDRE = DDRE & (~(1<<6));		/* Make PD2 as input pin */
	 PORTE = PORTE | (1<<6);		/* Enable pull-up on PD2 by writing 1 to it */
	
	/************************ timer 1 interrupt initilaize ************************/
// 	Timer1_Interrupt_Int();           /* used to initialize for 1 MSEC */ // now
// 	Timer0_Interrupt_Int();           /* used for 1 sec */

	_kDELAY_MS(1000);
	
	
	
	/******************************** ADC ****************************************/
	
	//_KSERIAL_AT_INIT(115200);
	
	//_kDELAY_MS(1000);                               /* 2 sec delay */
	
	Debug_Check();
	
	/******************************************* LORA ****************************************/
	
	if(_kLORA_INIT())                            /* check weather lora module is initiated successfully or not */
	{
		_kLORA_SET_FREQ();        /* set frequency for lora */
		_kLORA_MODULE_CONFIG();   /* lora module configuration */
		_kLORA_SET_TX_POWER();    /* set tx power of lora */
		_sRuble_Parameters.Lora_Module_Check = _kRESET;    /* if lora module is working then reset this variable */
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{	
			_kSERAIL_MON_WRITE("LORA INITIATED");    /* for debugging purpose */
 			_kSERIAL_MON_CLEAR();
		}
	}
	else
	{
		_sRuble_Parameters.Hardware_Faults |= _eLORA_INIT_FAIL;   /* update the hardware faults that lcd is fail to initialize */
		_sRuble_Parameters.Publish_Topic |= _ePUB_HARDWARE_FAULTS;
		_sRuble_Parameters.Lora_Module_Check = _kSET;             /* if lora module is not working then set this variable */
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("LORA NOT INITIATED");                 /* for debugging purpose */
 			_kSERIAL_MON_CLEAR();
		}
	}
	
	/******************************************* GSM *****************************************/
			
// 	DDRE |= (1<<PE5);
// 	PORTE &= ~(1<<PE5);
// 	
// 	_kDELAY_MS(5000);
// 	
// 	PORTE |= ~(1<<PE5);
// 	
// 	_sRuble_Parameters.Display_Update |= _eDATA_INITIALISING;
// 	Display_Update_1();
// 	_kDELAY_MS(5000);
	
	
// 	_sRuble_Parameters.Display_Update |= _ePLEASE_WAIT;
// 	Display_Update_1();
// 	_kDELAY_MS(5000);
// 	_kDELAY_MS(5000);


  /*while(1)
  {
	  Check_Phase();
  }*/


	DDRF |= (1<<PF0);
	PORTF &= ~(1<<PF0);
	
	_kDELAY_MS(5000);
	
	PORTF |= ~(1<<PF0);
	
	_sRuble_Parameters.Display_Update |= _eDATA_INITIALISING;
	Display_Update_1();
	_kDELAY_MS(5000);
	
	_sRuble_Parameters.Display_Update |= _ePLEASE_WAIT;
	Display_Update_1();
	_kDELAY_MS(5000);
	_kDELAY_MS(5000);
	
	_kDELAY_MS(5000);
	_kDELAY_MS(5000);
	_kDELAY_MS(5000);
	
	
 
	
	memset(temp_data,0,sizeof(temp_data));
	temp_data[0] = 0x1A;
	_kSEND_AT_COMMAND(temp_data,"OK",NULL,5000,1);
	
	
	if(_kGSM_INIT())                             /* check weather GSM Module is initiated successfully or not */
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("GSM INITIATED");     /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
	}
	else
	{
		_sRuble_Parameters.Hardware_Faults |= _eGSM_INIT_FAIL;   /* update the hardware faults that GSM Module is fail to initialize */
		_sRuble_Parameters.Publish_Topic |= _ePUB_HARDWARE_FAULTS;
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{	
			_kSERAIL_MON_WRITE("GSM NOT INITIATED");                 /* for debugging purpose */
			_kSERIAL_MON_CLEAR();
		}
	}
	_sRuble_Parameters.Switch_To_Battery = _eELECTRICITY_IS_THERE;
	
	
	
	
	/*WDTCSR |= (1 << WDCE) | (1 << WDE);   // Enable the Watchdog Timer Configuration Change Enable (WDCE) bit
	WDTCSR = (1 << WDIE) | (1 << WDE) | (1 << WDP3) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);  // Enable the Watchdog Timer Interrupt (WDIE) bit, set the prescaler to 1024, and enable system reset
	sei();  // Enable global interrupts*/
	
}

ISR(WDT_vect) {
	// Perform actions when the Watchdog Timer interrupt occurs
	// For example, you can reset the microcontroller or take any necessary actions

	// Perform a system reset
// 	_kSERAIL_MON_WRITE("GSM NOT INITIATED111111");                 /* for debugging purpose */
// 	_kSERIAL_MON_CLEAR();
	
	//wdt_disable();

	DDRK |= (1 << PK6);

	// Pull the Reset (RST) pin low for a short duration
	PORTK &= ~(1 << PK6);
	_delay_ms(1000);  // Adjust the delay duration as needed
	PORTK |= (1 << PK6);
	
	wdt_disable();
	
	 // Disable the Watchdog Timer to prevent it from resetting during the reset sequence
	//while (1) {}  // Infinite loop to trigger a system reset
}

void Timer1_Interrupt_Int(void)
{
	noInterrupts();
	// Clear registers
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	// 1 Hz (16000000/((15624+1)*1024))
	OCR1A = 15624;
	// CTC
	TCCR1B |= (1 << WGM12);
	// Prescaler 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// Output Compare Match A Interrupt Enable
	TIMSK1 |= (1 << OCIE1A);
	interrupts();
}

/**************** timer 1 interrupt service routine. Invoked after every 1 msec **********/
ISR(TIMER1_COMPA_vect)
{
	static unsigned char display_conter,ct_timer;
	static unsigned int pub_ruble_data_counter;
	static unsigned char tank_num,Wdt_Timer = 0;
	
	
	if(_gfault_Occure)
	{
		_gfault_Occure--;
	}
	
	_g_1Sec_Time = _kSET;
	
	_sRuble_Parameters.Start_Monitoring_Pump_Faults_Timer++;
	
	display_conter++;
	if(display_conter > _kDISPLAY_REFRESH_RATE)
	{
		display_conter = _kRESET;
		_sRuble_Parameters.Display_Refresh = _kSET;
	}
	
	ct_timer++;
	
	if(ct_timer > 9)
	{
		ct_timer = 0;
		_sRuble_Parameters.Read_Ct_Value_Timer = _kSET;
	}

	if(_sRuble_Parameters.Timer_Feedback_Send_Timer)
	{
		_sRuble_Parameters.Timer_Feedback_Send_Timer--;
		if(_sRuble_Parameters.Timer_Feedback_Send_Timer < 2)
		{
			//_sRuble_Parameters.Publish_Topic |= _ePUB_TIMER_FEEDBACK;
		}
	}
	
	
	
	if(_sSchedules.Scheduler_Retry_Timer)
	{
		_sSchedules.Scheduler_Retry_Timer--;
	}
	
	#ifdef _kBATTTERY_IS_AVAILABLE
		 if(!(PINE & (1<<6)))
 		{
	 		if(_sRuble_Parameters.Switch_To_Battery != _eELECTRICITY_IS_NOT_THERE)
	 		{
	 			_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_FAULTS;
	 			_sRuble_Parameters.Switch_To_Battery = _eELECTRICITY_IS_NOT_THERE;
	 			_gfault_Occure = _kFAULT_RESET_TIMER;
				 _sPump_Parameter.Irrigation_Status   = _eTHREE_PHASE_POWER_INT;
	 			_sRuble_Parameters.Publish_Topic |= _eELECTRICITY_STATUS;
 			}
    	}
 		else
 		{
 			if(_sRuble_Parameters.Switch_To_Battery != _eELECTRICITY_IS_THERE)
 			{
 				_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_FAULTS;
 				_sRuble_Parameters.Switch_To_Battery = _eELECTRICITY_IS_THERE;
 				_sRuble_Parameters.Publish_Topic |= _eELECTRICITY_STATUS;
 			}
 		}
		 
    	#endif
		
		pub_ruble_data_counter++;
		if(pub_ruble_data_counter > 300)
		{
			pub_ruble_data_counter = _kRESET;
			_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_DATA;
		}
		
		for(unsigned char i=0 ; i<_kMAX_EXTRA_PLOT; i++)
		{
			if(_sExtra_Plot[i].Run_Timer)
			{
				_sExtra_Plot[i].Run_Timer--;
			}
		}
		
		
		
		if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation != _kSET)
		{
			if(_sRuble_Parameters.Pump_Is_Running_Or_Not)
			{
				if(_sSchedules.Pump_On_In_Scheduler == _kSET)
				{
					_sSchedules.Keep_Scheduler_Run_Timer++;
					_sSchedules.Save_Schedule_Data_In_Eeprom++;
				}
				
				if(_sRuble_Parameters.Flush_Retry == 0)
				{
					_sRuble_Parameters.Pump_Run_Timer_for_flush++;
				}
					
				if(_sRuble_Parameters.Timer_Mode_Wait_Timer == 0)
				{
					if(_sRuble_Parameters.Pump_Run_Timer)   /* if timer is not over */     /* 28/12/2021*/
					{
						_sRuble_Parameters.Pump_Run_Timer--;    /* pump run timer should decrease after every 1 sec */
					}
				}
			}
			
			
			
			if(_sRuble_Parameters.Timer_Mode_Wait_Timer)
			{
				_sRuble_Parameters.Timer_Mode_Wait_Timer--;
			}
			else
			{
// 				if(_sRuble_Parameters.Pump_Run_Timer)   /* if timer is not over */     /* 28/12/2021*/
// 				{
// 					_sRuble_Parameters.Pump_Run_Timer--;    /* pump run timer should decrease after every 1 sec */
// 				}
			}
			
			for(unsigned char tank_num = 0; tank_num < _kMAX_FERTILIZER_TANK; tank_num++)
			{
				if(_sSchedules.Tank_On_Retry[tank_num])
				{
					_sSchedules.Tank_On_Retry[tank_num]--;
				}
				
				if(_sSchedules.Running_Tank[tank_num] == _kON)
				{
					_sSchedules.Tank_Running_Timer[tank_num]++;
				}
			}
		}
		
		
		
			
		if(_sSchedules.Blower_On_Retry)
		{
			_sSchedules.Blower_On_Retry --;
		}
		if(_sSchedules.Blower_On_Time)
		{
			_sSchedules.Blower_On_Time--;
		}
		
		if(_sRuble_Parameters.Flush_Retry == 0)
		{
			if(_sRuble_Parameters.Flush_Running)
			{
				_sRuble_Parameters.Flush_Running--;
			}
		}
		else
		{
			_sRuble_Parameters.Flush_Retry--;
		}
		
		if(_sRuble_Parameters.Wireless_Pump_Operate_Timer)
		{
			_sRuble_Parameters.Wireless_Pump_Operate_Timer--;
		}
		
		_gTime_To_Wait_Lora_Resp++;
		
		if(_gWait_Blower_Time)
		{
			_gWait_Blower_Time --;
		}
		
		_gWdt_Counter++;
		
		if(_gTank_Topic_check < 60)
		{
			_gTank_Topic_check++;
		}
		
		
		if(_gStart_Wdt == _kSET)
		{
			if(_gWdt_Counter < 240)
			{
				wdt_reset();
			}
		}
		else
		{
			wdt_reset();
		}
		/*if(Wdt_Timer > 2)
		{
			Wdt_Timer = 0;
			wdt_reset();
		}*/
		
}


void Timer0_Interrupt_Int(void)
{
	noInterrupts();
	TCCR2A = 0;
	TCCR2B = 0;
	TCNT2 = 0;

	// 1000 Hz (16000000/((124+1)*128))
	OCR2A = 124;
	// CTC
	TCCR2A |= (1 << WGM21);
	// Prescaler 128
	TCCR2B |= (1 << CS22) | (1 << CS20);
	// Output Compare Match A Interrupt Enable
	TIMSK2 |= (1 << OCIE2A);
	interrupts();
}

/**************** timer 1 interrupt service routine. Invoked after every 1 msec **********/
ISR(TIMER2_COMPA_vect)
{
	g_uTick++;
	//ADC_Operation();
	Read_Switch_Status();
	
}




/******************************************************* LCD DATA ********************************************/

bool Lcd_Init(void)
{
	 lcd.init();
}

void Lcd_Backlight_On(void)
{
	//lcd.backlight();
}

void Lcd_Print_Buff(char *Buff,unsigned char len, unsigned char y_pos, unsigned char x_pos)
{
	int i;
	for(i=0;i<len;i++)
	{
		lcd.setCursor(x_pos++,y_pos);
		lcd.print(Buff[i]);
	}
}

void Lcd_Special_Char(uint8_t num,unsigned char y, unsigned char x, unsigned char Char_Name)
{
	unsigned char Buff[8];
	memset(Buff,0,8);
	
	
	switch(Char_Name)
	{
		case _eRANGE:
		{
			Buff[0] = 0x1f;
			Buff[1] = 0x11;
			Buff[2] = 0x0a;
			Buff[3] = 0x04;
			Buff[4] = 0x04;
			Buff[5] = 0x04;
			Buff[6] = 0x04;
			Buff[7] = 0x00;
		}break;
		case _eRANGE1:
		{
			Buff[0] = 0x1c;
			Buff[1] = 0x10;
			Buff[2] = 0x18; 
			Buff[3] = 0x10;
			Buff[4] = 0x1c; 
			Buff[5] = 0x00; 
			Buff[6] = 0x00; 
			Buff[7] = 0x10;
		}break;
		case _eRANGE2:
		{
			Buff[0] = 0x1c; 
			Buff[1] = 0x10;
			Buff[2] = 0x18; 
			Buff[3] = 0x10; 
			Buff[4] = 0x1c; 
			Buff[5] = 0x00; 
			Buff[6] = 0x04;
			Buff[7] = 0x14;
		}break;
		case _eRANGE3:
		{
			Buff[0] = 0x1c;
			Buff[1] = 0x10;
			Buff[2] = 0x18;
			Buff[3] = 0x10;
			Buff[4] = 0x1d;
			Buff[5] = 0x01;
			Buff[6] = 0x05;
			Buff[7] = 0x15;
		}break;
		
		case _eRANGE_X:
		{
			Buff[0] = 0x1c;
			Buff[1] = 0x10;
			Buff[2] = 0x18;
			Buff[3] = 0x10;
			Buff[4] = 0x1c;
			Buff[5] = 0x05;
			Buff[6] = 0x02;
			Buff[7] = 0x05;
		}break;
		
		case _eGSM_MODULE:
		{
			Buff[0] = 0x0e;
			Buff[1] = 0x11;
			Buff[2] = 0x11;
			Buff[3] = 0x11;
			Buff[4] = 0x1b;
			Buff[5] = 0x15;
			Buff[6] = 0x1b;
			Buff[7] = 0x0e;
		}break;
		
		case _eNO_SIM_CARD:
		{
			Buff[0] = 0x07;
			Buff[1] = 0x09;
			Buff[2] = 0x11;
			Buff[3] = 0x11;
			Buff[4] = 0x1b;
			Buff[5] = 0x15;
			Buff[6] = 0x1b;
			Buff[7] = 0x1f;
		}break;
		
		case _eLORA_MODULE:
		{
			Buff[0] = 0x04;
			Buff[1] = 0x04;
			Buff[2] = 0x04; 
			Buff[3] = 0x07;
			Buff[4] = 0x00;
			Buff[5] = 0x00;
			Buff[6] = 0x00;
			Buff[7] = 0x00;
		}break;
		
		case _eRANGE_C:
		{
			Buff[0] = 0x1f;
			Buff[1] = 0x1f;
			Buff[2] = 0x0e;
			Buff[3] = 0x04;
			Buff[4] = 0x04;
			Buff[5] = 0x04;
			Buff[6] = 0x04;
			Buff[7] = 0x00;
		}break;
		
		case _eTIME_UPDATE:
		{
			sprintf((char *)Buff,"%02d:%02d",_sRuble_Parameters.Update_Local_Time[0],_sRuble_Parameters.Update_Local_Time[1]);
			_kLCD_BUFF_WRITE((char *)Buff,5,0,15);        /* print time */	
			return ;
		}break;
		
		
	}
	
	lcd.createChar(num,Buff);
	lcd.setCursor(y, x);
	lcd.write(num);
}

void Lcd_Clear(void)
{
	lcd.clear();
}


/******************************************************* EEPROM DATA **********************************************/



/****************************************************** Lora *****************************************************/
// bool Lora_Init(void)
// {
// 	return RF95_Init();
// }
// 
// bool Lora_Available(void)
// {
// 	return RF95_available();
// }
// 
// bool Lora_Data_Receive(void)
// {
// 	unsigned char len;
// 	memset(_sRuble_Parameters.Lora_Data_Rx,0,sizeof(_sRuble_Parameters.Lora_Data_Rx));
// 	len = sizeof(_sRuble_Parameters.Lora_Data_Rx);
// 	return  RF95_receive(_sRuble_Parameters.Lora_Data_Rx, len);  
// }
// 
// 
// void Lora_Data_Send(unsigned char len)
// {
// 	_kSERAIL_MON_WRITE("lora data send len : ");
// 	_kSERIAL_MON_PRINT_NUM(len);
// 	RF95_send(_sRuble_Parameters.Lora_Data_TX,len);
// 	_kSERAIL_MON_WRITE("SENT WAITING");                /* for debugging purpose */
// 	RF95_waitPacketSent();
// 	RF95_setModeRx_Continuous();
// }
// 
// void Lora_Set_Freq(void)
// {
// 	 RF95_setFrequency(_kLORA_FREQUENCY);
// }
// 
// void Lora_Set_Power(void)
// {
// 	RF95_setTxPower(_kLORA_TX_POWER, false);
// }
// 
// void Lora_Set_Config(void)
// {
// 	RF95_setModemConfig(Bw125Cr45Sf128);
// }
// 
// bool Check_Data_Available(unsigned int timeout)
// {
// 	return RF95_available_Timeout(timeout);
// }
// 
// char Read_Last_RSSI(void)
// {
// 	return RF95_Last_RSSI();
// }
// 
// char Read_Last_SNR(void)
// {
// 	return RF95_Last_SNR();
// }


/****************************************************** Lora *****************************************************/
bool Lora_Init(void)
{
	return lora.init();
}

bool Lora_Available(void)
{
	return lora.available();
}

bool Lora_Data_Receive(void)
{
	unsigned char len,from;
	memset(_sRuble_Parameters.Lora_Data_Rx,0,sizeof(_sRuble_Parameters.Lora_Data_Rx));
	len = sizeof(_sRuble_Parameters.Lora_Data_Rx);
	return lora.recv(_sRuble_Parameters.Lora_Data_Rx, &len);
}


void Lora_Data_Send(unsigned char len)
{
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("lora data send len : ");
		_kSERIAL_MON_PRINT_NUM(len);
		_kSERIAL_MON_CLEAR();
	}
	lora.send(_sRuble_Parameters.Lora_Data_TX,len);
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("SENT WAITING");                /* for debugging purpose */
		_kSERIAL_MON_CLEAR();
	}
	lora.waitPacketSent(500);
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("SENT succ"); 
	}
}

void Lora_Set_Freq(void)
{
	lora.setFrequency(_kLORA_FREQUENCY);
}

void Lora_Set_Power(void)
{
	lora.setTxPower(_kLORA_TX_POWER, false);
}

void Lora_Set_Config(void)
{
	lora.setModemConfig(RH_RF95::Bw125Cr45Sf128);
	lora.setSpreadingFactor(11);
}

bool Check_Data_Available(unsigned int timeout)
{
	return lora.waitAvailableTimeout(timeout);
}

char Read_Last_RSSI(void)
{
	return lora.lastRssi();
}

char Read_Last_SNR(void)
{
	return lora.lastRssi();
}






/*********************************************************** GSM *************************************************/
int _kGSM_SIG_STRENGTH(void)
{
	char i = 0;
	char sig_stren;
	_kSERIAL_AT.println(F("AT+CSQ"));
	if(_kSEND_AT_COMMAND(NULL, "OK",  NULL,2000,1))
	{
		if(strstr((char *)_gRecvd_Data,"+CS") != 0)     // +CSQ
		{
			for(i=0;i<20;i++)
			{
				if(_gRecvd_Data[i] == '+' && _gRecvd_Data[i+4] == ':')
				{
					break;
				}
			}
			i = i+6;
			sig_stren = (_gRecvd_Data[i] - 0x30) * 10 + (_gRecvd_Data[i+1] - 0x30);
			return sig_stren;
		}
		
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE_NO_LN("CSQ RETEURN 0 ");
		_kSERIAL_MON_CLEAR();
	}
	return 0;	
}

bool Gsm_Init(void)
{
	_kSERIAL_AT.println(F("AT"));
	if(_kSEND_AT_COMMAND(NULL, "OK",  NULL,1000,2))
	{
		_kSERIAL_AT.println(F("AT+CMEE=0"));
		_kSEND_AT_COMMAND(NULL, "OK",  NULL,3000,1);
		return 1;
	}
	return 0;
}

bool Gsm_Wait_For_Network(void)
{
	
}

bool Check_Gsm_Connection(void)
{
	_kSERIAL_AT.println(F("AT+CNMP=38"));
	Send_AT_Command(NULL, "OK",  NULL,5000,2);
	_kSEND_AT_COMMAND(NULL,"OK",NULL,4000,2);
	_kSERIAL_AT.println(F("AT+CREG=1"));
	_kSEND_AT_COMMAND(NULL,"OK",NULL,3000,2);
	_kSERIAL_AT.println(F("AT+CREG?"));
	
	if(!_kSEND_AT_COMMAND(NULL,"+CREG: 1,1","+CREG: 1,5",3000,2))
	{
		_kDELAY_MS(3000);
		_kSERIAL_AT.println(F("AT+CREG=1"));
		_kSEND_AT_COMMAND(NULL,"OK",NULL,3000,2);
		_kSERIAL_AT.println(F("AT+CREG?"));
		if(!_kSEND_AT_COMMAND(NULL,"+CREG: 1,1","+CREG: 1,5",3000,2))
		{
			
		}
	}
	_kDELAY_MS(1000);
	_kSERIAL_AT.println(F("AT+CGREG=1"));
	_kSEND_AT_COMMAND(NULL,"OK",NULL,3000,2);
	_kSERIAL_AT.println(F("AT+CGREG?"));
	if(!_kSEND_AT_COMMAND(NULL,"+CGREG: 1,1","+CGREG: 1,5",3000,2))
	{
		_kDELAY_MS(1000);
		_kSERIAL_AT.println(F("AT+CGREG=1"));
		_kSEND_AT_COMMAND(NULL,"OK",NULL,3000,2);
		_kSERIAL_AT.println(F("AT+CGREG?"));
		
		if(!_kSEND_AT_COMMAND(NULL,"+CGREG: 1,1","+CGREG: 1,5",3000,2))
		{
			return 0;
		}
	}
	return 1;
}

void Get_Operator_Name(char *sim_operator)
{
	_kSERIAL_AT.println(F("AT+CSPN?"));
	_kSEND_AT_COMMAND(NULL,"OK",NULL,3000,2);
	strcpy((char *)sim_operator,(char *)_gRecvd_Data);
}

bool Gprs_Connect(char *apn)
{
	char temp_data[100];
	
	//sprintf((char *)temp_data,"AT+CGSOCKCONT=1,\"IP\",\"%s\"",_sNetwork_and_Usr_Info.Ruble_Sim_APN);
	sprintf((char *)temp_data,"AT+CGDCONT=1,\"IP\",\"%s\"",_sNetwork_and_Usr_Info.Ruble_Sim_APN);
	_kSEND_AT_COMMAND(temp_data,"OK",NULL,1000,1);
	
	_kSERAIL_MON_WRITE("!!!!!SUCCESSFULLY CONNECTED TO GPRS!!!!!");
	
	return 1;
}

bool Check_Gprs_Connection(void)
{
	
}

bool Send_SMS(char *mob_num, char *sms_string)
{
// 	char temp_data[30];
// 	memset(temp_data,0,sizeof(temp_data));
// 	
// 	_kSERIAL_AT.println(F("AT+CFUN=1"));
// 	_kSEND_AT_COMMAND(NULL,"OK",NULL,1000,1);
// 	_kSERIAL_AT.println(F("AT+CMGF=1"));
// 	
// 	if(_kSEND_AT_COMMAND(NULL,"OK",NULL,1000,2))
// 	{
// 		sprintf(temp_data,"AT+CMGS=\"%s\"",mob_num);
// 		if(_kSEND_AT_COMMAND(temp_data,">",NULL,1000,1))
// 		{
// 			_kSERIAL_AT.print((char *)sms_string);
// 			memset(temp_data,0,sizeof(temp_data));
// 			temp_data[0] = 0x1A;
// 			if(_kSEND_AT_COMMAND(temp_data,"OK",NULL,5000,2))
// 			{
// 				_kSERAIL_MON_WRITE_NO_LN("SENT ");
// 				return 1;
// 			}
// 			else
// 			{
// 				_kSERAIL_MON_WRITE_NO_LN("SENT FAIL");
// 			}
// 		}
// 		else
// 		{
// 			_kSERAIL_MON_WRITE_NO_LN("AT+CMGS FAIL");
// 		}
// 	}
// 	else
// 	{
// 		_kSERAIL_MON_WRITE_NO_LN("AT+CMGF=1 FAIL");
// 	}
// 	memset(temp_data,0,sizeof(temp_data));
// 	temp_data[0] = 0x1A;
// 	_kSERIAL_AT.print((char *)temp_data);
// 	return 0;
}

bool Read_SMS(void)
{
// 	char temp_data[30];
// 	
// 	_kSERIAL_AT.println(F("AT+CMGF=1"));
// 	if(_kSEND_AT_COMMAND(NULL,"OK",NULL,1000,2))
// 	{
// 		memset(temp_data,0,sizeof(temp_data));
// 		sprintf(temp_data,"AT+CMGL=\"ALL\"");
// 		if(Read_SMS_COMMAND(temp_data,"PUMP OFF","REC UNREAD",5000))
// 		{
// 			if(strstr((const char *)_gRecvd_Data,(const char *)"PUMP OFF") != 0)
// 			{
// 				if(_sRuble_Parameters.Operation_Mode != _eOPER_MOD_MANUAL)
// 				{
// 					memset(_sRuble_Parameters.Irrig_Priority,0,sizeof(_sRuble_Parameters.Irrig_Priority));
// 					_sPump_Parameter.Irrigation_Status |= _eIRRIG_STATUS_KILL_SWITCH;
// 					_sRuble_Parameters.Publish_Topic   |= _ePUB_RUBLE_FAULTS;
// 					_sRuble_Parameters.Kill_Switch = _eKILL_SWITCH_SET;
// 					_kEEPROM_WRITE(_kEEPROM_KILL_SWITCH,_sRuble_Parameters.Kill_Switch);
// 				}
// 				
// 				_kSERAIL_MON_WRITE_NO_LN("kill switch function pump off ");
// 				_sRuble_Parameters.Kill_Switch = _eKILL_SWITCH_SET;
// 				
// 				_kSERIAL_AT.println(F("AT+CMGD=1,1"));
// 				if(_kSEND_AT_COMMAND(NULL,"OK",NULL,1000,2))
// 				{
// 					_kSERAIL_MON_WRITE_NO_LN("DELETE UNREAD SMS");
// 				}
// 				return 1;
// 			}
// 		}
// 	}
// 	_kDELAY_MS(1000);
// 	_kSERIAL_AT.println(F("AT+CMGD=1,1"));
// 	if(_kSEND_AT_COMMAND(NULL,"OK",NULL,1000,2))
// 	{
// 		_kSERAIL_MON_WRITE_NO_LN("delete all sms ");
// 	}
// 	return 0;
}



/******************************************************** MQTT ***********************************************/

bool MQTT_Publish(unsigned char *topic, unsigned char *payload)
{
	char temp_data[100];
	
	memset(temp_data,0,sizeof(temp_data));
	sprintf(temp_data,"AT+CMQTTTOPIC=0,%d",strlen((char *)topic));
	if(_kSEND_AT_COMMAND(temp_data,">",NULL,3000,2))
	{
		_kSERIAL_AT.println((char *)topic);
		
		memset(temp_data,0,sizeof(temp_data));
		sprintf(temp_data,"AT+CMQTTPAYLOAD=0,%d",strlen((char *)payload));
		if(_kSEND_AT_COMMAND(temp_data,">",NULL,3000,2))
		{
			_kSERIAL_AT.println((char *)payload);
			
			if(_kSEND_AT_COMMAND("AT+CMQTTPUB=0,1,60","+CMQTTPUB: 0,0",NULL,3000,2))
			{
				return 1;
			}
		}
	}
	
	_kSERIAL_AT.println(F("AT+CFUN=1"));
	_kSEND_AT_COMMAND(NULL,"OK",NULL,1000,1);
	//_kSEND_AT_COMMAND("AT+CFUN=1","OK",NULL,1000,1);
	memset(temp_data,0,sizeof(temp_data));
	temp_data[0] = 0x1A;
	_kSEND_AT_COMMAND(temp_data,"OK",NULL,200,1);
	
	return 0;
	
}

bool MQTT_SUBSCRIBE(char topic[])
{
	char temp_data[100];
	memset(temp_data,0,sizeof(temp_data));
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERIAL_MON_WRITE(topic,strlen(topic));
	}
	
	sprintf(temp_data,"AT+CMQTTSUBTOPIC=0,%d,1",strlen(topic));
	
	if(_kSEND_AT_COMMAND(temp_data,">",NULL,3000,2))
	{
		_kSERIAL_AT.println(topic);
		if(_kSEND_AT_COMMAND("AT+CMQTTSUB=0,10,1,1",">",NULL,3000,2))
		{
			_kSERIAL_AT.println(topic);
			if((_kSEND_AT_COMMAND(NULL,"+CMQTTSUB: 0,0",NULL,5000,2)))
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("SUB SUCCESS");
					_kSERIAL_MON_CLEAR();
				}
				return 1;
			}
		}
	}
	return 0;
}

void MQTT_SET_CALLBACK(void)
{
}

bool MQTT_Connect(const char  client_id[], const char  user_name[], const char  password[])
{
	char temp_data[120];
	unsigned int port = 1883;
	
	memset(temp_data,0,sizeof(temp_data));
	memset(temp_data,0,sizeof(temp_data));
	
	_kSERIAL_AT.println(F("AT+CMQTTDISC=0,120"));
	_kSEND_AT_COMMAND(NULL,"+CMQTTDISC:",NULL,3000,2);
	
	_kSERIAL_AT.println(F("AT+CMQTTREL=0"));
	_kSEND_AT_COMMAND(NULL,"OK",NULL,3000,2);
	
	_kSERIAL_AT.println(F("AT+CMQTTSTART"));
	_kSEND_AT_COMMAND(NULL,"+CMQTTSTART: 0","+CMQTTSTART: 23",3000,2);
	
	sprintf(temp_data,"AT+CMQTTACCQ=0,\"%s\",0",_kRUBLE_ID);
	_kSEND_AT_COMMAND(temp_data,"OK",NULL,3000,1);
	
	memset(temp_data,0,sizeof(temp_data));
	
	sprintf(temp_data,"AT+CMQTTCONNECT=0,\"tcp://%s:%d\",60,0,\"%s\",\"%s\"",_kBROKER_IP,port,user_name,password);
	_kSEND_AT_COMMAND(temp_data,"OK",NULL,5000,1);
	
	_kDELAY_MS(3000);
	//if(_kSEND_AT_COMMAND(NULL,"+CMQTTCONNECT: 0,0",NULL,2000,1))
	//{
	//	return 1;
	//}
	
	if(Is_MQTT_Connected())
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
// 	memset(temp_data,0,sizeof(temp_data));
// 	sprintf(temp_data,"+CMQTTCONNECT: 0,\"tcp://%s:%d\",60,0,\"%s\",\"%s\"",_kBROKER_IP,port,user_name,password);
// 	_kSERIAL_AT.println(F("AT+CMQTTCONNECT?"));
// 	if(_kSEND_AT_COMMAND(NULL,temp_data,NULL,3000,2))
// 	{
// 		_kSERAIL_MON_WRITE("check Connected to mqtt");
// 		return 1;
// 	}
// 	_kSERAIL_MON_WRITE("check not Connected to mqtt");
// 	return 0;
}

void MQTT_Set_Server(char ip_address[], int port_num)
{
}

bool Is_MQTT_Connected(void)
{
	/****************CHECK COMMAND WHICH IS USED TO CHECK THE COMMAND OF MQTT CONNECTION */
	_kSERIAL_AT.println(F("AT+CMQTTCONNECT?"));
	//if(_kSEND_AT_COMMAND(NULL,"+CMQTTCONNECT: 0,\"tcp://34.123.55.118:1883\",60,0,\"mq_siouser\",\"9Xk!ej6fL6<,e2P8\"",NULL,3000,1))
	//if(_kSEND_AT_COMMAND(NULL,"+CMQTTCONNECT: 0,\"tcp://34.123.55.118:1883\",60,0",NULL,3000,1))
	if(_kSEND_AT_COMMAND(NULL,"+CMQTTCONNECT: 0,\"tcp",NULL,3000,1))
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("check Connected to mqtt");
			_kSERIAL_MON_CLEAR();
		}
		return 1;
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("check Connected to mqtt");
		_kSERIAL_MON_CLEAR();
	}
	return 0;
}

char MQTT_Loop(void)
{
	Check_Sim(_eDIRESCT_CHECK);
	Check_GSM_Signal();
	if(_sRuble_Parameters.Sim_Check == _eSIM_NOT_INSERTED)
	{
		return 0;
	}
	
	_kSERIAL_AT.println(F("AT+CGREG?"));
	if(!_kSEND_AT_COMMAND(NULL,"+CGREG: 1,1","+CGREG: 1,5",3000,2))
	{
		_kSERIAL_AT.println(F("AT+CGREG=1"));
		_kSEND_AT_COMMAND(NULL,"OK",NULL,3000,2);
		_kDELAY_MS(2000);
		_kSERIAL_AT.println(F("AT+CGREG?"));
		if(!_kSEND_AT_COMMAND(NULL,"+CGREG: 1,1","+CGREG: 1,5",3000,2))
		{
			return 0;
		}
	}
	
	if(_kIS_MQTT_CONNECTED())    /* if controller is not connected to server then don't execute this function */
	{
		if(_sRuble_Parameters.Subscribe_Topic_Status == _eALL_TOPIC_SUBSCRIBED)
		{
			if(_sRuble_Parameters.Update_Local_Date[2] == 70)
			{
				return 0;
			}
			return 1;
		}
	}
	return 0;
}


/****************************************************** SD Card *************************************/

bool Sd_Card_Init(int chip_select)
{
}

char Send_AT_Command(char* ATcommand, char* expected_answer, char* expected_answer2,unsigned int timeout,unsigned char retry)
{
	unsigned int x = 0, answer = 0,command_conter;
	char response[100];
	unsigned long previous;
	char temp_data[2];
	retry = 1;   
	command_conter = 0;
	while(command_conter < retry)
	{
		command_conter++;
		memset(response, '\0', sizeof(response)); 
		if(ATcommand != NULL)
		{
			delay(100);
		}
		if(ATcommand != NULL)
		{
			_kSERIAL_AT.println(ATcommand);
		}
		x = 0;
		previous = millis();
		do
		{
			if(x < (sizeof(response)-1))
			{
				if (_kSERIAL_AT.available() != 0)
				{
					response[x] = _kSERIAL_AT.read();
					x++;
					if(expected_answer2 != NULL)
					{
						if (strstr(response, expected_answer) != NULL || strstr(response, expected_answer2) != NULL )
						{
							answer = 1;
							command_conter++;
						}
					}
					else
					{
						if (strstr(response, expected_answer) != NULL)
						{
							answer = 1;
							command_conter++;
						}
					}	
				}
			}
		} while ((answer == 0) && ((millis() - previous) < timeout));
		_kDELAY_MS(100);
		Clear_Serial_Data();
	}
	
	if(!answer)
	{
		memset(temp_data,0,sizeof(temp_data));
		temp_data[0] = 0x1A;
		_kSERIAL_AT.print((char *)temp_data);
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		Serial.println(response);
		_kSERIAL_MON_CLEAR();
	}
	sprintf((char *)_gRecvd_Data,"%s",(char *)response);
	return answer;
}

void Select_Gpio_Mode_PORTE(int port,int pin , int mode)
{
	
	DDRE &= ~(1 << pin);
	
	// 	switch(mode)
	// 	{
	// 		case _kINPUT_MODE:
	// 		{
	// 			switch(port)
	// 			{
	// 				case DDRE:
	// 				{
	// 					DDRE &= ~(1 << pin);
	// 				}break;
	// 			}
	// 		}break;
	//
	// 		case _kOUPTPUT_MODE:
	// 		{
	//
	// 		}break;
	// 	}
}

char My_Gpio_Status_Read(int PORT,int Pin)
{
	if ((PORT & (1 << Pin)) == (1 << Pin))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Subscribed_data_check(void)
{
	static int counter,data_available,len;
	memset((unsigned char *)_gPub_Buff,0,sizeof(_gPub_Buff));
	counter = 0;
	data_available = 0;
	unsigned long previous;
	int  index = 0;
	int  inByte = 0;
	char WS[3];
	
	inByte = _kSERIAL_AT.readBytes(WS, 1);
	_gPub_Buff[counter++] = WS[0];
	previous = millis();

	/*----------( wait of further characters until a pause of 30 ms occur )----------*/
	while (inByte > 0)
	{
		if(inByte)
		{
			if(index < sizeof(_gPub_Buff)-1)
			{
				data_available = 1;
				inByte = _kSERIAL_AT.readBytes(WS, 1);
				_gPub_Buff[index++] = WS[0];
			}
			else
			{
				inByte = _kSERIAL_AT.readBytes(WS, 1);
			}
		}
	}
	if(data_available)
	{
		Clear_Serial_Data();
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("enter 1");
			_kSERIAL_MON_WRITE((const char*)_gPub_Buff,strlen((const char*)_gPub_Buff));
		}
		if(strstr((const char *)_gPub_Buff,"+CMQTTRX"))
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("TOPIC MATCH");
				_kSERIAL_MON_CLEAR();
			}
			return 1;
		}
	}
	
	return 0;
}

bool Seperate_Out_Sub_Data(void)
{
	unsigned char topic[50];
	unsigned char pay_load[sizeof(_gPub_Buff)];
	unsigned char Topic_compare[]       = "+CMQTTRXTOPIC: 0,";
	unsigned char Pay_load_compare[]    = "+CMQTTRXPAYLOAD: 0,";
	unsigned char len_buf[4];
	unsigned int lenght,i;
	unsigned int increment_len = 0, track_len, topic_payload_len,topic_payload_set,j;
	topic_payload_set = _eREAD_TOPIC;
	memset(pay_load,0,sizeof(pay_load));
	memset(topic,0,sizeof(topic));
	
	increment_len = 0;
	track_len =0;
	topic_payload_len = 0;
	while(_gPub_Buff[increment_len] !=0)
	{
		increment_len++;
		if(increment_len > sizeof(_gPub_Buff)-1)
		{
			return 0;
		}
		
		switch(topic_payload_set)
		{
			case _eREAD_TOPIC:
			{
				if(_gPub_Buff[increment_len] == Topic_compare[topic_payload_len])
				{
					topic_payload_len++;
					if(Topic_compare[topic_payload_len] == 0)
					{
						for(i=0; _gPub_Buff[increment_len]!=0x0D; i++)
						{
							increment_len++;
							if(increment_len > sizeof(_gPub_Buff)-1)
							{
								return 0;
							}
							len_buf[i] = _gPub_Buff[increment_len];
						}
						len_buf[i] = 0;
						
						switch(strlen((const char*)len_buf))
						{
							case 0:
							{
								return 0;
							}break;
							
							case 1:
							{
								return 0;
							}break;
							
							case 2:
							{
								lenght = (len_buf[0]-0x30);
								if(lenght == 0)
								{
									return 0;
								}
							}break;
							
							case 3:
							{
								lenght = (len_buf[0]-0x30)*10 + (len_buf[1]-0x30);
								if(lenght == 0)
								{
									return 0;
								}
								if(lenght > 50)
								{
									return 0;
								}
							}break;
							
							case 4:
							{
								lenght = (len_buf[0]-0x30)*100 + (len_buf[1]-0x30)*10 + (len_buf[2]-0x30);
								return 0;
							}break;
						}
						
						topic_payload_set = _eREAD_PAYLOAD;
						topic_payload_len = 0;
						
						increment_len++;
						increment_len++;
						
						if(increment_len > sizeof(_gPub_Buff)-1)
						{
							return 0;
						}
						
						for(i=0;i<lenght;i++)
						{
							topic[i] = _gPub_Buff[increment_len++];
							if(increment_len > sizeof(_gPub_Buff)-1)
							{
								return 0;
							}
						}
					}
				}
				else
				{
					topic_payload_len = 0;
				}
			}break;
			
			case _eREAD_PAYLOAD:
			{
				if(topic_payload_set)
				{
					
					if(_gPub_Buff[increment_len] == Pay_load_compare[topic_payload_len])
					{
						topic_payload_len++;
						
						if(Pay_load_compare[topic_payload_len] == 0)
						{
							for(i=0; _gPub_Buff[increment_len]!=0X0D; i++)
							{
								increment_len++;
								if(increment_len > sizeof(_gPub_Buff)-1)
								{
									return 0;
								}
								len_buf[i] = _gPub_Buff[increment_len];
							}
							len_buf[i] = 0;
							
							switch(strlen((const char*)len_buf))
							{
								case 0:
								{
									return 0;
								}break;
								
								case 1:
								{
									return 0;
								}break;
								
								case 2:
								{
									lenght = (len_buf[0]-0x30);
									if(lenght == 0)
									{
										return 0;
									}
								}break;
								
								case 3:
								{
									lenght = (len_buf[0]-0x30)*10 + (len_buf[1]-0x30);
									if(lenght == 0)
									{
										return 0;
									}
								}break;
								
								case 4:
								{
									lenght = (len_buf[0]-0x30)*100 + (len_buf[1]-0x30)*10 + (len_buf[2]-0x30);
									if(lenght > sizeof(_gPub_Buff))
									{
										return 0;
									}
								}break;
							}
							
							topic_payload_set = 0;
							
							increment_len++;
							increment_len++;
							if(increment_len > sizeof(_gPub_Buff)-1)
							{
								return 0;
							}
							
							for(i=0;i<lenght;i++)
							{
								pay_load[i] = _gPub_Buff[increment_len++];
								if(increment_len > sizeof(_gPub_Buff)-1)
								{
									return 0;
								}
							}
						}
					}
					else
					{
						topic_payload_len = 0;
					}
				}
			}break;
		}
	}
	memset((char *)_gRecvd_Data,0,sizeof(_gRecvd_Data));
	memset((char *)_gPub_Buff,0,sizeof(_gPub_Buff));
	sprintf((char *)_gRecvd_Data,"%s",topic);
	sprintf((char *)_gPub_Buff,"%s",pay_load);
	return 1;
}

bool Read_SMS_COMMAND(char* ATcommand, char* expected_answer, char* expected_answer2,unsigned int timeout)
{
	unsigned int x = 0, answer = 0,command_conter;
	char response[100];
	unsigned long previous;
	
	command_conter = 0;
	_kSERIAL_AT_CLEAR();
	
	while(command_conter < 2)
	{
		command_conter++;
		memset(response, '\0', sizeof(response));   // initialize string
		delay(100);
		Clear_Serial_Data();
		
		_kSERIAL_AT.println(ATcommand);

		x = 0;
		previous = millis();

		do
		{
			if(x < (sizeof(response)-1))
			{
				if (_kSERIAL_AT.available() != 0)
				{
					response[x] = _kSERIAL_AT.read();
					x++;
					
					if (strstr(response, expected_answer) != NULL)
					{
						answer = 1;
						command_conter++;
					}
					if (strstr(response, expected_answer2) != NULL)
					{
						memset(response, '\0', sizeof(response));   // initialize string
						x=0;
						command_conter++;
					}
					
				}
			}
			else
			{
				memset(response, '\0', sizeof(response));   // initialize string
				x = 0;
				
			}
		} while ((answer == 0) && ((millis() - previous) < timeout));

		_kDELAY_MS(100);
		
		Clear_Serial_Data();
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		Serial.println(response);
		_kSERIAL_MON_CLEAR();
	}
	sprintf((char *)_gRecvd_Data,"%s",(char *)response);
	return answer;
}

void Display_Update_1(void)
{
	static uint32_t temp_display;
	static unsigned char plot_added = 0,displaying_plot,mode_display,plot_data_display_backup,total_plot,tickcount;
	char i = 0;
	
	
	
	/******************************************* update the range symbol **************************************/
	static unsigned int display_next =0;
	
	if(!Start_Display_In_Interrupt)
	{
		if(_sRuble_Parameters.Display_Update & _ePLEASE_WAIT)
		{
			_sRuble_Parameters.Display_Update &= ~_ePLEASE_WAIT;
			
			lcd.setCursor(0,1);
			lcd.print(F("     PLEASE WAIT    "));
		}
		
		if(_sRuble_Parameters.Display_Update & _eCONNECTING_TO_SERVER)
		{
			_sRuble_Parameters.Display_Update &= ~_eCONNECTING_TO_SERVER;
			lcd.setCursor(0,1);
			lcd.print(F("CONNECTING TO SERVER"));
			
			lcd.setCursor(0,2);
			lcd.print(F("                    "));
			
		}
		
		if(_sRuble_Parameters.Display_Update & _eREGISTERING_TO_SERVER)
		{
			_sRuble_Parameters.Display_Update &= ~_eREGISTERING_TO_SERVER;
			
			lcd.setCursor(0,1);
			lcd.print(F("     REGISTERING    "));
			
			lcd.setCursor(0,2);
			lcd.print(F("                    "));
		}
		
		if(_sRuble_Parameters.Display_Update & _eREGISTERED_TO_SERVER)
		{
			_sRuble_Parameters.Display_Update &=  _eREGISTERED_TO_SERVER;
			lcd.setCursor(0,1);
			lcd.print(F("REGISTERED TO SERVER"));
			
			lcd.setCursor(0,2);
			lcd.print(F("                    "));
		}
		
		if(_sRuble_Parameters.Display_Update & _eSEARCHING_FOR_NETWORK)
		{
			_sRuble_Parameters.Display_Update &= ~_eSEARCHING_FOR_NETWORK;
			
			lcd.setCursor(0,1);
			lcd.print(F("SEARCHING FOR NET..."));
			
			lcd.setCursor(0,2);
			lcd.print(F("                    "));
		}
		
		if(_sRuble_Parameters.Display_Update & _eNETWORK_SELECTED)
		{
			_sRuble_Parameters.Display_Update &= ~_eNETWORK_SELECTED;
			
			lcd.setCursor(0,1);
			lcd.print(F("  NETWORK SELECTED  "));
			
			lcd.setCursor(0,2);
			lcd.print(F("                    "));
		}
		
		if(_sRuble_Parameters.Display_Update & _eCONNECTED_TO_SERVER)
		{
			_sRuble_Parameters.Display_Update &= ~_eCONNECTED_TO_SERVER;
			
			lcd.setCursor(0,1);
			lcd.print(F("CONNECTED TO SERVER "));
			
			lcd.setCursor(0,2);
			lcd.print(F("                    "));
		}
		
		if(_sRuble_Parameters.Display_Update & _eTHANK_YOU_FOR_WAITING)
		{
			_sRuble_Parameters.Display_Update &= ~_eTHANK_YOU_FOR_WAITING;
			
			lcd.setCursor(0,1);
			lcd.print(F("      THANK YOU     "));
			
			lcd.setCursor(0,2);
			lcd.print(F("                    "));
		}
		
		if(_sRuble_Parameters.Display_Update & _eDATA_INITIALISING)
		{
			_sRuble_Parameters.Display_Update &= ~_eDATA_INITIALISING;
			
			lcd.setCursor(1,1);
			lcd.print(F("DATA INITIALISING"));
			
			lcd.setCursor(0,2);
			lcd.print(F("                    "));
		}
		
		temp_display = 0;
	}
	

	
	
	if(Start_Display_In_Interrupt)
	{
		if(_sGSM_Query.Gsm_Module_Test != _kSET)
		{
			if(_sRuble_Parameters.Sim_Check != _eSIM_NOT_INSERTED)
			{
				if(_sRuble_Parameters.Display_Update & _kDISP_NO_RANGE)
				{
					if(!(temp_display & _kDISP_NO_RANGE))
					{
						if((_sRuble_Parameters.lndications & _eRUBL_INDI_SERVER_CONN) == 0)
						{
							_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE);
						}
						else
						{
							_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE_C);
						}
						_kLCD_SPECIAL_CHAR(_kDISP_NO_RANGE,1,0,_eRANGE_X);
					}
					temp_display = 0xfffff1;
				}
				
				else if(_sRuble_Parameters.Display_Update & _kDISP_RANGE_1)
				{
					if(!(temp_display & _kDISP_RANGE_1))
					{
						if((_sRuble_Parameters.lndications & _eRUBL_INDI_SERVER_CONN) == 0)
						{
							_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE);
						}
						else
						{
							_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE_C);
						}
						_kLCD_SPECIAL_CHAR(9,1,0,_eRANGE1);
					}
					temp_display = 0xfffff2;
				}
				
				else if(_sRuble_Parameters.Display_Update & _kDISP_RANGE_2)
				{
					if(!(temp_display & _kDISP_RANGE_2))
					{
						if((_sRuble_Parameters.lndications & _eRUBL_INDI_SERVER_CONN) == 0)
						{
							_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE);
						}
						else
						{
							_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE_C);
						}
						_kLCD_SPECIAL_CHAR(10,1,0,_eRANGE2);
					}
					temp_display = 0xfffff4;
				}
				
				else if(_sRuble_Parameters.Display_Update & _kDISP_RANGE_3)
				{
					if(!(temp_display & _kDISP_RANGE_3))
					{
						if((_sRuble_Parameters.lndications & _eRUBL_INDI_SERVER_CONN) == 0)
						{
							_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE);
						}
						else
						{
							_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE_C);
						}
						_kLCD_SPECIAL_CHAR(2,1,0,_eRANGE3);
					}
					temp_display = 0xfffff8;
				}
			}
			else
			{
				if((_sRuble_Parameters.lndications & _eRUBL_INDI_SERVER_CONN) == 0)
				{
					_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE);
				}
				else
				{
					_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE_C);
				}
				_kLCD_SPECIAL_CHAR(2,1,0,_eNO_SIM_CARD);
				temp_display = 0;
			}
		}
		else
		{
			if((_sRuble_Parameters.lndications & _eRUBL_INDI_SERVER_CONN) == 0)
			{
				_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE);
			}
			else
			{
				_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE_C);
			}
			_kLCD_SPECIAL_CHAR(2,1,0,_eNO_SIM_CARD);
			temp_display = 0;
		}
		
		
		
		
		
		if((_sRuble_Parameters.lndications & _eRUBL_INDI_SERVER_CONN) == 0)
		{
			_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE);
		}
		else
		{
			_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eRANGE_C);
		}
		
		/*************************************************** update the time **************************************************/
		
		if(_sRuble_Parameters.Display_Update & _kTIME_UPDATE)
		{
			_sRuble_Parameters.Display_Update &= ~_kTIME_UPDATE;
			_kLCD_SPECIAL_CHAR(_eRANGE_SYBBOL,0,0,_eTIME_UPDATE);	
		}
		
		
		/************************************* update the plot data *******************************/
		plot_added = 0;
		for(i=0;i<_kMAX_LIRA_SENSOR_NODE;i++)
		{
			if(_sLira_Node_Param[i].Lira_Sensor_Number)
			{
				plot_added++;
			}
		}
		
		if(_sRuble_Parameters.Display_Refresh)     //  tickcount == 0)
		{
			_sRuble_Parameters.Display_Refresh = _kRESET;
			
			tickcount = _kDISPLAY_REFRESH_RATE;
			
			if(!_sRuble_Parameters.Lora_Module_Check)
			{
				_kLCD_SPECIAL_CHAR(4,2,0,_eLORA_MODULE);
			}
			
			
			switch(_sRuble_Parameters.Operation_Mode)
			{
				case _eOPER_MOD_SCHEDULER:
				{
					lcd.setCursor(0,1);
					lcd.print(F("   SCHEDULER MODE   "));
				}break;
				
				case _eOPER_MOD_TIMER:
				{
					lcd.setCursor(0,1);
					lcd.print(F("     TIMER MODE     "));
				}break;
				
				case _eOPER_MOD_MANUAL:
				{
					lcd.setCursor(0,1);
					lcd.print(F("     MANUAL MODE    "));
				}break;
			}
			
			
			if(!plot_added)
			{
				lcd.setCursor(0,2);
				lcd.print(F(" NO PLOT ADDED YET! "));
				
				lcd.setCursor(0,3);
				lcd.print(F("                    "));
			}
			else
			{
				if(_sRuble_Parameters.Irrigating_Plot)
				{
					sprintf((char *)_sRuble_Parameters.Display_Buff," PLOT %02d IS RUNNING ",_sRuble_Parameters.Irrigating_Plot);
					_kLCD_BUFF_WRITE((char *)_sRuble_Parameters.Display_Buff,20,2,0);
					
					lcd.setCursor(0,3);
					lcd.print(F("                    "));
				}
				else
				{
					lcd.setCursor(0,2);
					lcd.print(F(" NO PLOT IS GETTING "));
					
					lcd.setCursor(0,3);
					lcd.print(F("      IRRIGATED     "));
				}
			}
			
			if(_sRuble_Parameters.Pump_Is_Running_Or_Not == _kSET)
			{
				lcd.setCursor(3,0);
				lcd.print(F("   PUMP ON  "));
			}
			else
			{
				lcd.setCursor(3,0);
				lcd.print(F("  PUMP OFF  "));
			}
		}
	}
	
}

