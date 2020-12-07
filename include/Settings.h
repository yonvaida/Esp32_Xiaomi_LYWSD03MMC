#pragma once
#include <Arduino.h>
#include "SPIFFS.h"

class Settings
{
public:
    explicit Settings();
    ~Settings() = default;
    bool SaveSettings(bool t_override) const;
    bool ReadSettings();

private:
    String m_ssid = "SSID";
    String m_password = "PASS";
    int m_hightTemp = 20;
    int m_lowTemp = 10;
    void CreateConfigFileIfNotExist();
};