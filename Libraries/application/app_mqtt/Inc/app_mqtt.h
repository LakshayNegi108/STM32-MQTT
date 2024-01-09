#ifndef APPLICATION_APP_MQTT_INC_APP_MQTT_H_
#define APPLICATION_APP_MQTT_INC_APP_MQTT_H_

#include "MQTT.h"

//mqtt_typedef mqtt = {
//    .isConnected = false,
//    .clientID = "\0",
//    .clientIDLen = CLIENT_ID_LEN,
//    .connectionStatus = &mqtt_connectionStatus,
//    .clientConnect = &mqtt_ClientConnect,
//    .ConfigServer = &mqtt_configServer,
//    .KeepAlive = &mqtt_keepAlive,
//    .revMode = &mqtt_revMode,
//    .Subscribe = &mqtt_subscribe,
//    .Publish = &mqtt_publish,
//    .reconnect = &mqtt_reconnect,
//    .clientIDLoop = &mqtt_clientIDLoop
//};

#define CLIENT_ID_LEN 51

extern mqtt_typedef mqtt;

void configMQTTParameters(mqtt_typedef* mqtt);

#endif /* APPLICATION_APP_MQTT_INC_APP_MQTT_H_ */
