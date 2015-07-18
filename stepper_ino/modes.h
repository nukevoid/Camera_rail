#include "stepper.h"
#include "line_fader.h"
#include "accelerated_moving.h"
#include "tracked_moving.h"


typedef void(*ModeType) ();
ModeType mainMode = NULL;
const long REV = Stepper::MAX_MICROSTEPS * Stepper::STEP_COUNT;

const MotorParams MP_HORIZONTAL =   {3,  2,  9,  8,  7,  6,  0,  REV * 30L};//stepPin, dirPin, disablePin, mode0Pin, mode1Pin, mode2Pin, stepCount, minBound, maxBound;
const MotorParams MP_ROTATION =     {5,  4,  0, -1, -1, -1,  0,  REV * 5L};
Stepper motorHorizontal(MP_HORIZONTAL);
Stepper motorRotation(MP_ROTATION);

AcceleratedMoving accHorizontal(motorHorizontal, REV * 3L, 2000, REV * 3);
AcceleratedMoving accRotation(motorHorizontal,  REV* 3L, 2000, REV * 2);
TrackedMoving trackedRotation(motorRotation, motorHorizontal, REV * 2, 1000, 1000, (MP_HORIZONTAL.minBound + MP_HORIZONTAL.maxBound) / 2);

//---------------------------- Modes -----------------------------
void OnlyHorizontalMode()
{
    accHorizontal.update();
}

void OnlyRotationMode()
{
    accRotation.update();
}

void RotationTrackedMode()
{
    accHorizontal.update();
    trackedRotation.update();
}

//----------------------------Commands----------------------------
void setMode(ModeType newMode)
{
    mainMode = newMode;
}

void setSpeed(long speed, int stepperMode)
{
    accHorizontal.setSpeed(speed * Stepper::MAX_MICROSTEPS, stepperMode);
    accRotation.setSpeed(speed * Stepper::MAX_MICROSTEPS, stepperMode);
}

//-----------------------------------------------------------------
void initModes()
{
    motorHorizontal.init();
    motorRotation.init();
    setMode(OnlyHorizontalMode);
}

void updateModes()
{
    if(mainMode)
    {
        mainMode();
    }

    motorHorizontal.update();
    motorRotation.update();
}
