/*
 * CPPFile1.cpp
 *
 * Created: 14-03-2022 20:40:44
 *  Author: Tushar (SIO)
 */ 

#include "Schedules.h"
#include "Initialize.h"

volatile Schedules _sSchedules;

unsigned char Return_Previous_Day_Date(void)
{
	unsigned char date,month,year;
	
	date  = _sRuble_Parameters.Update_Local_Date[0];
	month = _sRuble_Parameters.Update_Local_Date[1];
	year  = _sRuble_Parameters.Update_Local_Date[2];
	
	switch(month)
	{
		case _eJAN:
		case _eFEB:
		case _eAPR:
		case _eJUN:
		case _eAUG:
		case _eSEP:
		case _eNOV:
		{
			if(date == 1)
			{
				switch(month)
				{
					case _eJAN:
					{
						_sSchedules.Started_Month = _eDEC;
					}break;
					case _eFEB:
					{
						_sSchedules.Started_Month = _eJAN;
					}break;
					case _eAPR:
					{
						_sSchedules.Started_Month = _eMARCH;
					}break;
					case _eJUN:
					{
						_sSchedules.Started_Month = _eMAY;
					}break;
					case _eAUG:
					{
						_sSchedules.Started_Month = _eJUL;
					}break;
					case _eSEP:
					{
						_sSchedules.Started_Month = _eAUG;
					}break;
					case _eNOV:
					{
						_sSchedules.Started_Month = _eOCT;
					}break;
				}
				
				return 31;	 
			}
			else
			{
				_sSchedules.Started_Month = month;
				return date - 1;
			}
		}break;
		
		case _eMAY:
		case _eJUL:
		case _eOCT:
		case _eDEC:
		{
			if(date == 1)
			{
				switch(month)
				{
					case _eMAY:
					{
						_sSchedules.Started_Month = _eAPR;
					}break;
					case _eJUL:
					{
						_sSchedules.Started_Month = _eJUN;
					}break;
					case _eOCT:
					{
						_sSchedules.Started_Month = _eSEP;
					}break;
					case _eDEC:
					{
						_sSchedules.Started_Month = _eNOV;
					}break;
				}
				return 30;
			}
			else
			{
				_sSchedules.Started_Month = month;
				return date - 1;
			}
		}break;
		
		case _eMARCH:
		{
			if(date == 1)
			{
				if(year%4)
				{
					_sSchedules.Started_Month = _eFEB;
					return 28;
				}
				else
				{
					_sSchedules.Started_Month = _eFEB;
					return 29;
				}
				
			}
			else
			{
				_sSchedules.Started_Month = month;
				return date - 1;
			}
		}break;	
		
	}
}

unsigned char Return_Calculated_Previous_Day_Date_For_Edit(unsigned char days_in_bet_schedule)
{
	int make_date;
	unsigned char return_date, return_month;
	unsigned char year;
	year  = _sRuble_Parameters.Update_Local_Date[2];
	
	if(_sSchedules.Irrigating_Date == 0)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("irrigating date upadte 1///////////////////////////////////////////");
			_kSERIAL_MON_CLEAR();
		}
		_sSchedules.Irrigating_Date = _sRuble_Parameters.Update_Local_Date[0];
		_sSchedules.Irrigating_Month = _sRuble_Parameters.Update_Local_Date[1];
		make_date = _sSchedules.Irrigating_Date - (_sSchedules.Days_In_Between_Schedules+1);
	}
	else
	{
		make_date = _sRuble_Parameters.Update_Local_Date[0] - (_sSchedules.Days_In_Between_Schedules+1);
		if(_sRuble_Parameters.Update_Local_Date[1] == 0)
		{
			_sSchedules.Irrigating_Month = _sRuble_Parameters.Update_Local_Date[1];
		}
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("sc make date");
		_kSERIAL_MON_PRINT_NUM(make_date);
		_kSERIAL_MON_CLEAR();
	}
	
	if(make_date > 0)
	{
		return_date = make_date;
		return_month = _sSchedules.Irrigating_Month;
	}
	else if(make_date == 0)
	{
		switch(_sSchedules.Irrigating_Month)
		{
			case _eJAN:
			{
				return_date = 31;
				return_month = _eDEC;
			}break;
			case _eFEB:
			{
				return_date = 31;
				return_month = _eJAN;
			}break;
			case _eMARCH:
			{
				if(year%4)
				{
					return_date = 28;
				}
				else
				{
					return_date = 29;
				}
				
				return_month = _eFEB;
			}break;
			case _eAPR:
			{
				return_date = 31;
				return_month = _eMARCH;
			}break;
			case _eMAY:
			{
				return_date = 30;
				return_month = _eAPR;
			}break;
			case _eJUN:
			{
				return_date = 31;
				return_month = _eMAY;
			}break;
			case _eJUL:
			{
				return_date = 30;
				return_month = _eJUN;
			}break;
			case _eAUG:
			{
				return_date = 31;
				return_month = _eJUL;
			}break;
			case _eSEP:
			{
				return_date = 31;
				return_month = _eAUG;
			}break;
			case _eOCT:
			{
				return_date = 30;
				return_month = _eSEP;
			}break;
			case _eNOV:
			{
				return_date = 31;
				return_month = _eOCT;
			}break;
			case _eDEC:
			{
				return_date = 30;
				return_month = _eNOV;
			}break;
			
		}
	}
	
	else if(make_date < 0)
	{
		switch(_sSchedules.Irrigating_Month)
		{
			case _eJAN:
			{
				return_date = 31 +  make_date;
				return_month = _eDEC;
			}break;
			case _eFEB:
			{
				return_date = 31+  make_date;
				return_month = _eJAN;
			}break;
			case _eMARCH:
			{
				if(year%4)
				{
					return_date = 28+  make_date;
				}
				else
				{
					return_date = 29+  make_date;
				}
				
				return_month = _eFEB;
			}break;
			case _eAPR:
			{
				return_date = 31+  make_date;
				return_month = _eMARCH;
			}break;
			case _eMAY:
			{
				return_date = 30+  make_date;
				return_month = _eAPR;
			}break;
			case _eJUN:
			{
				return_date = 31+  make_date;
				return_month = _eMAY;
			}break;
			case _eJUL:
			{
				return_date = 30+  make_date;
				return_month = _eJUN;
			}break;
			case _eAUG:
			{
				return_date = 31+  make_date;
				return_month = _eJUL;
			}break;
			case _eSEP:
			{
				return_date = 31+  make_date;
				return_month = _eAUG;
			}break;
			case _eOCT:
			{
				return_date = 30+  make_date;
				return_month = _eSEP;
			}break;
			case _eNOV:
			{
				return_date = 31+  make_date;
				return_month = _eOCT;
			}break;
			case _eDEC:
			{
				return_date = 30+  make_date;
				return_month = _eNOV;
			}break;
			
		}
	}
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("sc _sSchedules.Temp_date");
		_kSERIAL_MON_CLEAR();
	}
	
	_sSchedules.Temp_date  = return_date;
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERIAL_MON_PRINT_NUM(_sSchedules.Temp_date);
		_kSERAIL_MON_WRITE("sc _sSchedules.Temp_Month");
		_kSERIAL_MON_CLEAR();
	}
	_sSchedules.Temp_Month = return_month;
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERIAL_MON_PRINT_NUM(_sSchedules.Temp_Month);
		_kSERIAL_MON_CLEAR();
	}
}

unsigned char Return_Calculated_Previous_Day_Date(unsigned char days_in_bet_schedule)
{
	int make_date;
	unsigned char return_date, return_month;
	unsigned char year;
	year  = _sRuble_Parameters.Update_Local_Date[2];
	
	if(_sSchedules.Irrigating_Date == 0)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("irrigating date upadte 1///////////////////////////////////////////");
			_kSERIAL_MON_CLEAR();
		}
		_sSchedules.Irrigating_Date = _sRuble_Parameters.Update_Local_Date[0];
		_sSchedules.Irrigating_Month = _sRuble_Parameters.Update_Local_Date[1];
		make_date = _sSchedules.Irrigating_Date - (_sSchedules.Days_In_Between_Schedules+1);
	}
	else
	{
		/*make_date = _sSchedules.Irrigating_Date - (_sSchedules.Days_In_Between_Schedules+1);
		if(_sSchedules.Irrigating_Month == 0)
		{
			_sSchedules.Irrigating_Month = _sRuble_Parameters.Update_Local_Date[1];
		}*/
		
		make_date = _sRuble_Parameters.Update_Local_Date[0] - (_sSchedules.Days_In_Between_Schedules+1);
		if(_sSchedules.Irrigating_Month == 0)
		{
			_sSchedules.Irrigating_Month = _sRuble_Parameters.Update_Local_Date[1];
		}
		_sSchedules.Irrigating_Month = _sRuble_Parameters.Update_Local_Date[1];
	}
	
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("sc make date");
		_kSERIAL_MON_PRINT_NUM(make_date);
		_kSERIAL_MON_CLEAR();
	}
	
	
	if(make_date > 0)
	{
		return_date = make_date;
		return_month = _sSchedules.Irrigating_Month;
	}
	else if(make_date == 0)
	{
		switch(_sSchedules.Irrigating_Month)
		{
			case _eJAN:
			{
				return_date = 31;
				return_month = _eDEC;
			}break;
			case _eFEB:
			{
				return_date = 31;
				return_month = _eJAN;
			}break;
			case _eMARCH:
			{
				if(year%4)
				{
					return_date = 28;
				}
				else
				{
					return_date = 29;
				}
				
				return_month = _eFEB;
			}break;
			case _eAPR:
			{
				return_date = 31;
				return_month = _eMARCH;
			}break;
			case _eMAY:
			{
				return_date = 30;
				return_month = _eAPR;
			}break;
			case _eJUN:
			{
				return_date = 31;
				return_month = _eMAY;
			}break;
			case _eJUL:
			{
				return_date = 30;
				return_month = _eJUN;
			}break;
			case _eAUG:
			{
				return_date = 31;
				return_month = _eJUL;
			}break;
			case _eSEP:
			{
				return_date = 31;
				return_month = _eAUG;
			}break;
			case _eOCT:
			{
				return_date = 30;
				return_month = _eSEP;
			}break;
			case _eNOV:
			{
				return_date = 31;
				return_month = _eOCT;
			}break;
			case _eDEC:
			{
				return_date = 30;
				return_month = _eNOV;
			}break;
			
		}
	}
	
	else if(make_date < 0)
	{
		switch(_sSchedules.Irrigating_Month)
		{
			case _eJAN:
			{
				return_date = 31 +  make_date;
				return_month = _eDEC;
			}break;
			case _eFEB:
			{
				return_date = 31+  make_date;
				return_month = _eJAN;
			}break;
			case _eMARCH:
			{
				if(year%4)
				{
					return_date = 28+  make_date;
				}
				else
				{
					return_date = 29+  make_date;
				}
				
				return_month = _eFEB;
			}break;
			case _eAPR:
			{
				return_date = 31+  make_date;
				return_month = _eMARCH;
			}break;
			case _eMAY:
			{
				return_date = 30+  make_date;
				return_month = _eAPR;
			}break;
			case _eJUN:
			{
				return_date = 31+  make_date;
				return_month = _eMAY;
			}break;
			case _eJUL:
			{
				return_date = 30+  make_date;
				return_month = _eJUN;
			}break;
			case _eAUG:
			{
				return_date = 31+  make_date;
				return_month = _eJUL;
			}break;
			case _eSEP:
			{
				return_date = 31+  make_date;
				return_month = _eAUG;
			}break;
			case _eOCT:
			{
				return_date = 30+  make_date;
				return_month = _eSEP;
			}break;
			case _eNOV:
			{
				return_date = 31+  make_date;
				return_month = _eOCT;
			}break;
			case _eDEC:
			{
				return_date = 30+  make_date;
				return_month = _eNOV;
			}break;
			
		}
	}
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("sc _sSchedules.Temp_date");
		_kSERIAL_MON_CLEAR();
	}
	_sSchedules.Temp_date  = return_date;
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERIAL_MON_PRINT_NUM(_sSchedules.Temp_date);
		_kSERAIL_MON_WRITE("sc _sSchedules.Temp_Month");
		_kSERIAL_MON_CLEAR();
	}
	_sSchedules.Temp_Month = return_month;
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERIAL_MON_PRINT_NUM(_sSchedules.Temp_Month);
		_kSERIAL_MON_CLEAR();
	}
}

