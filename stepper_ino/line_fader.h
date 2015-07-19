
template<typename T>
class LineFader
{
public:
    typedef unsigned long TimeT;
public:
    LineFader(T start)
    :start_(start)
    ,target_(start)
    ,startTime_(0)
    ,duration_(1)
    {}

    T value(TimeT time) const
    {
        TimeT timeLeft = time - startTime_;
        if(timeLeft >= duration_)
        {
            return target_;
        }else 
        {
            return start_ + (target_ - start_) * (T)timeLeft / (T)duration_;
        }
    }

    TimeT duration() const 
    {
        return duration_;
    }

    void setTarget(T target, TimeT startTime, TimeT duration)
    {
        start_ = value(startTime);
        target_ = target;
        startTime_ = startTime;
        duration_ = duration;
    }
private:
    T start_;
    T target_;
    TimeT startTime_;
    TimeT duration_;
};
