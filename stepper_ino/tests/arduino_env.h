//--------------------------- includes -------------------------------
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <math.h>

//--------------------------- settings -------------------------------
#ifndef ARDUINO_VERBOSE_LEVEL 
	#define ARDUINO_VERBOSE_LEVEL 3
#endif 
//--------------------------- debug ----------------------------------
#if ARDUINO_VERBOSE_LEVEL == 0
	#define VERBOSE_0( x ) x
	#define VERBOSE_1( x ) 
	#define VERBOSE_2( x ) 
	#define VERBOSE_3( x ) 
#elif ARDUINO_VERBOSE_LEVEL == 1
	#define VERBOSE_0( x ) x
	#define VERBOSE_1( x ) x
	#define VERBOSE_2( x ) 
	#define VERBOSE_3( x ) 
#elif ARDUINO_VERBOSE_LEVEL == 2
	#define VERBOSE_0( x ) x
	#define VERBOSE_1( x ) x
	#define VERBOSE_2( x ) x
	#define VERBOSE_3( x ) 
#elif ARDUINO_VERBOSE_LEVEL == 3
	#define VERBOSE_0( x ) x
	#define VERBOSE_1( x ) x
	#define VERBOSE_2( x ) x
	#define VERBOSE_3( x ) x
#endif

#define LOG_DEBUG( tag, value ) VERBOSE_3(std::cout << "[D] " << tag << " " << value << std::endl)
#define LOG_INFO( tag, value ) VERBOSE_2(std::cout << "[I] " << tag << " " << value << std::endl)
#define LOG_WARNING( tag, value ) VERBOSE_1(std::cout << "[W] " << tag << " " << value << std::endl)
#define LOG_ERROR( tag, value ) VERBOSE_0(std::cout << "[E] " << tag << " " << value << std::endl)
//--------------------------- consts ---------------------------------
#define OUTPUT 134
#define INPUT  124
#define HIGH   1
#define	LOW    0
#define PIN_COUNT 256

//--------------------------- utils variables ------------------------
uint32_t localTimeMs = 0;
uint8_t pinModeState[PIN_COUNT];
uint8_t pinValue[PIN_COUNT];

//--------------------------- utils ----------------------------------
void timeStep(uint32_t stepSize)
{
	localTimeMs += stepSize;
}

//--------------------------- basic macros ---------------------------
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

//---------------------------- hardware -------------------------------
class SerialImp
{
public:
	void begin(int speed)
	{
		LOG_INFO("Serial.begin ", speed);
	}

	template<typename T>
	void println(T value)
	{
		LOG_INFO("Serial.println ", value);
	}

	template<typename T>
	void print(T value)
	{
		LOG_INFO("Serial.print ", value);
	}
} Serial;


void pinMode(int pin, int mode)
{
	pinModeState[pin] = mode;
	VERBOSE_3(std::cout << "pinMode " << pin << " " << mode << std::endl);
}

void digitalWrite(int pin, int value)
{
	pinValue[pin] = value;
	VERBOSE_3(std::cout << "digitalWrite " << pin << " " << value << std::endl);
}

int digitalRead(int pin)
{
	return pinValue[pin];
}

uint32_t micros()
{
	return localTimeMs;
}

uint32_t millis()
{
	return localTimeMs / 1000;
}