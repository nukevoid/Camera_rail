
struct MotorParams
{
    int stepPin, dirPin, disablePin;
    int mode0Pin, mode1Pin, mode2Pin;
    int invert;
    long minBound, maxBound;
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

    inline const MotorParams& params() const 
    {
        return params_;
    }

    void resetMicrosteps(long microsteps)
    {
        microsteps_ = microsteps;
    }

    inline long microsteps() const
    {
        return microsteps_;
    }

    inline void setSpeed(long microstepsInSec)
    {
        if(microstepsInSec != speed_)
        {
            speed_ = microstepsInSec;
            digitalWrite(params_.dirPin, (speed_ > 0) != params_.invert);
            refreshMotorDeley();
        }
    }

    inline long speed() const 
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

    inline void update()
    {
        if( needStep() )
        {
            motorStep();
        }
        digitalWrite(params_.disablePin, !isActive());
    }

private:
    inline long motorDelay(long speed, long modeCfc) const
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
        return microsteps < params_.maxBound && microsteps >= params_.minBound;
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
        long delta = speed_ > 0 ? stepSign_ : -stepSign_;
        delta *= MAX_MICROSTEPS / modeCfc_;
        if(inBounds(microsteps_ + delta))
        {
            microsteps_ += delta;
            motorMicrostep();
        }
    }
    
private:
    const MotorParams& params_;
    long microsteps_;
    long modeCfc_;
    long speed_;
    long stepSign_;
    unsigned long lastStepTime_;
    long motorDelay_;
};
