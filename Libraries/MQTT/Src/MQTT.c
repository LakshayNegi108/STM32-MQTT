/*
 * MQTT.c
 *
 *  Created on: Dec 5, 2023
 *      Author: Lakshay
 */
#include "MQTT.h"

bool mqtt_connectionStatus(mqtt_typedef *mqtt_type){
    return mqtt_type->isConnected;
}

bool mqtt_ClientConnect(mqtt_typedef *mqtt_type, char* userID, char* userPassword, uint16_t timeout){
	//TODO
    print("MQTT Connected to: %s, %s\r\n", userID, userPassword);
    mqtt_clientIDLoop(mqtt_type, 8);
    mqtt_type->isConnected = true;
    return true;
}

bool mqtt_configServer(char* brokerAddr, uint16_t brokerPort, uint16_t timeout){
	//TODO
    print("MQTT Server Cfg to: %s, %d\r\n", brokerAddr, brokerPort);
    return true;
}

bool mqtt_keepAlive(uint16_t timeisSec, uint16_t timeout){
	//TODO
    print("MQTT Keep Alive to: %d\r\n", timeisSec);
    return true;
}

bool mqtt_revMode(uint16_t timeout){
	//TODO
    return true;
}

bool mqtt_subscribe(char* topic, uint16_t timeout){
	//TODO
    return true;
}

bool mqtt_publish(char* msg, uint16_t timeout){
	//TODO
    return true;
}

bool mqtt_reconnect(uint16_t timeout){
	// Let's use mqtt_ClientConnect() for now till figure out how to send values
    return true;
}

void mqtt_clientIDLoop(mqtt_typedef *mqtt_type, uint16_t randSeed){
	//TODO
    srand(randSeed);
    char str[mqtt_type->clientIDLen - 1];
    rand_str(str, mqtt_type->clientIDLen - 1);
    print("Str: %s\r\n", str);
    strncpy(mqtt_type->clientID, str, mqtt_type->clientIDLen - 1);
}

bool mqtt_setCallback(mqtt_typedef *mqtt_type, void (*cb)(void)){
    mqtt_type->Callback = cb;
    return true;
}

__weak void myCB(void){
    return;
}

void rand_str(char *dest, size_t length) {
	//TODO
	char charset[] = "0123456789"
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while (length-- > 0) {
		size_t index = rand() % (sizeof(charset) - 1);
		*dest++ = charset[index];
	}
	*dest = '\0';
}
