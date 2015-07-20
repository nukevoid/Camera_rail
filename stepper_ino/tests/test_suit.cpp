#define ARDUINO_VERBOSE_LEVEL 2
#include "arduino_env.h"
#include "..\modes.h"
#include "QUnit.hpp"


QUnit::UnitTest qunit(std::cerr, QUnit::verbose);
//-------------------------------------- utils -----------------------------------------------
void stepper_update_1s(Stepper& motor)
{
	for(int i = 0;i < 1001; ++i)
	{
		motor.update();
		timeStep(1000);
	}
}

long stepper_get_stepCount_1s(Stepper& motor)
{
	long changeCount = 0;
	int pinState = pinValue[motor.params().stepPin];
	for(int i = 0;i < 1001; ++i)
	{
		motor.update();
		if(pinState == LOW && pinValue[motor.params().stepPin] == HIGH)
		{
			changeCount ++;
		}
		pinState = pinValue[motor.params().stepPin];
		timeStep(1000);
	}
	return changeCount;
}

//-------------------------------------- test arduino_evn ------------------------------------
void arduino_env_millis_test()
{
	localTimeMs = 0;
	for(int i = 0;i < 1000; ++i)
	{
		timeStep(1000);
	}
	QUNIT_IS_EQUAL(millis(), 1000);
}

void arduino_env_micros_test()
{
	localTimeMs = 0;
	for(int i = 0;i < 1000; ++i)
	{
		timeStep(1000);
	}
	QUNIT_IS_EQUAL(micros(), 1000 * 1000);
}

void arduino_env_test_suite()
{
	arduino_env_micros_test();
	arduino_env_millis_test();
}

//-------------------------------------- test LineFader --------------------------------------
void LineFader_test_suite()
{
	LineFader<long> fader(0);
	fader.setTarget(256, 15, 1000);
	long v = fader.value(15 + 500);
	QUNIT_IS_EQUAL(v, 128);
	v = fader.value(15 + 1000);
	QUNIT_IS_EQUAL(v, 256);
	fader.setTarget(64, 15 + 1000, 1000);
	QUNIT_IS_EQUAL(v, 256);
	v = fader.value(15 + 1000 + 500);
	QUNIT_IS_EQUAL(v, (256 - 64) / 2 + 64);
	v = fader.value(15 + 1000 + 1000);
	QUNIT_IS_EQUAL(v, 64);
}
//-------------------------------------- test modes ------------------------------------------

void modes_test_suite()
{
	initModes();
	setSpeed(64, 1);
	QUNIT_IS_EQUAL(motorHorizontal.speed(), 0);
	accHorizontal.forward();
	for(int i = 0;i < 20001; ++i)
	{
		updateModes();
		timeStep(100);
	}
	QUNIT_IS_EQUAL(motorHorizontal.speed(), 64 * 32);
	QUNIT_IS_EQUAL(motorHorizontal.microsteps(), 64 * 32);
}

//-------------------------------------- test TrackedMoving ----------------------------------
void TrackedMoving_test_suit()
{ 
	const MotorParams MP_HORIZONTAL =   {3,  2,  9,  8,  7,  6,  1,  0,  REV * 30L};
	const MotorParams MP_ROTATION =     {5,  4,  0, -1, -1, -1,  0,  0,  REV * 5L};
	Stepper motorHorizontal(MP_HORIZONTAL);
	Stepper motorRotation(MP_ROTATION);
	const long HALF_CICLE = REV * 4L;
	TrackedMoving trackedRotation(motorRotation, motorHorizontal, REV * 2, HALF_CICLE, 0, 1000);

	motorHorizontal.resetMicrosteps(0);
	QUNIT_IS_EQUAL(trackedRotation.trackingTarget(), HALF_CICLE / 2 );
	motorHorizontal.resetMicrosteps(1000);
	QUNIT_IS_EQUAL(trackedRotation.trackingTarget(), HALF_CICLE / 2 + HALF_CICLE / 4);


	TrackedMoving trackedRotation2(motorRotation, motorHorizontal, REV * 2, REV * 4L, 0, REV * 15L);

	motorHorizontal.resetMicrosteps(1000);
	QUNIT_IS_EQUAL(trackedRotation2.trackingTarget(), 12932);
}

//-------------------------------------- test AcceleratedMoving ------------------------------

void LineFader_test()
{
	LineFader<long> fader(0);
	fader.setTarget(256, 15, 1000);
	long v = fader.value(15 + 500);
	QUNIT_IS_EQUAL(v, 128);
	v = fader.value(15 + 1000);
	QUNIT_IS_EQUAL(v, 256);
	fader.setTarget(64, 15 + 1000, 1000);
	QUNIT_IS_EQUAL(v, 256);
	v = fader.value(15 + 1000 + 500);
	QUNIT_IS_EQUAL(v, (256 - 64) / 2 + 64);
	v = fader.value(15 + 1000 + 1000);
	QUNIT_IS_EQUAL(v, 64);
}

void AcceleratedMoving_bound_test()
{

}

