#include <WebApi.h>
#include <SPIFFS.h>
#include <Settings.h>

WebApi::WebApi() : m_server(new AsyncWebServer(80))
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    RegisterCallbacks();
    m_server->begin();
};

void WebApi::RegisterCallbacks()
{
    m_server->on("/", HTTP_GET, RootCallback);
    m_server->on("/wifi", HTTP_GET, SetupWifi);
    m_server->on("/temperature", HTTP_GET, SetupTemp);
    m_server->on("/settings", HTTP_GET, ShowSettings);
    m_server->on("/get", HTTP_GET, SubmitForm);
}

void WebApi::RootCallback(AsyncWebServerRequest *request)
{
    File tmpFile = SPIFFS.open("/index.html", FILE_READ);
    String content = tmpFile.readString();
    request->send(200, "text/html", content);
}

void WebApi::SetupWifi(AsyncWebServerRequest *request)
{
    File tmpFile = SPIFFS.open("/wifi_config.html", FILE_READ);
    String content = tmpFile.readString();
    request->send(200, "text/html", content);
}

void WebApi::SetupTemp(AsyncWebServerRequest *request)
{
    File tmpFile = SPIFFS.open("/temperature_config.html", FILE_READ);
    String content = tmpFile.readString();
    request->send(200, "text/html", content);
}

void WebApi::ShowSettings(AsyncWebServerRequest *request)
{
    File tmpFile = SPIFFS.open("/Settings.json", FILE_READ);
    String content = tmpFile.readString();
    request->send(200, "text/text", content);
}

void WebApi::SubmitForm(AsyncWebServerRequest *request)
{
    String inputMessage;
    String inputParam;
    int paramsChanged = 1;

    if (request->hasParam("ssid"))
    {
        const auto param = request->getParam("ssid")->value();
        if (!param.isEmpty())
        {
            Settings::GetInstance()->SetSSID(param);
            paramsChanged = 0;
        }
    }
    if (request->hasParam("pass"))
    {
        const auto param = request->getParam("pass")->value();
        if (!param.isEmpty())
        {
            Settings::GetInstance()->SetPass(param);
            paramsChanged = 0;
        }
    }
    if (request->hasParam("hight_temp"))
    {
        const auto param = request->getParam("hight_temp")->value().toDouble();
        if (param)
        {
            Settings::GetInstance()->SetHightTemp(param);
            paramsChanged = 0;
        }
    }
    if (request->hasParam("low_temp"))
    {
        const auto param = request->getParam("low_temp")->value().toDouble();
        if (param)
        {
            Settings::GetInstance()->SetLowTemp(param);
            paramsChanged = 0;
        }
    }
    if (request->hasParam("interval"))
    {
        const auto param = request->getParam("interval")->value().toDouble();
        if (param)
        {
            Settings::GetInstance()->SetReadInterval(param);
            paramsChanged = 0;
        }
    }
    if (request->hasParam("sensor_address"))
    {
        const auto param = request->getParam("sensor_address")->value();
        if (!param.isEmpty())
        {
            Serial.println(param);
            Settings::GetInstance()->SetSensorAddress(param);
            paramsChanged = 0;
        }
    }
    if (!paramsChanged)
    {
        Settings::GetInstance()->SaveSettings(true);
    }
    File tmpFile = SPIFFS.open("/index.html", FILE_READ);
    String content = tmpFile.readString();
    request->send(200, "text/html", content);
}