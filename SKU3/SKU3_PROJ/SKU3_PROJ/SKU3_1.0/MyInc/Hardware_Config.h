/*
 * Hardware_Config.h
 *
 * Created: 24-08-2021 10:33:13
 *  Author: Tushar (SIO)
 */ 


#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h"
#include <ArduinoJson.h>
//#include <RH_RF95.h>
#include <SPI.h>
#include <EEPROM.h>
#include <SD.h>


//#define TINY_GSM_MODEM_M95
//#define SerialMon                                      Serial
//#define SerialAT                                       Serial1
//#define GSM_AUTOBAUD_MIN                               9600
//#define GSM_AUTOBAUD_MAX                               115200
//#define TINY_GSM_USE_GPRS                              true
//#define TINY_GSM_TEST_SMS                              true

//#include <TinyGsmClient.h>
//#include <PubSubClient.h>

//#define _kJSON_BUFFER                              StaticJsonBuffer<270>


/************************************************ For 4G moduole **********************************************/
#define _kSEND_AT_COMMAND                                   Send_AT_Command
//#define test_data                                           "KISHOR RAVINDRA PATIL"



/*************************************************** LORA ***********************************************************/
  
#define _kCONTROLLER_ADDRESS                          20       /**< CONTROLLER ADDRESS FOR RELIABLE RECEPTION AND TRANSMISSION IT SHOULD BE 10 */
#define _kLORA_INIT                                Lora_Init
#define _kLORA_DATA_AVAIALABLE                     Lora_Available
#define _kLORA_TEMP_READ                                                  
#define _kLORA_SET_FREQ                            Lora_Set_Freq
#define _kLORA_RSSI_READ                                                
#define _kLORA_SET_TX_POWER                        Lora_Set_Power
#define _kLORA_RECV_DATA                           Lora_Data_Receive
#define _kLORA_SEND_DATA                           Lora_Data_Send
#define _kCHEK_DATA_AVAILABLE_TIMEOUT              Check_Data_Available   
#define _kLORA_SLEEP
#define _kLORA_FREQUENCY                           865.068
#define _kLORA_TX_POWER                            20
#define _kLORA_MODULE_CONFIG                       Lora_Set_Config   
#define _kLORA_LAST_RSSI                           Read_Last_RSSI  
#define _kLORA_LAST_SNR                            Read_Last_SNR              


/************************************************** JSON ***********************************************************/

//#define _kJSON_PARSE_OBJ                           parseObject
//#define _kJSON_OBJ                                 JsonObject& 
//#define _kJSON_CREAT_OBJECT                        json_data.createObject

//#define _kJSON_SUCCCESS                            temp_object.success()
//#define _JSON_CLEAR_BUFF                           json_data.clear
//#define _kJSON_DATA_COPY                           Transmit_Buff.prettyPrintTo((char *)_gPub_Buff, sizeof(_gPub_Buff))
//#define _kJSON_DATA_RECV                           json_data.parseObject

/************************************************** EEPROM *********************************************************/

#define _kEEPROM_WRITE                             EEPROM.put
#define _kEEPROM_READ                              EEPROM.get
//#define _kEEPROM_UPDATE                            EEPROM.update

/***************************************************** GSM *********************************************************/

//#define _kGSM_SIG_STRENGTH                         Signal_Quality 
#define _kGSM_INIT                                 Gsm_Init
#define _kGSM_WAIT_FOR_NETWORK                     Gsm_Wait_For_Network
#define _kIS_GSM_CONNECTED                         Check_Gsm_Connection
#define _kGET_GSM_OPERATOR                         Get_Operator_Name
#define _kGPRS_CONNECT                             Gprs_Connect
#define _kIS_GPRS_CONNECTED                        Check_Gprs_Connection
#define _kGET_SIGNAL_QUALITY
#define _kSEND_SMS                                 Send_SMS

/***************************************************** MQTT *******************************************************/

#define _kMQTT_PUB                                 MQTT_Publish            // mqtt.publish
#define _kMQTT_SUB                                 MQTT_SUBSCRIBE          // mqtt.subscribe
#define _kMQTT_CALLBACK                            MQTT_SET_CALLBACK       // mqtt.setCallback
#define _kMQTT_READ_PACKET
#define _kMQTT_CONNCET                             MQTT_Connect            // mqtt.connect
#define _kMQTT_SET_SERVER                          MQTT_Set_Server         // mqtt.setServer
#define _kIS_MQTT_CONNECTED                        Is_MQTT_Connected       // mqtt.connected
#define _kMQTT_LOOP                                MQTT_Loop               // mqtt.loop


/***************************************************** LCD ********************************************************/
//LiquidCrystal_I2C lcd(0x27, 20, 4);     /**< make the instance for the LCD */

