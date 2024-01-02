/*
 * Lira_Sensor.h
 *
 * Created: 24-08-2021 10:36:12
 *  Author: Tushar (SIO)
 */ 






/****************************************************** declare all macros **************************************/

#ifndef LIRA_SENSOR_H_
#define LIRA_SENSOR_H_

#include <Euro_Valve.h>


#define _kMAX_LIRA_SENSOR_NODE                                14
#define _kLIRA_SLOW_DATA_PROCESS                              900
#define _kLIRA_FAST_DATA_PROCESS                              700
#define _kMAX_MOISTURE_SENSOR                                 4
#define _kEACH_PLOT_DATA_LEN                                  44
#define _kSM_SENSOR_ERROR                                     124
#define _kLIRA_PLOT_LOW_BATTARY                               740
#define _kMAX_PLOTS_LIMIT                                     100 - _kMAX_LIRA_SENSOR_NODE




/**************************************************** declare all structures **************************************/

typedef struct Lira_Node_Param
{
	
	unsigned char Lira_Sensor_Number;
	//unsigned char Lira_Node_ID[7];
	//unsigned char Packet_ID;
	//unsigned int Polling_Time;
	//unsigned int Irrig_Timer;
	unsigned char Euro_Valve_Group[_kMAX_VALVE_FOR_1PLOT];
	//unsigned int Lira_Battery_Level;
	//unsigned int Light_Intensity;
	//unsigned int Soil_Temp;
	//unsigned int Humidity;
	//unsigned int Air_Temperature;
	//unsigned int Percent_Moisture[_kMAX_MOISTURE_SENSOR];
	//unsigned char Selected_Depth;
	//unsigned char WP_Value;
	//unsigned char FC_Value;
	//unsigned char Operation_Mode;
}Lira_Node_Param;



enum _eLIRA_SENSOR_PACKET
{
	_eBROADCASTING_PACKET                     =  0X01,
	_eLIRA_SENSOR_POLLING_DATA_PACKET,
};



/******************************************************* declare all variables ***************************************/

extern Lira_Node_Param _sLira_Node_Param[_kMAX_LIRA_SENSOR_NODE];





#endif /* LIRA_SENSOR_H_ */