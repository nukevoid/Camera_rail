#include "stepper.h"
#include "line_fader.h"
#include "accelerated_moving.h"
#include "tracked_moving.h"


typedef void(*ModeType) ();
ModeType mainMode = NULL;
const long REV = Stepper::MAX_MICROSTEPS * Stepper::STEP_COUNT;
//stepPin, dirPin, disablePin, mode0Pin, mode1Pin, mode2Pin, invert, minBound, maxBound;
const long ROTATION_MOTOR_PATH = REV * 4L + REV * 3 / 4;
const MotorParams MP_HORIZONTAL =   {3,  2,  9,  8,  7,  6,  1,  0,  REV * 38L};
const MotorParams MP_ROTATION =     {5,  4,  0, -1, -1, -1,  0,  0,  ROTATION_MOTOR_PATH};
Stepper motorHorizontal(MP_HORIZONTAL);
Stepper motorRotation(MP_ROTATION);

AcceleratedMoving accHorizontal(motorHorizontal, REV * 3L, 2000, REV * 3);
AcceleratedMoving accRotation(motorRotation,  REV * 1L, 2000, REV * 3);
const long trackedX = (MP_HORIZONTAL.minBound + MP_HORIZONTAL.maxBound) / 2;
TrackedMoving trackedRotation(motorRotation, motorHorizontal, REV * 2, MP_ROTATION.maxBound, trackedX, REV * 19L);

//---------------------------- Modes -----------------------------
void DirectMode()
{
    accHorizontal.update();
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

void dock()
{
    accHorizontal.setSpeed(400 * Stepper::MAX_MICROSTEPS, 4);
    accRotation.setSpeed(400 * Stepper::MAX_MICROSTEPS, 4);
    accHorizontal.backward();
    accRotation.backward();
    setMode(DirectMode);
}
//-----------------------------------------------------------------
void initModes()
{
    motorHorizontal.init();
    motorRotation.init();
    setMode(DirectMode);
    setSpeed(50, 5);
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
