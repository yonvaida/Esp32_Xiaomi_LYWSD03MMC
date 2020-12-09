#include <SetupAP.h>

SetupAP::SetupAP()
{
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
}