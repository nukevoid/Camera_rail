#include <arduino.h>
const int ledB = 13;
unsigned long ledBTime = 0;

void blinkB()
{
  ledBTime = millis() + 200;
}

void blinkAll()
{
   blinkB();
}

void initLeds()
{
   pinMode(ledB, OUTPUT); 
   digitalWrite(ledB, LOW); 
   blinkAll();
}

void updateLeds()
{
  unsigned long time = millis();
  digitalWrite(ledB, (ledBTime > time));
}
