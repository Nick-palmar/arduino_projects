#include <Arduino.h>

const int inputPin = 13;

int outputArr[5] = {0};
const int LOOPAMT = 32;
const int LOOPAMT2 = 4;

const int a1Pin = 11;
const int a0Pin = 9;
const int b1Pin = 7;
const int b0Pin = 5;
const int cinPin = 3;

const int controlPin0 = 4;
const int controlPin1 = 10;

void setup() {
  // put your setup code here, to run once:

  pinMode(inputPin, INPUT);
  pinMode(a0Pin, OUTPUT);
  pinMode(a1Pin, OUTPUT);
  pinMode(b0Pin, OUTPUT);
  pinMode(b1Pin, OUTPUT);
  pinMode(cinPin, OUTPUT);

  pinMode(controlPin0, OUTPUT);
  pinMode(controlPin1, OUTPUT);
  
  Serial.begin(9600);
}


int* decToBinary(int decimal) {
  int flipArr[5] = {0};
  int curr = decimal;
  for (int i = 0; curr > 0; i++) {
    flipArr[i] = curr%2;
    curr /= 2;
  }

  for (int i = 4; i >= 0; i--) {
    outputArr[4-i] = flipArr[i];
  }

  return outputArr;
}

void loop() {
 
  // put your main code here, to run repeatedly:

  if (digitalRead(inputPin) == HIGH) {
    // perform 32 bit logic
    Serial.println("32 bit logic - run through truth table");
    // change control for MUX to 3 - gives the sums and carry out
    digitalWrite(controlPin0, HIGH);
    digitalWrite(controlPin1, HIGH);

    for (int i = 0; i < LOOPAMT; i++) {
      // get a value between 0 and 31
      decToBinary(i);
      // gives outputArr from index 0 as msb and index 4 as lsb
      // map the values to corresponding outputs
      digitalWrite(cinPin, outputArr[4]);
      digitalWrite(b0Pin, outputArr[3]);
      digitalWrite(b1Pin, outputArr[2]);
      digitalWrite(a0Pin, outputArr[1]);
      digitalWrite(a1Pin, outputArr[0]);

      // print decimal
      Serial.println(i);
//      Serial.println(outputArr[4]);
//      Serial.println(outputArr[3]);
      Serial.println(outputArr[0]);
      // wait before looping again
      delay(3000);

      // reset all inputs to low before next input cycle
      digitalWrite(cinPin, LOW);
      digitalWrite(b1Pin, LOW);
      digitalWrite(b0Pin, LOW);
      digitalWrite(a0Pin, LOW);
      digitalWrite(a1Pin, LOW);

      delay(500);
    }
    

  }

  else {
    // perform other control related logic
    Serial.println("changing MUX control lines");

    for (int i = 0; i < LOOPAMT2; i++) {
      decToBinary(i);

      Serial.println(outputArr[4]);
      Serial.println(outputArr[3]);

       // set a to 2
      digitalWrite(a1Pin, HIGH);
      digitalWrite(a0Pin, LOW);
  
      // set b to 3
      digitalWrite(b0Pin, HIGH);
      digitalWrite(b1Pin, HIGH);
  
      // set cin to 1
      digitalWrite(cinPin, HIGH);

      // change control for MUX
      digitalWrite(controlPin0, outputArr[4]);
      digitalWrite(controlPin1, outputArr[3]);

      delay(3000);

      // all inputs low
      digitalWrite(cinPin, LOW);
      digitalWrite(b1Pin, LOW);
      digitalWrite(b0Pin, LOW);
      digitalWrite(a0Pin, LOW);
      digitalWrite(a1Pin, LOW);

      delay(500);
    }
  }
}
