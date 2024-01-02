/*
 * Initialize.h
 *
 * Created: 24-08-2021 10:34:20
 *  Author: Tushar (SIO)
 */ 

/*
 * Initialize.h
 *
 * Created: 23-08-2021 10:56:31
 *  Author: Tushar (SIO)
 */ 

#include <Arduino.h>
#include <Euro_Valve.h>
#include <Lira_Sensor.h>
#include <Hardware_Config.h>
#include <Lira_Sensor.h>
#include <Euro_Valve.h>
#include <MQTT_Handler.h>
#include <GSM_GPRS_Network.h>
//#include "My_Lora.h"
#include "Schedules.h"

#ifndef INITIALIZE_H_
#define INITIALIZE_H_


#define _kSET                                                 1
#define _kRESET                                               0

#define _kON                                                  1
#define _kOFF                                                 0
#define _kCHECK_TRY_TO_ON                                     2
#define _kCHECK_TRY_TO_OFF                                    3
#define _kFORCEFULYY_OFF                                      3
#define _kOFF_FOR_WIRELESS_SAFETY                             5
#define _kUNDEFINED                                           4 
#define  _kSTOP_IRRIGATION_WITH_PUMP                          5 
#define _kDO_CALIBRATION                                      6                                      


#define _kPUMP_ON                                             0
#define _kPUMP_OFF                                            1

#define _kVALVE_OFF                                           1
#define _kVALVE_ON                                            0

#define _kMAX_IRRIG_TIME                                      60 * 60      /**< 60 MINUTE */
#define _kDISPLAY_REFRESH_RATE                                4           /**< Refresh 3lcd in every 4 sec */
#define _kACK_LEN
#define _kMEDIUN_FILTER_LEN                                   25           /**< Median filter length */
#define _kMEDIUM_AVG_LEN                                      4            /**< find the avg len */    
#define _kMOVING_AVG_FRAME_SIZE                               10           /**< find the moving average len */

#define _kRUBLE_ID                                          "CB0002"       /**< store the Ruble ID */
#define _kRETRY_TO_SEND_VALVE                                 3
#define _kWAIT_DUE_TO_SERVER_NOT_RECHABLE                    1*30
#define _kRETRY_TO_CONNECT_SERVER                             3
#define _kCT_AVERAGE_TURNS                                   1480
#define _kCT_CALIBRATION_FACTOR                               6
#define _kCHECK_REGISTRATION                                  8    //8
#define _kSTART_MONITORING_PUMP_FAULTS_TIMER                  120
#define _kRESUME_OPERATION_FOGGER                            10*60

#define _kWIRED_PUMP_NUMBER                                   1

#define _kMAX_EXTRA_PLOT                                      14
#define _kFERTIIZER_PLOT_NUMBER                              101-100
#define _kEXTRA_PLOT_START_FROM                               101
#define _kBLOWER_PLOT_NUMBER                                  102
#define _kFAN_PLOT_NUMBER                                     103
#define _kFOOGER_PLOT_NUMBER                                  104
#define _kLIGHT_PLOT_NUMBER                                   105
#define _kWIRELESS_PUMP_ENABLE_DISABLE_PLOT                   109
#define _kFLUSH_PLOT_NUMBER                                   105



//#define _kBATTTERY_IS_AVAILABLE
#define _kRETRY_TIMER_IN_TIMER_MODE                           10*60 
#define _kTIMER_RESUME_ENABLE
#define _kTIMER_FEEDBACK_SEND_TIMER                            2*60
#define _kBLOWER_ON_TIME                                        90
#define _kEXTRA_PLOT_RESEND_TIMER                              1*60
#define _kMAX_FLUSH_RUNNING_TIME                               2*60
/************************************* EEPROM LOCATION *******************************************/

