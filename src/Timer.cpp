#include "Timer.h"
std::function<void()> Timer::m_execMethod;

Timer::Timer()
{

}

void Timer::Execute(std::function<void()> t_execMethod,int t_interval, bool t_repeat)
{
    Timer::m_execMethod = t_execMethod;
    StartTimer();
}
void Timer::StartTimer()
{
    m_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(m_timer, &OnTimer, true);
    timerAlarmWrite(m_timer, 10000000, true);
    timerAlarmEnable(m_timer);
};

void Timer::OnTimer()
{
    Timer::m_execMethod();
}