#include <Arduino.h>
#include "Ticker.h"
#include "HX711.h"
#include "mqtt.h"

#define DOUT  D4
#define CLK  D3
HX711 scale;

Ticker timer;

void initScale() {
    scale.begin(DOUT, CLK);
    scale.power_up();

    scale.set_scale(config.scale_cal);
    scale.set_offset(config.scale_zero);
}

void readScale() {
    state.grams = scale.get_units(30);
    state.pieces = ((config.piece_grams/2)+state.grams)/config.piece_grams;
}
void readBattery() {
    //  (bat)----[180k]----[220k]--(A0)--[100k]----(GND)
    state.battery = map(analogRead(A0), 0, 1024, 0, config.battery_range)/1000.0;
}
void selfDestruct() {
    Serial.print("Go back to sleep.");
    ESP.deepSleep(0);
}
// ===============================================

void setup() {
    Serial.begin(76800);
    Serial.println("\nBooting... ");

    timer.attach(EXECUTION_TIMEOUT, selfDestruct);
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
    if (state.wifi >= 2 && state.configured >0) {
        Serial.printf("%.3fg; %.3fV; wifi: %d\n", state.grams, state.battery, state.wifi);
        sendMessage();
        scale.power_down();
        delay(100);
        destroyMqtt();
        selfDestruct();
    }
}