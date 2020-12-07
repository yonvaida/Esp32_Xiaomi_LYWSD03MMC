#pragma once
#include <Arduino.h>

class Settings
{
public:
    Settings() = default;
    ~Settings() = default;
    void SaveSettings();
    void ReadSettings();

private:
    String m_ssid = "";
    String m_password = "";
    int m_hightTemp = 0;
    int m_lowTemp = 0;
}