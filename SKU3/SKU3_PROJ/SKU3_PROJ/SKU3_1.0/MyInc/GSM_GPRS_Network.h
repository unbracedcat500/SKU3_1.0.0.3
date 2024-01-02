/*
 * GSM_GPRS_Network.h
 *
 * Created: 25-08-2021 16:20:53
 *  Author: Tushar (SIO)
 */ 


#ifndef GSM_GPRS_NETWORK_H_
#define GSM_GPRS_NETWORK_H_

#include <stdio.h>


/***************************************************** declare all structcure ********************************/

typedef struct Sim_APNs
{
// 	char Vodafone[4];
 //	char Airtel[15];
// 	char Jio[7];
// 	char Idea[14];
// 	char BSNL[8];
	//unsigned char BSNL[20];
}Sim_APNs;


typedef struct GSM_Alert
{
	uint32_t SMS_Alert;
	unsigned char SMS_Already_Sent;
	//unsigned char SMS_RX_TX_Buff[30];
	unsigned char Gsm_Module_Test;
}GSM_Query;


enum _eGSM_ALERT
{
	_eRUBLE_START                                 = 0X01,
	_eMODE_CHANGED                                = 0x02,
	_eDATA_LOGGER_MODE_SELECTED                   = 0x04,
	_eMANUAL_MODE_SELECTED                        = 0x08,
	_eAUTO_MODE_SELECTED                          = 0x10,
	_eTIMER_MODE_SELECTED                         = 0x20,
	_eTIMER_STOP_MANUAL_MODE_SELECTED             = 0x40,
	_eTIMER_SET                                   = 0x80,
	_eTIMER_OFF                                   = 0x0100,
	_eTIMER_STOPPED                               = 0x0200,
	_eGPUMP_ON                                     = 0x0400,
	_eGPUMP_OFF                                    = 0x0800,
	_eDRY_RUN                                     = 0x1000,
	_eNO_ELECTRICITY                              = 0x2000,
	_eELECTRICITY_RESUMED                         = 0x4000,
	_e3PHASE_POWER_INTERRUPTED                    = 0x8000,
	_eSENSOR_OUT_OF_SOIL                          = 0x00010000,
	_eLIRA_NODE_BATT_LOW                          = 0x00020000,
	_eEURO_BATT_LOW                               = 0x00040000,
	_eGPUMP_OVERLOAD                               = 0x00080000,
	_ePUMP_DRY_RUN                                = 0x00100000,
	_eVALVE_SENSE_FAIL                            = 0x00200000,
	_eVALVE_FEEDBACK_FAIL                         = 0x00400000,
	_eSMS_ALERT_ON                                = 0X00800000,
	_eLINE_FAULT_SMS                              = 0X01000000,

};



/*********************************************** declare all variables *****************************************/

extern GSM_Query _sGSM_Query;
extern Sim_APNs  _sSim_APNs;


/********************************************** declare all functions ***************************************/

/**
* @Function Name: void Store_All_APN(void);
* @brief : used to store all APNS
*/
void Store_All_APN(void);


/**
* @Function Name: void Check_GSM_Signal(void);
* @brief : used to check the gsm signal. And update the range symbol
*/
void Check_GSM_Signal(void);


/**
* @Function Name: void Get_Local_Time(void);
* @brief : used to read the local time using AT command.
*/
void Get_Local_Time(void);



/**
* @Function Name: void Read_GSM_Response(void);
* @brief : used to read the response from the GSM 
*/
void Read_GSM_Response(void);


/**
* @Function Name: void Send_SMS_Alert(void);
* @brief : used to send the SMS
*/
void Send_SMS_Alert(void);


bool Gsm_Gprs_Network(void);



#endif /* GSM_GPRS_NETWORK_H_ */