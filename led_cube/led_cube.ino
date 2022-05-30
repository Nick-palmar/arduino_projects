#include "LED.h"
#include "SoundSensor.h"

#define TOP 8
#define BOTTOM 9
#define ONE 10
#define TWO 11
#define THREE 12
#define FOUR 13

/*
 * Global scope initialization
 */
int bufferCap = 350;
int bufferThreshPct = 1;
int sensorPin = A5;
SoundSensor sensor(sensorPin, bufferThreshPct, bufferCap);
bool calibrated = false;
unsigned long currTime;
unsigned long prevTime;
const unsigned long STORAGE_PERIOD = 28.5714; // Have a max bufferCap of 350, want to refill entire buffer every 10 seconds, so 10/350 = about 29ms per storage
int numStored = 0;

// setup the LED controls
LED topControl(TOP);
LED bottomControl(BOTTOM);
LED controlOne(ONE);
LED controlTwo(TWO);
LED controlThree(THREE);
LED controlFour(FOUR);
int TOT_LED = 8;
LED verticalControl[4] = {controlOne, controlTwo, controlThree, controlFour};
int LOOP = 0;

// function to activte a level. Enter T for top, B for bottom, A for all (both levels active)
void activateLevel(char level) {
  if (level == 'T' || level == 'A')
    topControl.on();
  if (level == 'B' || level == 'A') 
    bottomControl.on();
}

void allOff() {
  topControl.off();
  bottomControl.off();
}


void allReset() {
  controlOne.on();
  controlTwo.on();
  controlThree.on();
  controlFour.on();
}

void allSet() {
  controlOne.off();
  controlTwo.off();
  controlThree.off();
  controlFour.off();
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

void setup() { 
  Serial.begin(9600);               
}

// the loop routine runs over and over again forever:
void loop() {

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
  if (activateLed(upper, lower)) {
    allReset();
    switch (LOOP % (TOT_LED+1)) {
      case 0:
        activateLevel('T');
        controlOne.off();
        break;
      case 1:
        activateLevel('T');
        controlTwo.off();
        break;
      case 2:
        activateLevel('T');
        controlThree.off();
        break;
      case 3:
        activateLevel('T');
        controlFour.off();
        break;
      case 4:
        activateLevel('B');
        controlOne.off();
        break;
      case 5:
        activateLevel('B');
        controlTwo.off();
        break;
      case 6:
        activateLevel('B');
        controlThree.off();
        break;
      case 7:
        activateLevel('B');
        controlFour.off();
        break;
      default:
        activateLevel('A');
        allReset();  
    }
    LOOP++;
  } else {
    allOff();
  }
//  LOOP++;
}
