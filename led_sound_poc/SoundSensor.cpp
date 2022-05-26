#include "SoundSensor.h"

SoundSensor::SoundSensor(int pin, int threshPct): soundBuffer_(BUFFER_CAPACITY), soundBufferSorted_(BUFFER_CAPACITY) {
  pin_ = pin;
  threshPct_ = threshPct;
}

int SoundSensor::readVal() {
  val_ = analogRead(pin_);
  return val_;
}

int SoundSensor::getVal() {
  return val_;
}

// adds a sound to the queue and insert in order into the soundBufferSorted_ vector
// returns True if bufferCapacity reached, otherwise false
bool SoundSensor::addSound() {
  // read the value and add it to the end of the queue
  int val = readVal();
  soundBuffer_.enqueue(val);
  // add that same value in order to soundBufferSorted_
  // TODO: implement binary search for adding
  soundBufferSorted_.insert_ordered(val);
  // return whether or not the sound buffer is full to remove a value 
  if (soundBuffer_.full() || soundBufferSorted_.full())
    return true;
  else
    return false;
}


// removes the first sound in the sound queue and the corresponding value in the soundBufferSorted_ vector
void SoundSensor::rmSound() {
  int rmVal = soundBuffer_.dequeue();
  int sortedIdx = soundBufferSorted_.search(rmVal);
  if (sortedIdx == soundBufferSorted_.size()) 
    Serial.println("No corresponding sorted buffer value found");
  else
    soundBufferSorted_.remove(sortedIdx);
}


// compute the upper and lower threshold values from the percentage and capacity
int SoundSensor::convertThreshToIdx() {
  // assume both soundBuffer_ and soundBufferSorted_ are the same size
  int bufferSize = soundBuffer_.size();
  int idx = (int)((bufferSize * ((double)threshPct_/100.0))/2);
  threshIdxLower_ = idx;
  return bufferSize;
}

// gets the current threshold value to produce a reaction for
int SoundSensor::getLowerThreshVal() {
  return soundBufferSorted_.select(threshIdxLower_);
}
int SoundSensor::getUpperThreshVal(int bufferSize) {
  return soundBufferSorted_.select(bufferSize - threshIdxLower_);
}
