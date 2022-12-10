#include "drivers/relay.h"

Relay::Relay(uint8_t pin)
{
    pin_ = pin;
    pinMode(pin, OUTPUT);
    state_ = digitalRead(pin);
}

Relay::Relay(uint8_t pin, uint8_t state)
{
    pin_ = pin;
    state_ = state;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);
}

void Relay::SetPin(uint8_t pin)
{
    pin_ = pin;
}

uint8_t Relay::GetPin()
{
    return pin_;
}

void Relay::SetState(uint8_t state)
{
    state_ = state;
    digitalWrite(pin_, state_);
}

uint8_t Relay::GetState()
{
    state_ = digitalRead(pin_);
    return state_;
}

uint8_t Relay::Reverse()
{
    SetState((state_ == LOW)
                 ? HIGH
                 : LOW);
    return state_;
}
