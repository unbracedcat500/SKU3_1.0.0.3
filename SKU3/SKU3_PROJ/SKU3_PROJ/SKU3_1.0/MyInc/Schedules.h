/*
 * Shcedules.h
 *
 * Created: 14-03-2022 20:41:42
 *  Author: Tushar (SIO)
 */ 


#ifndef SHCEDULES_H_
#define SHCEDULES_H_

#include <Lira_Sensor.h>

#define  _kSCHEDULE_RETRY_TIMER                 10*60
#define  _kNUMBER_OF_SCHEDULES                 _kMAX_LIRA_SENSOR_NODE
#define _kMAX_FERTILIZER_TANK                    3
#define  _kTANK_RETRY_TIME                      2*60
#define _kMAX_BLOWER_TIME                        90
#define _kMAX_BLOWER_RETRY_TIME                  180
#define _kMAX_BLOWER_REPEAT_TIME                 30*60
  


typedef struct Schedules
{
	unsigned char Schedule_Enable;      //1
	unsigned char New_Schedules_Occur;  //1
	unsigned char Shedule_Run_Status;   //1
	unsigned char Which_Sheduler_Is_Running;        //1
	unsigned int  Scheduler_Retry_Timer;            //2
	unsigned char Shedule_Run_Status_Is_Triggered;  //1
	unsigned char All_Schedule_Run_Flag;            //1
	unsigned int  Keep_Scheduler_Run_Timer;         //2
	unsigned char Irrigating_Date;                  //1
	unsigned char Irrigating_Month;                 //1
	unsigned char Started_Date;
	unsigned char Started_Month;                    //1
	unsigned char Schedule_Are_Started;             //1
	unsigned long int Flushing_Time;                //4
	
	unsigned long int Trigger_Time;	//1		/* schedule trigger time */
	unsigned long int Trigger_Reset;                //1
	unsigned char Based_On_Trigger_Time;            //1
	unsigned char Days_In_Between_Schedules;        //1
	unsigned char Cyclic_Schedule_Flag;             //1
	unsigned char all_schedule_done_flag;           //1
	 
	unsigned int  Running_Hr_Of_Each_Plot[_kMAX_LIRA_SENSOR_NODE+1]; //38 /**< each schedule run time */ 
	unsigned char Schedule_Status[_kMAX_LIRA_SENSOR_NODE+1];         //19 /**< what are the status of schedule */
	unsigned char Schedule_Num_List[_kMAX_LIRA_SENSOR_NODE+1];       //19 /**< save the list of schedule number list */
	unsigned char Scheduler_Already_Run[_kMAX_LIRA_SENSOR_NODE+1];   //19 /**< save the date when schedule was run */
	unsigned char Scheduler_Already_Run_Month[_kMAX_LIRA_SENSOR_NODE+1]; //19
	unsigned char Pump_On_In_Scheduler;  //1                            /**< pump on after schedule trigger */
	
	unsigned char Fertilizer_Tank_Number[_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1];   //55
	unsigned int  Fertilizer_Tank_On_After[_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1]; //55*2
	unsigned int  Tank_Off_Time_Duration[_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1];   //55*2
	unsigned char Fertilizer_Tank_Status[_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1];   //55
	unsigned char Already_Fertilze_Status[_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1];  //55
	
	unsigned int  Tank_Run_Time[_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1];     //55*2
	unsigned int  Tank_Running_Date[_kMAX_LIRA_SENSOR_NODE*_kMAX_FERTILIZER_TANK + 1]; //55*2
	
	unsigned char Tank_On_Retry[_kMAX_FERTILIZER_TANK+1]; //4
	unsigned char Fertilizer_On_Scheduler_Status;         //4
	unsigned int  Fertilizer_Tank_Run_Timer[_kMAX_FERTILIZER_TANK + 1];  // 4*2
	unsigned char Running_Tank[_kMAX_FERTILIZER_TANK+1];        //4 
	unsigned int  Tank_Running_Timer[_kMAX_FERTILIZER_TANK+1];  //4*2
	
	unsigned char Blower_On_Time;             //1
	unsigned char Blower_On_Scheduler_Status; //1
	unsigned char Blower_On_Retry;            //1
	unsigned char blower_run_status[_kMAX_FERTILIZER_TANK+1];  //1
	
	unsigned char Dummy_Schedule_Enable;            //1
	unsigned char Dummy_Cyclic_Schedule_Flag;       //1
	unsigned char Run_Program_In_How_Much_Day;      //1
	unsigned char Dummy_Days_In_Between_Schedules;  //1
	 
	unsigned long int Dummy_Trigger_Time;           //8
	unsigned char Dummy_Based_On_Trigger_Time;      //1
	
	unsigned int  Dummy_Running_Hr_Of_Each_Plot[_kMAX_LIRA_SENSOR_NODE+1]; // 19*2 /* each schedule run time */
	unsigned char Dummy_Schedule_Status[_kMAX_LIRA_SENSOR_NODE+1];    //19
	unsigned char Dummy_Schedule_Num_List[_kMAX_LIRA_SENSOR_NODE+1];  //19
	
	unsigned char Save_Schedule_Data_In_Eeprom; //1
	unsigned char New_Schedul_Date;             //1
	
	unsigned char Back_Up_Running_Scheduler_Number;     //1
	unsigned char Back_Up_Running_Schedule_Plot_Number; //1
	unsigned int  Back_Up_Pump_Running_Time;            //2
	unsigned int  Back_Up_Tank_Running_Time[_kMAX_FERTILIZER_TANK+1]; // 4*2
	unsigned char Schedule_Pause;            //1
	unsigned char Schedule_Pause_Resume;     //1
	unsigned char Running_Schedule_At;       //1
	unsigned char Dummy_Running_Schedule_At; //1
	
	unsigned int Flush_Schedule_In_Every;    //2
	unsigned int Flush_Schedule_For;         //2
	unsigned char Temp_date;                 //1 
	unsigned char Temp_Month;                //1
	
	unsigned char First_Schedule_Irrigated_Date;   //1
	unsigned char First_Schedule_Irrigated_Month;  //1
	
	unsigned char Pump_Selected_For_Schedule;      //1
	unsigned char Dummy_Pump_Selected_For_Schedule;
	
	unsigned char Schedule_Start_From_Today;
	unsigned char Dummy_Start_From_Today;
    
}Schedules;