unsigned char Day_In_Bet_Schedulling(unsigned char Days_In_Between_Schedules)
{
	unsigned char date,month,year;
	
	unsigned char calculate_days;
	
	date  = _sRuble_Parameters.Update_Local_Date[0];
	month = _sRuble_Parameters.Update_Local_Date[1];
	year  = _sRuble_Parameters.Update_Local_Date[2];
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("date : ");
		_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Update_Local_Date[0]);
		_kSERIAL_MON_CLEAR();
		_kSERAIL_MON_WRITE("month : ");
		_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Update_Local_Date[1]);
		_kSERIAL_MON_CLEAR();
		_kSERAIL_MON_WRITE("year : ");
		_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Update_Local_Date[2]);
		_kSERIAL_MON_CLEAR();
		_kSERAIL_MON_WRITE("irrigating date : ");
		_kSERIAL_MON_PRINT_NUM(_sSchedules.Irrigating_Date);
		_kSERIAL_MON_CLEAR();
		_kSERAIL_MON_WRITE("irrigating month : ");
		_kSERIAL_MON_PRINT_NUM(_sSchedules.Irrigating_Month);
		_kSERIAL_MON_CLEAR();
	}
	
	Days_In_Between_Schedules = Days_In_Between_Schedules + 1;
		
		if((_sSchedules.Irrigating_Date + Days_In_Between_Schedules) <= date && _sSchedules.Irrigating_Month == month)
		{
			//printf("running 0.1");
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("day in btw 1");
				_kSERIAL_MON_CLEAR();
			}
			return 2;
		}
		else
		{
			if(_sSchedules.Irrigating_Month != month)
			{
				switch(_sSchedules.Irrigating_Month)
				{
					case _eDEC: // current month is jan
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eJAN)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}break;
					
					
					case _eJAN:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eFEB)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}break;
					
					case _eMARCH:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eAPR)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}break;
					
					case _eMAY:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eJUN)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}break;
					
					case _eJUL:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eAUG)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}break;
					
					case _eAUG:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eSEP)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}break;
					
					case _eOCT:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eNOV)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}break;
					
					
					case _eFEB:   // current month mar
					{
						if(year%4)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 4");
								_kSERIAL_MON_CLEAR();
							}
							calculate_days = 28 - _sSchedules.Irrigating_Date;
							
							if(month == _eMARCH)
							{
								if(Days_In_Between_Schedules <= (date + calculate_days))
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("day in btw 5");
										_kSERIAL_MON_CLEAR();
									}
									//printf("running 2.1");
									return 2;
								}
							}
						}
						else
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 6");
								_kSERIAL_MON_CLEAR();
							}
							calculate_days = 29 - _sSchedules.Irrigating_Date;
							
							if(month == _eMARCH)
							{
								if(Days_In_Between_Schedules <= (date + calculate_days))
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("day in btw 7");
										_kSERIAL_MON_CLEAR();
									}
									//printf("running 3.1");
									return 2;
								}
							}
							else
							{
								return 2;
							}
						}
					}break;
					
					case _eAPR:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 8");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 30 - _sSchedules.Irrigating_Date;
						
						if(month == _eMAY)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 9");
									_kSERIAL_MON_CLEAR();
								}
								//printf("running 4.1");
								return 2;
							}
						}
					}break;
					
					case _eJUN:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 8");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 30 - _sSchedules.Irrigating_Date;
						
						if(month == _eJUL)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 9");
									_kSERIAL_MON_CLEAR();
								}
								//printf("running 4.1");
								return 2;
							}
						}
					}break;
					
					case _eSEP:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 8");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 30 - _sSchedules.Irrigating_Date;
						
						if(month == _eOCT)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 9");
									_kSERIAL_MON_CLEAR();
								}
								//printf("running 4.1");
								return 2;
							}
						}
					}break;
					
					case _eNOV:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 8");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 30 - _sSchedules.Irrigating_Date;
						
						if(month == _eDEC)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 9");
									_kSERIAL_MON_CLEAR();
								}
								//printf("running 4.1");
								return 2;
							}
						}
					}break;
				}
				//printf("deny 3");
				//return 0;
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("day in btw 10");
					_kSERIAL_MON_CLEAR();
				}
			}
			else
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("day in btw 11");
					_kSERIAL_MON_CLEAR();
				}
				//printf("deny 4");
				//return 0;
			}
			
		}
		
		//printf("\n");
		
		Days_In_Between_Schedules = Days_In_Between_Schedules - 1;
		
		if((_sSchedules.Irrigating_Date + Days_In_Between_Schedules) <= date && _sSchedules.Irrigating_Month == month)
		{
			//printf("running 0.1");
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("day in btw 1");
				_kSERIAL_MON_CLEAR();
			}
			return 1;
		}
		else
		{
			if(_sSchedules.Irrigating_Month != month)
			{
				switch(_sSchedules.Irrigating_Month)
				{
					case _eDEC: // current month is jan
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eJAN)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					
					case _eJAN:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eFEB)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					case _eMARCH:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eAPR)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					case _eMAY:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eJUN)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					case _eJUL:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eAUG)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					case _eAUG:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eSEP)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					case _eOCT:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 2");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 31 - _sSchedules.Irrigating_Date;
						
						if(month == _eNOV)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 3");
									_kSERIAL_MON_CLEAR();
								}
								
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					
					case _eFEB:   // current month mar
					{
						if(year%4)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 4");
								_kSERIAL_MON_CLEAR();
							}
							calculate_days = 28 - _sSchedules.Irrigating_Date;
							
							if(month == _eMARCH)
							{
								if(Days_In_Between_Schedules <= (date + calculate_days))
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("day in btw 5");
										_kSERIAL_MON_CLEAR();
									}
									//printf("running 2.1");
									return 1;
								}
							}
							else
							{
								return 1;
							}
						}
						else
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 6");
								_kSERIAL_MON_CLEAR();
							}
							calculate_days = 29 - _sSchedules.Irrigating_Date;
							
							if(month == _eMARCH)
							{
								if(Days_In_Between_Schedules <= (date + calculate_days))
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("day in btw 7");
										_kSERIAL_MON_CLEAR();
									}
									//printf("running 3.1");
									return 1;
								}
							}
							else
							{
								return 1;
							}
						}
					}break;
					
					case _eAPR:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 8");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 30 - _sSchedules.Irrigating_Date;
						
						if(month == _eMAY)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 9");
									_kSERIAL_MON_CLEAR();
								}
								//printf("running 4.1");
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					case _eJUN:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 8");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 30 - _sSchedules.Irrigating_Date;
						
						if(month == _eJUL)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 9");
									_kSERIAL_MON_CLEAR();
								}
								//printf("running 4.1");
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					case _eSEP:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 8");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 30 - _sSchedules.Irrigating_Date;
						
						if(month == _eOCT)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 9");
									_kSERIAL_MON_CLEAR();
								}
								//printf("running 4.1");
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
					
					case _eNOV:
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 8");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 30 - _sSchedules.Irrigating_Date;
						
						if(month == _eDEC)
						{
							if( Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 9");
									_kSERIAL_MON_CLEAR();
								}
								//printf("running 4.1");
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}break;
				}
				//printf("deny 3");
				//return 0;
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("day in btw 10");
					_kSERIAL_MON_CLEAR();
				}
			}
			else
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("day in btw 11");
					_kSERIAL_MON_CLEAR();
				}
				//printf("deny 4");
				return 0;
			}
			
		}
}

unsigned char Day_In_Bet_Schedulling(unsigned char Days_In_Between_Schedules, unsigned char last_schedule_date,
unsigned char last_schedule_month)
{
	unsigned char date,month,year;
	
	unsigned char calculate_days;
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("Days in between : ");
		_kSERIAL_MON_PRINT_NUM(Days_In_Between_Schedules);
	}
	
	
	year  = _sRuble_Parameters.Update_Local_Date[2];
	
	if(_sSchedules.Started_Date != 0)
	{
		if(_sSchedules.Which_Sheduler_Is_Running == 1)
		{
			date  = _sSchedules.Started_Date;
			month = _sSchedules.Started_Month;
		}
		else
		{
			date  = _sSchedules.First_Schedule_Irrigated_Date;
			month = _sSchedules.First_Schedule_Irrigated_Month;
		}
		
	}
	else
	{
		date  = _sRuble_Parameters.Update_Local_Date[0];
		month = _sRuble_Parameters.Update_Local_Date[1];
	}
	
	
	Days_In_Between_Schedules = Days_In_Between_Schedules + 1;
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("date : ");
		_kSERIAL_MON_PRINT_NUM(date);
		_kSERIAL_MON_CLEAR();
	
		_kSERAIL_MON_WRITE("month : ");
		_kSERIAL_MON_PRINT_NUM(month);
		_kSERIAL_MON_CLEAR();

		_kSERAIL_MON_WRITE("year : ");
		_kSERIAL_MON_PRINT_NUM(_sRuble_Parameters.Update_Local_Date[2]);
		_kSERIAL_MON_CLEAR();
	
		_kSERAIL_MON_WRITE("last date: ");
		_kSERIAL_MON_PRINT_NUM(last_schedule_date);
		_kSERIAL_MON_CLEAR();
	
		_kSERAIL_MON_WRITE("last month : ");
		_kSERIAL_MON_PRINT_NUM(last_schedule_month);
		_kSERIAL_MON_CLEAR();
	
		_kSERAIL_MON_WRITE("day in btw started");
		_kSERIAL_MON_CLEAR();
		
		_kSERAIL_MON_WRITE("No of days in bet : ");
		_kSERIAL_MON_PRINT_NUM( Days_In_Between_Schedules);
		_kSERIAL_MON_CLEAR();
	}
	
	
	
	
	
	if((last_schedule_date + Days_In_Between_Schedules) <= date && last_schedule_month == month)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("day in btw 1");
			_kSERIAL_MON_CLEAR();
		}
		
		return 2;
	}
	else
	{
		if(last_schedule_month != month)
		{
			switch(last_schedule_month)
			{
				case _eDEC: // current month is jan
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eJAN)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				case _eJAN:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eFEB)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				case _eMARCH:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eAPR)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				case _eMAY:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					if(month == _eJUN)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days) )
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				case _eJUL:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eAUG)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days) )
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				case _eAUG:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eSEP)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				case _eOCT:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					if(month == _eNOV)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				
				case _eFEB:   // current month mar
				{
					if(year%4)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 4");
							_kSERIAL_MON_CLEAR();
						}
						
						calculate_days = 28 - last_schedule_date;
						
						if(month == _eMARCH)
						{
							if(Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 5");
									_kSERIAL_MON_CLEAR();
								}
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 6");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 29 - last_schedule_date;
						if(month == _eMARCH)
						{
							if(Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 7");
									_kSERIAL_MON_CLEAR();
								}
								return 2;
							}
						}
						else
						{
							return 2;
						}
					}
				}break;
				
				case _eAPR:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 8");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 30 - last_schedule_date;
					
					if(month == _eMAY)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 9");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				case _eJUN:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 8");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 30 - last_schedule_date;
					
					if(month == _eJUL)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 9");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				
				case _eSEP:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 8");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 30 - last_schedule_date;
					if(month == _eOCT)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 9");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
				case _eNOV:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 8");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 30 - last_schedule_date;
					
					if( month == _eDEC)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 9");
								_kSERIAL_MON_CLEAR();
							}
							return 2;
						}
					}
					else
					{
						return 2;
					}
				}break;
			}
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("day in btw 10");
				_kSERIAL_MON_CLEAR();
			}
		}
		else
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("day in btw 11");
				_kSERIAL_MON_CLEAR();
			}
		}
		
	}
	
	
	Days_In_Between_Schedules = Days_In_Between_Schedules - 1;
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("No of days in bet 1: ");
		_kSERIAL_MON_PRINT_NUM( Days_In_Between_Schedules);
		_kSERIAL_MON_CLEAR();
	}
	
	
	if((last_schedule_date + Days_In_Between_Schedules) <= date && last_schedule_month == month)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("day in btw 1.1");
			_kSERIAL_MON_CLEAR();
		}
		return 1;
	}
	else
	{
		if(last_schedule_month != month)
		{
			switch(last_schedule_month)
			{
				case _eDEC: // current month is jan
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eJAN)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				case _eJAN:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eFEB)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				case _eMARCH:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eAPR)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				case _eMAY:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					if(month == _eJUN)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days) )
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				case _eJUL:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eAUG)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days) )
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				case _eAUG:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					
					if(month == _eSEP)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				case _eOCT:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 2.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 31 - last_schedule_date;
					if(month == _eNOV)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 3.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				
				case _eFEB:   // current month mar
				{
					if(year%4)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 4.1");
							_kSERIAL_MON_CLEAR();
						}
						
						calculate_days = 28 - last_schedule_date;
						
						if(month == _eMARCH)
						{
							if(Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 5.1");
									_kSERIAL_MON_CLEAR();
								}
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("day in btw 6.1");
							_kSERIAL_MON_CLEAR();
						}
						calculate_days = 29 - last_schedule_date;
						if(month == _eMARCH)
						{
							if(Days_In_Between_Schedules <= (date + calculate_days))
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("day in btw 7.1");
									_kSERIAL_MON_CLEAR();
								}
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}
				}break;
				
				case _eAPR:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 8.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 30 - last_schedule_date;
					
					if(month == _eMAY)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 9.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				case _eJUN:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 8");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 30 - last_schedule_date;
					
					if(month == _eJUL)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 9.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				
				case _eSEP:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 8");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 30 - last_schedule_date;
					if(month == _eOCT)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 9.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
				case _eNOV:
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("day in btw 8.1");
						_kSERIAL_MON_CLEAR();
					}
					calculate_days = 30 - last_schedule_date;
					
					if( month == _eDEC)
					{
						if( Days_In_Between_Schedules <= (date + calculate_days))
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("day in btw 9.1");
								_kSERIAL_MON_CLEAR();
							}
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}break;
			}
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("day in btw 10.1");
				_kSERIAL_MON_CLEAR();
			}
		}
		else
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("day in btw 11.1");
				_kSERIAL_MON_CLEAR();
			}
			return 0;
		}
		
	}
	
}		
	
