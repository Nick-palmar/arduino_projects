#include "LED.h"
#include "SoundSensor.h"

/*
 * Global scope initialization
 */
// create an LED at pin 2
LED led(2);
SoundSensor sensor(A5, 10, 10);
int SOUND_THRESH = 550;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (sensor.readVal() > SOUND_THRESH) {
    led.on();
  } else {
    led.off();
  }
  Serial.println(sensor.getVal());
//  delay(1000);
}

// TODO: Create an interrupt to make lights if the analog input is above a threshold
