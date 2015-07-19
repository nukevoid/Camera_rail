
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
        float xDiff = trackedMotor_.microsteps() - x_;
        float angle = atan2(xDiff, y_);
        return angle  / M_PI * halfCicleSize_ + halfCicleSize_ / 2;
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