void Schedules_Monitoring(void)
{
	/* check which date's scheduler is running */
	unsigned char check_true_condition;
	unsigned char check_new_true_condition;
	unsigned char Check_for_no_repeat;
	static unsigned char irrigation_Start_Set = _kRESET,flush_running_status=_kRESET;
	static unsigned char mode_change = 0;
	
	_sSchedules.Schedule_Pause_Resume = _kRESET;
	_sSchedules.Based_On_Trigger_Time = _sSchedules.Based_On_Trigger_Time;
	
	switch(_sSchedules.Based_On_Trigger_Time)
	{
		case _eBASED_ON_ELECTRICITY:
		case _eBASED_ON_TRIGGER_TIME:
		{
			if(_sSchedules.Schedule_Enable == _kON && _sSchedules.New_Schedules_Occur == _kRESET
			&& _sSchedules.Schedule_Pause_Resume != _kSET && 
			_sRuble_Parameters.Update_Local_Date[2] != 70)
			{
				if(_sRuble_Parameters.Operation_Mode == _eOPER_MOD_SCHEDULER)
				{
					mode_change = _kSET;
					if(flush_running_status == _kSET)
					{
						if(_sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation  !=_kSET
						&& _sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status   !=_kSET)
						{
							flush_running_status = _kRESET;
							_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
						}
						else
						{
							
						}
					}
					
					if(irrigation_Start_Set == _kSET)
					{
						irrigation_Start_Set = _kRESET;
						if(_sRuble_Parameters.Last_Irrigating_Schedule != _sRuble_Parameters.Irrigating_Plot)
						{
							if(_sRuble_Parameters.Last_Irrigating_Schedule != _kRESET)
							{
								Valve_Grouping_Handler_For_wireless_Pump(_sRuble_Parameters.Last_Irrigating_Schedule, _kOFF);
								_sRuble_Parameters.Last_Irrigating_Schedule = _kRESET;
							}
						}
					}
					
					//_kSERAIL_MON_WRITE("sc enter 0.1");   //1/05
					if(_sSchedules.Scheduler_Retry_Timer == _kRESET)   /* if there is no wait timer to start the schedule again */
					{
						//_kSERAIL_MON_WRITE("sc enter 0.2");     //1/05
						flush_running_status = _kRESET;
						if(_sSchedules.Shedule_Run_Status_Is_Triggered == _kSET && flush_running_status == _kRESET)
						{
													
							/* check todays date with previous date */		
							if(1)
							{
								check_new_true_condition =
								Day_In_Bet_Schedulling(_sSchedules.Days_In_Between_Schedules,_sSchedules.Scheduler_Already_Run[_sSchedules.Which_Sheduler_Is_Running-1],
								_sSchedules.Scheduler_Already_Run_Month[_sSchedules.Which_Sheduler_Is_Running-1]);
								
								if(check_new_true_condition == 2)
								{
									check_new_true_condition = _kSET;
								}
								else if(check_new_true_condition == 1)
								{
									check_new_true_condition = _kSET;
								}
								else
								{
									check_new_true_condition = _kRESET;
								}
							}
							
							if((check_new_true_condition == _kSET)
							&& _sSchedules.Schedule_Status[_sSchedules.Which_Sheduler_Is_Running-1] == _kSET
							&& (_sLira_Node_Param[_sSchedules.Schedule_Num_List[_sSchedules.Which_Sheduler_Is_Running-1]-1].Lira_Sensor_Number !=0 && 
							_sLira_Node_Param[_sSchedules.Schedule_Num_List[_sSchedules.Which_Sheduler_Is_Running-1]-1].Lira_Sensor_Number <= _kMAX_LIRA_SENSOR_NODE ))
							{
								Check_for_no_repeat = Do_Not_Repeat_Schedule();	
								//_kSERAIL_MON_WRITE("sc enter 0.3");   //1/05
								/* save which scheduler is running in eeprom */
								if(_sSchedules.Pump_On_In_Scheduler == _kSET)   /* turn on scheduler */
								{
									/* save running time in eeprom  when no tank is running */
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										Serial.println("Running time in sec");
										_kSERIAL_MON_PRINT_NUM(_sSchedules.Keep_Scheduler_Run_Timer);
										Serial.println();
										_kSERIAL_MON_CLEAR();
										Serial.println("Running schedule");
										_kSERIAL_MON_PRINT_NUM(_sSchedules.Which_Sheduler_Is_Running-1);
										Serial.println(); //1/05
										_kSERIAL_MON_CLEAR();
									}
									
									Fertilizer_Tank_Monitoring();
									
									Blower_Monitoring(_kON);  // 11-4
									
									if(_sSchedules.Save_Schedule_Data_In_Eeprom > 90)
									{
										_sSchedules.Save_Schedule_Data_In_Eeprom = _kRESET;
										_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
									}
									else
									{
										/* when any tank is running */
										if(_sSchedules.Save_Schedule_Data_In_Eeprom > 35)
										{
											unsigned char tank_number, number_of_running_tanks;
											for(tank_number=0; tank_number < _kMAX_FERTILIZER_TANK; tank_number++)
											{
												if(_sSchedules.Running_Tank[tank_number] == _kON)
												{
													number_of_running_tanks++;
												}
											}
											if(number_of_running_tanks > 0)
											{
												_sSchedules.Save_Schedule_Data_In_Eeprom = _kRESET;
												_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
											}
										}
									}
									
									/* while running schedule check the time and compare with schedule time with that plot  */
									if(_sSchedules.Keep_Scheduler_Run_Timer >  _sSchedules.Running_Hr_Of_Each_Plot[_sSchedules.Which_Sheduler_Is_Running-1]
									||  Check_for_no_repeat != 0)
									{
										_sRuble_Parameters.Last_Irrigating_Schedule = _sSchedules.Schedule_Num_List[_sSchedules.Which_Sheduler_Is_Running-1];
										_sSchedules.Pump_On_In_Scheduler = _kRESET;
										Serial.println("time over");
										Doser_Pump_On_Off(_kFORCEFULYY_OFF);
										Turn_Off_All_Tank_For_Safety();
										Blower_Monitoring(_kFORCEFULYY_OFF);  // 11-4
										Turn_Off_Blower_For_Safety();
										
										_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
										if(Check_for_no_repeat != 1)
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("_sSchedules.First_Schedule_Irrigated_Date 0.//// :");
												_kSERIAL_MON_CLEAR();
											}
											
											_sSchedules.Scheduler_Already_Run[_sSchedules.Which_Sheduler_Is_Running-1] = _sSchedules.Started_Date;
											_sSchedules.Scheduler_Already_Run_Month[_sSchedules.Which_Sheduler_Is_Running-1] = _sSchedules.Started_Month;
											
											for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
											{
												_sSchedules.Tank_Running_Date[((_sSchedules.Which_Sheduler_Is_Running-1) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Started_Date;
											}
										}
										else
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("_sSchedules.First_Schedule_Irrigated_Date 0.0//// :");
												_kSERIAL_MON_CLEAR();
											}
											if(Check_for_no_repeat == 1)
											{
												if(_sRuble_Parameters.Debug_Mode == _kSET)
												{
													_kSERAIL_MON_WRITE("_sSchedules.First_Schedule_Irrigated_Date 0.0.0//// :");
													_kSERIAL_MON_CLEAR();
												}
												_sSchedules.Which_Sheduler_Is_Running = _kRESET;  /* reset the running schedule */
												_sSchedules.All_Schedule_Run_Flag = _kSET;       /* all schedule are completed so set this flag */
												_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;  /* reset the trigger flag which earlier set by trigger time */
												
												if(_sSchedules.Schedule_Are_Started == _kSET)
												{
													if(_sRuble_Parameters.Debug_Mode == _kSET)
													{
														_kSERAIL_MON_WRITE("sc enter 11");
														_kSERIAL_MON_CLEAR();
													}
													_sSchedules.Schedule_Are_Started = _kRESET;
													/* save in eeprom */
												}
												_sSchedules.all_schedule_done_flag = _eSET;
												_sSchedules.Irrigating_Date = _sSchedules.First_Schedule_Irrigated_Date;
												_sSchedules.Irrigating_Month = _sSchedules.First_Schedule_Irrigated_Month;
											}
										}
										
										if(_sSchedules.Which_Sheduler_Is_Running == 1)
										{
											_sSchedules.First_Schedule_Irrigated_Date = _sSchedules.Started_Date;
											_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Started_Month;
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("_sSchedules.First_Schedule_Irrigated_Date 0//// :");
												_kSERIAL_MON_PRINT_NUM(_sSchedules.First_Schedule_Irrigated_Date);
												_kSERIAL_MON_CLEAR();
											}
											Return_Calculated_Previous_Day_Date(1);	
											
											for(unsigned char index_num = 1; index_num<_kMAX_LIRA_SENSOR_NODE; index_num++)
											{
												_sSchedules.Scheduler_Already_Run[index_num] =  _sSchedules.Temp_date;
												_sSchedules.Scheduler_Already_Run_Month[index_num] = _sSchedules.Temp_Month;
											} // 22/8/2023
										}
										_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
									}
									
									if(_sPump_Parameter.Irrigation_Status /*||
									(_sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status != _kRESET || _sExtra_Plot[_kFOOGER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation != _kRESET)*/)
									{
										/* stops operation */
										if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("sc enter 4..................");
												_kSERIAL_MON_CLEAR();
											}
											if(_sPump_Parameter.Irrigation_Status)
											{
												if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER && _sRuble_Parameters.Line_Fault_Flag == _kSET)
												{
													_sSchedules.Scheduler_Retry_Timer = 4*60;
												}
												else
												{
													_sSchedules.Scheduler_Retry_Timer = _kSCHEDULE_RETRY_TIMER;
												}
											}
											else
											{
												//flush_running_status = _kSET;
											}
											_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
											_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
										}
										else
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("sc enter 5...................");
												_kSERIAL_MON_CLEAR();
											}
											if(_sPump_Parameter.Irrigation_Status)
											{
												if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER && _sRuble_Parameters.Line_Fault_Flag == _kSET)
												{
													_sSchedules.Scheduler_Retry_Timer = 4*60;
												}
												else
												{
													_sSchedules.Scheduler_Retry_Timer = _kSCHEDULE_RETRY_TIMER;
												}
											}
											else
											{
												//flush_running_status = _kSET;
											}
											_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
											_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
										}
										
										Doser_Pump_On_Off(_kFORCEFULYY_OFF);
										Turn_Off_All_Tank_For_Safety();
										Blower_Monitoring(_kFORCEFULYY_OFF);  // 11-4
										Turn_Off_Blower_For_Safety();
										
										_sSchedules.Pump_On_In_Scheduler = _kRESET;
										_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
										
										_sSchedules.Schedule_Are_Started = _kRESET;
										_sSchedules.all_schedule_done_flag = _kRESET;
									}
									
									if(_sRuble_Parameters.Switch_To_Battery == _eELECTRICITY_IS_NOT_THERE)
									{
										/* stops operation */
										if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("sc enter 6...................");
												_kSERIAL_MON_CLEAR();
											}
											_sSchedules.Scheduler_Retry_Timer = _kSCHEDULE_RETRY_TIMER;
											_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
											_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
										}
										else
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("sc enter 7..............................");
												_kSERIAL_MON_CLEAR();
											}
											_sSchedules.Scheduler_Retry_Timer = _kSCHEDULE_RETRY_TIMER;
											_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
											_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
										}
										Doser_Pump_On_Off(_kFORCEFULYY_OFF);
										Turn_Off_All_Tank_For_Safety();
										Blower_Monitoring(_kFORCEFULYY_OFF);   // 11-4
										Turn_Off_Blower_For_Safety();
										
										_sSchedules.Pump_On_In_Scheduler = _kRESET;
										_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
										_sSchedules.Schedule_Are_Started = _kRESET;
										_sSchedules.all_schedule_done_flag = _kRESET;
									}
								}
								else
								{
									_sRuble_Parameters.Irrigating_Plot =_sSchedules.Schedule_Num_List[_sSchedules.Which_Sheduler_Is_Running-1];
									//_sRuble_Parameters.Pump_Selected_For_Irrigation = _sSchedules.Pump_Selected_For_Schedule;
									_sRuble_Parameters.Pump_Selected_For_Irrigation = _sRuble_Parameters.Pump_Selected_For_Irrigation;
									if(!Start_Irrigation())
									{
										/* what should happen if irrigation doesn't start */
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("sc enter 8...........................");
											_kSERIAL_MON_CLEAR();
										}
										_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
										_sSchedules.Pump_On_In_Scheduler = _kRESET;  // 15/09/2023
										
										if(_sRuble_Parameters.Pump_Selected_For_Irrigation == _kWIRED_PUMP_NUMBER && _sRuble_Parameters.Line_Fault_Flag == _kSET)
										{
											_sSchedules.Scheduler_Retry_Timer = 4*60;
										}
										else
										{
											_sSchedules.Scheduler_Retry_Timer = _kSCHEDULE_RETRY_TIMER;
										}
										
										_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
									}
									else
									{
										irrigation_Start_Set = _kSET;
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("sc enter 9");
											_kSERIAL_MON_CLEAR();
										}
										_sSchedules.Pump_On_In_Scheduler = _kSET;   /* pump on by the scheduler */
										_sSchedules.Shedule_Run_Status_Is_Triggered = _kSET;
										_sSchedules.Back_Up_Running_Scheduler_Number = _sSchedules.Which_Sheduler_Is_Running;
										_sSchedules.Back_Up_Running_Schedule_Plot_Number = _sRuble_Parameters.Irrigating_Plot;
										_sSchedules.Back_Up_Pump_Running_Time = _sSchedules.Keep_Scheduler_Run_Timer;						
										
										for(unsigned char fert_tank=0; fert_tank < _kMAX_FERTILIZER_TANK;fert_tank++)
										{
											_sSchedules.Back_Up_Tank_Running_Time[fert_tank] = _sSchedules.Tank_Running_Timer[fert_tank];
										}
																					
										Set_Trigger_Time();								
										_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
									}
								}
							}
							else
							{
								if(_sSchedules.Which_Sheduler_Is_Running > _kMAX_LIRA_SENSOR_NODE+1) /* when reach to the last scheduler */
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("sc enter 10");
									}
									_sSchedules.Which_Sheduler_Is_Running = _kRESET;  /* reset the running schedule */
									_sSchedules.All_Schedule_Run_Flag = _kSET;       /* all schedule are completed so set this flag */
									_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;  /* reset the trigger flag which earlier set by trigger time */
									
									if(_sSchedules.Schedule_Are_Started == _kSET)
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("sc enter 11");
										}
										_sSchedules.Schedule_Are_Started = _kRESET;
										/* save in eeprom */
									}
									
									_sSchedules.all_schedule_done_flag = _eSET;								
									_sSchedules.Irrigating_Date = _sSchedules.First_Schedule_Irrigated_Date;
									_sSchedules.Irrigating_Month = _sSchedules.First_Schedule_Irrigated_Month;
									_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
									/* save in eeprom */
								}
								else
								{
									_sSchedules.Which_Sheduler_Is_Running++;  /* when time of current schedule over then next scheduler should be select */
									if(_sSchedules.Which_Sheduler_Is_Running == 1)
									{
										_sSchedules.First_Schedule_Irrigated_Date = _sSchedules.Started_Date;
										_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Started_Month;
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("_sSchedules.First_Schedule_Irrigated_Date 2//// :");
											_kSERIAL_MON_PRINT_NUM(_sSchedules.First_Schedule_Irrigated_Date);										
											_kSERIAL_MON_CLEAR();
										}
										Return_Calculated_Previous_Day_Date(1);										
										for(unsigned char index_num = 1; index_num<_kMAX_LIRA_SENSOR_NODE; index_num++)
										{
											_sSchedules.Scheduler_Already_Run[index_num] =  _sSchedules.Temp_date;
											_sSchedules.Scheduler_Already_Run_Month[index_num] = _sSchedules.Temp_Month;
										}										
									}
								}
							}		
						}
						/* ############################################################ */
						else
						{
							
// 							if(!_sRuble_Parameters.Indication_Of_Faults & _eLINE_FAULT_STATUS)
// 							{
// 								return;
// 							}
							/* write logic to trigger the schedule */
							/* calculate trigger reset of time *there will be lag of 2 min* */
							if(_sSchedules.Based_On_Trigger_Time == _kSET)
							{
								if(_sSchedules.Trigger_Time <  _kMAX_BUFFER_TIME)
								{
									_sSchedules.Trigger_Reset = _kMAX_TIME_IN_SEC - _kMAX_BUFFER_TIME;
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("sc enter 11.1");
										_kSERIAL_MON_CLEAR();
									}
								}
								else
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("sc enter 11.2");
										_kSERIAL_MON_CLEAR();
									}
									_sSchedules.Trigger_Reset = _sSchedules.Trigger_Time - _kMAX_BUFFER_TIME;
								}
							}
							else
							{
								if((_sSchedules.Based_On_Trigger_Time == _eBASED_ON_ELECTRICITY) && (_sSchedules.Trigger_Time == 0) &&
								(_sSchedules.Which_Sheduler_Is_Running == 0) && (_sRuble_Parameters.Electricity_Status == _kON) )
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("sc enter 11.2.1");
										_kSERIAL_MON_CLEAR();
									}
									_sSchedules.Trigger_Reset = _kRESET;
								}
								else
								{
									if(_sSchedules.Trigger_Time != 0)
									{
										if(_sSchedules.Trigger_Time <  _kMAX_BUFFER_TIME)
										{
											_sSchedules.Trigger_Reset = _kMAX_TIME_IN_SEC - _kMAX_BUFFER_TIME;
										}
										else
										{
											_sSchedules.Trigger_Reset = _sSchedules.Trigger_Time - _kMAX_BUFFER_TIME;
										}
									}
								}
							}	
							
							irrigation_Start_Set = _kRESET;
							if(_sRuble_Parameters.Last_Irrigating_Schedule != _sRuble_Parameters.Irrigating_Plot)
							{
								if(_sRuble_Parameters.Last_Irrigating_Schedule != _kRESET)
								{
									Valve_Grouping_Handler_For_wireless_Pump(_sRuble_Parameters.Last_Irrigating_Schedule, _kOFF);
									_kSERIAL_MON_CLEAR();
									_sRuble_Parameters.Last_Irrigating_Schedule = _kRESET;
								}
							}	
													
							if(_sRuble_Parameters.Irrigating_Plot != _kRESET)
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("sc enter 6.0..........................");
									_kSERIAL_MON_CLEAR();
								}
								if(_sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Status          !=_kSCHEDULER_ON
								&& _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation  !=_kSET
								&& _sExtra_Plot[(_kFLUSH_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status   !=_kSET
								/*&& _sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Auto_Operation !=_kSET
								&& _sExtra_Plot[(_kFOOGER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status   !=_kSET*/)
								{
									if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("sc enter 6.1..........................");
											_kSERIAL_MON_CLEAR();
										}
										_sSchedules.Scheduler_Retry_Timer = _kSCHEDULE_RETRY_TIMER;
										_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
										_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
									}
									else
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("sc enter 7.1");
											_kSERIAL_MON_CLEAR();
										}
										_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;
										_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
									}
								}
							}							
							/* check the current time with trigger reset time */
							/*********************************************************************************/
							if(_sSchedules.Trigger_Time < _sSchedules.Trigger_Reset)	/* compare the reset time with trigger time */
							{
								check_true_condition = Compare_Trigger_And_Reset_Time();
								if(check_true_condition || (_sSchedules.all_schedule_done_flag == _eNOT_SET && _sSchedules.Cyclic_Schedule_Flag == _kSET))
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("sc enter 16");
										_kSERIAL_MON_CLEAR();
									}
									if((_sRuble_Parameters.Current_Time_In_Sec >= _sSchedules.Trigger_Time) &&(_sRuble_Parameters.Current_Time_In_Sec < _sSchedules.Trigger_Reset))
									{
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("sc enter 17");
											_kSERIAL_MON_CLEAR();
										}
										/* set the trigger time for ON */
										Check_Time_For_Schedule_To_Trigger(1,check_true_condition);
										_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
									}
								}
							}
							else
							{
								if(_sSchedules.Trigger_Time >= _sSchedules.Trigger_Reset)
								{
									check_true_condition = Compare_Trigger_And_Reset_Time();
									
									if(check_true_condition || (_sSchedules.all_schedule_done_flag == _eNOT_SET && _sSchedules.Cyclic_Schedule_Flag == _kSET))
									{
										/* no need to check next date */
										if(_sRuble_Parameters.Debug_Mode == _kSET)
										{
											_kSERAIL_MON_WRITE("sc enter 26");
											_kSERIAL_MON_CLEAR();
										}
										
										/*if(1)
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("sc enter 27");
												_kSERIAL_MON_CLEAR();
											}
											Check_Time_For_Schedule_To_Trigger(1,check_true_condition);
										}*/
										
										if(_sSchedules.Trigger_Time <= _sRuble_Parameters.Current_Time_In_Sec &&
										_sSchedules.Trigger_Reset < _sRuble_Parameters.Current_Time_In_Sec)
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("sc enter 27");
												_kSERIAL_MON_CLEAR();
											}
											/* set the trigger time for ON */
											Check_Time_For_Schedule_To_Trigger(1,check_true_condition);	
										}
										
										else if((_sSchedules.Trigger_Time >= _sRuble_Parameters.Current_Time_In_Sec) && 
										(_sSchedules.Trigger_Reset > _sRuble_Parameters.Current_Time_In_Sec))
										{
											/* set the trigger time for ON */
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("sc enter 32");
												_kSERIAL_MON_CLEAR();
											}
											Check_Time_For_Schedule_To_Trigger(2,check_true_condition);
										}
									}
									else
									{
										if(check_true_condition || (_sSchedules.all_schedule_done_flag == _eNOT_SET && _sSchedules.Cyclic_Schedule_Flag == _kSET))
										{
											if(_sRuble_Parameters.Debug_Mode == _kSET)
											{
												_kSERAIL_MON_WRITE("sc enter 37");
												_kSERIAL_MON_CLEAR();
											}
											if(_sSchedules.Trigger_Time >= _sRuble_Parameters.Current_Time_In_Sec)
											{
												if(_sRuble_Parameters.Debug_Mode == _kSET)
												{
													_kSERAIL_MON_WRITE("sc enter 38");
													_kSERIAL_MON_CLEAR();
												}
												if(_sSchedules.Trigger_Time <= _sRuble_Parameters.Current_Time_In_Sec &&
												_sSchedules.Trigger_Reset < _sRuble_Parameters.Current_Time_In_Sec)
												{
													if(_sRuble_Parameters.Debug_Mode == _kSET)
													{
														_kSERAIL_MON_WRITE("sc enter 39");
														_kSERIAL_MON_CLEAR();
													}
													Check_Time_For_Schedule_To_Trigger(1,check_true_condition);
												}
												
												else if((_sSchedules.Trigger_Time >= _sRuble_Parameters.Current_Time_In_Sec) && (_sSchedules.Trigger_Reset > _sRuble_Parameters.Current_Time_In_Sec))
												{
													if(_sRuble_Parameters.Debug_Mode == _kSET)
													{
														_kSERAIL_MON_WRITE("sc enter 44");
														_kSERIAL_MON_CLEAR();
													}
													/* set the trigger time for ON */
													Check_Time_For_Schedule_To_Trigger(2,check_true_condition);
												}
												
											}
											else
											{
												if(_sRuble_Parameters.Debug_Mode == _kSET)
												{
													_kSERAIL_MON_WRITE("sc enter 49");
													_kSERIAL_MON_CLEAR();
												}
												if(_sSchedules.Trigger_Time <= _sRuble_Parameters.Current_Time_In_Sec &&
												_sSchedules.Trigger_Reset < _sRuble_Parameters.Current_Time_In_Sec)
												{
													if(_sRuble_Parameters.Debug_Mode == _kSET)
													{
														_kSERAIL_MON_WRITE("sc enter 50");
														_kSERIAL_MON_CLEAR();
													}
													/* set the trigger time for ON */
													Check_Time_For_Schedule_To_Trigger(1,check_true_condition);
												}
												
												else if((_sSchedules.Trigger_Time >= _sRuble_Parameters.Current_Time_In_Sec) && (_sSchedules.Trigger_Reset > _sRuble_Parameters.Current_Time_In_Sec))
												{
													/* set the trigger time for ON */
													if(_sRuble_Parameters.Debug_Mode == _kSET)
													{
														_kSERAIL_MON_WRITE("sc enter 55");
														_kSERIAL_MON_CLEAR();
													}
													Check_Time_For_Schedule_To_Trigger(2,check_true_condition);
												}
											}
										}
									}
								}
							}
							
							/*********************************************************************************/
						}
					}
					else
					{
						if(_sSchedules.Scheduler_Retry_Timer < 10)
						{	
							if(_sPump_Parameter.Irrigation_Status)
							{
								_sPump_Parameter.Irrigation_Status = _kRESET;
							}
						}
						/* what should happen when scheduler is in wait mode */
					}
				}
				else
				{
					/* what should happen if scheduler operation mode is not selected */
					if(mode_change)
					{
						mode_change = _kRESET;
						if(_sPump_Parameter.Irrigation_Status)
						{
							_sPump_Parameter.Irrigation_Status = _kRESET;
						}
						if(_sSchedules.Which_Sheduler_Is_Running)    /* check if any scheduler is running */
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("sc enter 58");
								_kSERIAL_MON_CLEAR();
							}
							
							Doser_Pump_On_Off(_kFORCEFULYY_OFF);
							Turn_Off_All_Tank_For_Safety();
							Blower_Monitoring(_kFORCEFULYY_OFF);    // 11-4
							Turn_Off_Blower_For_Safety();
						
							//_sSchedules.Which_Sheduler_Is_Running = _kRESET;
							if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("sc enter 59");
									_kSERIAL_MON_CLEAR();
								}
								
								_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
							}				
							_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;  /* reset the trigger flag which earlier set by trigger time */
							_sSchedules.Pump_On_In_Scheduler = _kRESET;
							_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);	
						}
						
						if(_sRuble_Parameters.Last_Irrigating_Schedule != _sRuble_Parameters.Irrigating_Plot)
						{
							if(_sRuble_Parameters.Last_Irrigating_Schedule != _kRESET)
							{
								Valve_Grouping_Handler_For_wireless_Pump(_sRuble_Parameters.Last_Irrigating_Schedule, _kOFF);
								_sRuble_Parameters.Last_Irrigating_Schedule = _kRESET;
							}
						}
					}
				}
			}
			else
			{
				if((_sSchedules.Which_Sheduler_Is_Running && _sRuble_Parameters.Irrigating_Plot !=0)
				|| _sSchedules.Scheduler_Retry_Timer != 0)
				{
					if(_sPump_Parameter.Irrigation_Status)
					{
						_sPump_Parameter.Irrigation_Status = _kRESET;
					}
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("sc enter 60");
						_kSERIAL_MON_CLEAR();
					}
					if(_sSchedules.Schedule_Pause_Resume != 0 || _sSchedules.New_Schedules_Occur == _kSET || _sRuble_Parameters.Update_Local_Date[2] == 70)
					{
						/* should add what happen if schedule pause */
						/* in ruble data resume section will be there */
						
						if(_sSchedules.Which_Sheduler_Is_Running)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("sc enter 61");
								_kSERIAL_MON_CLEAR();
							}
							_sSchedules.Back_Up_Running_Scheduler_Number = _sSchedules.Which_Sheduler_Is_Running;
							_sSchedules.Back_Up_Running_Schedule_Plot_Number = _sRuble_Parameters.Irrigating_Plot;
							_sSchedules.Back_Up_Pump_Running_Time = _sSchedules.Keep_Scheduler_Run_Timer;
							
							for(unsigned char fert_tank=0; fert_tank < _kMAX_FERTILIZER_TANK;fert_tank++)
							{
								_sSchedules.Back_Up_Tank_Running_Time[fert_tank] = _sSchedules.Tank_Running_Timer[fert_tank];
							}
							
							/* ########################## add schedule pause to ruble data topic ############# */
						}
						
						if(_sSchedules.New_Schedules_Occur == _kSET)
						{
							Edited_In_Schedule();
						}
					}
					
					
					
					/* what should happen when scheduler mode id not enabled */
					_sSchedules.New_Schedules_Occur = _kRESET;
					_sSchedules.Scheduler_Retry_Timer = _kRESET;
					_sSchedules.Pump_On_In_Scheduler = _kRESET;
					
					if(_sSchedules.Which_Sheduler_Is_Running)    /* check if any scheduler is running */
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sc enter 62");
							_kSERIAL_MON_CLEAR();
						}
						Doser_Pump_On_Off(_kFORCEFULYY_OFF);
						Turn_Off_All_Tank_For_Safety();
						Blower_Monitoring(_kFORCEFULYY_OFF);   // 11-4
						Turn_Off_Blower_For_Safety();
					}
					
					//_sSchedules.Which_Sheduler_Is_Running = _kRESET;  // 17/02/2023
					
						if(!Stop_Irrigation())       /* stops the irrigation if any faults occur */
						{
							_sRuble_Parameters.Operation_Status = _eOPERATION_STAT_OPRN_END;
						}
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("Scheduler gets off 11 : ");
						_kSERIAL_MON_CLEAR();
						_kSERIAL_MON_PRINT_NUM(_sSchedules.Which_Sheduler_Is_Running);
						
						_kSERAIL_MON_WRITE("sc enter 59.1");
						_kSERIAL_MON_CLEAR();
					}
					
					
					_sSchedules.Which_Sheduler_Is_Running = _kRESET;  /* 17/02/2023reset the running schedule */
					_sSchedules.All_Schedule_Run_Flag = _kSET;       /* all schedule are completed so set this flag */
					_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;  /* reset the trigger flag which earlier set by trigger time */
					
					_sSchedules.Schedule_Are_Started = _kRESET;
					_sSchedules.all_schedule_done_flag = _kRESET;
					
					if(_sRuble_Parameters.Last_Irrigating_Schedule != _sRuble_Parameters.Irrigating_Plot)
					{
						if(_sRuble_Parameters.Last_Irrigating_Schedule != _kRESET)
						{
							Valve_Grouping_Handler_For_wireless_Pump(_sRuble_Parameters.Last_Irrigating_Schedule, _kOFF);
							_sRuble_Parameters.Last_Irrigating_Schedule = _kRESET;
						}
					}
					
					_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
					
				}
				
				if(_sSchedules.New_Schedules_Occur != _kRESET)
				{
					_sSchedules.New_Schedules_Occur = _kRESET;
					
					
					
					_sSchedules.Which_Sheduler_Is_Running = _kRESET;  /* 17/02/2023reset the running schedule */
					_sSchedules.All_Schedule_Run_Flag = _kSET;       /* all schedule are completed so set this flag */
					_sSchedules.Shedule_Run_Status_Is_Triggered = _kRESET;  /* reset the trigger flag which earlier set by trigger time */
					_sSchedules.Schedule_Are_Started = _kRESET;
					_sSchedules.all_schedule_done_flag = _kRESET;
					_sSchedules.New_Schedules_Occur = _kRESET;
					_sSchedules.Scheduler_Retry_Timer = _kRESET;
					_sSchedules.Pump_On_In_Scheduler = _kRESET;
				
				
				
					Edited_In_Schedule();
				}
				
				if(_sRuble_Parameters.Irrigating_Plot && _sRuble_Parameters.Operation_Mode == _eOPER_MOD_SCHEDULER)
				{
					Stop_Irrigation();
					_sRuble_Parameters.Pump_Run_Timer = _kRESET;
				}
			}
		}break;
		
		default:
		{
			_sSchedules.Based_On_Trigger_Time = _eBASED_ON_TRIGGER_TIME;
		}break;
	}
	
}					

