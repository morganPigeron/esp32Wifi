#ifndef Led_h
#define Led_h

#include "Arduino.h"

class Led
{
  public:
    Led();
    uint32_t getId();
    uint32_t getBrightness();
    uint32_t getRed();
    uint32_t getGreen();
    uint32_t getBlue();
    
    void setId(int id);
    void settings(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);
    
  private:
    uint32_t id;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t brightness;
};

#endif
