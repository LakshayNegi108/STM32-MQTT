#include "app_mqtt.h"

void configMQTTParameters(mqtt_typedef* mqtt){
	mqtt->isConnected = false;
//	mqtt->clientID = "\0";
	memset(mqtt->clientID, '\0', CLIENT_ID_LEN);
	mqtt->clientIDLen = CLIENT_ID_LEN;
	mqtt->connectionStatus = &mqtt_connectionStatus;
	mqtt->clientConnect = &mqtt_ClientConnect;
	mqtt->ConfigServer = &mqtt_configServer;
	mqtt->KeepAlive = &mqtt_keepAlive;
	mqtt->revMode = &mqtt_revMode;
	mqtt->Subscribe = &mqtt_subscribe;
	mqtt->Publish = &mqtt_publish;
	mqtt->reconnect = &mqtt_reconnect;
	mqtt->clientIDLoop = &mqtt_clientIDLoop;

	mqtt->setCallback(&mqtt, &myCB);
}
