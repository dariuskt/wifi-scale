#include "mqtt.h"
#include "ArduinoJson.h"

const char* ssid = "";
const char* password = "";

WiFiClient espClient;
PubSubClient mqtt(espClient);

void sendMessage()
{
    if (mqtt.connected()) {
        char msg[128];
        char topic[64];
        sprintf(msg
            , "{\"host\":\"%s\",\"grams\":%f,\"pieces\":%d,\"battery\":%f,\"configured\":%d}"
            , config.name
            , state.grams
            , state.pieces
            , state.battery
            , state.configured
        );
        sprintf(topic, "scale/%s/data", config.name);
        mqtt.publish(topic, msg, true);
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
    //WiFi.persistent(false);
    //WiFi.mode(WIFI_STA);
    //WiFi.begin(ssid, password);

    randomSeed(micros());
}

void setupMqtt()
{
    String clientId = WiFi.hostname();

    mqtt.setServer("broker.lan", 1883);
    mqtt.setCallback(configCallback);
    mqtt.connect(clientId.c_str());
    mqtt.subscribe((char*)("config/"+clientId).c_str(), 1);
    Serial.println("mqtt setup done");
}

void configCallback(char* topic, byte* payload, unsigned int length) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    config.name = doc["name"];
    config.battery_range = doc["battery_range"];
    config.scale_zero = doc["scale_zero"];
    config.scale_cal = doc["scale_cal"];
    config.piece_grams = doc["piece_grams"];
    state.configured = 1;
}

void updateWifiStatus()
{
    switch (WiFi.status()) {
        case WL_CONNECTED:
            state.wifi = 2;
            if (mqtt.connected()) {
                state.wifi=3;
                mqtt.loop();
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