#define _kEEPROM_LOC_RUBLE_ID                                   0                 /**< ID 6 BYTE*/
#define _kEEPROM_LOC_WP_VALUE                                   6
#define _kEEPROM_LOC_FC_VALUE                                   7
#define _kEEPROM_LOC_LIRA_SENSOR_DATA(L)                   (L-1)*40 + 8
#define _kEEPROM_LOC_EURO_VALVE_DATA(L)                    (L-1)*20 + 850
#define _kEEPROM_LOC_FIRMWARE_VERSION                         1102                /**< 6 BYTES */
#define _kEEPROM_LOC_HARDWARE_VESRION                         1111                /**< 6 BYTES */
#define _kEEPROM_LOC_OPERATION_MODE                           1120
#define _kEEPROM_LOC_RUBLE_SIM_NUM                            1122
#define _kEEPROM_LOC_USER_SIM_NUM                             1135
#define _kEEPROM_LOC_READ_REGISTRATION                        1148
#define _kEEPROM_LOC_COEFF_1                                  1149
#define _kEEPROM_LOC_COEFF_2                                  1157
#define _kEEPROM_LOC_COEFF_3                                  1166
#define _kEEPROM_LOC_PUMP_NOMINAL_CURRENT                     1175
#define _kEEPROM_SMS_ALERT_START                              1184
#define _kEEPROM_KILL_SWITCH                                  1185
#define _kEEPROM_SCHEDULE_DATA                                1186

#define _kEEPROM_CHOOSE_OPERATION                             2420

#define _kEEPROM_EXTRA_PLOT_DATA(L)                       (L-1)*30 + 2424

#define _kKILL_SWITCH_THROUGH_APP                              3100
#define _kTIMER_IRRIGATING_PLOT                                3102
#define _kPUMP_REMAINING_TIME                                  3104
#define _kCT_BY_PASS                                           3113
#define _kEEPROM_LOC_WIRELESS_PUMP_COMMAND                     3120
#define _kEEPROM_LOC_WIRELESS_PUMP_STATUS                      3125
#define _kWIRELESS_PUMP_DATA_SAVE(L)                           (L)*20 + 3126
#define _kHANDLE_WIRELESS_STATUS_LIST                          3600   /* 11 BYTES */
#define _kPUMP_SELECTED_FOR_IRRIGATION                         3635
#define _kFLUSH_RUN_BACK_UP                                    3637
#define  _kRUNNING_FLUSH_NUMBER                                3647



#define _kMAX_PUB_BUFF_SIZE                                    530+450

#define _kFOGGER_RUN_TIMER                                     3*60

#define _kMAX_TIME_TO_MONITOR_WIRELESS_PUMP                    150

#define _kDEFAULT_FLUSH_TIME                                    3   // min
#define _kDEFAULT_FlUSH_REPEAT                                  10 // min


#define _kMAX_TIME_IN_SEC                                      86339
#define _kMAX_BUFFER_TIME                                      5*60
#define _kMAX_FILTER_REPEAT_TIME                               6*3600
#define _kMAX_FILTER_TIME                                      3*60
#define  _kFAULT_RESET_TIMER                                   100







