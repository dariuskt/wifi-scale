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
void updateWifiStatus();

#endif // MQTT_H