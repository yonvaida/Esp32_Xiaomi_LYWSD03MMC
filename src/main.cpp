#include <Arduino.h>
#include <BLEDevice.h>
#include <BluetoothReader.h>
#include <ClientCallback.h>
#include <Settings.h>


#define LYWSD03MMC_ADDR "A4:C1:38:0A:B3:DB"

BluetoothReader *m_reader = nullptr;
Settings *m_settings = nullptr;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting client...");
  delay(500);
  pinMode(23, OUTPUT);
  BLEDevice::init("ESP32");
  m_reader = new BluetoothReader(BLEUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6"), LYWSD03MMC_ADDR);
  m_reader->SetCallback(new ClientCallback());
  m_settings = new Settings();
}

void loop()
{
  if (!m_reader->IsConnected())
  {
    try
    {
      Serial.println("Read living sensor");
      m_reader->SetAddress("A4:C1:38:6B:82:5E");
      m_reader->Connect();
      m_reader->RegisterNotifications();
    }
    catch (...)
    {
    }
  }
  delay(20000);
  Serial.println("Read again");


}