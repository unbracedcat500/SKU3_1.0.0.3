/*
 * CPPFile1.cpp
 *
 * Created: 14-04-2023 16:06:44
 *  Author: Tushar (SIO)
 */ 

// void Faults_Handler(void)
// {
// 	static unsigned char temp_back_up;
// 	static unsigned char faults_check_timer = 0;
// 	static unsigned long int fault_occure = 0;
// 	static unsigned long int fault_status;
// 	
// 	static unsigned char temp_back_up;
	static unsigned char faults_check_timer = 0,fault_occure = 0;
// 	
// 	
// 	if(_sRuble_Parameters.Wireless_Pump_Command != _kRESET)
	{
		return;
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
	
	
	if(!_sRuble_Parameters.By_Pass_CT)
	{
		return;
	}
// 
// 
// 	/************************************************** Read CT *****************************************/
// 	if(_sRuble_Parameters.Read_Ct_Value_Timer)
// 	{
// 		if(fault_occure)
// 		{
// 			fault_occure = _kRESET;
// 			faults_check_timer++;
// 		}
// 		_sRuble_Parameters.Read_Ct_Value_Timer = _kRESET;
// 		_sPump_Parameter.Pump_Run_Current = RUBLE_PUMP_CURRENT.calcIrms(_kCT_AVERAGE_TURNS);
// 	}
// 
// 
// 	/******************************************************* CHECK OVERLOAD **********************************************/
// 	if(_sPump_Parameter.Pump_Run_Current > (_sPump_Parameter.Pump_Nominal_Current) + ((_sPump_Parameter.Pump_Nominal_Current/100) * 32))
// 	{
// 		if(! _sRuble_Parameters.Indication_Of_Faults & _ePUMP_OVERLOAD)
// 		{
// 			fault_occure = _ePUMP_OVERLOAD;
// 			_kSERAIL_MON_WRITE("Pump Overlaoded ");
// 			_kSERIAL_MON_PRINT_NUM(faults_check_timer);
// 			if(faults_check_timer > 3)
// 			{
// 				_sRuble_Parameters.Indication_Of_Faults |= _ePUMP_OVERLOAD;
// 				faults_check_timer = 0;
// 				_kSERAIL_MON_WRITE("Pump Overlaoded");
// 			}
// 			
// 		}
// 	}
// 	else
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults &= ~_ePUMP_OVERLOAD;
// 		if(fault_occure == _ePUMP_OVERLOAD)
// 		{
// 			fault_occure = _kRESET;
// 		}
// 	}
// 
// 
// 	/******************************************************* CHECK DRY RUN **********************************************/
// 	if(_sPump_Parameter.Pump_Run_Current < ((_sPump_Parameter.Pump_Nominal_Current/100) * 58))
// 	{
// 		if(_sPump_Parameter.Pump_Run_Current > ((_sPump_Parameter.Pump_Nominal_Current/100) * 20))
// 		{
// 			if(! _sRuble_Parameters.Indication_Of_Faults & _eDRY_RUN_STATUS)
// 			{
// 				fault_occure = _eDRY_RUN_STATUS;
// 				_kSERAIL_MON_WRITE("Pump Dry Run ");
// 				_kSERIAL_MON_PRINT_NUM(faults_check_timer);
// 				if(faults_check_timer > 3)
// 				{
// 					_sRuble_Parameters.Indication_Of_Faults |= _eDRY_RUN_STATUS;
// 					faults_check_timer = 0;
// 					_kSERAIL_MON_WRITE("Pump Dry Run ");
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults &= ~_eDRY_RUN_STATUS;
// 		if(fault_occure == _eDRY_RUN_STATUS)
// 		{
// 			fault_occure = _kRESET;
// 		}
// 	}
// 	
// 
// 	/******************************************************* Read R PHASE **********************************************/
// 	if(digitalRead(_kR_PHASE_INPUT))
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults |= _eR_PHASE_STATUS;
// 	}
// 	else
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults &= ~_eR_PHASE_STATUS;
// 	}
// 
// 
// 	/******************************************************* Read Y PHASE **********************************************/
// 	if(digitalRead(_kY_PHASE_INPUT))
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults |= _eY_PHASE_STATUS;
// 	}
// 	else
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults &= ~_eY_PHASE_STATUS;
// 	}
// 
// 
// 	/******************************************************* Read B PHASE **********************************************/
// 	if(digitalRead(_kB_PHASE_INPUT))
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults |= _eB_PHASE_STATUS;
// 	}
// 	else
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults &= ~_eB_PHASE_STATUS;
// 	}
// 
// 
// 	/******************************************************* CHECK LINE FAULT *******************************************/
// 	if( _sRuble_Parameters.Indication_Of_Faults & _eR_PHASE_STATUS ||
// 		_sRuble_Parameters.Indication_Of_Faults & _eY_PHASE_STATUS ||
// 		_sRuble_Parameters.Indication_Of_Faults & _eB_PHASE_STATUS )
// 	{
// 		if(! _sRuble_Parameters.Indication_Of_Faults & _eLINE_FAULT_STATUS)
// 		{
// 			fault_occure = _eLINE_FAULT_STATUS;
// 			_kSERAIL_MON_WRITE("Pump line fault ");
// 			_kSERIAL_MON_PRINT_NUM(faults_check_timer);
// 			if(faults_check_timer > 3)
// 			{
// 				_sRuble_Parameters.Indication_Of_Faults |= _eLINE_FAULT_STATUS;
// 				faults_check_timer = 0;
// 				_kSERAIL_MON_WRITE("Pump line fault ");
// 			}
// 		}
// 	}
// 	else
// 	{
// 		_sRuble_Parameters.Indication_Of_Faults &= ~_eLINE_FAULT_STATUS;
// 		if(fault_occure == _eLINE_FAULT_STATUS)
// 		{
// 			fault_occure = _kRESET;
// 		}
// 	}
// 
// 
// 	/********************************************** Pump off if any fault ******************************************/
// 	//Pump_Off_For_Faults(1);
// 
// 	
// 	/********************************************** check pump on condition ******************************************/
// 	if(_sPump_Parameter.Pump_Run_Current < ((_sPump_Parameter.Pump_Nominal_Current/100) * 15))
// 	{
// 		if(_sRuble_Parameters.Indication_Of_Faults & _ePUMP_STATUS)
// 		{
// 			_sRuble_Parameters.Indication_Of_Faults &= ~_ePUMP_STATUS;
// 			fault_occure = _kRESET;
// 			faults_check_timer = _kRESET;
// 		}
// 	}
// 
// 	
// 	/************************************************** check pump off condition *************************************/
// 	if(_sPump_Parameter.Pump_Run_Current > ((_sPump_Parameter.Pump_Nominal_Current/100) * 18))
// 	{
// 		if(!_sRuble_Parameters.Indication_Of_Faults & _ePUMP_STATUS)
// 		{
// 			_sRuble_Parameters.Indication_Of_Faults |= _ePUMP_STATUS;
// 			fault_occure = _kRESET;
// 			faults_check_timer = _kRESET;
// 		}
// 	}
// 
// 
// 	/******************************************** CHECK IF ANY NEW STATUS UPDATED **********************************/
// 	if(fault_status != _sRuble_Parameters.Indication_Of_Faults)
// 	{
// 		fault_status = _sRuble_Parameters.Indication_Of_Faults;
// 		_sRuble_Parameters.Publish_Topic |= _ePUB_RUBLE_FAULTS;
// 		
// 		
// 			if(_sRuble_Parameters.Indication_Of_Faults & _ePUMP_OVERLOAD)
// 			{
// 				_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_OVER_LOAD;
// 			}
// 			if(_sRuble_Parameters.Indication_Of_Faults & _eDRY_RUN_STATUS)
// 			{
// 				_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_DRY_RUN;
// 			}
// 			if(_sRuble_Parameters.Indication_Of_Faults & _eLINE_FAULT_STATUS)
// 			{
// 				_sPump_Parameter.Irrigation_Status = _eIRRIG_STAT_LINE_FAULT;
// 			}
// 	}
// 
// }


// void Update_Indication(void)
// {
// 	
// 	if(_sRuble_Parameters.Indication_Of_Faults & _eR_PHASE_STATUS)
// 	{
// 		digitalWrite(_kR_PHASE_STATUS, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(_kR_PHASE_STATUS, LOW);
// 	}
// 
// 	if(_sRuble_Parameters.Indication_Of_Faults & _eY_PHASE_STATUS)
// 	{
// 		digitalWrite(_kY_PHASE_STATUS, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(_kY_PHASE_STATUS, LOW);
// 	}
// 
// 	if(_sRuble_Parameters.Indication_Of_Faults & _eB_PHASE_STATUS)
// 	{
// 		digitalWrite(_kB_PHASE_STATUS, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(_kB_PHASE_STATUS, LOW);
// 	}
// 
// 	if(_sRuble_Parameters.Indication_Of_Faults & _eDRY_RUN_STATUS)
// 	{
// 		digitalWrite(_kDRY_RUN_STATUS, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(_kDRY_RUN_STATUS, LOW);
// 	}
// 
// 	if(_sRuble_Parameters.Indication_Of_Faults & _eLINE_FAULT_STATUS)
// 	{
// 		digitalWrite(_kLINE_FAULT_STATUS, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(_kLINE_FAULT_STATUS, LOW);
// 	}
// 
// 	if(_sRuble_Parameters.Indication_Of_Faults & _ePUMP_OVERLOAD)
// 	{
// 		digitalWrite(_kLINE_FAULT_STATUS, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(_kLINE_FAULT_STATUS, LOW);
// 	}
// 
// 	if(_sRuble_Parameters.Indication_Of_Faults & _ePUMP_OVERLOAD)
// 	{
// 		digitalWrite(_kLINE_FAULT_STATUS, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(_kLINE_FAULT_STATUS, LOW);
// 	}
// 
// 	if(_sRuble_Parameters.Indication_Of_Faults & _ePUMP_STATUS)
// 	{
// 		digitalWrite(_kPUMP_STATUS, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(_kPUMP_STATUS, LOW);
// 	}
// 	
// }