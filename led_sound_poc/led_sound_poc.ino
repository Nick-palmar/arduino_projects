#include "LED.h"
#include "SoundSensor.h"

/*
 * Global scope initialization
 */
// create an LED at pin 2
LED led(2);
SoundSensor sensor(A5, 3);
int SOUND_THRESH = 550;
bool calibrated = false;

void setup() {
  Serial.begin(9600);
//  // calibrate sensor
//  Serial.println("Calibrating");
//  while (!sensor.addSound()) {
//    continue;
//  }
//  sensor.rmSound();
//  Serial.println("Calibrating finished");
}

void loop() {
  if (!calibrated) {
    // calibrate sensor
//    Serial.println("Calibrating");
    bool full;
    do {
      full = sensor.addSound();
    } while (!full);
    sensor.rmSound();
//    Serial.println("Calibrating finished");
    calibrated = true;
  }
  
  // add value to buffer
  bool bufferFull = sensor.addSound();
  if (bufferFull)
    sensor.rmSound();

  int bufferSize = sensor.convertThreshToIdx();
  int lower = sensor.getLowerThreshVal();
  int upper = sensor.getUpperThreshVal(bufferSize);
//  Serial.println(sensor.getVal());
  Serial.println(upper);
  Serial.println(lower);

  // turn on the led if in the buffer top percentile zone, else keep off
  if (sensor.getVal() >= upper || sensor.getVal() <= lower) 
    led.on();
  else 
    led.off();
}

// TODO: Create an interrupt to make lights if the analog input is above a threshold
