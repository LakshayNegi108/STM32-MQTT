#include "app_gsm.h"

char ATcommand[MAX_MESSAGE_BUFFER];
uint8_t buffer[MAX_MESSAGE_BUFFER] = { 0 };

gsmCmdStatTypedef gsmCmdStat = ATisOK;

uint32_t PreviousTick;
char data_collected_at[50];
char signal_strength[10];
bool timeReceivedFlag = false;
bool signalReceivedFlag = false;
bool mqttMsgFreqOK = false;

// gsmMqtt function is responsible for the connection with MQTT and publishing and subscribing the topic.
void gsmMqtt(iotParam_Typedef *iot) {
	switchStart: switch (gsmCmdStat) {
	case ATisOK:
		if (AT_OK(RECEIVE_WAIT_TIME_GSM)) {
			gsmCmdStat = CGREGisOK;
		} else {
			gsmCmdStat = ATisOK;
		}
		break;
	case CGREGisOK:
		if (AT_CGREG_OK(RECEIVE_WAIT_TIME_GSM)) {
			gsmCmdStat = NETOPENisOK;
		} else {
			gsmCmdStat = ATisOK;
		}
		break;
	case NETOPENisOK:
		if (AT_NETOPEN(RECEIVE_WAIT_TIME_GSM)) {
			gsmCmdStat = MqttOPENisOK;
		} else {
			gsmCmdStat = CGREGisOK;
		}
		break;
	case GetTimeisOK:
		AT_GET_TIME();
		break;
	case MqttOPENisOK:
		if (AT_MQTTOPEN(iot, RECEIVE_WAIT_TIME_GSM)) {
			gsmCmdStat = MqttCONNisOK;
			goto switchStart;
		} else {
			gsmCmdStat = MqttDISCisOK;
		}
		break;
	case MqttCONNisOK:
		if (AT_MQTTCONN(iot, RECEIVE_WAIT_TIME_GSM)) {
			gsmCmdStat = MqttPUBHisOK;
		} else {
			gsmCmdStat = MqttOPENisOK;
		}
		randClientIDGenerate(&iot, iot->mqtt.ClientID,
				(iot->deviceID[3] << 16
						| (iot->deviceID[10] << 8 | iot->deviceID[11])), 4);
		break;
	case MqttPUBHisOK:
		if (mqttMsgFreqOK) {
			AT_GET_TIME();
			AT_GET_SIGNAL_STRENGTH(1000);
			if (AT_MQTTPUBH(iot, RECEIVE_WAIT_TIME_GSM)) {
				gsmCmdStat = MqttPUBHisOK;
			} else {
				gsmCmdStat = MqttDISCisOK;
			}
			mqttMsgFreqOK = false;
		}
		break;
	case MqttDISCisOK:
		if (AT_DISC(500)) {
			gsmCmdStat = MqttOPENisOK;
		} else {
			gsmCmdStat = ATisOK;
		}
		break;
	default:
		gsmCmdStat = ATisOK;
		break;
	}
}

bool AT_Transmit(char *cmd, uint16_t timeout) {
	if (HAL_UART_Transmit(&huart6, (char*) cmd, strlen(cmd), timeout)
			!= HAL_OK) {
		return false;
	} else {
		return true;
	}
}

void AT_Receive(char *response, uint16_t timeout) {
	HAL_UART_Receive(&huart6, (char*) response, 100, timeout);
}

void AT_ReceiveTillIdle(char *response, uint16_t timeout) {
	int revlen = 0;
	HAL_UARTEx_ReceiveToIdle(&huart6, (char*) response, 100, &revlen, timeout);
}

bool AT_Transmit_Receive(char *cmd, char *response, uint16_t timeout) {
	if (cmd == NULL || response == NULL) {
// Invalid input parameters
		return false;
	}
	char buffer[100] = { 0 };
	if (!AT_Transmit(cmd, timeout)) {
		return false;
	}
	AT_Receive(buffer, timeout);
	if (strstr(buffer, response) != NULL) {
		return true;
	} else {
		return false;
	}
}

bool AT_OK(uint32_t timeout) {
	if (AT_Transmit_Receive("AT\r\n", "OK", 500)) {
		if (AT_Transmit_Receive("AT+CFUN=1\r\n", "OK\r", 1000)) {
			if (AT_Transmit_Receive("AT+CPIN?\r\n", "+CPIN: READY\r\n\r\nOK",
					500)) {
				return true;
			}
		}
	}
	return false;
}

