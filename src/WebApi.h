#include <Arduino.h>
#include <memory>
#include <ESPAsyncWebServer.h>

class WebApi
{
public:
    WebApi();
    ~WebApi() = default;

private:
    std::unique_ptr<AsyncWebServer> m_server;
    void RegisterCallbacks();
    static void RootCallback(AsyncWebServerRequest *request);
    static void SetupWifi(AsyncWebServerRequest *request);
    static void SetupTemp(AsyncWebServerRequest *request);
    static void SubmitForm(AsyncWebServerRequest *request);
    static void ShowSettings(AsyncWebServerRequest *request);
};