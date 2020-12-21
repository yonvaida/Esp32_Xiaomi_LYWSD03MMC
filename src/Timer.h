#include <functional>
#include <Arduino.h>
class Timer
{
public:
    Timer();
    ~Timer() = default;
    void Execute(std::function<void()> t_execMethod,
                 int t_interval = 10,
                 bool t_repeat = true);

private:
    static std::function<void()> m_execMethod;
    hw_timer_t *m_timer = nullptr;

    void StartTimer();
    static void OnTimer();
};