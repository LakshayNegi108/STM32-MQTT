/*
 * MQTT.h
 *
 *  Created on: Dec 5, 2023
 *      Author: Engineering
 */

#ifndef MQTT_INC_MQTT_H_
#define MQTT_INC_MQTT_H_

#include <stdint.h>
#include <stdbool.h>
#include "usart.h"

#define CLIENT_ID_LEN 51

typedef struct __mqtt_typedef{
	bool isConnected;
    char clientID[CLIENT_ID_LEN];
    uint16_t clientIDLen;
	bool (*connectionStatus)(struct __mqtt_typedef *m);
    bool (*clientConnect)(struct __mqtt_typedef *m, char* userID, char* userPassword, uint16_t timeout);
    bool (*ConfigServer)(char* brokerAddr, uint16_t brokerPort, uint16_t timeout);
    bool (*KeepAlive)(uint16_t timeinSec, uint16_t timeout);
    bool (*revMode)(uint16_t timeout);
    bool (*Subscribe)(char* topic, uint16_t timeout);
    bool (*Publish)(char* msg, uint16_t timeout);
    bool (*setCallback)(struct __mqtt_typedef *m, void (*cb)(void));
    void (*Callback)(void);
    bool (*reconnect)(uint16_t timeout);
    void (*clientIDLoop)(struct __mqtt_typedef *m);
}mqtt_typedef;

bool mqtt_connectionStatus(mqtt_typedef *mqtt_type);
bool mqtt_ClientConnect(mqtt_typedef *mqtt_type, char* userID, char* userPassword, uint16_t timeout);
bool mqtt_configServer(char* brokerAddr, uint16_t brokerPort, uint16_t timeout);
void mqtt_clientIDLoop(mqtt_typedef *mqtt_type, uint16_t randSeed);
bool mqtt_keepAlive(uint16_t timeisSec, uint16_t timeout);
bool mqtt_revMode(uint16_t timeout);
bool mqtt_subscribe(char* topic, uint16_t timeout);
bool mqtt_publish(char* msg, uint16_t timeout);
bool mqtt_reconnect(uint16_t timeout);
bool mqtt_setCallback(mqtt_typedef *mqtt_type, void (*cb)(void));
__weak void myCB(void);


void rand_str(char *dest, size_t length);
// void myCB();

#endif /* MQTT_INC_MQTT_H_ */