#define _kLCD_INIT                                 Lcd_Init              
#define _kLCD_BACKLIGHT_ON                         Lcd_Backlight_On
#define _kLCD_BUFF_WRITE                           Lcd_Print_Buff
#define _kLCD_SPECIAL_CHAR                         Lcd_Special_Char
#define _kLCD_CLEAR                                Lcd_Clear


/***************************************************  ADC *******************************************************/

#define _kADC_CHANNEL_SEL
#define _kADC_READ                                 analogRead
#define _kADC_BATT_VOL                             1             /**< channel to read battery voltage */
#define _kADC_PUMP_CUR                             A15           /**< channel to read pump current */

/***************************************************** GPIO *****************************************************/

#define _kGPIO_MODE                                pinMode
#define _kGPIO_SET                                 digitalWrite
#define _kGPIO_GET                                 digitalRead
#define _kOUPTPUT_MODE                             1
#define _kINPUT_MODE                               0
#define _kINPUT_PULLUP                             2
#define _kHIGH                                     1
#define _kLOW                                      0
#define _kGPIO_PORTE                              DDRE
#define _kMY_GPIO_MODE_SET(PORT,Pin_Num,mode)    Select_Gpio_Mode_PORTE(PORT,Pin_Num,mode)  
#define _kMY_GPIO_STATUS_READ(Port,Pin)          My_Gpio_Status_Read(Port,Pin)                                  


/**
*
* @brief : hardware signal list of "GPIO DIGITAL OUTPUT" pins
*/

#define  _kPUMP_1              10
#define  _kPUMP_2              11

#define  _kVALVE_1             22
#define  _kVALVE_2             24
#define  _kVALVE_3             26
#define  _kVALVE_4             28
#define  _kVALVE_5             37
#define  _kVALVE_6			   34
#define  _kVALVE_7             32
#define  _kVALVE_8             30
#define  _kVALVE_9             23
#define  _kVALVE_10            25
#define  _kVALVE_11            27
#define  _kVALVE_12            29

// #define  _kVALVE_1             6
// #define  _kVALVE_2             7
// #define  _kVALVE_3             8
// #define  _kVALVE_4             9
// #define  _kVALVE_5             22
// #define  _kVALVE_6			   24
// #define  _kVALVE_7             26
// #define  _kVALVE_8             28
// #define  _kVALVE_9             37
// #define  _kVALVE_10            32
// #define  _kVALVE_11            34
// #define  _kVALVE_12            30

#define  _kVALVE_13            35
#define  _kVALVE_14            33
#define  _kVALVE_15            31
#define  _kVALVE_16            49


#define  _kVALVE_17             6
#define  _kVALVE_18             7
#define  _kVALVE_19             8
#define  _kVALVE_20             9
#define  _kVALVE_21             48
#define  _kVALVE_22			    47
#define  _kVALVE_23             46
#define  _kVALVE_24             45

// #define  _kVALVE_25             23
// #define  _kVALVE_26             25
// #define  _kVALVE_27             27
// #define  _kVALVE_28             29
// #define  _kVALVE_29             35
// #define  _kVALVE_30             33
// #define  _kVALVE_31             31
// #define  _kVALVE_32             49

#define _kSD_CARD_CS           44
#define _kLORA_CS              53
#define _kLORA_INT             2
//#define _kEEPROM_CS            1    /* commented for this microcontroller */

#define _kSWITCH_VCC_PIN       36
#define _kPUMP_STATUS_LED      1 // 40
#define _kMODE_STATUS_LED      12
#define _kNETWORK_STATUS_LED   39

#define _kGSM_MODULE_ENABLE    30 // 41
#define _kPHASE_DETECTION           A1
//#define _kPHASE_DETECTION_SWITCH    A3
#define _kDEBUG_PIN                 A2



/**
*
* @brief : hardware signal list of "GPIO INPUT" pins
*/
#define  _kVALVE_1_FEEDBACK    48
#define  _kVALVE_2_FEEDBACK    47
#define  _kVALVE_3_FEEDBACK    46
#define  _kVALVE_4_FEEDBACK    45
#define  _kVALVE_5_FEEDBACK    23
#define  _kVALVE_6_FEEDBACK    25
#define  _kVALVE_7_FEEDBACK    27
#define  _kVALVE_8_FEEDBACK    29
#define  _kVALVE_9_FEEDBACK    35
#define  _kVALVE_10_FEEDBACK   33
#define  _kVALVE_11_FEEDBACK   32
#define  _kVALVE_12_FEEDBACK   49

//#define  _kGSM_RI_PIN          1   /* commented for this microcontroller */
#define  _kPOWER_SOURCE_CHECK   PINE6 //PE6
#define  _kAUTO_MANUAL_SWITCH  38


