//Simple Freq counter pseudocode for 192L lab 5 2021
///////////////////////////////////////// Start Variable Definitions for Frequency Generator /////////////////////////////////////////////////////
const float frequencyOfFreqGen = 1;       //Set this number to the desired frequency you want to use.
// GO TO THE FIRST LINE OF THE SETUP() ROUTINE AND PICK WHICH counter2StartrValue LINE TO USE. IT DEPENDS ON WHAT FREQUENCY YOU'RE USING 
volatile int targetOverFlowNumber = 1;    // Holds the number of overflows we need to count to for half the period of the desired frequency. 
volatile int counter2StartValue = 6;      // The numbers (out of 256) to load into the counter. 
                                          // We use this number to get the timer to overflow at a specific time. We will set Timer2 Frequency to 250 kHz
                                          // 6 is the theoretical value but practically this number needs to be tuned
                                          // By starting at 6, that leaves 250 clock cylcle until the next Timer2 overflow (250 cycle * 4uS/cycle = 1ms)
const int functionGenPin = 6;             // The pin we will use an the output of the function generator
volatile unsigned int overFlowCount = 0;  // Counts the number of overflows of timer2, this enables us to keep track of longer times. 
                                          // i.e. count the number of overflows then multiply by the time it takes to overflow once. 
///////////////////////////////////////// End Variable Definitions for Frequency Generator /////////////////////////////////////////////////////

const int switchPin = 2; // the number of the input pin
long startcount; // the value returned from millis when the signal is detected low
long duration; // variable to store the duratation of the low level of the signal
float frequency; // variable to store the duration in seconds
float period; // variable to store the calculated period of the incoming signal
float scaled_period; // variable to store scaled values of period due to timer clock change
float scalePeriodBy; // variable to devide the period by to adjust for changes in millis() due to changes in Timer0 frequency

void setup()
{
///////////////////////////////////////// Start Setup Code for Frequency Generator /////////////////////////////////////////////////////
// counter2StartValue tunes the value we sets TCNT2 to so that time2 overflows once every 1ms.
  counter2StartValue = 6 + frequencyOfFreqGen;          // Use this line for frequencies from 1 Hz to 20 Hz. 
  //counter2StartValue = 6 + 0.25*frequencyOfFreqGen;     // Use this line for frequencies from 21 Hz to 40 Hz.
  //counter2StartValue = 6 + 0.5*frequencyOfFreqGen;    // Use this line for frequencies from 41 Hz and up.
  
  targetOverFlowNumber = 1/(0.002*frequencyOfFreqGen);  // Calculates the number of overflows (based on 1ms per overflow) in half the desired period. 
                                                        // (frequency = 1/(2*targetOverflowNumber) 
  pinMode(functionGenPin, OUTPUT);                      // Set the function generator pin to an output

  TIFR2  = 0x00;                                        // Writes 0 to the timer2 INT flag register to clear the timer overflow flag bit
  TIMSK2 = 0x01;                                        // Writes 1 to the timer2 INT register to set timer2 overflow interrupt enable bit
  TCCR2A = 0x00;                                        // Writes 1 ot the timer2 control register A to set the Wave Gen Mode to normal
 
  TCCR2B |= _BV(CS02);    // Set bit CS02 of register TCCROB, the most signifigant bit of Timer0's prescaler. 
                          // See page 162 of the datasheet. TCCR0B |= (_BV(CS02) sets CS02 to HIGH, TCCR0B &= ~(_BV(CS02) sets CS02 to LOW.
  TCCR2B &= ~_BV(CS01);   // Clear bit CS01 of register TCCROB, Timer0's prescaler. 
                          // See page 162 of the datasheet. TCCR0B |= (_BV(CS01) sets CS01 to HIGH, TCCR0B &= ~(_BV(CS01) sets CS01 to LOW.
  TCCR2B &= ~_BV(CS00);   // Clear bit CS00 of register TCCROB, the least signifigant bit of Timer0's prescaler. 
                          // See page 162 of the datasheet. TCCR0B |= (_BV(CS00) sets CS00 to HIGH, TCCR0B &= ~(_BV(CS00) sets CS00 to LOW.
  //The above three lines of code set the CS22:20 value to 4, which tells the ATMega328 to divide the 16 MHz clock by 64.
  //This sets Timer2's clock frequency to 250 kHz or 4 microSeconds per clock cycle
///////////////////////////////////////// End Setup Code for Frequency Generator /////////////////////////////////////////////////////

//Setup your pin that reads in function generator
  pinMode(switchPin, INPUT);
  Serial.begin(9600);             //starts serial monitor at a Baudrate of 9600

  // for 4.2, register bits on default setting of CS = 011
  TCCR0B &= ~_BV(CS02);   // Set or Clear bit CS02 of register TCCROB, the most signifigant bit of Timer0's prescaler. 
                          // See page 110 of the datasheet. TCCR0B |= (_BV(CS02) sets CS02 to HIGH, TCCR0B &= ~(_BV(CS02) sets CS02 to LOW.
  TCCR0B |= _BV(CS01);   // Set or Clear bit CS01 of register TCCROB, Timer0's prescaler. 
                          // See page 110 of the datasheet. TCCR0B |= (_BV(CS01) sets CS01 to HIGH, TCCR0B &= ~(_BV(CS01) sets CS01 to LOW.
  TCCR0B |= _BV(CS00);    // Set or Clear bit CS00 of register TCCROB, the least signifigant bit of Timer0's prescaler. 
                          // See page 110 of the datasheet. TCCR0B |= (_BV(CS00) sets CS00 to HIGH, TCCR0B &= ~(_BV(CS00) sets CS00 to LOW.
// ANSWER THIS: Based on the three lines above what frequency is Timer0 running at = 16 MHz / ?? = ?? kHz 
// ANSWER THIS: The arduino default is to divide by 64, so if you now devide by say 256 how many time fast/slower is Timer0 now running? 
// put 1.0 as scale by period for 4.2 
  scalePeriodBy = 1.0/1.0;  // This value will adjust the value millis() returns to the new counter frequency you are using. 
// HEY! you changed how fast Timer0 is running, so you need to scale the value millis() returns (as it uses Timer0 and assumes a prescaler of divide by 64.
// BE AWARE: the code will devide the period by this number, what should you do if you want to increase the period (not decrease i.e. devide)?
// These are floating numbers, make sure to put a .0 on the end of any whole number              
}

