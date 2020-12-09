#include <WiFi.h>

class SetupAP
{
public:
    SetupAP();
    ~SetupAP() = default;

private:
    const char* ssid     = "XiaomiReader_AP";
    const char* password = "";
};