void Fertilizer_Tank_Monitoring(void)
{
	unsigned char tank_number;
	
	if(_sExtra_Plot[_kFLUSH_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Auto_Operation == _kSET)
	{	
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("Fertilizer_Tank_Monitoring : ");
		}
		if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Status == _kSCHEDULER_ON)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("Fertilizer_Tank_Monitoring 1 : ");
			}
			Doser_Pump_On_Off(_kFORCEFULYY_OFF);
		}
		return;
	}
	
	for(tank_number = 0; tank_number < _kMAX_FERTILIZER_TANK; tank_number++)
	{
		if(!_sSchedules.Tank_On_Retry[tank_number])
		{
			if(_sSchedules.Fertilizer_Tank_Status[((_sSchedules.Which_Sheduler_Is_Running-1) * _kMAX_FERTILIZER_TANK)+tank_number] == _kSET)
			{
				if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number]-_kEXTRA_PLOT_START_FROM].Extra_Plot_Number !=0 && _sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number]-_kEXTRA_PLOT_START_FROM].Extra_Plot_Number > 100)
				{
					/*_kSERAIL_MON_WRITE("_sSchedules.Keep_Scheduler_Run_Timer 1: ");*/
					if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number]-_kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
					{
						if(_sSchedules.Tank_Running_Date[((_sSchedules.Which_Sheduler_Is_Running-1) * _kMAX_FERTILIZER_TANK)+tank_number] != _sSchedules.Started_Date)
						{	
							/*_kSERAIL_MON_WRITE("_sSchedules.Keep_Scheduler_Run_Timer 3: ");*/
							if(_sSchedules.Running_Tank[tank_number] == _kON)
							{
								Doser_Pump_On_Off(_kON);
								/*_kSERAIL_MON_WRITE("_sSchedules.Keep_Scheduler_Run_Timer 4: ");*/
								if(_sSchedules.Tank_Running_Timer[tank_number] > _sSchedules.Tank_Off_Time_Duration[((_sSchedules.Which_Sheduler_Is_Running-1) * _kMAX_FERTILIZER_TANK) + tank_number])
								{
									/* stop tank schedule */
									Doser_Pump_On_Off(_kOFF);
									_sSchedules.Tank_Running_Timer[tank_number] = _kRESET;
									_sSchedules.Running_Tank[tank_number] = _kOFF;
									//_sSchedules.Already_Fertilze_Status[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Started_Date;
									_sSchedules.Tank_Running_Date[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Started_Date;	
									Fertilizer_Tank_On_Off(((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number,_kOFF);
									
								}
							}
							else
							{
								if(_sSchedules.Keep_Scheduler_Run_Timer >  _sSchedules.Fertilizer_Tank_On_After[((_sSchedules.Which_Sheduler_Is_Running-1) * _kMAX_FERTILIZER_TANK) + tank_number])
								{
									if(_sSchedules.Tank_Running_Date[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number] != _sSchedules.Started_Date)
									{
										/* start tank schedule */
										if(Fertilizer_Tank_On_Off(((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number,_kON))
										{
											//_sSchedules.Blower_On_Time = _kRESET;
											_gWait_Blower_Time = _kRESET;
											if(Doser_Pump_On_Off(_kON))
											{
												_sSchedules.Running_Tank[tank_number] = _kON;
											}
											else
											{
												_sSchedules.Tank_On_Retry[tank_number] = _kTANK_RETRY_TIME;
											}		
										}
										else
										{
											_sSchedules.Tank_On_Retry[tank_number] = _kTANK_RETRY_TIME;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

char Fertilizer_Tank_On_Off(unsigned char tank_number, unsigned char on_off)
{
	unsigned char count = 0,i=0,j=0,id_len =0, compare_id=0, data_send_counter = 0, current_valve_number = 0;
	
	if(_sSchedules.Fertilizer_Tank_Number[tank_number] < 100)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("fertilizer tank number is less than 100 : ");
			_kSERIAL_MON_CLEAR();
		}
		return 1;
	}
	
	if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number] - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kSET)
	{
		return 1;
	}
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("fertilizer tank number : ");
		_kSERIAL_MON_CLEAR();
		_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(tank_number);
		_kSERIAL_MON_CLEAR();
		_kSERAIL_MON_WRITE("fertilizer tank valve list : ");
		_kSERIAL_MON_CLEAR();
	
	
		for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
		{
			_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[i]);
			_kSERAIL_MON_WRITE_NO_LN(" ");
		}
	}
	
	//_sSchedules.Fertilizer_Tank_Status[];
	
	for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
	{
		if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && _sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[i] > 0)
		{
			unsigned char valve_number = 0;
			unsigned char euro_valve_num = 0,command_valve = 0;
			
			valve_number =  _sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[i];
			
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
				
				_kSERAIL_MON_WRITE("Send On/off Command to EURO valve 1");
				_kSERIAL_MON_CLEAR();
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
									for(int wired_valve = 0; wired_valve < _kMAX_VALVE_FOR_1PLOT ; wired_valve++)
									{
										if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
										{
											/********************************* turn on/off the wired valve *********************************/
											
											_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
								for(int wired_valve = 0; wired_valve < _kMAX_VALVE_FOR_1PLOT ; wired_valve++)
								{
									if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
									{
										/********************************* turn on/off the wired valve *********************************/
										
										_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
										_kDELAY_MS(200);
										if(on_off == _kON)
										{
											
										}
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
							for(int wired_valve = 0; wired_valve < _kMAX_VALVE_FOR_1PLOT ; wired_valve++)
							{
								if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
								{
									/********************************* turn on/off the wired valve *********************************/
									
									_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
									_kDELAY_MS(200);
									
									if(on_off == _kON)
									{
									}
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
						
						for(int wired_valve = 0; wired_valve < _kMAX_VALVE_FOR_1PLOT ; wired_valve++)
						{
							if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
							{
								/********************************* turn on/off the wired valve *********************************/
								
								_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
								_kDELAY_MS(200);
								if(on_off == _kON)
								{
									
								}
							}
						}
						return 0;          /* no data available */
					}
				}
				_kDELAY_MS(1000);
			}
		}
		
		/******************************************************** handle wired valve *******************************************************************/
		
		else if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[i] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
		{
			/********************************* turn on/off the wired valve *********************************/
			
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Euro_Valve_Group[i]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(!on_off));
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
	
	
	// 	_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
	// 	_sRuble_Parameters.Extra_Plot_Feedback = _sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM;
	// 	_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
	//
	// 	if(on_off == _kOFF)
	// 	{
	// 		_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_OFF;
	// 	}
	// 	else
	// 	{
	// 		_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_ON;
	// 	}
	
	if(on_off == _kOFF)
	{
		if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Status != _kSCHEDULER_OFF)
		{
			_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
			_sRuble_Parameters.Extra_Plot_Feedback = _sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM;
			_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
			
			_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_OFF;
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("tank to send schedule Off 0 : ");
				_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM);
				_kSERIAL_MON_CLEAR();
			}
		}
		
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("tank to send schedule Off 1 : ");
			_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM);
			_kSERIAL_MON_CLEAR();
		}
	}
	else
	{
		_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
		_sRuble_Parameters.Extra_Plot_Feedback = _sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM;
		_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
		
		_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_ON;
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("tank to send schedule ON : ");
			_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM);
			_kSERIAL_MON_CLEAR();
		}
		//_kSERAIL_MON_WRITE("publish status : ");
		//_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM);
	}
	
	
	return 1;
}

char Doser_Pump_On_Off(unsigned char on_off)
{
	unsigned char count = 0,i=0,j=0,id_len =0, compare_id=0, data_send_counter = 0, current_valve_number = 0;
	unsigned char tank_number,number_of_running_tanks; 
	number_of_running_tanks = 0;
	
	if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Extra_Plot_Number == 0 || 
	_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Extra_Plot_Number < 100)
	{
		return 1;
	}
	
	if(on_off == _kON && _sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Status == _kSCHEDULER_ON)
	{
		return 1;
	}
	
	if(on_off == _kOFF)
	{
		for(tank_number=0; tank_number < _kMAX_FERTILIZER_TANK; tank_number++)
		{
			if(_sSchedules.Running_Tank[tank_number] == _kON)
			{
				number_of_running_tanks++;
			}
		}
		if(number_of_running_tanks > 1)
		{
			return 1;
		}
	}
	
	
	if(on_off == _kFORCEFULYY_OFF)
	{
		on_off = _kOFF;
	}
	
	if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].On_Off_Status == _kSET)
	{
		return 1;
	}
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("doser pump valve list : ");
		_kSERIAL_MON_CLEAR();
	
	
		for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)
		{
			_kSERIAL_MON_PRINT_NUM_NO_NEW_LINE(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[i]);
			_kSERAIL_MON_WRITE_NO_LN(" ");
			_kSERIAL_MON_CLEAR();
		}
	}
	
	
	for(i=0;i<_kMAX_VALVE_FOR_1PLOT;i++)    /* check the valve number from the array which hold the group of valve numbers */
	{
		if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[i] <= _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO && _sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[i] > 0)
		{
			unsigned char valve_number =0;
			unsigned char euro_valve_num=0,command_valve=0;
			
			valve_number =  _sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[i];
			
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
									for(int wired_valve = 0; wired_valve < _kMAX_VALVE_FOR_1PLOT ; wired_valve++)
									{
										if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
										{
											/********************************* turn on/off the wired valve *********************************/
											
											_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
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
								for(int wired_valve = 0; wired_valve < _kMAX_VALVE_FOR_1PLOT ; wired_valve++)
								{
									if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
									{
										/********************************* turn on/off the wired valve *********************************/
																				
										_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
										_kDELAY_MS(200);
										
										if(on_off == _kON)
										{
											
										}
										
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
							for(int wired_valve = 0; wired_valve < _kMAX_VALVE_FOR_1PLOT ; wired_valve++)
							{
								if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
								{
									/********************************* turn on/off the wired valve *********************************/
									
									_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
									_kDELAY_MS(200);
									
									if(on_off == _kON)
									{
										
									}
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
						
						for(int wired_valve = 0; wired_valve < _kMAX_VALVE_FOR_1PLOT ; wired_valve++)
						{
							if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[wired_valve] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
							{
								/********************************* turn on/off the wired valve *********************************/
								
								_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[wired_valve]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(1));
								_kDELAY_MS(200);
								if(on_off == _kON)
								{
										
								}
								
							}
						}
						return 0;          /* no data available */
					}
				}
				_kDELAY_MS(1000);
			}
		}
		
		/******************************************************** handle wired valve *******************************************************************/
		
		else if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[i] > _kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)
		{
			/********************************* turn on/off the wired valve *********************************/
			
			_kGPIO_SET(_sRuble_Parameters.Wired_valve_List[_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Euro_Valve_Group[i]-((_kMAX_EURO_VALVE*_kNUMBER_OF_VALVE_CONNECT_TO_EURO)+1)],(!on_off));
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
	
	
	
	if(on_off == _kOFF)
	{
		if(_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Status != _kSCHEDULER_OFF)
		{
			_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
			_sRuble_Parameters.Extra_Plot_Feedback = _kFERTIIZER_PLOT_NUMBER-1;
			_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
			
			_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Status = _kSCHEDULER_OFF;
		}
	}
	else
	{
		_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
		_sRuble_Parameters.Extra_Plot_Feedback = _kFERTIIZER_PLOT_NUMBER-1;
		_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
		
		_sExtra_Plot[_kFERTIIZER_PLOT_NUMBER-1].Status = _kSCHEDULER_ON;
	}
	
	return 1;
}

char Turn_Off_All_Tank_For_Safety(void)
{
	unsigned char tank_number_list[_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK], tank_number;
	
	for(unsigned char temp_tank_number=0 ; temp_tank_number < _kMAX_FERTILIZER_TANK; temp_tank_number++)
	{
		_sSchedules.Running_Tank[temp_tank_number] = _kRESET;
	}
	
	for(int i=0; i<(_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK); i++)
	{
		tank_number_list[i] = 0;
	}
	
	
	for(int check_tank =0; check_tank<(_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK) ; check_tank++)
	{
		if(check_tank == 0)
		{
			if(tank_number_list[check_tank] == 0)
			{
				tank_number_list[check_tank] = _sSchedules.Fertilizer_Tank_Number[check_tank];
			}
		}
		else
		{
			for(int check_tank2 =0; check_tank2<_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK; check_tank2++)
			{
				if(tank_number_list[check_tank2] == _sSchedules.Fertilizer_Tank_Number[check_tank])
				{
					break;
				}
				if(tank_number_list[check_tank2] == 0)
				{
					tank_number_list[check_tank2] = _sSchedules.Fertilizer_Tank_Number[check_tank];
					break;
				}
			}
		}
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("tank number list");
		_kSERIAL_MON_CLEAR();
	}
	
	for(int tank_number=0; tank_number_list[tank_number] != 0; tank_number++)
	{
		_kSERIAL_MON_PRINT_NUM(tank_number_list[tank_number]);
		
		if(_sExtra_Plot[tank_number_list[tank_number]-_kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
		{
			if(Extra_Plot_Valve_Grouping_Handler(_kOFF, tank_number_list[tank_number]-_kEXTRA_PLOT_START_FROM))
			{
				//_kSERAIL_MON_WRITE("tank number list with tank number array : ");
				//_kSERIAL_MON_PRINT_NUM(_sSchedules.Fertilizer_Tank_Number[tank_number]-_kEXTRA_PLOT_START_FROM);
				
				if(_sExtra_Plot[tank_number_list[tank_number]-_kEXTRA_PLOT_START_FROM].Status != _kSCHEDULER_OFF)
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("tank number gets to off safely");
						_kSERIAL_MON_PRINT_NUM(tank_number_list[tank_number]-_kEXTRA_PLOT_START_FROM);
						_kSERIAL_MON_CLEAR();
					}
					
					_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
					_sRuble_Parameters.Extra_Plot_Feedback = tank_number_list[tank_number];
					_sExtra_Plot[tank_number_list[tank_number] -_kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
					_sExtra_Plot[tank_number_list[tank_number]-_kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_OFF;
				}
				
			}
			//return 1;
		}
	}
	
}

char Blower_Monitoring(unsigned char Forcefully_Off)
{	
	unsigned char tank_number,time_buffer;
	static unsigned char temp_number;
	
	if(_sExtra_Plot[(_kBLOWER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].Extra_Plot_Number == _kBLOWER_PLOT_NUMBER)
	{
		
		if(_sExtra_Plot[(_kBLOWER_PLOT_NUMBER-_kEXTRA_PLOT_START_FROM)].On_Off_Status == _kRESET)
		{
			if(Forcefully_Off == _kFORCEFULYY_OFF)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("blower gets 1.1 : ");
					_kSERIAL_MON_CLEAR();
				}
				
				_sSchedules.Blower_On_Scheduler_Status =_kOFF;
				_sSchedules.Blower_On_Retry = _kRESET;
				_sSchedules.Blower_On_Time = _kRESET;
				temp_number = 0;
				/* turn off blower */
				
				if(Extra_Plot_Valve_Grouping_Handler(_kOFF, (_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM)))
				{
					//_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
					//_sRuble_Parameters.Extra_Plot_Feedback = _kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
					
					if(_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Status != _kSCHEDULER_OFF)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("blower gets 2.1 : ");
							_kSERIAL_MON_CLEAR();
						}
						_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
						_sRuble_Parameters.Extra_Plot_Feedback = _kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
						_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
						_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_OFF;
					}
				}
				
				_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
				return 1;
				
			}
		}
	}
	
	unsigned long int tank1_time = _sSchedules.Fertilizer_Tank_On_After[((_sSchedules.Which_Sheduler_Is_Running - 1)* 3) + 0] + 
	_sSchedules.Tank_Off_Time_Duration[((_sSchedules.Which_Sheduler_Is_Running - 1)* 3) + 0];
	
	if(_sSchedules.Fertilizer_Tank_Status[((_sSchedules.Which_Sheduler_Is_Running - 1) *3) + 0] != _kSET)
	{
		tank1_time = _kRESET;
	}
	
	unsigned long int tank2_time = _sSchedules.Fertilizer_Tank_On_After[((_sSchedules.Which_Sheduler_Is_Running - 1)* 3) + 1] + 
	_sSchedules.Tank_Off_Time_Duration[((_sSchedules.Which_Sheduler_Is_Running - 1)* 3) + 1];
	
	if(_sSchedules.Fertilizer_Tank_Status[((_sSchedules.Which_Sheduler_Is_Running - 1) *3) + 1] != _kSET)
	{
		tank2_time = _kRESET;
	}
	
	unsigned long int tank3_time = _sSchedules.Fertilizer_Tank_On_After[((_sSchedules.Which_Sheduler_Is_Running - 1)* 3) + 2] + 
	_sSchedules.Tank_Off_Time_Duration[((_sSchedules.Which_Sheduler_Is_Running - 1)* 3) + 2];
	
	if(_sSchedules.Fertilizer_Tank_Status[((_sSchedules.Which_Sheduler_Is_Running - 1) *3) + 2] != _kSET)
	{
		tank3_time = _kRESET;
	}
	
	if(_sSchedules.Blower_On_Retry != 0 || _sSchedules.Scheduler_Retry_Timer != 0)
	{
		return 0;
	}
	
	/*_kSERAIL_MON_WRITE("tank1_time : ");
	_kSERIAL_MON_PRINT_NUM(tank1_time);
	_kSERAIL_MON_WRITE("tank2_time : ");
	_kSERIAL_MON_PRINT_NUM(tank2_time);
	_kSERAIL_MON_WRITE("tank3_time : ");
	_kSERIAL_MON_PRINT_NUM(tank3_time);*/
	
	if(_sSchedules.Blower_On_Scheduler_Status != _kON)
	{
		//_kSERAIL_MON_WRITE("blower gets 1 : ");
		if(_sSchedules.Fertilizer_Tank_Status[((_sSchedules.Which_Sheduler_Is_Running - 1) *3) + 0] == _kSET
		|| _sSchedules.Fertilizer_Tank_Status[((_sSchedules.Which_Sheduler_Is_Running - 1) *3) + 1] == _kSET 
		|| _sSchedules.Fertilizer_Tank_Status[((_sSchedules.Which_Sheduler_Is_Running - 1) *3) + 2] == _kSET)
		{
			//_kSERAIL_MON_WRITE("blower gets 2 : ");
			if(_gWait_Blower_Time == 0)
			{
				//_kSERAIL_MON_WRITE("blower gets 3 : ");
				if(tank1_time > _sSchedules.Keep_Scheduler_Run_Timer  
				|| tank2_time > _sSchedules.Keep_Scheduler_Run_Timer
				|| tank3_time > _sSchedules.Keep_Scheduler_Run_Timer)
				{
					if(Extra_Plot_Valve_Grouping_Handler(_kON, (_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM)))
					{
						//_kSERAIL_MON_WRITE("blower gets 4 : ");
						//_kSERIAL_MON_CLEAR();
						_sSchedules.Blower_On_Scheduler_Status = _kON;
						_sSchedules.Blower_On_Time = _kBLOWER_ON_TIME;
						_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
						_sRuble_Parameters.Extra_Plot_Feedback = _kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
						_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
						_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_ON;
					}
					else
					{
						_sSchedules.Blower_On_Retry = _kMAX_BLOWER_RETRY_TIME;
					}
				}
			}
		}
		
	}
	else
	{
		unsigned char check_tank_status = 0;
		if(_sSchedules.Blower_On_Time == _kRESET || (tank1_time <= _sSchedules.Keep_Scheduler_Run_Timer && tank2_time <= _sSchedules.Keep_Scheduler_Run_Timer
		&& tank3_time <= _sSchedules.Keep_Scheduler_Run_Timer))
		{
			check_tank_status = 0;
			for(unsigned char tank_number =0; tank_number < _kMAX_FERTILIZER_TANK; tank_number++)
			{
				if(_sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number]-_kEXTRA_PLOT_START_FROM].Extra_Plot_Number !=0 
				&& _sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number]-_kEXTRA_PLOT_START_FROM].Extra_Plot_Number > 100
				&& _sExtra_Plot[_sSchedules.Fertilizer_Tank_Number[((_sSchedules.Which_Sheduler_Is_Running-1)*_kMAX_FERTILIZER_TANK)+tank_number]-_kEXTRA_PLOT_START_FROM].Status == _kSCHEDULER_ON)
				{
					check_tank_status = 1;
				}
			}
			
			if(check_tank_status)
			{
				_gWait_Blower_Time = _kMAX_BLOWER_REPEAT_TIME;
			}
			else
			{
				_gWait_Blower_Time = 60 * 60;
			}
			
			_sSchedules.Blower_On_Scheduler_Status = _kOFF;
			//_kSERAIL_MON_WRITE("blower gets 5 : ");
			
			if(Extra_Plot_Valve_Grouping_Handler(_kOFF, (_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM)))
			{
				//_kSERAIL_MON_WRITE("blower gets 6 : ");
				//_kSERIAL_MON_CLEAR();
				if(_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Status != _kSCHEDULER_OFF)
				{
					_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
					_sRuble_Parameters.Extra_Plot_Feedback = _kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
					_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
					_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_OFF;
				}
			}
		}
	}	
}

