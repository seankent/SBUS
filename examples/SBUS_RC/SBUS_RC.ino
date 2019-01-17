#include "SBUS.h"

SBUS X4R(&Serial); // initialize SBUS object

void setup() {}

void loop() 
{
    Serial.flush();
    Serial.begin(9600);
    Serial.print("8=D ");
    X4R.begin();
}