void loop()
{
///////////////////////////////////////// Frequency Generator Requires No Code in the Main Loop /////////////////////////////////////////////////////

//Read the state of the pin and use it to determine where you are in the clock cycle
//Find a starting point to start measuring frequency. DO NOT ASSUME YOU ARE AT THE START OR END OF A CYLCE, ASSUME YOU ARE IN THE MIDDLE OF A CYCLE,
//    i.e. the signal has been high or low for sometime and will continue to be high or low for sometime. So do you know you're at a good starting time?

int startState = digitalRead(switchPin);
// loop until the states are different
while (startState == digitalRead(switchPin)) {
}

//To record the current time use "startcount = millis();" it will returned the time (in milliseconds) since the Arduino was powered up. Use this as a timestamp 
//**NOTE: millis() assumes that the CS02:CS01 = 3 (default set by Uno board) which divides the Arduino clock of 16 Mhz by 64. 
//    If this is not the case (section 4.3) you will need to scale the millis() result, see page 109/110 of datasheet, and the three commented lines in setup
    
// on rising or falling edge of cycle so start counting the time
startcount = millis();

// continue reading while the state is opposite of the starting state
while (startState == !digitalRead(switchPin)) {
 }
 
//Intelligently read the state of the pin to determine when to get a second timestamp measurement to use to calculate the incoming frequency. 

    duration = millis() - startcount; // calculating the duration in miliseconds of a complete or of a part cycle.
    // note that the duration measured is only for half of a cycle, so multiply by two and convert to seconds for full period length
    period = ((float)duration * 2.0) / 1000.0;
    // note that for 4.2, will be getting same for period and scaled_period
    scaled_period = period/scalePeriodBy; //scale the period value to take into account the different timer clock speeds 
    // freq = 1/period, get a frequency value in hz
    frequency = 1.0 / scaled_period;
    Serial.print(" Frequency is = ");
    Serial.print(frequency);
    Serial.println(" hz");
}

///////////////////////////////////////// Frequency Generator Requires this Interrupt Service Routine /////////////////////////////////////////////////////
ISR(TIMER2_OVF_vect)                                            // Timer2's overflow service routine
{
  overFlowCount++;                                              // Add one to the overflow counter 
  if (overFlowCount > targetOverFlowNumber )                    // Once the overflow counter has reached 1/2 the desire frequency (assuming a 50% duty cycle)
                                                                // toggle the state of the output pin
  {   
    digitalWrite(functionGenPin,!digitalRead(functionGenPin));  // Toggle functionGenPin's pin state
    overFlowCount = 0;                                          // Once the digital pin has been toggled reset the overflow counter variable back to zero
  }
  TCNT2 = counter2StartValue;                                   // Set TCNT2 to overflow every 1ms by starting the timer at counter2StartValue.
}
