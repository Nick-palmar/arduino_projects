const float frequencyOfFreqGen = 50000;  // TO DO: Set this number to the desired frequency you want to test your code to.

const int functionGenPin = 6;           // The pin we will use an the output of the function generator (using the tone function)
const int inputPin = 2;                 // The pin receiving the input signal to be measured
volatile int edge_counter = 0;          // Stores the number of incoming edges to pin 2 (increamented by interrupt)

float counter_freq;                     // Stores the actual frequency of Timer0, set at the start of setup()
float period;                           // Stores the period calculated by the frequency counter
volatile int duration;                           // Stores the number of Timer0 counts between between one period of the input signal
float frequency;                        // Stores the calculated frequency of the incoming signal

void setup()
{

  pinMode(functionGenPin, OUTPUT);  // Set the function generator pin as an output
  pinMode(inputPin, INPUT);         // Set the input pin as in input

  tone(functionGenPin, frequencyOfFreqGen); /* The tone routine generates a squarewave signal (between 31 Hz and 60 kHz) on the specified pin.
                                            Use it to generate your input squarewave signal */ 
  
  Serial.begin(9600);               // Starts serial monitor at a Baudrate of 9600
  
  /*
  For CS = 010: 
  TCCR0B &= ~_BV(CS02); TCCR0B |= _BV(CS01); TCCR0B &= ~_BV(CS00); 
  counter_freq = 16e6/8;

  For CS = 011: 
  TCCR0B &= ~_BV(CS02); TCCR0B |= _BV(CS01); TCCR0B |= _BV(CS00); 
  counter_freq = 16e6/64;

  For CS = 101: 
  TCCR0B |= _BV(CS02); TCCR0B &= ~_BV(CS01); TCCR0B |= _BV(CS00); 
  counter_freq = 16e6/1024;
   
   */
   
  TCCR0B &= ~_BV(CS02); TCCR0B |= _BV(CS01); TCCR0B &= ~_BV(CS00); 
  counter_freq = 16e6/8;        // TO DO: Calculate actual counter frequency, see page 110 of the datasheet to select the denominator

  attachInterrupt(digitalPinToInterrupt(inputPin), edge_ISR, RISING); /* pin 2 is mapped to interrupt 0 on the Uno, currently set to signal an interrupt 
                                                                                   on a rising edge to pin 2, however you don't have to use Rising */

}

void loop()
{

  //    If all the data needed has been collected using your interrupt handler then
  if (edge_counter >= 2) {
    //    calculate the period of the incoming signal using Timer0's frequency and the number of Timer0 counts that have happen during one period of the incoming signal 
    period = duration/counter_freq;
    //    calculate the incoming signal frequency and print it. (put in a delay so you can easily read Serial Monitor)
    frequency = 1/period;
    Serial.println(frequency);
    delay(100);
    
    //    reset edge_counter to zero. (Use edge_counter to determine if you want to start collecting data, when to end collecting data, or when you have already collected all the data you need)
    edge_counter = 0;

  }
}

void edge_ISR()
{
  // If we want to start collecting data (check edge_counter) set Timer0 register, TCNT0, to zero. This allows the most counts to occur before a roll over occurs
  // start collecting data only if edge counter = 0
  if (edge_counter == 0) {
    // start the timer
    TCNT0 = 0;
  }
  
  // If the value of edge_counter tells you one period of the incoming signal has elapsed then record the number of Timer0 counts (register, TCNT0)
  else if (edge_counter == 1) {
    // take the volatile duration value as the timer0 register value 
    duration = TCNT0;
  }
  
//  At each interrup increase edge_counter, which counts how many rising edges have occurred. (remember edge_counter will continue to increase even 
//    after one period has been counted)
  edge_counter++;
}
