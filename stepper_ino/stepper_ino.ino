#include <IRremote.h>
#include "leds.h"
#include "modes.h"
#include "ir_mapping.h"

const int IR_RECV_PIN = A3;

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

void setup()
{
    irrecv.enableIRIn();
    initModes();
    initLeds();
}

inline void updateIRPort()
{
   if (irrecv.decode(&results))
    {
        onIRValue(results.value);
        irrecv.resume();
    }
}

void loop() 
{
    updateIRPort();
    updateLeds();
    updateModes();
}
