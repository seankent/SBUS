#include "SBUS.h"

SBUS X4R(&Serial1); // initialize SBUS object 

void setup() 
{
    X4R.begin(); // start serial communication
    Serial.begin(9600);
}

void loop() 
{
    if (X4R.read())
        {
        Serial.print(X4R.channels[0]); // read channel 1
        Serial.print(" ");
        }
}