/********************************************************** declare all structure ****************************/
typedef struct Ruble_Parameters
{
	uint64_t Display_Update;                             /**< To update the display */
	uint64_t Pump_On_Flag;
	unsigned long int Current_Time_In_Sec;
	unsigned int  Ruble_Battery_Level;                   /**< To read the status of the battery */
	unsigned int  Pump_Run_Timer;                        /**< if selected mode is timer */
	unsigned int  Publish_Topic;                          /**< which topic is going to publish */
	unsigned int  Start_Monitoring_Pump_Faults_Timer;
	unsigned int  Read_Ct_Value_Timer;
	unsigned int  Timer_Mode_Wait_Timer;
	unsigned char Ruble_ID[7];                           /**< used to store Ruble Id */
	unsigned char Operation_Mode;                        /**< auto,manual,timer etc */
	unsigned char Operation_Status;
	unsigned char Temp_Operation_Mode;
	unsigned char Mode_Switch_Pos;                       /**< To read the mode switching switch status */
	unsigned char lndications;                           /**< all led indications */
	unsigned char Display_Buff[21];                      /**< Buffer to hold the display data */
	unsigned char Update_Local_Time[3];                  /**< GSM time update request */
	unsigned char Update_Local_Date[3];                  /**< DATE, MONTH, YEAR */
	unsigned char Lora_Data_TX[20];                      /**< tx and rx to lira */
	unsigned char Lora_Data_Rx[20];                      /**< tx and rx to lira */
	unsigned char Hardware_Faults;                            /**< hardware related faults like sd card, lora, lcd */
	unsigned char Irrigating_Plot;                          
	unsigned char Ruble_Registration_Status;                  /**< Plot which is irrigating now */
	unsigned char Subscribe_Topic_Status;                     /**< status for weather all topics are subscribed or not */
	unsigned char EEPROM_Update;                              /**< Update the eeprom status */
	unsigned char List_Of_Valve_ID[_kMAX_EURO_VALVE ][7];     /**< Holds the euro valve ids of each valve */ 
	unsigned char Wired_valve_List[_kMAX_WIRED_VALVE+1];        /**< list the GPIO to turn on and off the wired valve */
	unsigned char Temp_Irrigating_Plot;                       /**< Temporary irrigating plot used for timer topic published from srver */ 
	unsigned char Last_Plot;                                  /**< Data get from the last plot(lira node) */
	unsigned char Do_Calibration;
	unsigned char Wait_Server_Not_Reachable;
	unsigned char SMS_Alert_Start;                            /**< when user will set the mobile number then sms alert will send to user */
	unsigned char Electricity_Status;
	unsigned char Sim_Check;
	unsigned char Lora_Module_Check;
	unsigned char Display_Refresh;
	unsigned char Last_Valve;
	unsigned char Last_Pump;
	unsigned char Line_Fault_Flag;
	unsigned char Kill_Switch;
	unsigned char Extra_Plot_Feedback;
	unsigned char Extra_Plot_Feedback1; 
	unsigned char Kill_Switch_Through_App;
	unsigned char Schedules_Activated;
	unsigned char Choose_Operation;
	unsigned char Last_Timer_Plot;
	unsigned char Timer_Feedback_Send_Timer;
	unsigned char By_Pass_CT;
	unsigned char Last_Irrigating_Schedule;
	unsigned char Wireless_Pump_Command;
	unsigned char Wirelss_Pump_Status;
	unsigned int  Wireless_Pump_Operate_Timer;
	unsigned char Pending_Plots[_kMAX_LIRA_SENSOR_NODE+5];
	unsigned char Wireless_Pump_Enabled;
	unsigned char Wireless_Pump_Handle_List[_kMAX_WIRELESS_PUMP+2];
	unsigned char Pump_Selected_For_Irrigation;
	unsigned char Pump_Selected_In_Timer;
	unsigned char Immediately_Turn_Off_Pump;
	unsigned long int Pump_Run_Timer_for_flush;
	unsigned int  Flush_Running;
	unsigned char Pump_Is_Running_Or_Not;
	unsigned char Force_Stop;
	unsigned int  Manual_Mode_Wait;
	unsigned int Flush_Retry;
	unsigned int  Flush_Valve_Off_Timer;
	unsigned int  Indication_Of_Faults;
	unsigned char Switch_To_Battery;
	unsigned char Debug_Mode;
	unsigned char Calibration_Over_Ruled;
	
}Ruble_Parameters;

typedef struct Pump_Parameter
{
	float Pump_Nominal_Current;                           /**< current consumption of pump at first time */
	float Pump_Run_Current;                               /**< power consumption */
	
	unsigned char Irrigation_Status;                      /**< ongoing,dry run etc */
	unsigned char Pump_Status;                            /**< pump on or off */
}Pump_Parameter;


typedef struct Extra_Plot
{
	bool          On_Off_Status;
	unsigned char Extra_Plot_Number;
	unsigned char Status;
	unsigned int  Run_Timer;		
	unsigned char Euro_Valve_Group[_kMAX_VALVE_FOR_1PLOT];
	unsigned char One_Touch;
	unsigned char Set_Through_Timer;
	unsigned int  Min_Valve[3];  // 0 : temperature 1 : humidity
	unsigned int  Max_Value[3];
	unsigned char Selected_Plot;
	unsigned char Auto_Operation;
	unsigned char Publish_status;
}Extra_Plot;

extern unsigned char _gRunning_Flush_Num;
extern volatile unsigned int _gWdt_Counter;
extern volatile unsigned char _gStart_Wdt;
extern volatile unsigned char _gTank_Topic_check;
extern volatile unsigned char plot_schedule_topic;






/********************************** declare all enum **********************************************************/
/**
* @enum Name: _eMODE_SWITCH
* @brief :Used to read and update the status of the auto manual switch
*/
enum _eMODE_SWITCH
{
	_eMOD_SWITCH_AUTO           = 1,
	_eMOD_SWITCH_MANUAL
};

/**
* @enum Name: _eHARDWARE_FAULTS
* @brief :Used to read and update the hardware faults
*/
enum _eHARDWARE_FAULTS
{
	_eGSM_INIT_FAIL             = 0X01,
	_eLORA_INIT_FAIL            = 0X02,
	_eSD_CARD_INIT_FAIL         = 0X04,
	_eEEPROM_INIT_FAIL          = 0X08,
	_eLCD_INIT_FAIL             = 0X10,
	_eRELAY_FAULTY              = 0X20,
};

