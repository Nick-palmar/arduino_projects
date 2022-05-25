#include <Arduino.h>    // This line will allow this program to use the definitions and functions from the Arduiono.h header file 
const int ledPin = 9;   
const int ledPin2 = 10;

const int inputPin = 2;

const int baseDelay = 500;


int voltagePin = A0;
int voltageValueDigital = 0;
double voltageValueConverted = 0;


int delayList[3] = {500, 200, 1000};

void setup() {         
//    pinMode(ledPin2, OUTPUT);
//    pinMode(inputPin, INPUT);
//    pinMode(ledPin, OUTPUT); 
    Serial.begin(9600);// pinMode tells the program how you want to use a digital input/output pin. In this case ledPin (which =9)
                        // will be used as an output. This means the program can set the voltage at this pin to be ground (0 V or LOW) or 5 V (1 or HIGH)
}                       // This ends the setup() function, when the program reaches this point it will move on to loop().
void loop() {           // The loop() function loops consecutively and continuously, allowing your program to change and respond. It's used to control the Arduino board.

//            for (int currDelay: delayList) {
//              digitalWrite(ledPin, HIGH);
//              delay(currDelay);
//              digitalWrite(ledPin, LOW);
//              delay(currDelay);
//            }

//  if (digitalRead(inputPin) == HIGH) {
//    digitalWrite(ledPin2, LOW);
//    digitalWrite(ledPin, HIGH);
//    delay(baseDelay);
//    digitalWrite(ledPin, LOW);
//    delay(baseDelay);
//    
//  }
//
//  else {
//
//    digitalWrite(ledPin, HIGH);
//    digitalWrite(ledPin2, HIGH);
//    delay(baseDelay/2);
//    digitalWrite(ledPin2, LOW);
//    delay(baseDelay/2);
//    digitalWrite(ledPin, LOW);
//    digitalWrite(ledPin2, HIGH);
//    delay(baseDelay);
//    
//  }

  voltageValueDigital = analogRead(voltagePin);
  voltageValueConverted = double(voltageValueDigital)*(5.0/1024.0);
  Serial.println(voltageValueConverted, 3);
  delay(500);

}                       // when the program reaches this point it will go back to the top of loop(), in our case it will execute digitalWrie(ledPin, HIGH).
