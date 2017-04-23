#pragma once

#include <Arduino.h>

class SHT21 {
public:
    SHT21();

    void begin();
    void reset();
    float getHumidity() const;
    float getTemperature() const;

private:
    uint16_t readSHT21(uint8_t command) const;
};

