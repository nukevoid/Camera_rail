

const long MS_IN_SECOND = 1000L * 1000L;
class PhisicalPoint
{
public:
    PhisicalPoint()
    :speed_(0)
    ,acceleration_(1)
    ,maxSpeed_(0)
    {}

    void update(long deltaPos, long deltaTimeMs)
    {
        long dist = 3 * speed_ * speed_  / acceleration_ / 2;
        long sign = deltaPos - dist; 
        if(sign != 0)
        {
            long acc = copysign(acceleration_, sign);
            speed_ += acc * deltaTimeMs;
            speed_ = max(min(maxSpeed_, speed_), -maxSpeed_);
        }
    }

    inline long speed() const
    {
        return maxSpeed_ / MS_IN_SECOND;//speed_ / MS_IN_SECOND;
    }

    inline void setMaxSpeed(long maxSpeed)
    {
        maxSpeed_ = maxSpeed * MS_IN_SECOND;
    }

    inline void setAcceleration(long acceleration)
    {
        acceleration_ = acceleration;
    }
private:
    long speed_;
    long acceleration_;
    long maxSpeed_;
};
