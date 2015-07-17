#include "stepper.h"

//stepPin, dirPin, disablePin, mode0Pin, mode1Pin, mode2Pin, stepCount, minBoud, maxBound;
const MotorParams MP_H = {3,  2,  9,  8,  7,  6,  0,  32 * 200 * 1000L};
const MotorParams MP_R = {5,  4,  0, -1, -1, -1,  0,  32 * 200 * 10L};
Stepper motorH(MP_H);
Stepper motorR(MP_R);

class SimpleStrategy 
{
public:
    void update()
    {

    }
} simpleStrategy;


class TrackingStrategy 
{
public:
    void update()
    {

    }
} trackingStrategy;



//----------------------------Commands----------------------------
long savedSpeedH = 50;

void changeHSpeed(long speed)
{
    savedSpeedH = copysign(speed, motorH.speed());
    if( motorH.speed() !=0 )
    {
        motorH.setSpeed(savedSpeedH);
    }   
}

void cmdForwardH()
{
    motorH.setSpeed(abs(savedSpeedH));
}

void cmdBacwardH()
{
    motorH.setSpeed(-abs(savedSpeedH));
}

void cmdStopH()
{
    motorH.setSpeed(0);
}

void cmdSpeed0H()
{
    changeHSpeed(32 * 50L);
    motorH.setMode(5);
}

void cmdSpeed1H()
{
    changeHSpeed(32 * 100L);
    motorH.setMode(2);
}

void cmdSpeed2H()
{
    changeHSpeed(32 * 200L);
    motorH.setMode(0);
}

void cmdSpeed3H()
{
    changeHSpeed(32 * 300L);
    motorH.setMode(0);
}

long savedSpeedR = 50;

void changeRSpeed(long speed)
{
    savedSpeedR = copysign(speed, motorR.speed());
    if( motorR.speed() !=0 )
    {
        motorR.setSpeed(savedSpeedR);
    }
}

void cmdForwardR()
{
    motorR.setSpeed(abs(savedSpeedR));
}

void cmdBacwardR()
{
    motorR.setSpeed(-abs(savedSpeedR));
}

void cmdStopR()
{
    motorR.setSpeed(0);
}




void cmdSpeed0R()
{
    changeRSpeed(32 * 20L);
    motorR.setMode(5);
}

void cmdSpeed1R()
{
    changeRSpeed(32 * 50L);
    motorR.setMode(5);
}

void cmdSpeed2R()
{
    changeRSpeed(32 * 70L);
    motorR.setMode(5);
}

void cmdSpeed3R()
{
    changeRSpeed(32 * 100L);
    motorR.setMode(5);
}


//-----------------------------------------------------------------

void initStrategy()
{
    motorH.init();
    motorR.init();
    cmdSpeed0H();
    cmdSpeed0R();
}

void updateStrategy()
{
    motorH.update();
    motorR.update();

}
