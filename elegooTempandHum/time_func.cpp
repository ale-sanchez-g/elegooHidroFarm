#include "time_func.h"

void timeTeller()
{
    time_t t = now(); // store the current time in time variable t
    Serial.println("time:"+ String(t));
    delay(5000);
}