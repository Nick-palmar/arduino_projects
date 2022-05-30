#ifndef LED_h
#define LED_h

#include <Arduino.h>

class LED {
  private:
    int pin_;
    unsigned char state_;

  public:
    LED(int pin);
    void on();
    void off();
    int getState();
};

#endif