char Turn_Off_Blower_For_Safety(void)
{
// 	for(unsigned char i=0;i<_kMAX_FERTILIZER_TANK;i++)
// 	{
// 		_sSchedules.blower_run_status[i] = _kRESET;
// 		if(_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].On_Off_Status == _kRESET)
// 		{
// 			if(Extra_Plot_Valve_Grouping_Handler(_kOFF, (_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM)))
// 			{
// 				_kSERAIL_MON_WRITE("blower gets 8 : ");
// 				_kSERIAL_MON_CLEAR();
// 				if(_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Status != _kSCHEDULER_OFF)
// 				{
// 					_sRuble_Parameters.Publish_Topic |= _eONE_TOUCH_FEEDBACK;
// 					_sRuble_Parameters.Extra_Plot_Feedback = _kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM;
// 					_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Publish_status = _eEXTRA_PLOT_ONETOUCH_RESEND;
// 					_sExtra_Plot[_kBLOWER_PLOT_NUMBER - _kEXTRA_PLOT_START_FROM].Status = _kSCHEDULER_OFF;
// 				}
// 			}
// 		}
// 	}
}

unsigned char Pause_Current_Schedule(unsigned char schedule_pause)
{
	/* pause current schedule */
	if(schedule_pause)
	{
		if(_sSchedules.Schedule_Pause != _kSET)
		{
			_sSchedules.Schedule_Pause = _kSET;
		}
		else
		{
			
		}
	}
	else
	{
		if(_sSchedules.Schedule_Pause != _kRESET)
		{
			_sSchedules.Schedule_Pause = _kRESET;
		}
		
	}
	
}

