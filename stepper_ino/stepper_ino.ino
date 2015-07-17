//s#define DEBUG
#include <IRremote.h>
#include "leds.h"
#include "strategy.h"

const int BUTTON_PIN = 7;
const int IR_RECV_PIN = 1;

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

void setup()
{
    pinMode(BUTTON_PIN, INPUT); 

irrecv.enableIRIn(); // включить приемник
initStrategy();
initLeds();

#ifdef DEBUG
      Serial.begin(115200);
      Serial.println("Start debug");
      cmdForwardR();
#endif
}

void onIRValue(int value)
{

    switch(value)
    {
        case 16720605:
        cmdForwardH();
        break;
        case 16712445:
        cmdBacwardH();
        break;
        case 16761405:
        cmdStopH();
        break;
        case 16769055:
        cmdForwardR();
        break;
        case 16754775:
        cmdBacwardR();
        break;
        case 16748655:
        cmdStopR();
        break;
        case 16738455:
        cmdSpeed0H();
        break;
        case 16750695:
        cmdSpeed1H();
        break;
        case 16756815:
        cmdSpeed2H();
        break;
        case 16724175:
        cmdSpeed0R();
        break;
        case 16718055:
        cmdSpeed1R();
        break;
        case 16743045:
        cmdSpeed2R();
        break;
    }
}

void loop() 
{
#ifndef DEBUG
    if (irrecv.decode(&results))
    {
        blinkB();
        onIRValue(results.value);
        irrecv.resume();
    }
#endif
    if(motorR.isActive())
    {
        blinkB();
    }
    updateLeds();
    updateStrategy();
}

/*
Button IDS
16753245 16736925 16769565
16720605 16712445 16761405
16769055 16754775 16748655
16738455 16750695 16756815
16724175 16718055 16743045
16716015 16726215 16734885
16728765 16730805 16732845
//4294967295

*/
