#pragma once

class SHT21 {
public:
    SHT21();

    void begin();
    void reset();
    float getHumidity() const;
    float getTemperature() const;

private:
    unsigned short readSHT21(unsigned char command) const;
};