unsigned char Delete_Schedule(unsigned char Schedule_Number)
{
	if(Schedule_Number)
	{
		if(_sSchedules.Back_Up_Running_Scheduler_Number)
		{
			/* delete the plot which is before the schedule which is irrigating */
			
// 			if(_sSchedules.Back_Up_Running_Scheduler_Number < Schedule_Number)
// 			{
// 				_sSchedules.Back_Up_Running_Scheduler_Number -= 1;
// 			}
			
			if(_sSchedules.Back_Up_Running_Scheduler_Number == Schedule_Number)
			{
				/* delete the plot which is irrigating */
				_sSchedules.Back_Up_Running_Scheduler_Number = _kRESET;
				
				_sSchedules.Back_Up_Running_Schedule_Plot_Number = _kRESET;
				_sSchedules.Back_Up_Pump_Running_Time = _kRESET;
				for(unsigned char fert_tank=0; fert_tank < _kMAX_FERTILIZER_TANK;fert_tank++)
				{
					_sSchedules.Back_Up_Tank_Running_Time[fert_tank] = _kRESET;
				}
				
				for(unsigned char tank_number; tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
				{
					_sSchedules.Tank_Running_Timer[tank_number] = _kRESET;
				}
				
				_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
				for(unsigned char tank_number; tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
				{
					_sSchedules.Running_Tank[tank_number] = _kOFF;
				}	
			}
		}
		else
		{
			/* delete the plot which after the schedule which is currently irrigating */
		}
	}
}

unsigned char Edited_In_Schedule(void)
{
	/* edit in the current schedule */
	/* edit in the current schedule */
	if(_sSchedules.Running_Schedule_At == 0)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("my 14");
			_kSERIAL_MON_CLEAR();
		}
		_sSchedules.Back_Up_Running_Scheduler_Number = _kRESET;
		_sSchedules.Back_Up_Running_Schedule_Plot_Number = _kRESET;
		_sSchedules.Back_Up_Pump_Running_Time = _kRESET;
		for(unsigned char fert_tank=0; fert_tank < _kMAX_FERTILIZER_TANK;fert_tank++)
		{
			_sSchedules.Back_Up_Tank_Running_Time[fert_tank] = _kRESET;
		}
		
		for(unsigned char tank_number = 0; tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
		{
			_sSchedules.Tank_Running_Timer[tank_number] = _kRESET;
		}
		
		_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
		for(unsigned char tank_number = 0; tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
		{
			_sSchedules.Running_Tank[tank_number] = _kOFF;
		}
		
		//_sSchedules.Schedule_Start_From_Today = 1;
		
		if(_sSchedules.Schedule_Start_From_Today != 1)
		{
			Return_Calculated_Previous_Day_Date(1);
		}
		else
		{
			_sSchedules.Temp_date  = _sRuble_Parameters.Update_Local_Date[0];
			_sSchedules.Temp_Month = _sRuble_Parameters.Update_Local_Date[1];
			_sSchedules.Started_Date = _sSchedules.Temp_date;
			_sSchedules.Started_Month = _sSchedules.Temp_Month;
			_sSchedules.First_Schedule_Irrigated_Date = _sSchedules.Temp_date;
			_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Temp_Month;
		}
		
		
		for(unsigned char index_num = 0; index_num<_kMAX_LIRA_SENSOR_NODE; index_num++)
		{		
			_sSchedules.Scheduler_Already_Run[index_num] =  _sSchedules.Temp_date;
			_sSchedules.Scheduler_Already_Run_Month[index_num] = _sSchedules.Temp_Month;
			
			for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
			{
				_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Temp_date;
			}
			
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				Serial.println(F("Running list date "));
				_kSERIAL_MON_PRINT_NUM(_sSchedules.Scheduler_Already_Run[index_num]);
				Serial.println(F("Running list month "));
				_kSERIAL_MON_PRINT_NUM(_sSchedules.Scheduler_Already_Run_Month[index_num]);
			}
		}
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("irrigating date upadte 14///////////////////////////////////////////");
		}
		_sSchedules.Irrigating_Date = _sSchedules.Temp_date;
		_sSchedules.Irrigating_Month = _sSchedules.Temp_Month;
		return 1;
		/* add previous dates here */
	}
	
	if(_sSchedules.Scheduler_Retry_Timer == _kRESET && _sSchedules.Running_Schedule_At !=0)
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("my 0");
		}
		if(_sSchedules.Back_Up_Running_Scheduler_Number)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("my 1");
			}
			
			if(_sSchedules.Back_Up_Running_Schedule_Plot_Number)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("my 2");
					_kSERAIL_MON_WRITE("Running schedule :");
					_kSERIAL_MON_CLEAR();
					_kSERIAL_MON_PRINT_NUM(_sSchedules.Running_Schedule_At);
					_kSERAIL_MON_WRITE("Back up schedule :");
					_kSERIAL_MON_CLEAR();
					_kSERIAL_MON_PRINT_NUM(_sSchedules.Back_Up_Running_Scheduler_Number);
				}
				
				if(_sSchedules.Running_Schedule_At == _sSchedules.Back_Up_Running_Scheduler_Number)
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("my 3");
						_kSERIAL_MON_CLEAR();
					}
					
					if(_sSchedules.Schedule_Num_List[_sSchedules.Back_Up_Running_Scheduler_Number-1] != _sSchedules.Back_Up_Running_Schedule_Plot_Number
					|| _sSchedules.Running_Schedule_At == _kRESET)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("my 4");
							_kSERIAL_MON_CLEAR();
						}
						if(_sSchedules.Running_Schedule_At != _kRESET)
						{
							_sSchedules.Which_Sheduler_Is_Running = _sSchedules.Back_Up_Running_Scheduler_Number;
						}
						else
						{
							_sSchedules.Which_Sheduler_Is_Running = _kRESET;
						}
						
						_sSchedules.Back_Up_Running_Scheduler_Number = _kRESET;
						_sSchedules.Back_Up_Running_Schedule_Plot_Number = _kRESET;
						_sSchedules.Back_Up_Pump_Running_Time = _kRESET;
						
						for(unsigned char fert_tank=0; fert_tank < _kMAX_FERTILIZER_TANK;fert_tank++)
						{
							_sSchedules.Back_Up_Tank_Running_Time[fert_tank] = _kRESET;
						}
				
						for(unsigned char tank_number=0; tank_number<_kMAX_FERTILIZER_TANK; tank_number++)
						{
							_sSchedules.Tank_Running_Timer[tank_number] = _kRESET;
						}
				
						_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
						for(unsigned char tank_number=0; tank_number < _kMAX_FERTILIZER_TANK; tank_number++)
						{
							_sSchedules.Running_Tank[tank_number] = _kOFF;
						}
					}
					
					if(_sSchedules.Running_Schedule_At > 1)
					{
						for(unsigned char index_num=0; index_num<_sSchedules.Running_Schedule_At-1; index_num++)
						{
							_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Started_Date;
							_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Started_Month;
							
							for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
							{
								_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Started_Date;
							}
						}
						for(unsigned char index_num=_sSchedules.Running_Schedule_At-1; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++ )
						{
							_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Irrigating_Date;
							_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Irrigating_Month;
							
							for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
							{
								_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Irrigating_Date;
							}
						}
						_sSchedules.First_Schedule_Irrigated_Date = _sSchedules.Started_Date;
						_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Started_Month;
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("_sSchedules.First_Schedule_Irrigated_Date 3//// :");
							_kSERIAL_MON_PRINT_NUM(_sSchedules.First_Schedule_Irrigated_Date);
							_kSERIAL_MON_CLEAR();
						}
					}
					else if(_sSchedules.Running_Schedule_At == 1)
					{
						Return_Calculated_Previous_Day_Date(1);
						for(unsigned char index_num=0; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++)
						{
							_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Temp_date;
							_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Temp_Month;
							
							for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
							{
								_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Temp_date;
							}
						}
						_sSchedules.Irrigating_Date = _sSchedules.Temp_date;
						_sSchedules.Irrigating_Month = _sSchedules.Temp_Month;
					}
				}
				else 
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("my 5");
						_kSERIAL_MON_CLEAR();
					}
					if(_sSchedules.Running_Schedule_At < _sSchedules.Back_Up_Running_Scheduler_Number)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("my 6");
							_kSERIAL_MON_CLEAR();
						}
						
						_sSchedules.Back_Up_Running_Scheduler_Number = _sSchedules.Running_Schedule_At;
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("my 6.1");
							_kSERIAL_MON_CLEAR();
						}
						if(_sSchedules.Schedule_Num_List[_sSchedules.Back_Up_Running_Scheduler_Number-1] != _sSchedules.Back_Up_Running_Schedule_Plot_Number
						|| _sSchedules.Running_Schedule_At == _kRESET)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("my 6.2");
								_kSERIAL_MON_CLEAR();
							}
							if(_sSchedules.Running_Schedule_At != _kRESET)
							{
								_sSchedules.Which_Sheduler_Is_Running = _sSchedules.Back_Up_Running_Scheduler_Number;
							}
							else
							{
								_sSchedules.Which_Sheduler_Is_Running = _kRESET;
							}
							
							_sSchedules.Back_Up_Running_Scheduler_Number = _kRESET;
							_sSchedules.Back_Up_Running_Schedule_Plot_Number = _kRESET;
							_sSchedules.Back_Up_Pump_Running_Time = _kRESET;
							for(unsigned char fert_tank=0; fert_tank < _kMAX_FERTILIZER_TANK;fert_tank++)
							{
								_sSchedules.Back_Up_Tank_Running_Time[fert_tank] = _kRESET;
							}
							
							for(unsigned char tank_number=0; tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
							{
								_sSchedules.Tank_Running_Timer[tank_number] = _kRESET;
							}
							
							_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
							for(unsigned char tank_number=0; tank_number < _kMAX_FERTILIZER_TANK;tank_number++)
							{
								_sSchedules.Running_Tank[tank_number] = _kOFF;
							}
						}
						
						if(_sSchedules.Running_Schedule_At > 1)
						{
							for(unsigned char index_num=0; index_num<_sSchedules.Running_Schedule_At-1; index_num++)
							{
								_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Started_Date;
								_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Started_Month;
								
								for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
								{
									_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Started_Date;
								}
							}
							for(unsigned char index_num=_sSchedules.Running_Schedule_At-1; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++ )
							{
								_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Irrigating_Date;
								_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Irrigating_Month;
								
								for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
								{
									_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Irrigating_Date;
								}
							}
							
							
							_sSchedules.First_Schedule_Irrigated_Date = _sSchedules.Started_Date;
							_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Started_Month;
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("_sSchedules.First_Schedule_Irrigated_Date 1//// :");
								_kSERIAL_MON_PRINT_NUM(_sSchedules.First_Schedule_Irrigated_Date);
								_kSERIAL_MON_CLEAR();
							}
						}
						else if(_sSchedules.Running_Schedule_At == 1)
						{
							Return_Calculated_Previous_Day_Date(1);
							for(unsigned char index_num=0; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++)
							{
								_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Temp_date;
								_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Temp_Month;
								
								for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
								{
									_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Temp_date;
								}
							}
							
							_sSchedules.Irrigating_Date = _sSchedules.Temp_date;
							_sSchedules.Irrigating_Month = _sSchedules.Temp_Month;
						}
						
					}
					
					else if(_sSchedules.Running_Schedule_At > _sSchedules.Back_Up_Running_Scheduler_Number)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("my 7");
							_kSERIAL_MON_CLEAR();
						}
						_sSchedules.Back_Up_Running_Scheduler_Number = _sSchedules.Running_Schedule_At;
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("my 8");
							_kSERIAL_MON_CLEAR();
						}
						if(_sSchedules.Schedule_Num_List[_sSchedules.Back_Up_Running_Scheduler_Number-1] != _sSchedules.Back_Up_Running_Schedule_Plot_Number
						|| _sSchedules.Running_Schedule_At == _kRESET)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("my 9");
								_kSERIAL_MON_CLEAR();
							}
							if(_sSchedules.Running_Schedule_At != _kRESET)
							{
								_sSchedules.Which_Sheduler_Is_Running = _sSchedules.Back_Up_Running_Scheduler_Number;
							}
							else
							{
								_sSchedules.Which_Sheduler_Is_Running = _kRESET;
							}
							
							_sSchedules.Back_Up_Running_Scheduler_Number = _kRESET;
							_sSchedules.Back_Up_Running_Schedule_Plot_Number = _kRESET;
							_sSchedules.Back_Up_Pump_Running_Time = _kRESET;
							for(unsigned char fert_tank=0; fert_tank < _kMAX_FERTILIZER_TANK;fert_tank++)
							{
								_sSchedules.Back_Up_Tank_Running_Time[fert_tank] = _kRESET;
							}
							
							for(unsigned char tank_number=0; tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
							{
								_sSchedules.Tank_Running_Timer[tank_number] = _kRESET;
							}
							
							_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
							for(unsigned char tank_number=0; tank_number < _kMAX_FERTILIZER_TANK;tank_number++)
							{
								_sSchedules.Running_Tank[tank_number] = _kOFF;
							}
						}
						
						if(_sSchedules.Running_Schedule_At > 1)
						{
							for(unsigned char index_num=0; index_num<_sSchedules.Running_Schedule_At-1; index_num++)
							{
								_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Started_Date;
								_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Started_Month;
								
								for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
								{
									_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Started_Date;
								}
							}
							
							for(unsigned char index_num=_sSchedules.Running_Schedule_At-1; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++ )
							{
								_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Irrigating_Date;
								_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Irrigating_Month;
								
								for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
								{
									_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Irrigating_Date;
								}
							}
							
							_sSchedules.First_Schedule_Irrigated_Date = _sSchedules.Started_Date;
							_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Started_Month;
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("_sSchedules.First_Schedule_Irrigated_Date 4//// :");
								_kSERIAL_MON_CLEAR();
								_kSERIAL_MON_PRINT_NUM(_sSchedules.First_Schedule_Irrigated_Date);
							}
						}
						else if(_sSchedules.Running_Schedule_At == 1)
						{
							Return_Calculated_Previous_Day_Date(1);
							for(unsigned char index_num=0; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++)
							{
								_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Temp_date;
								_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Temp_Month;
								
								for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
								{
									_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Temp_date;
								}
							}
							
							_sSchedules.Irrigating_Date = _sSchedules.Temp_date;
							_sSchedules.Irrigating_Month = _sSchedules.Temp_Month;
						}
					}
				} 
			}
		}
	}
	
	else if((_sSchedules.Running_Schedule_At == 0 && _sSchedules.Back_Up_Running_Scheduler_Number != _kRESET)
	|| (_sSchedules.Running_Schedule_At == 0 && _sSchedules.Scheduler_Retry_Timer == _kRESET))
	{
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("my 14");
			_kSERIAL_MON_CLEAR();
		}
		_sSchedules.Back_Up_Running_Scheduler_Number = _kRESET;
		_sSchedules.Back_Up_Running_Schedule_Plot_Number = _kRESET;
		_sSchedules.Back_Up_Pump_Running_Time = _kRESET;
		for(unsigned char fert_tank=0; fert_tank < _kMAX_FERTILIZER_TANK;fert_tank++)
		{
			_sSchedules.Back_Up_Tank_Running_Time[fert_tank] = _kRESET;
		}
		
		for(unsigned char tank_number = 0; tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
		{
			_sSchedules.Tank_Running_Timer[tank_number] = _kRESET;
		}
		
		_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
		for(unsigned char tank_number = 0; tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
		{
			_sSchedules.Running_Tank[tank_number] = _kOFF;
		}
		
		Return_Calculated_Previous_Day_Date(1);
		
		for(unsigned char index_num = 0; index_num<_kMAX_LIRA_SENSOR_NODE; index_num++)
		{
			
			_sSchedules.Scheduler_Already_Run[index_num] =  _sSchedules.Temp_date;
			_sSchedules.Scheduler_Already_Run_Month[index_num] = _sSchedules.Temp_Month;
			
			for(unsigned char tank_number=0;tank_number<_kMAX_FERTILIZER_TANK;tank_number++)
			{
				_sSchedules.Tank_Running_Date[((index_num) * _kMAX_FERTILIZER_TANK)+tank_number] = _sSchedules.Temp_date;
			}
		}
		if(_sRuble_Parameters.Debug_Mode == _kSET)
		{
			_kSERAIL_MON_WRITE("irrigating date upadte 14///////////////////////////////////////////");
		}
		_sSchedules.Irrigating_Date = _sSchedules.Temp_date;
		_sSchedules.Irrigating_Month = _sSchedules.Temp_Month;
		/* add previous dates here */

	}
	
	/* added the plot which is after the irrigating plot */
	/* added the plot which is before the irrigating plot */	
}

