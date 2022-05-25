#include "LED.h"

LED::LED(int pin) {
  pin_ = pin;
  state_ = LOW;
  pinMode(pin, OUTPUT);
}

void LED::on() {
  state_ = HIGH;
  digitalWrite(pin_, state_);
}

void LED::off() {
  state_ = LOW;
  digitalWrite(pin_, state_);
}

int LED::getState() {
  return state_;
}