enum _eRUBLE_REGISTARTION_STATUS
{
	_eRUBLE_REGISTERED                = 0X01,
	_eRUBLE_NOT_REGISTERED,
};

enum _eDISPLAY_UPDATE
{
	_eRANGE_SYBBOL                    = 0x0000,
	_kDISP_NO_RANGE                   = 0X0001,
	_kDISP_RANGE_1                    = 0X0002,
	_kDISP_RANGE_2                    = 0X0004,
	_kDISP_RANGE_3                    = 0X0008,
	
	_kWAITING_FOR_REG                 = 0X0010,
	
	_KPUMP_IS_OFF                     = 0X0020,
	_KPUMP_IS_ON                      = 0X0040,
	_kPUMP_IS_OVERLOADED              = 0X0080,

	_kTIME_UPDATE                     = 0X0100,
	
	_ePLEASE_WAIT                     = 0X0200,
	_eCONNECTING_TO_SERVER            = 0X0400,
	_eREGISTERING_TO_SERVER           = 0X0800,
	_eREGISTERED_TO_SERVER            = 0X1000,
	_eSEARCHING_FOR_NETWORK           = 0X2000,
	_eNETWORK_SELECTED                = 0X4000,
	_eCONNECTED_TO_SERVER             = 0X8000,
	_eTHANK_YOU_FOR_WAITING           = 0X010000,
	_eREBOOTING                       = 0X020000,
	_eDATA_INITIALISING               = 0x040000, 
};

enum _eSUB_TOPIC_NAME
{
	_eALL_TOPIC_SUBSCRIBED            = 0X01,
	_eALL_TOPIC_NOT_SUBCRIBED
};

enum _eOPERATION_MODE
{
	_eOPER_MOD_AUTO                   = 0x01,
	_eOPER_MOD_MANUAL,
	_eOPER_MOD_TIMER,
	_eOPER_MOD_DATALOG,
	_eOPER_MOD_SCHEDULER
};


enum _eIRRIGATION_STATUS
{
	_eIRRIG_STAT_DRY_RUN                 = 0X01,
	_eIRRIG_STAT_LINE_FAULT              = 0X04,
	_eIRRIG_STAT_EURO_CURRENT_SENSE      = 0X05,
	_eIRRIG_STAT_EURO_FEEDBACK           = 0X06,
	_eIRRIG_STAT_TIMER_NOT_SET           = 0X07,
	_eIRRIG_STAT_SUSP_END                = 0X08,
	_eIRRIG_STAT_OVER_LOAD               = 0X09,
	_eIRRIG_STAT_RUNNING                 = 0X0A,
	_eIRRIG_STATUS_KILL_SWITCH           = 0X0B,
	_eTHREE_PHASE_POWER_INT              = 0X0C,
	_eEURO_BATTERY_LOW                   = 0X0D,
	_eNO_ELECTRICITY_FAULT               = 0X0E,
	_eWIRELESS_PUMP_FAIL_FAULT           = 0X0F
};


enum _eOPERATION_STATUS
{
	_eOPERATION_STAT_SUSPENDED               = 0X01,
	_eOPERATION_STAT_CHANGED_MODE            = 0X02,
	_eOPERATION_STAT_RUNNING                 = 0X03,
	_eOPERATION_STAT_OPRN_END                = 0X04
};


enum _eRUBLE_INDICATION
{
	_eRUBL_INDI_AUTO_MANUAL             = 0X01,
	_eRUBL_INDI_PUMP_ON_OFF             = 0X02,
	_eRUBL_INDI_SERVER_CONN             = 0X04,
};


enum _eSTATUS_OF_ELECTRICITY
{
	_eSTAT_NO_ELECTERICITY              = 0X01,
	_eSTAT_ELECTERICITY_RESUMED         = 0X02
};

enum _eSIM_CARD_STATUS
{
	_eSIM_NOT_INSERTED = 1,
	_eSIM_INSERTED,
	_eDIRESCT_CHECK,
	_eLOOP_CHECK
};

enum _eKILL_SWITCH
{
	_eKILL_SWITCH_SET = 1,
	_eKILL_SWITCH_NOT_SET
};


