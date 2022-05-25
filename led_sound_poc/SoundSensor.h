#ifndef sound_sensor_h
#define sound_sensor_h
#include <Arduino.h>
//#include <vector>
//#include <queue>

/*
 * Smart sound sensor to calculate a moving target threshold
 */
class SoundSensor {
  private:
    int pin_;
    int val_;
    int bufferCapacity_;
    int threshPct_; // the percentage of desired values to use for the threshold (ie. 10 for top 10%)
//    queue<int> soundBuffer_;
//    vector<int> soundBufferSorted_;

  public:
    SoundSensor(int pin, int bufferCapacity, int threshPct);
    int readVal();
    int getVal();
    // adds a sound to the queue and uses binary search to insert in order into the soundBufferSorted_ vector
    // returns True if bufferCapacity reached, otherwise false
    bool addSound();
    // removes the first sound in the sound queue and the corresponding value in the soundBufferSorted_ vector
    void rmSound();
    // gets the current threshold value to produce a reaction for
    int getThreshVal();
};

#endif
