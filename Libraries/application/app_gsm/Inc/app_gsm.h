#ifndef APPLICATION_APP_GSM_INC_APP_GSM_H_
#define APPLICATION_APP_GSM_INC_APP_GSM_H_

#include <stdbool.h>
#include "usart.h"
#include "cJSON.h"
#include "app_eeprom.h"

#define     NETWORK_TIMEOUT          60000
#define     PUBLISH_TIMEOUT          5000
#define     MAX_MESSAGE_BUFFER       2023
#define     RECEIVE_WAIT_TIME_GSM    1000

extern uint8_t dgState[10];

extern char data_collected_at[50];
extern char signal_strength[10];
extern bool timeReceivedFlag;
extern bool signalReceivedFlag;
extern bool apnFlag;

extern bool mqttMsgFreqOK;

typedef enum{
	AT,
	UE_Functionality,     // CFUN
	SIM_Inserted,         // CPIN
	Singal_Level,         // CSQ
	Domain_Nwt_Reg_Stat,  // CGREG
	APN_Conn,             // QICSGP
    PDP_Context,          // QIACT
	Get_Time,             // QLTS
    MQTT_Config_Param,    // QMTCFG
	MQTT_Open_Nwt,        // QMTOPEN
	MQTT_Close_Nwt,       // QMTCLOSE
	MQTT_Conn_Client,     // QMTCONN
	MQTT_Disc,            // QMTDISC
    MQTT_Sub_Topic,       // QMTSUB
	MQTT_Unsub_Topic,     // QMTUNS
	MQTT_publish_msg      // QMTPUB
}gsmATCmd;

typedef enum {
	ATisOK,
	CGREGisOK,
	NETOPENisOK,
	GetTimeisOK,
	MqttOPENisOK,
	MqttCONNisOK,
	MqttPUBHisOK,
	MqttDISCisOK,
	MQTTRetry
} gsmCmdStatTypedef;

extern gsmCmdStatTypedef gsmCmdStat;

typedef struct{
	uint8_t CmdToSend;
	bool SimConnected : 1;
	bool APNSent : 1;
	char time[50];
} gsmConfigSimTypedef;

void gsmMqtt(iotParam_Typedef *iot);
bool AT_Transmit(char *cmd, uint16_t timeout);
void AT_Receive(char *response, uint16_t timeout);
void AT_ReceiveTillIdle(char *response, uint16_t timeout);
bool AT_Transmit_Receive(char *cmd, char* response, uint16_t timeout);
bool AT_OK(uint32_t timeout);
bool AT_CGREG_OK(uint32_t timeout);
bool AT_NETOPEN(uint32_t timeout);
void AT_GET_TIME(void);
bool AT_GET_SIGNAL_STRENGTH(uint32_t timeout);
bool AT_MQTTOPEN(iotParam_Typedef *iot, uint32_t timeout);
bool AT_MQTTCONN(iotParam_Typedef *iot, uint32_t timeout);
bool AT_MQTTPUBH(iotParam_Typedef *iot, uint32_t timeout);
bool AT_DISC(uint32_t timeout);
void rand_str(char *dest, size_t length);
void randClientIDGenerate(iotParam_Typedef *iot, char *mqCID, unsigned seed, uint8_t randStrLen);

void convertDateFormat(char *input, char *output);
void start_gsm();
void stop_gsm();

void gsm_sendMsg(char* msg, char* number);

#endif /* APPLICATION_APP_GSM_INC_APP_GSM_H_ */
