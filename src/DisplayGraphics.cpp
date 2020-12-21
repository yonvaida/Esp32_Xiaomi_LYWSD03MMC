#include <DisplayGraphics.h>
#include <Fonts.h>
#include <Settings.h>

DisplayGraphics::DisplayGraphics() 
{
    m_touch = std::unique_ptr<TouchScreen>(new TouchScreen(XP, YP, XM, YM, 300));
    m_tftDisplay.init();
    m_tftDisplay.setRotation(0);
    m_tftDisplay.fillScreen(TFT_BLACK);

    m_tftDisplay.fillRect(110, 0, 100, 100, TFT_BLACK);
    m_tftDisplay.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
    m_tftDisplay.setFreeFont(FSS18);
    m_tftDisplay.drawString(String(Settings::GetInstance()->GetHighTemp(), 1), 140, 50, GFXFF);
 m_upButton = std::unique_ptr<Button>(new Button(&m_tftDisplay, m_touch.get(), 60, 10, 10, 110, 110, 110, 0, 0, "+"));
    m_upButton->SetCallback([&]() {
        auto temp = Settings::GetInstance()->GetHighTemp() + 0.5;
        Settings::GetInstance()->SetHightTemp(temp);
        Settings::GetInstance()->SetLowTemp(temp - 0.5);
        Settings::GetInstance()->SaveSettings(true);
        m_tftDisplay.fillRect(110, 0, 100, 100, TFT_BLACK);
        m_tftDisplay.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
        m_tftDisplay.setFreeFont(FSS18);
        m_tftDisplay.drawString(String(temp, 1), 140, 50, GFXFF);
    });
  m_downButton = std::unique_ptr<Button>(new Button(&m_tftDisplay, m_touch.get(), 10, 130, 60, 230, 110, 130, 0, -45, "-"));
  m_downButton->SetCallback([&]() {
      auto temp = Settings::GetInstance()->GetHighTemp() - 0.5;
      Settings::GetInstance()->SetHightTemp(temp);
      Settings::GetInstance()->SetLowTemp(temp - 0.5);
      Settings::GetInstance()->SaveSettings(true);
      m_tftDisplay.fillRect(110, 0, 100, 100, TFT_BLACK);
      m_tftDisplay.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
      m_tftDisplay.setFreeFont(FSS18);
      m_tftDisplay.drawString(String(temp, 1), 140, 50, GFXFF);
  });
  m_tftDisplay.fillRect(110, 110, 100, 100, TFT_BLACK);
  m_tftDisplay.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
  m_tftDisplay.setFreeFont(FSS9);
  m_tftDisplay.drawString("Set T", 150, 20, GFXFF);
  m_tftDisplay.drawString("Read T", 150, 100, GFXFF);

}

void DisplayGraphics::DrawCurrentTemp()
{
    m_tftDisplay.fillRect(110, 120, 100, 100, TFT_BLACK);
    m_tftDisplay.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
    m_tftDisplay.setFreeFont(FSS18);
    m_tftDisplay.drawString(String(Settings::GetInstance()->GetCurrTemp(), 1), 140, 130, GFXFF);
}

void DisplayGraphics::CheckTouch()
{
    TSPoint p = m_touch->getPoint();
    pinMode(YP, OUTPUT); //restore shared pins
    pinMode(XM, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
        Serial.println(p.y);
        int y = 320 - (p.x - 690) / 10;
        int x = (p.y - 760) / 12.27;

        if (m_lastXValue && std::abs(x - m_lastXValue) > 5)
        {
            x = m_lastXValue;
        }
        else
        {
            m_lastXValue = x;
        }
        if (m_lastYValue && std::abs(y - m_lastYValue) > 5)
        {
            y = m_lastYValue;
        }
        else
        {
            m_lastYValue = y;
        }
        m_upButton->CheckPoint(x, y);
        m_downButton->CheckPoint(x, y);
    }
    else
    {
        if (p.z != 0)
        {
            m_upButton->CheckPoint(0, 0);
            m_downButton->CheckPoint(0, 0);
            m_lastXValue = 0;
            m_lastYValue = 0;
        }
    }
}