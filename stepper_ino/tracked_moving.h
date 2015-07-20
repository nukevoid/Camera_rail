
inline long fastAtan2(long y, long x, long maxValue)
{
    const long pi_4 = maxValue  / 4;
    const long pi_3_4 = maxValue * 3 / 4;
    if(y>0)
    {
        if (x >= 0) 
            return pi_4 - pi_4 * (x - y) / (x + y);
        else
            return pi_3_4 - pi_4 * (x + y) / (y - x);
    }
    else
    {
        if (x >= 0) 
            return -pi_4 + pi_4 * (x + y) / (x - y);            
    }
    return -pi_3_4 - pi_4 * (x - y) / (y + x);
}

class TrackedMoving 
{
public:
    TrackedMoving(Stepper & motor, Stepper & trackedMotor, long maxSpeed, long halfCicleSize, long x, long y)
    : motor_(motor)
    , trackedMotor_(trackedMotor)
    , maxSpeed_(maxSpeed)
    , x_(x)
    , y_(y)
    , halfCicleSize_(halfCicleSize)
    {}

    inline void update()
    {
        motor_.setSpeed(speedFromTarget(trackingTarget()));
    }

    inline const long& x() const  {  return x_; }
    inline long& x()  {  return x_; }
    inline const long& y() const  {  return y_; }
    inline long& y()  {  return y_; }

    inline long trackingTarget() const 
    {
        long xDiff = x_ - trackedMotor_.microsteps();
        return fastAtan2(y_, xDiff, halfCicleSize_);
    }

private:
    inline long speedFromTarget(long target) const
    {
        long delta = target - motor_.microsteps();
        long speed = copysign(maxSpeed_, delta);
        if(delta == 0)
        {
            speed = 0;
        }
        return speed;
    } 

private:
    Stepper & motor_;
    Stepper & trackedMotor_;
    long maxSpeed_;
    long x_;
    long y_;
    long halfCicleSize_;
};