unsigned char Do_Not_Repeat_Schedule(void)
{
	unsigned char check_for_repeat;
	
	if(_sSchedules.Cyclic_Schedule_Flag != _kSET)
	{
		if(_sSchedules.Which_Sheduler_Is_Running)
		{
			if(_sSchedules.Which_Sheduler_Is_Running == 1)
			{
				
				check_for_repeat = Next_Day_Calculate(_sSchedules.Started_Date,_sSchedules.Started_Month);
			}
			else
			{
				check_for_repeat = Next_Day_Calculate(_sSchedules.First_Schedule_Irrigated_Date,_sSchedules.First_Schedule_Irrigated_Month);
			}
			
			if(check_for_repeat == 1)
			{
				if(_sSchedules.Trigger_Time < _sRuble_Parameters.Current_Time_In_Sec)
				{
					if(_sSchedules.Which_Sheduler_Is_Running == 1)
					{
						/*for(unsigned char index_num=0; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++)
						{
							_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.Started_Date;
							_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.Started_Month;
						}*/
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Do_Not_Repeat_Schedule 1 ##############################");
							_kSERIAL_MON_CLEAR();
						}
						_sSchedules.Keep_Scheduler_Run_Timer = _sSchedules.Running_Hr_Of_Each_Plot[_sSchedules.Which_Sheduler_Is_Running-1] +1;
						return 0;
					}
					else
					{
						for(unsigned char index_num=0; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++)
						{
							_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.First_Schedule_Irrigated_Date;
							_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.First_Schedule_Irrigated_Month;
						}
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("Do_Not_Repeat_Schedule 1.1 ##############################");
							_kSERIAL_MON_CLEAR();
						}
					}
					
					return 1;
				}
			}
			if(check_for_repeat == 2)
			{
				for(unsigned char index_num=0; index_num<_kMAX_LIRA_SENSOR_NODE+1; index_num++)
				{
					_sSchedules.Scheduler_Already_Run[index_num]        = _sSchedules.First_Schedule_Irrigated_Date;
					_sSchedules.Scheduler_Already_Run_Month[index_num]  = _sSchedules.First_Schedule_Irrigated_Month;
				}
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("Do_Not_Repeat_Schedule 2 ##############################");
					_kSERIAL_MON_CLEAR();
				}
				return 1;
			}
			
			return 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

unsigned char Next_Day_Calculate(unsigned char date,unsigned char month)
{
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("Current date 1: ");
		_kSERIAL_MON_PRINT_NUM(date);
		_kSERIAL_MON_CLEAR();
	}
	
	date = date + _sSchedules.Days_In_Between_Schedules;
	unsigned char c_month = month;
	unsigned char year = _sRuble_Parameters.Update_Local_Date[2];
	
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("Current date 2: ");
		_kSERIAL_MON_PRINT_NUM(date);
		_kSERIAL_MON_CLEAR();
		
		_kSERAIL_MON_WRITE("diays in beteweeen .............: ");
		_kSERIAL_MON_PRINT_NUM(_sSchedules.Days_In_Between_Schedules);
		_kSERIAL_MON_CLEAR();
	}
	
	switch(month)
	{
		case _eJAN:
		{
			if(date > 31)
			{
				date = date - 31;
				if(( _sRuble_Parameters.Update_Local_Date[0] >=   date&& c_month == _eFEB) ||
				(c_month != _eJAN &&  c_month != _eFEB))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eFEB)
					{
						return 1;
					}
					else
					{
						return 2;
					}
					
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >=  date)
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eFEB:
		{
			if(year%4)
			{
				if(date > 28)
				{
					date = date - 28;
					if((_sRuble_Parameters.Update_Local_Date[0] >= date && c_month == _eMARCH) ||
					(c_month != _eFEB &&  c_month != _eMARCH))
					{
						if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eMARCH)
						{
							return 1;
						}
						else
						{
							return 2;
						}
					}
				}
				else
				{
					if(_sRuble_Parameters.Update_Local_Date[0] >=  date)
					{
						if(date ==  _sRuble_Parameters.Update_Local_Date[0])
						{
							return 1;
						}
						else
						{
							return 2;
						}
					}
				}
			}
			else
			{
				if(date > 29)
				{
					date = date - 29;
					if(( _sRuble_Parameters.Update_Local_Date[0] >= date && c_month == _eMARCH) ||
					(c_month != _eFEB &&  c_month != _eMARCH))
					{
						if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eMARCH)
						{
							return 1;
						}
						else
						{
							return 2;
						}
					}
				}
				else
				{
					if( _sRuble_Parameters.Update_Local_Date[0] >=   date)
					{
						if(date ==  _sRuble_Parameters.Update_Local_Date[0])
						{
							return 1;
						}
						else
						{
							return 2;
						}
					}
				}
			}
			
		}break;
		
		case _eMARCH:
		{
			if(date > 31)
			{
				date = date - 31;
				if((_sRuble_Parameters.Update_Local_Date[0] >= date  && c_month == _eAPR) ||
				(c_month != _eMARCH &&  c_month != _eAPR))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eAPR)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date)
				{		
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eAPR:
		{
			if(date > 30)
			{
				date = date - 30;
				if((_sRuble_Parameters.Update_Local_Date[0] >=  date&& c_month == _eMAY) ||
				(c_month != _eAPR &&  c_month != _eMAY))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eMAY)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date)
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eMAY:
		{
			if(date > 31)
			{
				date = date - 31;
				if((_sRuble_Parameters.Update_Local_Date[0] >= date&& c_month == _eJUN) ||
				(c_month != _eMAY &&  c_month != _eJUN))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eJUN)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date)
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eJUN:
		{
			if(date > 30)
			{
				date = date - 30;
				if((date >=  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eJUL) ||
				(c_month != _eJUN &&  c_month != _eJUL))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eJUL)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0]  >=  date)
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eJUL:
		{
			if(date > 31)
			{
				date = date - 31;
				if((_sRuble_Parameters.Update_Local_Date[0] >= date  && c_month == _eAUG) ||
				(c_month != _eJUL &&  c_month != _eAUG))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eAUG)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date)
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eAUG:
		{
			if(date > 31)
			{
				date = date - 31;
				if((_sRuble_Parameters.Update_Local_Date[0] >= date  && c_month == _eSEP) ||
				(c_month != _eAUG &&  c_month != _eSEP))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eSEP)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date )
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eSEP:
		{
			if(date > 30)
			{
				date = date - 30;
				if((_sRuble_Parameters.Update_Local_Date[0] >= date  && c_month == _eOCT) ||
				(c_month != _eSEP &&  c_month != _eOCT))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eOCT)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date)
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eOCT:
		{
			if(date > 31)
			{
				date = date - 31;
				if((_sRuble_Parameters.Update_Local_Date[0] >= date  && c_month == _eNOV) ||
				(c_month != _eOCT &&  c_month != _eNOV))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eNOV)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date )
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eNOV:
		{
			if(date > 30)
			{
				date = date - 30;
				if((_sRuble_Parameters.Update_Local_Date[0] >= date  && c_month == _eDEC) ||
				(c_month != _eNOV &&  c_month != _eDEC))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eDEC)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date )
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
		case _eDEC:
		{
			if(date > 31)
			{
				date = date - 31;
				if((_sRuble_Parameters.Update_Local_Date[0] >=  date && c_month == _eJAN) ||
				(c_month != _eDEC &&  c_month != _eJAN))
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0] && c_month == _eJAN)
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Update_Local_Date[0] >= date)
				{
					if(date ==  _sRuble_Parameters.Update_Local_Date[0])
					{
						return 1;
					}
					else
					{
						return 2;
					}
				}
			}
			
		}break;
		
	}
	
	return 0;
	
}