void  AcceleratedMoving_forward_test()
{
	//stepPin, dirPin, disablePin, mode0Pin, mode1Pin, mode2Pin, stepCount, minBound, maxBound;
	const MotorParams params =   {3,  2,  9,  8,  7,  6,  1, 0,  REV * 30L};
	Stepper motor(params);
	AcceleratedMoving accHorizontal(motor, REV * 3L, 1000, REV * 3);
	accHorizontal.setSpeed(256, 5);
	accHorizontal.forward();
	QUNIT_IS_EQUAL(motor.microsteps(), 0);
	for(int i = 0;i < 1000; ++i)
	{
		accHorizontal.update();
		motor.update();
		timeStep(1000);
	}
	accHorizontal.update();
	QUNIT_IS_EQUAL(motor.speed(), 256);
	QUNIT_IS_EQUAL(motor.microsteps(), 256 / 2);
	//
	for(int i = 0;i < 1000; ++i)
	{
		accHorizontal.update();
		motor.update();
		timeStep(1000);
	}
	QUNIT_IS_EQUAL(motor.speed(), 256);
	QUNIT_IS_EQUAL(motor.microsteps(), 256 / 2 + 256);
	//backward
	accHorizontal.backward();
	for(int i = 0;i < 1000; ++i)
	{
		accHorizontal.update();
		motor.update();
		timeStep(1000);
	}
	accHorizontal.update();
	QUNIT_IS_EQUAL(motor.speed(), -256);
	QUNIT_IS_EQUAL(motor.microsteps(), 256 / 2 + 256);
}

void AcceleratedMoving_testSuite()
{
	LineFader_test();
	AcceleratedMoving_forward_test();
	AcceleratedMoving_bound_test();
}
//-------------------------------------- test Stepper ----------------------------------------


void stepper_isActive_test()
{
	const MotorParams params = {3,  2,  9,  8,  7,  6,  1,  0,  200 * 1000};
	Stepper motor(params); 
	QUNIT_IS_FALSE(motor.isActive());
	motor.setMode(0);
	motor.setSpeed(256);
	stepper_update_1s(motor);
	QUNIT_IS_TRUE(motor.isActive());
	motor.setSpeed(0);
	stepper_update_1s(motor);
	QUNIT_IS_FALSE(motor.isActive());
}

void stepper_bounds_test()
{
	const MotorParams params = {3,  2,  9,  8,  7,  6,  1,  -10,  200 };
	Stepper motor(params); 
	motor.setMode(5);
	motor.setSpeed(-256);
	stepper_update_1s(motor);
	QUNIT_IS_EQUAL(motor.microsteps(), params.minBound);
	motor.setMode(5);
	motor.setSpeed(256);
	stepper_update_1s(motor);
	QUNIT_IS_EQUAL(motor.microsteps(), params.maxBound-1);
}

void stepper_update2_test()
{
	//stepPin, dirPin, disablePin, mode0Pin, mode1Pin, mode2Pin, stepCount, minBoud, maxBound;
	const MotorParams params = {3,  2,  9,  8,  7,  6,  1,  0,  32 * 200 * 1000};
	Stepper motor(params); 
	motor.setMode(5);
	motor.setSpeed(32 * 50);
	for(int i = 0;i < 4000; ++i)
	{
		motor.update();
		timeStep(250);
	}
	QUNIT_IS_EQUAL(motor.microsteps(), 32 * 50);
}

void stepper_update_test()
{
	//stepPin, dirPin, disablePin, mode0Pin, mode1Pin, mode2Pin, stepCount, minBoud, maxBound;
	const MotorParams params = {3,  2,  9,  8,  7,  6,  1,  0,  200 * 1000};
	Stepper motor(params); 
	
	motor.setMode(0);
	motor.setSpeed(256);
	stepper_update_1s(motor);
	QUNIT_IS_EQUAL(motor.microsteps(), 256);
	motor.setMode(5);
	motor.setSpeed(100);
	stepper_update_1s(motor);
	QUNIT_IS_EQUAL(motor.microsteps(), 256 + 100);
	motor.setMode(2);
	motor.setSpeed(32);
	stepper_update_1s(motor);
	QUNIT_IS_EQUAL(motor.microsteps(), 256 + 100 + 32);
	motor.setMode(5);
	motor.setSpeed(-77);
	stepper_update_1s(motor);
	QUNIT_IS_EQUAL(motor.microsteps(), 256 + 100 + 32 - 77);
	motor.setMode(0);
	motor.setSpeed(-32);
	stepper_update_1s(motor);
	QUNIT_IS_EQUAL(motor.microsteps(), 256 + 100 + 32 - 77 - 32);
}

void stepper_update_stepCount_test()
{
	const MotorParams params = {3,  2,  9,  8,  7,  6,  1, 0,  32 * 200 * 30L};
	Stepper motor(params); 
	motor.setMode(0);
	motor.setSpeed(256);
	long stepCount = stepper_get_stepCount_1s(motor);
	QUNIT_IS_EQUAL(stepCount, 256 / 32);

	motor.setMode(5);
	motor.setSpeed(256);
	stepCount = stepper_get_stepCount_1s(motor);
	QUNIT_IS_EQUAL(stepCount, 256);
}

void stepper_testSuite()
{
	stepper_update_test();
	//stepper_update2_test();
	stepper_isActive_test();
	stepper_bounds_test();
	stepper_update_stepCount_test();
}

void testSuite()
{
	TrackedMoving_test_suit();
	modes_test_suite();
	LineFader_test_suite();
	arduino_env_test_suite();
	AcceleratedMoving_testSuite();
	stepper_testSuite();
}

int main()
{
	testSuite();
	return qunit.errors(); 
}