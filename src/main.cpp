#include <Arduino.h>
#include <BLEDevice.h>
#include <BluetoothReader.h>
#include <ClientCallback.h>
#include <Settings.h>
#include <SetupAP.h>
#include <WebApi.h>
#include <DisplayGraphics.h>
#include <TFT_eSPI.h>
#include <TouchUtils.h>
#include <Defines.h>
#include <Button.h>

#include <Timer.h>

BluetoothReader *m_reader = nullptr;
SetupAP *m_setupAP = nullptr;
std::unique_ptr<WebApi> server;
std::unique_ptr<DisplayGraphics> m_graphics;
unsigned long startTimer;


void initBluetooth()
{
  m_reader = new BluetoothReader(BLEUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6"), Settings::GetInstance()->GetSensorAddress().c_str());
  m_reader->SetCallback(new ClientCallback());
}

void setup()
{
  Serial.begin(115200);

  BLEDevice::init("TemperatureReader");
  Settings::GetInstance();
  m_setupAP = new SetupAP();
  server = std::unique_ptr<WebApi>(new WebApi());
  m_graphics = std::unique_ptr<DisplayGraphics>(new DisplayGraphics());


  Serial.begin(57600); // For debug
                       //TouchUtils::CalibrateDisplay(&tft, &ts);

  initBluetooth();

  pinMode(23, OUTPUT);

  startTimer = millis();
}
int lastXValue = 0;
int lastYValue = 0;
void loop()
{

  long long elapsedTime = millis() - startTimer;
  if (elapsedTime > Settings::GetInstance()->GetReadInterval() * 1000 || elapsedTime < 0)
  {
    if (!m_reader)
    {
      initBluetooth();
    }
    if (!m_reader->IsConnected() && !Settings::GetInstance()->GetSensorAddress().isEmpty())
    {
      try
      {
        Serial.println("Read sensor");
        m_reader->SetAddress(Settings::GetInstance()->GetSensorAddress().c_str());
        if (m_reader->Connect())
        {
          m_reader->RegisterNotifications();
        }
        else
        {
          delete m_reader;
          m_reader = nullptr;
        }
      }
      catch (...)
      {
      }
    }
    m_graphics->DrawCurrentTemp();
    startTimer = millis();
  }
  m_graphics->CheckTouch();
}