enum
{
	_eELECTRICITY_IS_NOT_THERE = 0,
	_eELECTRICITY_IS_THERE 
};

enum _eEXTRA_PLOT_OPEARATION
{
	_eSTOP_EXTRA_PLOT_OPERATION = 1,
	_eSTART_EXTRA_PLOT_OPERATION,
	_eRUN_EXTRA_PLOT_OPERATION,
	_kSCHEDULER_ON,
	_kSCHEDULER_OFF,	
};

enum _eMONITOR_DATA
{
	_eSET_THROUGH_TIMER = 1,
	_eSET_THROUGH_ONE_TOUCH,
};


enum _eEXTRA_PLOT_RESEND
{
	_eEXTRA_PLOT_TIMER_RESEND = 1,
	_eEXTRA_PLOT_ONETOUCH_RESEND,
	_eEXTRA_PLOT_AUTO_RESEND
	
};


enum _eWIRELESS_PUMP_COMMAND
{
	_eWIRELESS_PUMP_ON = 1,
	_eWIRELESS_PUMP_OFF,
	_eWIRELESS_PUMP_WAIT
		
};

/*********************************************** ENUMS ******************************************************************/
/*enum _eUPDATE_STATUS_OF_FAULTS
{
	_eR_PHASE_STATUS      = 0x01,
	_eY_PHASE_STATUS      = 0x02,
	_eB_PHASE_STATUS      = 0x04,
	_eDRY_RUN_STATUS      = 0x08,
	_eLINE_FAULT_STATUS   = 0x10,
	_ePUMP_OVERLOAD       = 0X20,
	_ePUMP_STATUS         = 0x40,
	_eMANUAL_MODE         = 0x80,
	_eAUTO_MODE           = 0X100
};*/



/*********************************************** ENUMS ******************************************************************/
enum _eUPDATE_STATUS_OF_FAULTS
{
	_eR_PHASE_STATUS      = 0x01,
	_eY_PHASE_STATUS      = 0x02,
	_eB_PHASE_STATUS      = 0x04,
	_eDRY_RUN_STATUS      = 0x08,
	_eLINE_FAULT_STATUS   = 0x10,
	_ePUMP_OVERLOAD       = 0X20,
	_ePUMP_STATUS         = 0x40,
	_eMANUAL_MODE         = 0x80,
	_eAUTO_MODE           = 0X100,
	_ePUMP_NOT_WORKING    = 0x200
};




enum _eCOMMAND_FROM_RUBLE
{
	_eDO_CALIBRATION   = 0X01,
	_ePUMP_ON          = 0X02,
	_ePUMP_OFF         = 0X04
};

enum _eRESPONSE_TO_RUBLE
{
	_eCALIBARATION_SUCCESS         = 0x01,
	_eCALIBRATION_FAIL             = 0X02,
	_eSTATUS_OF_WIRELESS_PUMP      = 0X04,
	_ePUMP_OR_ELECTRICITY_RELATED  = 0X08
};

enum _eWIRELESS_PUMP_COMMNAD_OR_RESP
{
	_eCOMMAND_FROM_WIRELESS_PUMP = 1,
	_eCALIBRATION_RESPONSE,
	_eWIRELESS_FAULT,
};

enum _eFEEDBACK_FROM_RUBLE
{
	_eGOT_DATA = 0X01,
};



enum
{
	_eSUBSCRIBE_DATA = 1,
	_ePUBLSIH_DATA
	};




/************************************ declare all variable for the structure **********************************/
extern Ruble_Parameters _sRuble_Parameters;
extern Pump_Parameter   _sPump_Parameter;


/*********************************** declare all global variable ****************************************************/ 
//extern const unsigned char _gauchCRCHi[100];
//extern const unsigned char _gauchCRCLo[100];
extern volatile unsigned char _gPub_Buff[_kMAX_PUB_BUFF_SIZE];
extern volatile unsigned char _gRecvd_Data[110];
extern Extra_Plot      _sExtra_Plot[_kMAX_EXTRA_PLOT];
extern unsigned int _gTime_To_Wait_Lora_Resp;
extern unsigned int _gWait_Blower_Time;
extern unsigned int _gfault_Occure;


/**
* @Function Name: void ADC_Operation(void);
* @brief :Used to read the ADC data.
*/

void ADC_Operation(void);

/**
* @Function Name: void Read_Switch_Status(void);
* @brief : used to read the switch
*/

