#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "global.h"

void sendMessage();

void initMqtt();
void setupWifi();
void setupMqtt();
void destroyMqtt();
void updateWifiStatus();
void configCallback(char* topic, byte* payload, unsigned int length);

#endif // MQTT_H