#define  _kAUTO_MANUAL_SWITCH_STATUS     _kGPIO_GET(_kAUTO_MANUAL_SWITCH)
#define _kPOWER_SOURCE_STATUS_READ       _kMY_GPIO_STATUS_READ(_kGPIO_PORTE,_kPOWER_SOURCE_CHECK)  


/*************************************** SWITCH TO BATTERY **********************************/
#define _kSWITCH_TO_BATTERY          49   /* If there is no electricity then this pin goes high */






#define _kR_PHASE_INPUT              A3   /* Digital input pin to read R phase status */
#define _kY_PHASE_INPUT              A4   /* Digital intput pin to read Y phase status */
#define _kB_PHASE_INPUT              A5   /* Digital intput pin to read B phase status */

#define _kLORA_INTERRUPT_PIN          2   /* Digital intput pin to check Interrupt of lora */
                





/********************************************* SERIAL COMMUNICATION ********************************************/
#define _kSERIAL_MON								 Serial
#define _KSERIAL_MON_INIT							 Serial.begin
#define _kSERAIL_MON_WRITE(string)                   Serial.println(F(string)) //Serial.write
#define _kSERAIL_MON_WRITE_NO_LN(string)             Serial.print(F(string))
//#define _kSERIAL_MON_READ                          Serial.read
//#define _kSERIAL_MON_CHECK                         Serial.available
#define _kSERIAL_MON_CLEAR                           Serial.flush
#define _kSERIAL_MON_PRINT_NUM                       Serial.println
#define _kSERIAL_MON_WRITE                           Serial.write 
#define _kSERIAL_MON_PRINT_NUM_NO_NEW_LINE           Serial.print   

         


#define _kSERIAL_AT                                 Serial1
#define _KSERIAL_AT_INIT                            Serial1.begin
#define _kSERAIL_AT_WRITE                           Serial1.write
#define _kSERIAL_AT_READ                            Serial1.read
#define _kSERIAL_AT_READ_BYTES                      Serial1.readBytes
#define _kSERAIL_AT_CHECK                           Serial1.available
#define _kSERIAL_AT_TIMEOUT                         Serial1.setTimeout  
#define _kSERAIL_AT_INTERRUPT
#define _kSERIAL_AT_CLEAR                           Serial1.flush

#define _kGSM_AUTOBAUD_MIN                         9600
#define _kGSM_AUTOBAUD_MAX                         115200
#define _kTINY_GSM_USE_GPRS                        true
#define _kTINY_GSM_TEST_SMS                        true
#define _kBAUD_RATE                                9600


/*********************************************** delays ***************************************************/

#define _kDELAY_MS                                delay

/************************************************ SD Card *************************************************/

//#define _kSD_CARD_INIT(chipselect)                          Sd_Card_Init(chipselect)



/**************************************** declare all global variable ************************************/
extern volatile uint32_t g_uTick,_g_1Sec_Time;       /**< increment the value of this tick variable in every 1 milisecond(in timer 1 interrupt) */
extern volatile unsigned char Start_Display_In_Interrupt;



enum
{
	_eREAD_TOPIC = 1,
	_eREAD_PAYLOAD,
};

enum _eDISPLAY_BUFF
{
	_eRANGE = 1,
	_eRANGE1,
	_eRANGE2,
	_eRANGE3,
	_eRANGE_X,
	_eGSM_MODULE,
	_eNO_SIM_CARD,
	_eLORA_MODULE,
	_eRANGE_C,
	_eTIME_UPDATE
};
/**
* @Function Name: void Initialise_Hardware(void);
* @brief :Used to initialize all hardware peripherals and interrupt
*/

void Initialise_Hardware(void);


/**
* @Function Name: void Timer1_Interrupt_Int(void);
* @brief : Used for the 1ms second interrupt
*/
void Timer1_Interrupt_Int(void);


void Timer0_Interrupt_Int(void);


/***************************************************** lcd functions ******************************************************/
/**
* @Function Name: void Lcd_Init(void);
* @brief : initialized the lcd 
*/
bool Lcd_Init(void);

/**
* @Function Name: void Lcd_Backlight_On(void);
* @brief : turn ON the lcd backlight
*/
void Lcd_Backlight_On(void);

/**
* @Function Name: void Lcd_Print_Buff(char *Buff,unsigned char len, unsigned char x_pos, unsigned char y_pos);
* @brief : print the buffer on the lcd 
*/

void Lcd_Print_Buff(char *Buff,unsigned char len, unsigned char x_pos, unsigned char y_pos);

/**
* @Function Name: void Lcd_Special_Char(uint8_t num, uint8_t Buff[],unsigned char y, unsigned char x);
* @brief : print the special char on the lcd 
*/
void Lcd_Special_Char(uint8_t num,unsigned char y, unsigned char x, unsigned char Char_Name);

/**
* @Function Name: void Lcd_Clear(void);
* @brief : used to clear the lcd
*/
void Lcd_Clear(void);


