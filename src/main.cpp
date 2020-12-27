#include <Arduino.h>
#include "HX711.h"
#include "mqtt.h"

#define DOUT  D4
#define CLK  D3
HX711 scale;

void initScale() {
    scale.begin(DOUT, CLK);
    scale.power_up();

    scale.set_scale(state.scale_cal);
    scale.set_offset(state.scale_zero);
}

void readScale() {
    state.grams = scale.get_units(30);
    state.pieces = ((state.piece_grams/2)+state.grams)/state.piece_grams;
}
void readBattery() {
    //  (bat)----[180k]----[220k]--(A0)--[100k]----(GND)
    state.battery = map(analogRead(A0), 0, 1024, 0, state.battery_range)/1000.0;
}

// ===============================================

void setup() {
    Serial.begin(76800);
    Serial.println("\nBooting... ");

    initMqtt();
    initScale();
 }

void loop() {
    delay(1);
    updateWifiStatus();

    if (state.grams == 0) {
        readScale();
        readBattery();
    }
    if (state.wifi >= 2) {
        Serial.printf("%.3fg; %.3fV; wifi: %d\n", state.grams, state.battery, state.wifi);
        sendMessage();
        scale.power_down();
        delay(100);
        Serial.print("Go back to sleep.");
        ESP.deepSleep(0);
    }
}