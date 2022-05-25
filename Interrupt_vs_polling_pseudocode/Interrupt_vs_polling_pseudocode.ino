volatile int state=LOW; //this is the variable that will be changed if an interrupt is detected
int inputPin=4;         //Polling will check this pin
int i=0;                //For loop counting variable

void setup()
{

  // set pin 4 to input and pin 2 to INPUT_PULLUP
  pinMode(inputPin, INPUT);
  pinMode(2, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(2),interrupt_ISR, CHANGE); /* Pin 2 is mapped to interrupt 0, currently set to signal an interrupt 
                                                                                   on a change to pin 2, however you don't have to use Change */
  Serial.begin(9600);
}

void loop()
{
  //    Poll state of pin 4, if it is hight print: "Polling found pin 4 to be HIGH, count to 15"
  while (digitalRead(inputPin) == HIGH) {
    //    Delay for 0.5sec
    Serial.println("Polling found pin 4 to be HIGH, count to 15");
    delay(500);
    //    Setup for loop to count from 1 to 15
    for (i = 1; i <= 15; i++) {
      //      If your interrupt variable (state) is high print 1 to 15 with a 0.5sec delay between prints statements
      if (state == HIGH) {
        Serial.println(i);
        delay(500);
      }
      //      If your interrupt variable (state) is LOW print: "Interrupt Detected stop counting" stop counting and wait for a change in the interrupt variable
      else {
        Serial.println("Interrupt Detected stop counting");
        // state is low, loop in here until it changes to high
        while (state == LOW) {}
        // decrease count to account for this cycle
        i--;
      }
      
    }
    //    After you have counted to 15 print: "Check polling" and poll pin 4, if it is hight repeat counting/interrupting.
    Serial.println("Check polling");
  
  }

  //    If the polled pin is LOW print: "Polling found pin 4 to be LOW pause program" then wait until polling pin changes
  // After breaking out of the previous while loop, polled pin must be low
  Serial.println("Polling found pin 4 to be LOW pause program");
  // wait until polling pin changes
  while (digitalRead(4) == LOW) {}

}

void interrupt_ISR()
{
  volatile const int BOUNCE_TIME = 100;
  volatile static unsigned long prevTime = 0;
  volatile unsigned long thisTime = millis();
  // If interrupts greater than bounce time, execute code
  if (thisTime - prevTime > BOUNCE_TIME) 
  {
    state=!state; //change the interrupt variable from LOW to HIGH or HIGH to LOW
  }
  prevTime = thisTime;
}