bool AT_CGREG_OK(uint32_t timeout) {
	if (AT_Transmit("AT+CGREG?\r\n", 100)) {
		AT_Receive(buffer, 1000);
		if (buffer != NULL) {
			if ((strstr(buffer, "+CGREG: 0,1") != NULL)
					|| (strstr(buffer, "+CGREG: 0,5") != NULL)) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
	return false;
}

bool AT_GET_SIGNAL_STRENGTH(uint32_t timeout) {
//	signal_strength
	char dataBuf[30];
	int signal = 0, error = 0;
	if (AT_Transmit("AT+CSQ\r\n", 100)) {
		AT_Receive(dataBuf, timeout);
		char *start = strstr(dataBuf, "+CSQ:");
		int items = sscanf(start, "+CSQ: %d,%d", &signal, &error);
		if (items == 2) {
			sprintf(signal_strength, "%d,%d", signal, error);
			signalReceivedFlag = true;
			return true;
		}
	}
	signalReceivedFlag = false;
	return false;
}

bool AT_NETOPEN(uint32_t timeout) {
	char APN[] = "airtelgprs.com";
	static bool apnFlag = false;
	if (!apnFlag) {
		sprintf(ATcommand, "AT+QICSGP=1,1,\"%s\"\r\n", APN);
		if (AT_Transmit_Receive(ATcommand, "OK", 1000)) {
			if (AT_Transmit_Receive("AT+QIACT=1\r\n", "OK", 1000)) { // for GPRS Connection
				apnFlag = true;
			}
		}
	}
	char dataBuf[50];
	if (AT_Transmit("AT+QIACT?\r\n", 100)) {
		AT_Receive(dataBuf, 1000);
		if (strstr((char*) dataBuf, "+QIACT: 1,1,1")) {
			return true;
		} else {
			apnFlag = false;
		}
	}
	return false;
}

bool AT_MQTTOPEN(iotParam_Typedef *iot, uint32_t timeout) {
	char buff[100] = { 0 };
	sprintf(ATcommand, "AT+QMTCFG=\"recv/mode\",0,0,1 \r\n");
	if (AT_Transmit_Receive(ATcommand, "OK", timeout)) {
		sprintf(ATcommand, "AT+QMTOPEN=4,\"%s\",%d \r\n", iot->mqtt.BrokerAddress,
				iot->mqtt.brokerPort);
		if (AT_Transmit(ATcommand, timeout)) {
			AT_ReceiveTillIdle(buff, timeout);
			if (strstr(buff, "OK") != NULL) {
				AT_ReceiveTillIdle(buff, timeout);
				if (strstr(buff, "+QMTOPEN: 4,0") != NULL) {
					return true;
				} else if (strstr(buff, "+QMTOPEN: 4,2") != NULL) {
					randClientIDGenerate(&iot, iot->mqtt.ClientID,
							(iot->deviceID[3] << 16
									| (iot->deviceID[10] << 8 | iot->deviceID[11])), 4);
					return false;
				}
			}
		}
	}
	return false;
}

bool AT_MQTTCONN(iotParam_Typedef *iot, uint32_t timeout) {
	char buff[100] = { 0 };
	sprintf(ATcommand, "AT+QMTCONN=4,\"%s\",\"%s\",\"%s\" \r\n", iot->mqtt.ClientID,
			iot->mqtt.userID, iot->mqtt.userPassword);
	if (AT_Transmit(ATcommand, timeout)) {
		AT_ReceiveTillIdle(buff, timeout);
		if (strstr(buff, "OK") != NULL) {
			AT_ReceiveTillIdle(buff, timeout);
			if (strstr(buff, "+QMTCONN: 4,0,0") != NULL) {
				sprintf(ATcommand, "AT+QMTSUB=4,1,\"%s\",0" "\r",
						iot->mqtt.topicSubscribe);
				if (AT_Transmit_Receive(ATcommand, "+QMTSUB:", timeout)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool AT_MQTTPUBH(iotParam_Typedef *iot, uint32_t timeout) {
	cJSON *root = get_json_object();
	char *json_string = cJSON_Print(root);
	if (json_string == NULL) {
		cJSON_Delete(root);
		cJSON_free(json_string);
		return false;
	} else {
		int len = strlen(json_string);
		sprintf(ATcommand, "AT+QMTPUBEX=4,1,0,0,\"%s\",%d\r\n", iot->mqtt.topicPublish[0],
				iot->mqtt.TOPIC_PUBLISH_LEN[0]);
		if (AT_Transmit_Receive(ATcommand, ">", timeout)) {
			sprintf(ATcommand, json_string);
			json_string[len] = '\0';
			if (AT_Transmit_Receive(ATcommand, "+QMTPUBEX:", timeout)) {
				cJSON_Delete(root);
				cJSON_free(json_string);
				return true;
			}
		}
		cJSON_Delete(root);
		cJSON_free(json_string);
		return false;
	}
}

bool AT_DISC(uint32_t timeout) {
	return AT_Transmit_Receive("AT+QMTDISC=4,0\r\n", "OK", timeout);
}

void AT_GET_TIME(void) {
	char dataBuf[60];
	if (AT_Transmit("AT+QLTS=2\r\n", 100)) {
		AT_Receive(dataBuf, 1000);
		if ((dataBuf != NULL) && strstr((char*) dataBuf, "+QLTS: \"")) {
			char timestamp[40];
			// we are getting "AT+QLTS=2\r\r\n+QLTS: \"2023/06/26,19:04:30+22,0\"\r\n\r\nOK\r\n" after the AT command for time, So after the 19 character we are getting timestamp.
//			strncpy(timestamp, dataBuf + 20, 22);
			char *idx = strstr(dataBuf, "+QLTS:");
			if (idx != NULL) {
				if ((sscanf(idx, "+QLTS: \"%s\"\r\n\r\nOK\r\n", timestamp) == 1)
						&& data_collected_at != NULL) {
					convertDateFormat(timestamp, data_collected_at);
					timeReceivedFlag = true;
					return;
				}
			}
		}
	}
	timeReceivedFlag = false;
	return;
}

void convertDateFormat(char *input, char *output) {
// Extract the date, time, and timezone offset from the input string
	char date[11];
	char time[9];
	int timezoneOffset;
	sscanf(input, "%[^,],%[^+]+%d", date, time, &timezoneOffset);

// Replace '/' with '-' in the date
	for (int i = 0; i < strlen(date); i++) {
		if (date[i] == '/')
			date[i] = '-';
	}

// Replace ',' with '$' in the date-time separator
	char separator = 'T';

// Replace ':' with ':' and add ".000" for milliseconds
	char milliseconds[] = ":000";

// Calculate the modified timezone offset in hours and minutes
	int modifiedOffsetHours = (timezoneOffset % 4) * 150 / 60;
	int modifiedOffsetMinutes = (timezoneOffset % 4) * 15 % 60;

// Format the output string
	sprintf(output, "%s%c%s%s+0%d:%d", date, separator, time, milliseconds,
			modifiedOffsetHours, modifiedOffsetMinutes);
}

void gsm_sendMsg(char *msg, char *number) {
	char buffer[50];
	AT_Transmit_Receive("AT+CMGF=1\r\n", "OK", 1000);
	AT_Transmit_Receive("AT+CSCS=\"GSM\"\r\n", "OK", 1000);
	sprintf(buffer, "AT+CMGS=\"%s\"\r\n", number);
	AT_Transmit(buffer, 100);
	sprintf(buffer, "%s%c", msg, 0x1A);
//	sprintf(buffer, "%s\032", msg);
	AT_Transmit(buffer, 100);
	AT_Transmit("AT+CMGL=\"ALL\"\r\n", 1000);
}

void start_gsm() {
	HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, RESET); // Module Supply on
}
void stop_gsm() {
	HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, SET); // Module Supply on
}
void reset_gsm(){
	HAL_GPIO_WritePin(GSM_RST_GPIO_Port, GSM_RST_Pin, SET);
}

/*
 * @brief Makes random string according to the seed given.
 *        Used to produce unique MQTT_CLIENT_ID for each IOT.
 */
void rand_str(char *dest, size_t length) {
	char charset[] = "0123456789"
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while (length-- > 0) {
		size_t index = rand() % (sizeof(charset) - 1);
		*dest++ = charset[index];
	}
	*dest = '\0';
}

void randClientIDGenerate(iotParam_Typedef *iot, char *mqCID, unsigned seed, uint8_t randStrLen) {
	static uint16_t i = 0;
	memset(mqCID, '\0', iot->mqtt.CLIENT_ID_LEN);
	strncat(mqCID, iot->deviceID, iot->DEVICE_ID_LEN);
	mqCID[iot->mqtt.CLIENT_ID_LEN - 6] = '_';
	srand(i + (iot->deviceID[3] << 16 | (iot->deviceID[10] << 8 | iot->deviceID[11])));
	char randStr[randStrLen];
	rand_str(randStr, randStrLen);
	strncat(mqCID, randStr, randStrLen);
	strncat(mqCID, '\0', 1);
	i++;
}
