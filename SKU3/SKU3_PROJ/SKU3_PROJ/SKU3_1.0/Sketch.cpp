// /*Begining of Auto generated code by Atmel studio */
// #include <Arduino.h>
// 
// 
// #include "EmonLib.h"
// #include <ArduinoJson.h>
// #include <LiquidCrystal_I2C.h>
// #include <RHReliableDatagram.h>
// #include <RH_RF95.h>
// #include <SPI.h>
// #include <EEPROM.h>
// #include <SD.h>                                     
// 
// 
// 
// 
// #define TINY_GSM_MODEM_M95                             
// #define SerialMon                                      Serial
// #define SerialAT                                       Serial1
// #define GSM_AUTOBAUD_MIN                               9600
// #define GSM_AUTOBAUD_MAX                               115200
// #define TINY_GSM_USE_GPRS                              true
// #define TINY_GSM_TEST_SMS                              true
// 
// #include <TinyGsmClient.h>
// #include <PubSubClient.h>

// 

// #define CLIENT_ADDRESS 1
// #define SERVER_ADDRESS 2
// 
// //LiquidCrystal_I2C lcd(0x27, 20, 4);
// 
// 
// RH_RF95 driver;
// //RH_RF95 driver(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
// 
// RHReliableDatagram manager(driver, SERVER_ADDRESS);
// 
// 
// void setup() 
// {
// 
//   Serial.begin(9600);
//   while (!Serial) ; // Wait for serial port to be available
//   if (!manager.init())
//     Serial.println("init failed");
//   
// }
// 
// uint8_t data[] = "And hello back to you";
// // Dont put this on the stack:
// uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
// 
// void loop()
// {
//   if (manager.available())
//   {
//     
//     uint8_t len = sizeof(buf);
//     uint8_t from;
//     if (manager.recvfromAck(buf, &len, &from))
//     {
//       Serial.print("got request from : 0x");
//       Serial.print(from, HEX);
//       Serial.print(": ");
//       Serial.println((char*)buf);
// 
//       if (!manager.sendtoWait(data, sizeof(data), from))
//         Serial.println("sendtoWait failed");
//     }
//   }
// }
// #include <Hardware_Config.h>
// 
// 
// void my(void)
// {
// 	_kLCD_INIT();
// }