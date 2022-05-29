#include "LED.h"
#include "SoundSensor.h"

/*
 * Global scope initialization
 */
// create an LED at pin 2
int ledPin = 2;
int bufferCap = 350;
int bufferThreshPct = 1;
int sensorPin = A5;
LED led(ledPin);
SoundSensor sensor(sensorPin, bufferThreshPct, bufferCap);
bool calibrated = false;
unsigned long currTime;
unsigned long prevTime;
const unsigned long STORAGE_PERIOD = 28.5714; // Have a max bufferCap of 350, want to refill entire buffer every 10 seconds, so 10/350 = about 29ms per storage
int numStored = 0;

void setup() {
  Serial.begin(9600);
}

// Add a sound and remove the oldest one if full
void storeNewSound() {
  int full = sensor.addSound();
  numStored++;
  if (numStored % bufferCap == 0)
    sensor.printBufferInfo(true);
  if (full) {
    sensor.rmSound();
  }
}

bool activateLed(int upper, int lower) {
  // TODO: Investiage if <=lower is valid (logically speaking)
  return (sensor.getVal() >= upper || sensor.getVal() <= lower);
}

void loop() {
//  sensor.printBufferInfo(false);
// TODO: create a proper calibration function

//  if (!calibrated) {
//    // calibrate sensor
//    do {
//      full = sensor.addSound();
//    } while (!full || sensor.getBufferSize() < 50);
//    sensor.rmSound();
//    sensor.printBufferInfo(false);
//    calibrated = true;
//  }

  // must read value first now
  sensor.readVal();
  
  // add value to buffer
  // TODO: Only add value to buffer randomly after 'n' reads (maybe timing with millis? Quite random)
  if (sensor.getBufferSize() == 0) {
    storeNewSound();
    prevTime = millis();
  } else {
    currTime = millis();
    if (currTime - prevTime >= STORAGE_PERIOD){
      storeNewSound();
      prevTime = currTime;
    } 
    
  }

  // covert the threshold pct to an index based on current size
  sensor.convertThreshToIdx();
  int lower = sensor.getLowerThreshVal();
  int upper = sensor.getUpperThreshVal(sensor.getBufferSize());

//  Serial.println(upper);
//  Serial.println(lower);

  // turn on the led if in the buffer top percentile zone, else keep off
  if (activateLed(upper, lower)) 
    led.on();
  else 
    led.off();
}