enum _eSCHEDULE_SET_RESET
{
	_eNOT_SET = 1,
	_eSET
};


enum _eMONTHS
{
	_eJAN = 1,
	_eFEB,
	_eMARCH,
	_eAPR,
	_eMAY,
	_eJUN,
	_eJUL,
	_eAUG,
	_eSEP,
	_eOCT,
	_eNOV,
	_eDEC	
};






enum _eSCHEDULES
{
	_eBASED_ON_ELECTRICITY,
	_eBASED_ON_TRIGGER_TIME,
	
};


extern volatile Schedules _sSchedules;


void Schedules_Monitoring(void);

void Fertilizer_Monitoring(void);

char Fertilizer_On_Off(unsigned char on_off);

void Fertilizer_Tank_Monitoring(void);

char Fertilizer_Tank_On_Off(unsigned char tank_number, unsigned char on_off);

char Turn_Off_All_Tank_For_Safety(void);

char Doser_Pump_On_Off(unsigned char on_off);

char Turn_Off_Blower_For_Safety(void);

unsigned char Pause_Current_Schedule(unsigned char schedule_pause);

unsigned char Delete_Schedule(unsigned char Schedule_Number);

unsigned char Edited_In_Schedule(void);

unsigned char Return_Calculated_Previous_Day_Date(unsigned char days_in_bet_schedule);

//unsigned char Return_Calculated_Previous_Day_Date(unsigned char days_in_bet_schedule);

unsigned char Day_In_Bet_Schedulling(unsigned char Days_In_Between_Schedules, unsigned char last_schedule_date,
unsigned char last_schedule_month);

unsigned char Next_Day_Calculate(unsigned char date,unsigned char month);

unsigned char Do_Not_Repeat_Schedule(void);

unsigned char Check_Time_For_Schedule_To_Trigger(unsigned char Case_Number, unsigned char check_days);

unsigned char Compare_Trigger_And_Reset_Time(void);

unsigned char Set_Trigger_Time(void);

unsigned char Hard_Coded_Flush_Valve(unsigned char on_off);

#endif /* SHCEDULES_H_ */


/*
{
	"Sen": 0,
	"ScOn": [
	1000,
	2000,
	3000,
	4000,
	5000,
	6000,
	7000,
	8000,
	9000,
	1001,
	2001,
	2002
	],
	"ScOff": [
	1002,
	2000,
	3000,
	4000,
	5000,
	6000,
	7000,
	8000,
	9000,
	1001,
	2001,
	2002
	],
	"ScStat": [
	1,
	0,
	1,
	1,
	1,
	0,
	1,
	0,
	1,
	0,
	1,
	0
	],
	"ferOn": [
	1000,
	2000,
	3000,
	4000,
	5000,
	6000,
	7000,
	8000,
	9000,
	1001,
	2001,
	2002
	],
	"ferOff": [
	1000,
	2000,
	3000,
	4000,
	5000,
	6000,
	7000,
	8000,
	9000,
	1001,
	2001,
	2002
	],
	"ferStat": [
	1,
	0,
	1,
	1,
	1,
	0,
	1,
	0,
	1,
	0,
	1,
	0,
	1
	]
}
*/