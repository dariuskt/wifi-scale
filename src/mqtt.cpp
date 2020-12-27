#include "mqtt.h"

const char* ssid = "";
const char* password = "";

WiFiClient espClient;
PubSubClient mqtt(espClient);

void sendMessage()
{
    if (mqtt.connected()) {
        mqtt.publish("scale/grams", (char*)String(state.grams).c_str(), true);
        mqtt.publish("scale/battery", (char*)String(state.battery).c_str(), true);
        mqtt.publish("scale/pieces", (char*)String(state.pieces).c_str(), true);
    }
}

void initMqtt()
{
    setupWifi();
}

void setupWifi()
{
    //Serial.begin(460800);
    //Serial.setDebugOutput(true);
    //system_phy_set_powerup_option(3);
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    randomSeed(micros());
}

void setupMqtt()
{
    IPAddress mqtt_ip;
    WiFi.hostByName("broker.lan", mqtt_ip);
    mqtt.setServer(mqtt_ip, 1883);


    String clientId = WiFi.hostname();
    clientId += String(random(0xffff), HEX);
    mqtt.connect(clientId.c_str());
}

void updateWifiStatus()
{
    switch (WiFi.status()) {
        case WL_CONNECTED:
            state.wifi = 2;
            if (mqtt.connected()) {
                state.wifi=3;
                sendMessage();
            } else {
                Serial.println("Connecting to MQTT broker");
                setupMqtt();
            }
            break;
        case WL_IDLE_STATUS:
            state.wifi = 1;
            break;
        case WL_CONNECT_FAILED:
            state.wifi = 4;
            break;
        default:
            state.wifi = 0;
    }
}