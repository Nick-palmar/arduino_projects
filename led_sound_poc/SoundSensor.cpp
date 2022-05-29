#include "SoundSensor.h"

SoundSensor::SoundSensor(int pin, int threshPct, int bufferCap): soundBuffer_(bufferCap), soundBufferSorted_(bufferCap) {
  pin_ = pin;
  threshPct_ = threshPct;
  bufferCapacity_ = bufferCap;
}

void SoundSensor::printBufferInfo(bool debug) {
  Serial.print("Buffer capacity: ");
  Serial.println(soundBuffer_.capacity_);
  Serial.print("Buffer size: ");
  Serial.println(soundBuffer_.size());

  Serial.print("Buffer sorted capacity: ");
  Serial.println(soundBufferSorted_.capacity());
  Serial.print("Buffer sorted size: ");
  Serial.println(soundBufferSorted_.size());

  if (debug) {
    if (soundBufferSorted_.size() < 50) {
      Serial.println("Buffer sorted full 50: ");
      soundBufferSorted_.printList(0, soundBufferSorted_.size());
      Serial.println("BufferQ full 50: ");
      soundBuffer_.printQ(0, soundBufferSorted_.size());
    } else {
      Serial.println("Buffer sorted first and last 20");
      soundBufferSorted_.printList(0, 20);
      soundBufferSorted_.printList(soundBufferSorted_.size() - 20, soundBufferSorted_.size());
      Serial.println("BufferQ first and last 20");
      soundBuffer_.printQ(0, 20);
      soundBuffer_.printQ(soundBufferSorted_.size() - 20, soundBufferSorted_.size());
    }
  }
  
}

int SoundSensor::readVal() {
  val_ = analogRead(pin_);
  return val_;
}

int SoundSensor::getVal() {
  return val_;
}

int SoundSensor::getBufferSize() {
  return soundBuffer_.size();
}

// adds a sound to the queue and insert in order into the soundBufferSorted_ vector
// returns True if bufferCapacity reached, otherwise false
bool SoundSensor::addSound() {
  // read the value and add it to the end of the queue
  soundBuffer_.enqueue(val_);
  // add that same value in order to soundBufferSorted_
  // TODO: implement binary search for adding
  soundBufferSorted_.insert_ordered(val_);
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
  else {
    soundBufferSorted_.SequentialList::remove(sortedIdx);
  }
}


// compute the upper and lower threshold values from the percentage and capacity
void SoundSensor::convertThreshToIdx() {
  // assume both soundBuffer_ and soundBufferSorted_ are the same size
  threshIdxLower_ = (int)((soundBuffer_.size() * ((double)threshPct_/100.0))/2);
}

// gets the current threshold value to produce a reaction for
int SoundSensor::getLowerThreshVal() {
  return soundBufferSorted_.select(threshIdxLower_);
}
int SoundSensor::getUpperThreshVal(int bufferSize) {
  return soundBufferSorted_.select(bufferSize - threshIdxLower_);
}
