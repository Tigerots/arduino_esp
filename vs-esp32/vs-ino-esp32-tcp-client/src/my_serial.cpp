

#include <Arduino.h>
#include <my_serial.h>



void my_seial_init(int band)
{
    Serial.begin(band);
}


