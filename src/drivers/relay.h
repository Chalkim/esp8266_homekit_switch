#ifndef HOMEKIT_DRIVERS_RELAY_H_
#define HOMEKIT_DRIVERS_RELAY_H_

#include <Arduino.h>

class Relay
{
private:
    uint8_t pin_;
    uint8_t state_;

public:
    Relay(uint8_t pin);
    Relay(uint8_t pin, uint8_t state);
    void SetPin(uint8_t pin);
    uint8_t GetPin();
    void SetState(uint8_t state);
    uint8_t GetState();
    uint8_t Reverse();
};

#endif // HOMEKIT_DRIVERS_RELAY_H_
