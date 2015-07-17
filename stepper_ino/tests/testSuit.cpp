#define ARDUINO_VERBOSE_LEVEL 2
#include "arduino_env.h"
#include "..\strategy.h"
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

void strategy_update_1s()
{
	for(int i = 0;i < 4001; ++i)
	{
		timeStep(250);
		updateStrategy();
	}
}

void strategy_cmdForwardH_test()
{
	initStrategy();
	cmdSpeed1H();
	cmdForwardH();
	QUNIT_IS_EQUAL(motorH.microsteps(), 0);
	strategy_update_1s();
	QUNIT_IS_EQUAL(motorH.microsteps(), 32 * 100);
	cmdBacwardH();
	strategy_update_1s();
	QUNIT_IS_EQUAL(motorH.microsteps(), 0);
}

void strategy_testSuite()
{
	strategy_cmdForwardH_test();
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
	QUNIT_IS_EQUAL(motor.microsteps(), params.minBoud);
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

void stepper_testSuite()
{
	timeStep_test();
	stepper_update_test();
	//stepper_update2_test();
	stepper_isActive_test();
	stepper_bounds_test();
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