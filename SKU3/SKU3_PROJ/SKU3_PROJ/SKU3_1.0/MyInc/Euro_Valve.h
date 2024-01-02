/*
 * Euro_Valve.h
 *
 * Created: 24-08-2021 10:34:50
 *  Author: Tushar (SIO)
 */ 



#ifndef EURO_VALVE_H_
#define EURO_VALVE_H_





/********************************************* declare all macros ***************************************/

#define _kTOTAL_VALVE                                         28
#define _kMAX_EURO_VALVE                                      4
#define _kMAX_WIRED_VALVE                                     24
#define _kWIRED_VALVE_NUMBER                                  (_kTOTAL_VALVE -_kMAX_LIRA_SENSOR_NODE) + 1
#define _kEURO_WILL_ACK_LEN
#define _kEURO_DATA_ACK_LEN
#define _kMAX_VALVE_FOR_1PLOT                                 5
#define _kVALVE_ID_LEN                                        6
#define _kVALVE_ONLINE                                        1
#define _kVALVE_OFFLINE                                       0
#define _kVALVE_ACK_SEND                                      0xff
#define _kNUMBER_OF_VALVE_CONNECT_TO_EURO                     3

/****************************** WIRELESS PUMP HANDLE DATA *************************/
#define _kMAX_WIRELESS_PUMP                                   3          

/********************************************* declare all structures ************************************/

typedef struct Euro_Valve_Param
{
	unsigned char Euro_Valve_Number;
	unsigned char Euro_Valve_ID[7];
	unsigned int  Euro_Battery_Level;
	unsigned char Euro_Valve_Status;
	unsigned char Euro_Valve_Online;
	unsigned char Faults;
	//unsigned char RSSI;
	//unsigned char SNR;
}Euro_Valve_Param;

typedef struct Wireless_Pump_Param
{
	unsigned char Wireless_Pump_Number;
	unsigned char Wireless_Pump_ID[7];
	unsigned int  Indication_Of_Faults;
	unsigned char Response_To_Ruble;
	//unsigned int  Euro_Battery_Level;
	//unsigned char Wireless_Pump_Status;
	//unsigned char Wireless_Pump_Online;
	//unsigned char Wireless_Pump_Faults;
}Wireless_Pump_Param;


enum _eEURO_VALVE_PACKET
{
	
	_eEURO_VALVE_FAULT_DATA = 0X01,
	_eEURO_VALVE_COMMAND,
};

enum _eWIRELESS_PUMP_PACKET
{
	
	_eWIRELESS_PUMP_FAULT_DATA = 0X01,
	_eWIRELESS_PUMP_COMMAND,
};

enum _eBATTERY_FAUTS
{
	_eNO_FAULT                    = 0,
	_eLOW_BAT_FAULT,
	_eVALVE_OFFLINE_FAULT,
};


/************************************************ declare all variables *****************************/
extern Euro_Valve_Param _sEuro_Valve_Param[_kMAX_EURO_VALVE];
extern Wireless_Pump_Param _sWireless_Pump_Param[_kMAX_WIRELESS_PUMP];


void Wireless_Pump_EEPROM(unsigned char Pump_Number);

bool Update_Wireless_Pump_Data(unsigned char *temp_id);

unsigned char Turn_Off_All_Pump_For_Safety(void);

//unsigned char Wireless_Pump_On_Off(unsigned char Pump_Num,unsigned char On_Off_command);

unsigned char Wireless_Pump_On_Off(unsigned char On_Off, unsigned char wireless_pump_number);


#endif /* EURO_VALVE_H_ */