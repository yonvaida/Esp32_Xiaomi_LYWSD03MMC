#pragma once
#include <Arduino.h>
#include "SPIFFS.h"

class Settings
{
public:
    static Settings *GetInstance();
    bool SaveSettings(bool t_override) const;
    bool ReadSettings();
    void SetSSID(const String &t_ssid) { m_ssid = t_ssid; };
    String GetSSID() const { return m_ssid; };

    void SetPass(const String &t_password) { m_password = t_password; };
    String GetPass() const { return m_password; };

    void SetHightTemp(double t_hightTemp) { m_hightTemp = t_hightTemp; };
    double GetHighTemp() const { return m_hightTemp; };

    void SetLowTemp(double t_lowTemp) { m_lowTemp = t_lowTemp; };
    double GetLowTemp() const { return m_lowTemp; };

    void SetCurrTemp(double t_currentTemperature) { m_currentTemperature = t_currentTemperature; };
    double GetCurrTemp() const { return m_currentTemperature; };

    void SetReadInterval(double t_readInterval) { m_readInterval = t_readInterval; };
    double GetReadInterval() const { return m_readInterval; };

    void SetSensorAddress(const String &t_sensorAddress) { m_sensorAddress = t_sensorAddress; };
    String GetSensorAddress() const { return m_sensorAddress; };

private:
    Settings();
    String m_ssid = "SSID";
    String m_password = "PASS";
    String m_sensorAddress = "";
    double m_hightTemp = 20.00;
    double m_lowTemp = 10.00;
    double m_currentTemperature = 0;
    int m_readInterval = 30;
    static Settings *m_instance;
    void CreateConfigFileIfNotExist();
};