#ifndef sound_sensor_h
#define sound_sensor_h
#include <Arduino.h>
#include <Vector.h>
#include "queue.h"
#include "sequential_list.h"

/*
 * Smart sound sensor to calculate a moving target threshold
 */
class SoundSensor {
  private:
    int bufferCapacity_; // make buffer capacity a constant that must be tuned before running
    int pin_;
    int val_;
    int threshPct_; // the percentage of desired values to use for the threshold (ie. 10 for top 10%)
    int threshIdxLower_;
    CircularQueue soundBuffer_;
    SequentialList soundBufferSorted_;
//    SequentialList sList1;

  public:
    SoundSensor(int pin, int threshPct, int bufferCap);
    int readVal();
    int getVal();
    int getBufferSize();
    void printBufferInfo(bool debug);
    // adds a sound to the queue and uses binary search to insert in order into the soundBufferSorted_ vector
    // returns True if bufferCapacity reached, otherwise false
    bool addSound();
    // removes the first sound in the sound queue and the corresponding value in the soundBufferSorted_ vector
    void rmSound();
    // converts threshPct to an index (upper/lower)
    void convertThreshToIdx();
    // gets the current threshold value to produce a reaction for
    int getLowerThreshVal();
    int getUpperThreshVal(int bufferSize);
};

#endif
