/*
 * MQTT_Handler.h
 *
 * Created: 25-08-2021 15:48:39
 *  Author: Tushar (SIO)
 */ 


#ifndef MQTT_HANDLER_H_
#define MQTT_HANDLER_H_

#include <Initialize.h>


/*********************************************** declare all macro here ***************************************************/

#define _kBROKER_IP									"34.123.55.118" //"34.133.72.15"    //"34.123.55.118" //"34.133.72.15"		// "34.123.55.118"
#define _kMQTT_CLIENT_ID                            "mq_siouser" //"mq_siousertest"  //"mq_siouser"    //"mq_siousertest"   // mq_siouser
#define _kMQTT_USER                                 "mq_siouser" // "mq_siousertest"  //"mq_siouser"    //"mq_siousertest"       // mq_siouser
#define _kMQTT_PASSWORD                             "9Xk!ej6fL6<,e2P8" //"Sicca@712"       //"9Xk!ej6fL6<,e2P8" //"Sicca@712"   //9Xk!ej6fL6<,e2P8

/**************************************************** declare all the structures *****************************************/


typedef struct Publish_Topic
{
	char Ruble_Register[19];                /* weather registered or not */
	char Ruble_Faults[19];
	char Ruble_Data[29];
	char Timer_FeedBack[28];
	char Ruble_Restart[32];
	char Lira_Node_Data[18];
	char Euro_Valve_Data[17];
	char Hardware_Faults[29];
	char General_Feedback[22];
	//char Electricity_Status_Topic[24];
	char Calibration_FeedBack[32];
	//char Extra_Plot_Feedback[25];
	char One_Touch_Monitor[29];
}Publish_Topic;


typedef struct Subscribe_Topic
{
	char Ruble_Register_Feedback[28];
	char Ruble_Plot_Setup[31];
	char Ruble_Set_Timer[19];
	char Ruble_Soil_Coff[24];
	char Ruble_Euro_Valve_Set[26];
	char Ruble_Controller_Setup[26];
	char Calibration[23];
	char Schedule_Topic[22];
	char One_Touch_Feedback[20];
	char Tank_Schedule[27];
	//char Delete_Plot_Devices[27];
	//char Kill_Switch_Topic[19];
}Subscribe_Topic;



typedef struct Broker_Info
{
	//unsigned int Broker_Port_No;
	//char Boker_IP[15];
	//char Broker_URL[10];
	//char Broker_User_Name[10];
	//char Broker_Password[20];
}Broker_Info;



typedef struct Network_and_Usr_Info
{
	char Ruble_Sim_APN[20];
	char Ruble_SIM_No[11];
	char User_Sim_No[11];
	//char Client_Id[10];
	//char User_Name[10];
	//char User_Password[10];
}Network_and_Usr_Info;

enum _eCOMMON_TOPICS
{
	_eVALVE_DELETE = 1,
	_ePLOT_DALETE,
	_eSCHEDULE_PLOT_DELETE,
	_eEXTRA_PLOT_DELETE,
	_ePAUSE_SCHEDULE,
	_eRESUME_SCHEDULE,
	_eFORCED_STOP,
	_eFORCED_STOP_RESUME,
	_eRECOVER_DATA,
	_eADD_WIRELESS_PUMP,
	_eDELETE_WIRELESS_PUMP
	
};



/*********************************************** declare all  enums ******************************************/

enum _ePUBLISH_TOPIC_NAME
{
	_ePUB_REGISTERED_TOPIC         = 0X01 ,
	_ePUB_RUBLE_FAULTS             = 0X02,
	_ePUB_RUBLE_DATA               = 0X04,
	_ePUB_TIMER_FEEDBACK           = 0X08,
	_ePUB_RUBLE_RESTART            = 0X10,
	_ePUB_RUBLE_FEEDBACK           = 0X20 ,
	_ePUB_LIRA_NODE_DATA           = 0X40,
	_ePUB_EURO_VALVE_DATA          = 0X80,
	_ePUB_HARDWARE_FAULTS          = 0X0100,
	_ePUB_PUBLISH_FEEDBACK         = 0X0200,
	_eELECTRICITY_STATUS           = 0X0400,
	_eWIRED_VLAVE_FAIL             = 0X0800, 
	_eEXTRA_PLOT_FEEDBACK          = 0X1000,
	_eONE_TOUCH_FEEDBACK           = 0X2000,
	_eEXTRA_PLOT_AUTO_FEEDBACK     = 0X4000,
	_eWIRELESS_PUMP_FAIL           = 0X8000
};






/****************************************** declare all the variable *********************************************/

extern Subscribe_Topic _sSubscribe_Topic;
extern Publish_Topic   _sPublish_Topic;
extern Broker_Info     _sBroker_Info;
extern Network_and_Usr_Info _sNetwork_and_Usr_Info;




/****************************************** declare all function **********************************************/

/**
* @Function Name: void Store_MQTT_Pub_Topics(void);
* @brief : used to store all the publish topics 
*/
void Store_MQTT_Pub_Topics(void);


/**
* @Function Name: void Store_MQTT_Sub_Topics(void);
* @brief : used to store the topics which we want to subscribe
*/
void Store_MQTT_Sub_Topics(void);


/**
* @Function Name: void Store_Broker_Info(void);
* @brief : used to store the broker related information
*/
void Store_Broker_Info(void);


/**
* @Function Name: void Store_User_Info(void);
* @brief : used to store the user information
*/
void Store_User_Info(void);


/**
* @Function Name: bool Init_Broker(void);
* @brief : used to initiate the broker
*/
bool Init_Broker(void);

/**
* @Function Name: bool Ping_to_Server(void);
* @brief : used to ping the server
*/
bool Ping_to_Server(void);


/**
* @Function Name: subscribe all the topics 
* @brief : used to subscribe all topics
*/
bool Subscribe_MQTT_Topics(void);


void Enable_Uart_Interrupt(void);

#endif /* MQTT_HANDLER_H_ */