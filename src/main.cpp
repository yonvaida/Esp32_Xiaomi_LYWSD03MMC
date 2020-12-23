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
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
bool pompWorking = true;
bool bluetoothReading = false;
unsigned long startTimer;

void initBluetooth()
{
  m_reader = new BluetoothReader(BLEUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6"), Settings::GetInstance()->GetSensorAddress().c_str());
  m_reader->SetCallback(new ClientCallback(&bluetoothReading));
}

void Task1code(void *pvParameters)
{
  BLEDevice::init("TemperatureReader");
  initBluetooth();
  startTimer = millis();
  int readInterval = Settings::GetInstance()->GetReadInterval();
  String sensorAddres = Settings::GetInstance()->GetSensorAddress();
  for (;;)
  {
    long long elapsedTime = millis() - startTimer;

    if (elapsedTime > readInterval * 1000 || elapsedTime < 0)
    {

      portENTER_CRITICAL(&timerMux);
      bluetoothReading = true;
      portEXIT_CRITICAL_ISR(&timerMux);
      if (!m_reader)
      {
        initBluetooth();
      }
      if (!m_reader->IsConnected() && !sensorAddres.isEmpty())
      {
        try
        {
          Serial.println("Read sensor");
          m_reader->SetAddress(sensorAddres.c_str());
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
      startTimer = millis();
      portENTER_CRITICAL(&timerMux);
      readInterval = Settings::GetInstance()->GetReadInterval();
      sensorAddres = Settings::GetInstance()->GetSensorAddress();
      portEXIT_CRITICAL_ISR(&timerMux);
    }
    Serial.println(bluetoothReading);
    delay(1000);
  }
}

void IRAM_ATTR DrawAnnimation()
{
  portENTER_CRITICAL_ISR(&timerMux);
  pompWorking = Settings::GetInstance()->GetCurrTemp() < Settings::GetInstance()->GetHighTemp();
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup()
{
  Serial.begin(115200);
  timer = timerBegin(0, 80, true);
  Settings::GetInstance();
  m_setupAP = new SetupAP();
  server = std::unique_ptr<WebApi>(new WebApi());
  m_graphics = std::unique_ptr<DisplayGraphics>(new DisplayGraphics());
  pinMode(23, OUTPUT);
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, NULL, 0);
  timerAttachInterrupt(timer, &DrawAnnimation, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);
}
int lastXValue = 0;
int lastYValue = 0;
void loop()
{

  if (pompWorking)
  {
    m_graphics->DrawAnimation(true);
    m_graphics->DrawBTAddress();
    portENTER_CRITICAL(&timerMux);
    pompWorking = false;
    portEXIT_CRITICAL(&timerMux);
  }

  m_graphics->DrawBTIcon(bluetoothReading);

  m_graphics->DrawCurrentTemp();
  m_graphics->CheckTouch();
}