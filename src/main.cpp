#include <Arduino.h>
#include <BLEDevice.h>
#include <BluetoothReader.h>
#include <ClientCallback.h>
#include <Settings.h>
#include <SetupAP.h>
#include <WebApi.h>

#define LYWSD03MMC_ADDR "A4:C1:38:0A:B3:DB"

BluetoothReader *m_reader = nullptr;
SetupAP *m_setupAP = nullptr;
std::unique_ptr<WebApi> server;
unsigned long startTimer;

void initBluetooth()
{
  BLEDevice::init("ESP32");
  m_reader = new BluetoothReader(BLEUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6"), LYWSD03MMC_ADDR);
  m_reader->SetCallback(new ClientCallback());
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting client...");
  delay(500);
  pinMode(23, OUTPUT);
  initBluetooth();
  startTimer = millis();
  Settings::GetInstance();
  m_setupAP = new SetupAP();
  server = std::unique_ptr<WebApi>(new WebApi());
}

void loop()
{
long long elapsedTime = millis() - startTimer;
  if ( elapsedTime > Settings::GetInstance()->GetReadInterval() * 1000 || elapsedTime < 0)
  {
    if (m_reader->IsConnected())
    {
      delete m_reader;
      m_reader = nullptr;
      initBluetooth();
    }
    Serial.println(Settings::GetInstance()->GetSensorAddress().c_str());
    if (!m_reader->IsConnected() && !Settings::GetInstance()->GetSensorAddress().isEmpty())
    {
      try
      {
        Serial.println("Read sensor");
        m_reader->SetAddress(Settings::GetInstance()->GetSensorAddress().c_str());
        m_reader->Connect();
        m_reader->RegisterNotifications();
      }
      catch (...)
      {
      }
    }
    startTimer = millis();
  }
}