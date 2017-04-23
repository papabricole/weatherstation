#include "SHT21.h"
#include <Wire.h>

#define SHT21_ADDRESS 0x40 // I2C address for the sensor

#define TRIGGER_TEMP_MEASURE_NOHOLD 0xF3
#define TRIGGER_HUMD_MEASURE_NOHOLD 0xF5
#define TRIGGER_SOFT_RESET 0xFE

SHT21::SHT21()
{
}

void SHT21::begin()
{
    Wire.begin(D1, D2);
}

void SHT21::reset()
{
    Wire.beginTransmission(SHT21_ADDRESS);
    Wire.write(TRIGGER_SOFT_RESET); // go to sleep ?
    Wire.endTransmission();
}

float SHT21::getHumidity() const
{
    return (-6.f + 125.f / 65536.f * readSHT21(TRIGGER_HUMD_MEASURE_NOHOLD));
}

float SHT21::getTemperature() const
{
    return (-46.85f + 175.72f / 65536.f * readSHT21(TRIGGER_TEMP_MEASURE_NOHOLD));
}

uint16_t SHT21::readSHT21(uint8_t command) const
{
    Wire.beginTransmission(SHT21_ADDRESS);
    Wire.write(command);
    Wire.endTransmission();
    delay(100);

    Wire.requestFrom(SHT21_ADDRESS, 3);
    while (Wire.available() < 3) {
        delay(1);
    }

    // return result
    uint16_t result = ((Wire.read()) << 8);
    result += Wire.read();
    result &= ~0x0003; // clear two low bits (status bits)

    return result;
}
