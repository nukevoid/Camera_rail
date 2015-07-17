#include "phisical_point.h"

struct MotorParams
{
    int stepPin, dirPin, disablePin;
    int mode0Pin, mode1Pin, mode2Pin;
    long minBoud, maxBound;
};


class Stepper
{
public:
    static const unsigned long SLEEP_TIMEOUT = 1L /*SECOND*/* 1000L * 1000L;
    static const int STEP_COUNT = 200;
    static const int MAX_MICROSTEPS = 32;
    static const int DEFAULT_MODE = 0;
public:
    Stepper(const MotorParams& params)
    :params_(params)
    ,microsteps_(0)
    ,modeCfc_(1)
    //,target_(0)
    ,speed_(0)
    ,stepSign_(0)
    ,lastStepTime_(0)
    ,motorDelay_(1000)
    {  

    }

    void init()
    {
        pinMode(params_.stepPin, OUTPUT); 
        pinMode(params_.dirPin, OUTPUT); 
        pinMode(params_.disablePin, OUTPUT);  
        if(params_.mode0Pin != -1)
        {
            pinMode(params_.mode0Pin, OUTPUT);  
            pinMode(params_.mode1Pin, OUTPUT);  
            pinMode(params_.mode2Pin, OUTPUT);  
        }
        digitalWrite(params_.disablePin, HIGH); 
        setMode(DEFAULT_MODE);
    }

    void resetMicrosteps(long microsteps)
    {
        microsteps_ = microsteps;
    }

    //void setTarget(long target)
    //{
    //    target_ = target;
    //}

    long microsteps() const
    {
        return microsteps_;
    }

    void setSpeed(long microstepsInSec)
    {
        if(microstepsInSec != speed_)
        {
            speed_ = microstepsInSec;
            digitalWrite(params_.dirPin, speed_ > 0);
            refreshMotorDeley();
        }
    }

    long speed() const 
    {
        return speed_;
    }

    void setMode(int mode)
    {
        if(params_.mode0Pin != -1)
        {
            digitalWrite(params_.mode0Pin, mode & (1 << 0));
            digitalWrite(params_.mode1Pin, mode & (1 << 1));
            digitalWrite(params_.mode2Pin, mode & (1 << 2));
        }else
        {
            mode = 5;
        }
        modeCfc_ = pow(2, mode);
        refreshMotorDeley();
    }

    inline bool isActive() const
    {
        return micros() - lastStepTime_ < SLEEP_TIMEOUT;
    }

    void update()
    {
        if( needStep() )
        {
            motorStep();
        }
        digitalWrite(params_.disablePin, !isActive());
    }

private:
    inline long motorDelay(long speed, int modeCfc) const
    {
        if(speed != 0)
        {
            return MAX_MICROSTEPS * 1000L * 1000L / (abs(speed) * 2L * modeCfc); 
        }else 
        {
            return -1;
        }
    }

    inline void refreshMotorDeley()
    {
        motorDelay_ = motorDelay(speed_, modeCfc_);
    }

    inline long timeInStep() const 
    {
        return (long)(micros() - lastStepTime_) - motorDelay_;
    }

    inline bool needStep() const
    {
        return speed_!= 0 && timeInStep() >= 0;
    }

    inline bool inBounds(long microsteps) const 
    {
        return microsteps < params_.maxBound && microsteps >= params_.minBoud;
    }

    inline void motorMicrostep()
    {
        digitalWrite(params_.stepPin, stepSign_); 
        stepSign_ = 1 - stepSign_; //switch 0..1..0..1.. 
        long timeOffset = timeInStep();
        if(timeOffset >= motorDelay_)
        {
            timeOffset = 0;
        }
        lastStepTime_ = micros() - timeOffset;
    }

    inline void motorStep()
    {
        int delta = speed_ > 0 ? stepSign_ : -stepSign_;
        delta *= MAX_MICROSTEPS / modeCfc_;
        if(inBounds(microsteps_ + delta))
        {
            microsteps_ += delta;
            //Serial.print(delta);
            motorMicrostep();
        }
    }
    
private:
    const MotorParams& params_;
    long microsteps_;
    int modeCfc_;
    //long target_;
    long speed_;
    int stepSign_;
    unsigned long lastStepTime_;
    long motorDelay_;
};
