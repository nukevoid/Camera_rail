
class AcceleratedMoving 
{
public:
    AcceleratedMoving(Stepper & motor, long borderSize, long speedChangeTime, long maxSpeed)
    : speed_(0)
    , motor_(motor)
    , selectedSpeed_(0)
    , borderSize_(borderSize)
    , speedChangeTime_(speedChangeTime)
    , maxSpeed_(maxSpeed)
    {}

    inline void update()
    {
        long newSpeed = speed_.value(millis());
        //newSpeed = boundSpeed(newSpeed);
        motor_.setSpeed(newSpeed);
    }

    void setSpeed(long speed, int stepperMode)
    {
        selectedSpeed_ = speed;
        if(motor_.speed() != 0)
        {
            setTargetSpeed(copysign(speed, motor_.speed()));
        }
        motor_.setMode(stepperMode);
    }

    void forward()
    {
        setTargetSpeed(selectedSpeed_);
    }

    void backward()
    {
        setTargetSpeed(-selectedSpeed_);
    }

    void stop()
    {
        setTargetSpeed(0);
    }

private:
    inline void setTargetSpeed(long speed)
    {
        speed_.setTarget(speed, millis(), speedChangeTime_);
    }

    inline long boundSpeed(long speed) const 
    {
        if(speed > 0 && motor_.microsteps() > motor_.params().maxBound - borderSize_)
        {
            long maxSpeed = motor_.microsteps() - motor_.params().maxBound + borderSize_;
            maxSpeed = maxSpeed * maxSpeed_ / borderSize_;
            speed = min(speed, maxSpeed);
        }
        else if(speed < 0 && motor_.microsteps() < motor_.params().minBound + borderSize_) 
        {
            long maxSpeed = motor_.microsteps() + motor_.params().minBound - borderSize_;
            maxSpeed = maxSpeed * maxSpeed_ / borderSize_;
            speed = max(speed, maxSpeed);
        }
        return speed;
    }

private:
    typedef LineFader<long> SpeedFader;
    SpeedFader speed_;
    long selectedSpeed_;
    Stepper & motor_;
    long borderSize_;
    long speedChangeTime_;
    long maxSpeed_;
};