/******************************************** EEPROM FUNCTION ************************/


/******************************************* Lora ***********************************/

/**
* @Function Name: Lora_Init(void);
* @brief : initialize the lora module
*/
bool Lora_Init(void);

/**
* @Function Name: bool Lora_Available(void);
* @brief : check weather the data available(received frok any node)
*/
bool Lora_Available(void);

/**
* @Function Name: bool Lora_Data_Receive(void);
* @brief : copy the received data into the lora_rx_tx buffer
*/
bool Lora_Data_Receive(void);

/**
* @Function Name: void Lora_Data_Send(unsigned char len);
* @brief : send data from lora_rx_tx buffer. Mention the lenght of data which we want send
*/
void Lora_Data_Send(unsigned char len);

/**
* @Function Name: void Lora_Set_Freq(void);
* @brief : set the frequency of lora
*/
void Lora_Set_Freq(void);

/**
* @Function Name: void Lora_Set_Power(void);
* @brief : set the transmission power
*/
void Lora_Set_Power(void);


void Lora_Set_Config(void);

/**
* @Function Name: void Check_Data_Available(unsigned int timeout);
* @brief : check data is available or not after sending the data to the lora node.
*/

bool Check_Data_Available(unsigned int timeout);

/**
* @Function Name: char Read_Last_RSSI(void);
* @brief : Read the last RSSI
*/
char Read_Last_RSSI(void);


/**
* @Function Name: char Read_Last_SNR(void);
* @brief : Read the last SNR
*/
char Read_Last_SNR(void);


/******************************************************* GSM *********************************************/

/**
* @Function Name: int Signal_Quality(void);
* @brief : get the signal quality.
*/
int _kGSM_SIG_STRENGTH(void);

/**
* @Function Name: bool Gsm_Init(void);
* @brief : initialize gsm 
*/
bool Gsm_Init(void);


/**
* @Function Name: bool Gsm_Wait_For_Network(void);
* @brief : wait for the network connection with gsm
*/
bool Gsm_Wait_For_Network(void);

/**
* @Function Name: bool Check_Gsm_Connection(void);
* @brief : check weather connected to GSM or not
*/
bool Check_Gsm_Connection(void);


/**
* @Function Name: void Get_Operator_Name(void);
* @brief : Get the GSM operator name
*/
void Get_Operator_Name(char *);

/**
* @Function Name: bool Gprs_Connect(char *apn);
* @brief : Connect to the GPRS
*/
bool Gprs_Connect(char *apn);

/**
* @Function Name: bool Check_Gprs_Connection(void);
* @brief : check the weather connected to gprs or not
*/
bool Check_Gprs_Connection(void);

/**
* @Function Name: bool Send_SMS(char *mob_num, char *sms_string);
* @brief : used to send the SMS.
*/

bool Send_SMS(char *mob_num, char *sms_string);




/********************************************************* MQTT *******************************************/
/**
* @Function Name: bool MQTT_Publish(void);
* @brief : used to publish the data to the server
*/
bool MQTT_Publish(unsigned char *topic, unsigned char *payload);


/**
* @Function Name: bool MQTT_SUBSCRIBE(char *topic);
* @brief : used to subscribe the topics on which we can get the data.
*/
bool MQTT_SUBSCRIBE(char topic[]);


/**
* @Function Name: void MQTT_SET_CALLBACK(void);
* @brief :used to set callback function so whenever data published from server that function should invoked.
*/
void MQTT_SET_CALLBACK(void);


/**
* @Function Name: void MQTT_Connect(void);
* @brief :used to connect the server
*/
bool MQTT_Connect(const char  client_id[], const char  user_name[], const char  password[]);


/**
* @Function Name: void MQTT_Set_Server(char *ip_address, int port_num);
* @brief :used to set server related data.
*/
void MQTT_Set_Server(char ip_address[], int port_num);


/**
* @Function Name: bool Is_MQTT_Connected(void);
* @brief : used to check weather connected to gprs or not
*/
bool Is_MQTT_Connected(void);


/**
* @Function Name: void MQTT_Loop(void);
* @brief : used to initialize all services of MQTT
*/
char MQTT_Loop(void);

/************************************************************ SD CARD ***************************************/
bool Sd_Card_Init(int chip_select);



void Serial_Mon_Write(char *string);


String convertToString(char* a, int size);

bool Check_Mqtt_Data_Available(void);

bool Read_SMS(void);


char Send_AT_Command(char* ATcommand, char* expected_answer,  char* expected_answer2,unsigned int timeout, unsigned char retry);

void Select_Gpio_Mode_PORTE(int,int,int);

char My_Gpio_Status_Read(int PORT,int Pin); 

bool AT_Command_Test(char *at_command, char* expected_output1, char* expected_output2, int timeout);


void USART_Flush(void);

#endif /* HARDWARE_CONFIG_H_ */