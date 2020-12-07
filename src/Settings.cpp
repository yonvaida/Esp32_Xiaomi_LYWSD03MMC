#include <Settings.h>
#include <ArduinoJson.h>

Settings::Settings()
{
    CreateConfigFileIfNotExist();
    ReadSettings();
}

bool Settings::ReadSettings()
{
    File tmpFile = SPIFFS.open("/Settings.json", FILE_READ);
    String content = tmpFile.readString();

    DynamicJsonDocument settingsJson(512);

    deserializeJson(settingsJson, content);
    JsonObject settingsObj = settingsJson.as<JsonObject>();

    JsonObject wifiObj = settingsObj["WIFI"].as<JsonObject>();
    m_ssid = wifiObj["SSID"].as<String>();
    m_password = wifiObj["Password"].as<String>();

    JsonObject homeObj = settingsObj["HOME"].as<JsonObject>();
    m_hightTemp = homeObj["HighTemp"];
    m_lowTemp = homeObj["LowTemp"];
    tmpFile.close();

    return true;
}

bool Settings::SaveSettings(bool t_override) const
{
    if (t_override)
    {
        SPIFFS.remove("/Settings.json");
    }
    File tmpFile = SPIFFS.open("/Settings.json", FILE_WRITE);
    DynamicJsonDocument settingsJson(512);

    DynamicJsonDocument wifi(200);
    wifi["SSID"] = m_ssid;
    wifi["Password"] = m_password;
    settingsJson["WIFI"] = wifi;

    DynamicJsonDocument home(200);
    home["HighTemp"] = m_hightTemp;
    home["LowTemp"] = m_lowTemp;
    settingsJson["HOME"] = home;

    serializeJson(settingsJson, tmpFile);
    tmpFile.close();

    return true;
}

void Settings::CreateConfigFileIfNotExist()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    if (!SPIFFS.exists("/Settings.json"))
    {
        Serial.println("Settings file not found");
        SaveSettings(true);
    }
}