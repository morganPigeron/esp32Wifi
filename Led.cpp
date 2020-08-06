#include "Arduino.h"
#include "Led.h"

Led::Led()
{
  
}

uint32_t Led::getId()
{
  return this->id;
}

uint32_t Led::getBrightness()
{
  return this->brightness;
}

uint32_t Led::getRed()
{
  return this->red;
}

uint32_t Led::getGreen()
{
  return this->green;
}

uint32_t Led::getBlue()
{
  return this->blue;
}

void Led::setId(int id)
{
  this->id = id;
}

void Led::settings(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness)
{
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->brightness = (B111<<5)^(brightness>>3);
}