void Read_Switch_Status(void);

/**
* @Function Name: void Param_Initialization(void);
* @brief : used to initialize lcd data, all pub, sub topics, apn , registration etc
*/
void Param_Initialization(void);


/**
* @Function Name: void Read_EEPROM(void);
* @brief : used to read all the neccessary data which is stored in the eeprom 
*/
void Read_EEPROM(void);



/**
* @Function Name: void Update_Indication(void);
* @brief : update the led indications
*/

void Update_Indication(void);

/**
* @Function Name: void Select_Operation_Mode(void); 
* @brief : update the operation mode
*/
void Select_Operation_Mode(void); 


/**
* @Function Name: void Run_Operation(void);
* @brief : run the operation according to selected mode 
*/
void Run_Operation(void);


/**
* @Function Name: bool Start_Irrigation(void);
* @brief : start the irrigation
*/
bool Start_Irrigation(void);


/**
* @Function Name: bool Stop_Irrigation(void);
* @brief : stops the irrigation 
*/
bool Stop_Irrigation(void);


/**
* @Function Name: char Valve_Grouping_Handler(unsigned char on_off);
* @brief : turn ON or OFF the group valves
*/
char Valve_Grouping_Handler(unsigned char on_off);



//void mqttCallback(char* topic, unsigned char * payload, unsigned int len);

void Pub_Sub_Data(char* topic, unsigned char * payload, unsigned int len, unsigned char Pub_Sub);

/**
* @Function Name: void Display_Update(void);
* @brief : update the display 
*/
void Display_Update_1(void);


/**
* @Function Name: void Faults_Handler(void);
* @brief : update all faults 
*/
void Faults_Handler(void);


/**
* @Function Name: void Lora_Operation(void);
* @brief : read and update the lora packtes
*/
void Lora_Operation(void);


/**
* @Function Name: bool Update_Lira_Sensor_Data(unsigned char *temp_id);
* @brief : read the packet of lira sensor node and update accordingly
*/
bool Update_Lira_Sensor_Data(unsigned char *temp_id);


/**
* @Function Name: bool Update_Euro_Valve_Data(unsigned char *temp_id);
* @brief : read the packet of euro valve and update accordingly
*/
bool Update_Euro_Valve_Data(unsigned char *temp_id);


/**
* @Function Name: void Publish_Data(void);
* @brief : Publish the data to server
*/
void Publish_Data(void);


/**
* @Function Name: void Publish_Data(void);
* @brief : update the plot data in EEPROM
*/
void Lira_Data_EEPROM(unsigned char plot_num);

void Euro_Data_EEPROM(unsigned char valve_number);

void Dummy_Data_Store(void);

int curveFit(int x);

int Calibration_Of_Pump(void);

bool Check_Sim(char check_status);

void Clear_Serial_Data(void);

bool Subscribed_data_check(void);

bool Seperate_Out_Sub_Data(void);

bool Read_SMS_COMMAND(char* ATcommand, char* expected_answer, char* expected_answer2,unsigned int timeout);

void Kill_Switch_Through_SMS(void);

void Keeps_Monitor_Server_Connection(void);

void Local_Time_Setting(void);

void Exra_Plot_Data_EEPROM(unsigned char plot_number);

void Run_Extra_Plot_Operation(void);

bool Start_Extra_Plot_Operation(unsigned char check_plot);

bool Stop_Extra_Plot_Operation(unsigned char check_plot);

bool Extra_Plot_Valve_Grouping_Handler(unsigned char on_off, unsigned char check_plot);

char Temperature_Control(void);

char Humidity_Control(void);

char Light_Control(void);

void Testing_Function(void);

char Blower_Monitoring(unsigned char forcefully_off);

unsigned char Wireless_Pump_On_Off(unsigned char On_Off);

unsigned char Handle_Wireless_Pump(void);

unsigned char Plot_Status_List_Monitor(void);

unsigned char Fogger_Status_List_Monitor(void);

unsigned char Wireless_Motor_Enable_Disable_Check(void);

unsigned char Pump_Status_List_Monitor(void);

char Valve_Grouping_Handler_For_wireless_Pump(unsigned char Target_Plot_Num, unsigned char on_off);

char Pump_Run_Monitoring(void);

unsigned char Flush_Controlling(void);

unsigned char Check_Phase(void);

unsigned char Debug_Check(void);


#endif /* INITIALIZE_H_ */

