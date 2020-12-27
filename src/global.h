#ifndef GLOBAL_H
#define GLOBAL_H

struct globalState {
    float grams = 0;
    float battery = 0;
    int pieces = 0;
    int wifi = 0;
    int battery_range = 4860;
    float piece_grams = 85.0;
    float scale_zero = 185000;
    float scale_cal = 402.80;
};

extern globalState state;


#endif // GLOBAL_H