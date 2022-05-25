#include "SoundSensor.h"

SoundSensor::SoundSensor(int pin, int bufferCapacity, int threshPct) {
  pin_ = pin;
  bufferCapacity_ = bufferCapacity;
  threshPct_ = threshPct;
}

int SoundSensor::readVal() {
  val_ = analogRead(pin_);
  return val_;
}

int SoundSensor::getVal() {
  return val_;
}
