#define ARDUINO_VERBOSE_LEVEL 2
#include "arduino_env.h"
#include "..\modes.h"
#include "QUnit.hpp"


QUnit::UnitTest qunit(std::cerr, QUnit::verbose);

//-------------------------------------- test arduino_evn ------------------------------------
void timeStep_test()
{
	localTimeMs = 0;
	for(int i = 0;i < 1000; ++i)
	{
		timeStep(1000);
	}
	QUNIT_IS_EQUAL(localTimeMs, 1000 * 1000);
}
//-------------------------------------- test strategy ------------------------------------


void strategy_testSuite()
{
	
}
//-------------------------------------- test stepper ------------------------------------
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

void stepper_isActive_test()
{
	const MotorParams params = {3,  2,  9,  8,  7,  6,  0,  200 * 1000};
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
	const MotorParams params = {3,  2,  9,  8,  7,  6,  -10,  200 };
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
	const MotorParams params = {3,  2,  9,  8,  7,  6,  0,  32 * 200 * 1000};
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
	const MotorParams params = {3,  2,  9,  8,  7,  6,  0,  200 * 1000};
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
	const MotorParams params = {3,  2,  9,  8,  7,  6,  0,  32 * 200 * 30L};
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
	timeStep_test();
	stepper_update_test();
	//stepper_update2_test();
	stepper_isActive_test();
	stepper_bounds_test();
	stepper_update_stepCount_test();
}

void testSuite()
{
	strategy_testSuite();
	stepper_testSuite();
}

int main()
{
	testSuite();
	return qunit.errors(); 
}