unsigned char Check_Time_For_Schedule_To_Trigger(unsigned char Case_Number, unsigned char check_days)
{
	_sSchedules.Shedule_Run_Status_Is_Triggered = _kSET;   /* turn on the last schedule or new schedule */
	
	if(_sSchedules.Which_Sheduler_Is_Running == _kRESET)
	{
		//_kSERAIL_MON_WRITE("sc enter 28");
		_sSchedules.Which_Sheduler_Is_Running = _kSET;
		_sSchedules.Keep_Scheduler_Run_Timer = _kRESET;
	}
	
	/* check weather schedules are already started save in eeprom */
	if(_sSchedules.Schedule_Are_Started == _kRESET || (_sSchedules.Which_Sheduler_Is_Running == 1))
	{
		//_kSERAIL_MON_WRITE("sc enter 29");
		_sSchedules.Schedule_Are_Started = _kSET;
		
		if(_sSchedules.Based_On_Trigger_Time == _kSET)
		{
			if(_sSchedules.Trigger_Time < _kMAX_TIME_IN_SEC && Case_Number == 1)
			{
				//_kSERAIL_MON_WRITE("sc enter 30");
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("_sSchedules.Started_Date upadte 4///////////////////////////////////////////");
					_kSERIAL_MON_CLEAR();
				}
				if((_sSchedules.Which_Sheduler_Is_Running == 1) && (_sSchedules.Keep_Scheduler_Run_Timer == 0))
				{
					_sSchedules.Started_Date = _sRuble_Parameters.Update_Local_Date[0];
					_sSchedules.Started_Month = _sRuble_Parameters.Update_Local_Date[1];
					_sSchedules.Pump_On_In_Scheduler = _kRESET;
				}
				
				_kSERIAL_MON_PRINT_NUM(_sSchedules.Started_Date);
			}
			else
			{
				//_kSERAIL_MON_WRITE("sc enter 31");
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("irrigating date upadte 5///////////////////////////////////////////");
					_kSERIAL_MON_CLEAR();
				}
				if((_sSchedules.Which_Sheduler_Is_Running == 1) && (_sSchedules.Keep_Scheduler_Run_Timer == 0))
				{
					_sSchedules.Started_Date = Return_Previous_Day_Date();
					_sSchedules.Pump_On_In_Scheduler = _kRESET;
				}
				
			}
		}
		else
		{
			if(_sSchedules.Which_Sheduler_Is_Running == 1 && _sSchedules.Keep_Scheduler_Run_Timer == 0)
			{
				//_sSchedules.Trigger_Time = (unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[0]*3600)+(unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[1]*60) +(unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[2]);
				
				if(_sSchedules.Trigger_Time < 600)
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("sc enter 11.1.0.0.0");
						_kSERIAL_MON_CLEAR();
					}
					_sSchedules.Trigger_Time = 600;
					_sSchedules.Trigger_Reset = _sSchedules.Trigger_Time - _kMAX_BUFFER_TIME;
				}
				else
				{
					if(_sSchedules.Trigger_Time <  _kMAX_BUFFER_TIME)
					{
						_sSchedules.Trigger_Reset = _kMAX_TIME_IN_SEC - _kMAX_BUFFER_TIME;
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sc enter 11.1.0.1");
							_kSERIAL_MON_CLEAR();
						}
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sc enter 11.2.0.2");
							_kSERIAL_MON_CLEAR();
						}
						_sSchedules.Trigger_Reset = _sSchedules.Trigger_Time - _kMAX_BUFFER_TIME;
					}
				}
			}
			
			if(_sSchedules.Trigger_Time < _kMAX_TIME_IN_SEC && Case_Number == 1)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("sc enter 30.1.1");
					_kSERAIL_MON_WRITE("_sSchedules.Started_Date upadte 4///////////////////////////////////////////");
					_kSERIAL_MON_CLEAR();
				}
				if((_sSchedules.Which_Sheduler_Is_Running == 1) && (_sSchedules.Keep_Scheduler_Run_Timer == 0))
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("_sSchedules.Started_Date upadte 4.1///////////////////////////////////////////");
						_kSERIAL_MON_CLEAR();
					}
					
					_sSchedules.Started_Date = _sRuble_Parameters.Update_Local_Date[0];
					_sSchedules.Started_Month = _sRuble_Parameters.Update_Local_Date[1];
					
					_sSchedules.First_Schedule_Irrigated_Date  = _sSchedules.Started_Date;
					_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Started_Month;
					_sSchedules.Pump_On_In_Scheduler = _kRESET;
				}
				
				_kSERIAL_MON_PRINT_NUM(_sSchedules.Started_Date);
			}
			else
			{
				//_kSERAIL_MON_WRITE("sc enter 31.1.2");
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("irrigating date upadte 5///////////////////////////////////////////");
					_kSERIAL_MON_CLEAR();
				}
				if((_sSchedules.Which_Sheduler_Is_Running == 1) && (_sSchedules.Keep_Scheduler_Run_Timer == 0))
				{
					if(check_days == 2)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("_sSchedules.Started_Date upadte 5.1///////////////////////////////////////////");
							_kSERIAL_MON_CLEAR();
						}
						
						_sSchedules.Started_Date = _sRuble_Parameters.Update_Local_Date[0];
						_sSchedules.Started_Month = _sRuble_Parameters.Update_Local_Date[1];
						
						_sSchedules.First_Schedule_Irrigated_Date  = _sSchedules.Started_Date;
						_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Started_Month;
						_sSchedules.Pump_On_In_Scheduler = _kRESET;
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("irrigating date upadte 5.2///////////////////////////////////////////");
							_kSERIAL_MON_CLEAR();
						}
						_sSchedules.Started_Date = Return_Previous_Day_Date();
						_sSchedules.First_Schedule_Irrigated_Date  = _sSchedules.Started_Date;
						_sSchedules.First_Schedule_Irrigated_Month = _sSchedules.Started_Month;
						_sSchedules.Pump_On_In_Scheduler = _kRESET;
					}
					
				}
			}
		}
		/* save in eeprom */
	}
	
	_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
}

unsigned char Compare_Trigger_And_Reset_Time(void)
{
	unsigned char check_true_condition = 0;
	
	if(_sSchedules.Based_On_Trigger_Time == _kSET)
	{
		if(_sSchedules.New_Schedul_Date == _sRuble_Parameters.Update_Local_Date[0])
		{
			if(_sSchedules.Trigger_Time < _sRuble_Parameters.Current_Time_In_Sec)
			{
				//_kSERAIL_MON_WRITE("sc enter 21.0.1");
				_sSchedules.New_Schedul_Date = _kRESET;
				check_true_condition = 1;
			}
			else
			{
				//_kSERAIL_MON_WRITE("sc enter 21.0.1");
				check_true_condition = 0;
			}
		}
		else
		{
			/* check which is the current date */
			//_kSERAIL_MON_WRITE("sc enter 21.1");
			if(_sSchedules.all_schedule_done_flag == _eSET)
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("sc enter 22");
					_kSERIAL_MON_CLEAR();
				}
				
				check_true_condition = Day_In_Bet_Schedulling(_sSchedules.Days_In_Between_Schedules);
				
				if(check_true_condition == 2)
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("sc enter 23");
						_kSERIAL_MON_CLEAR();
					}
					
					check_true_condition = 2;
					_sSchedules.all_schedule_done_flag = _eNOT_SET;
					_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
				}
				else
				{
					if(check_true_condition == 1)
					{
						if(_sSchedules.Trigger_Time < _kMAX_TIME_IN_SEC)
						{
							//_kSERAIL_MON_WRITE("sc enter 24");
							check_true_condition = 1;
							_sSchedules.all_schedule_done_flag = _eNOT_SET;
							_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
						}
						else
						{
							//_kSERAIL_MON_WRITE("sc enter 25");
							check_true_condition = 0;
						}
					}
					else
					{
						//_kSERAIL_MON_WRITE("sc enter 25.0");
						check_true_condition = 0;
					}
				}
			}
			else
			{
				//_kSERAIL_MON_WRITE("sc enter 25.1");
				check_true_condition = 1;
			}
		}
	}
	
	
	
	
	
	
	else
	{
		if((_sSchedules.Based_On_Trigger_Time == _eBASED_ON_ELECTRICITY) &&
		(_sSchedules.Which_Sheduler_Is_Running == 0) && (_sRuble_Parameters.Electricity_Status == _kON)
		&& _sSchedules.Back_Up_Pump_Running_Time == _kRESET && _sSchedules.Trigger_Time == _kRESET)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("sc 1 enter 12.0");
				_kSERIAL_MON_CLEAR();
			}
			check_true_condition = 1;
		}
		else
		{
			if(_sRuble_Parameters.Electricity_Status == _kON)
			{
				if(_sSchedules.New_Schedul_Date == _sRuble_Parameters.Update_Local_Date[0])
				{
					if(_sSchedules.Trigger_Time < _sRuble_Parameters.Current_Time_In_Sec)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sc 1 enter 12.0.0.1");
							_kSERIAL_MON_CLEAR();
						}
						_sSchedules.New_Schedul_Date = _kRESET;
						check_true_condition = 1;
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sc 1 enter 12.0.0.2");
							_kSERIAL_MON_CLEAR();
						}
						check_true_condition = _kRESET;
					}
				}
				else
				{
					if(_sRuble_Parameters.Debug_Mode == _kSET)
					{
						_kSERAIL_MON_WRITE("sc 1 enter 12");
						_kSERIAL_MON_CLEAR();
					}
					if(_sSchedules.all_schedule_done_flag == _eSET)
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sc 1 enter 13");
							_kSERIAL_MON_CLEAR();
						}
						check_true_condition = Day_In_Bet_Schedulling(_sSchedules.Days_In_Between_Schedules);
							
						if(check_true_condition == 2)
						{
							if(_sRuble_Parameters.Debug_Mode == _kSET)
							{
								_kSERAIL_MON_WRITE("sc 1 enter 14");
								_kSERIAL_MON_CLEAR();
							}
							check_true_condition = 2;
							_sSchedules.all_schedule_done_flag = _eNOT_SET;
							_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
						}
						else
						{
							if(check_true_condition == 1)
							{
								if(_sSchedules.Trigger_Time < _kMAX_TIME_IN_SEC)
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("sc 1 enter 15");
										_kSERIAL_MON_CLEAR();
									}
									check_true_condition = 1;
									_sSchedules.all_schedule_done_flag = _eNOT_SET;
									_kEEPROM_WRITE(_kEEPROM_SCHEDULE_DATA, _sSchedules);
								}
								else
								{
									if(_sRuble_Parameters.Debug_Mode == _kSET)
									{
										_kSERAIL_MON_WRITE("sc 1 enter 15.1");
										_kSERIAL_MON_CLEAR();
									}
									check_true_condition = 0;
								}
							}
							else
							{
								if(_sRuble_Parameters.Debug_Mode == _kSET)
								{
									_kSERAIL_MON_WRITE("sc 1 enter 15.2");
									_kSERIAL_MON_CLEAR();
								}
								check_true_condition = 0;
							}
						}
					}
					else
					{
						if(_sRuble_Parameters.Debug_Mode == _kSET)
						{
							_kSERAIL_MON_WRITE("sc 1 enter 15.3");
							_kSERIAL_MON_CLEAR();
						}
						check_true_condition = 1;
					}
				}
			}
			else
			{
				if(_sRuble_Parameters.Debug_Mode == _kSET)
				{
					_kSERAIL_MON_WRITE("sc 1 enter 12.0.0.4");
					_kSERIAL_MON_CLEAR();
				}
				check_true_condition = _kRESET;
			}
		}
	}
	if(_sRuble_Parameters.Debug_Mode == _kSET)
	{
		_kSERAIL_MON_WRITE("check_true_condition");
		_kSERIAL_MON_PRINT_NUM(check_true_condition);
		_kSERIAL_MON_CLEAR();
	}
	return check_true_condition;
}

unsigned char Set_Trigger_Time(void)
{
	unsigned char i = 0;
	
	if(_sSchedules.Based_On_Trigger_Time == _eBASED_ON_ELECTRICITY)
	{
		for(i=0; _sSchedules.Schedule_Status[i]>0; i++)
		{
			break;
		}
		
		if(_sSchedules.Which_Sheduler_Is_Running-1 == i && _sSchedules.Keep_Scheduler_Run_Timer == 0)
		{
			if(_sRuble_Parameters.Debug_Mode == _kSET)
			{
				_kSERAIL_MON_WRITE("trigger time set");
				_kSERIAL_MON_CLEAR();
			}
			_sSchedules.Trigger_Time = _sRuble_Parameters.Current_Time_In_Sec;
			//_sSchedules.Trigger_Time = (unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[0]*3600)+(unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[1]*60) +(unsigned long int)((unsigned long int)_sRuble_Parameters.Update_Local_Time[2]);
		}
	}	
}



