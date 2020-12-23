#include <Arduino.h>
#include <DisplayGraphics.h>
#include <Fonts.h>
#include <Settings.h>
#include <Defines.h>
#include <SPIFFS.h>
#include <FS.h>

DisplayGraphics::DisplayGraphics()
{
    m_touch = std::unique_ptr<TouchScreen>(new TouchScreen(XP, YP, XM, YM, 300));
    m_tftDisplay.init();
    m_tftDisplay.setRotation(0);
    m_tftDisplay.fillScreen(TFT_BLACK);
    m_animation = std::unique_ptr<RotationAnimation>(new RotationAnimation(&m_tftDisplay, 170, 165, 20));
    m_tftDisplay.setTextColor(TEXT_COLOR, BACKGROUND);
    m_tftDisplay.setFreeFont(FSSB18);
    m_tftDisplay.drawString(String(Settings::GetInstance()->GetHighTemp(), 1), 90, 30, GFXFF);
    m_upButton = std::unique_ptr<Button>(new Button(&m_tftDisplay, m_touch.get(), 45, 15, 15, 75, 75, 75, 0, -10, "+"));
    m_upButton->SetCallback([&]() {
        auto temp = Settings::GetInstance()->GetHighTemp() + 0.5;
        Settings::GetInstance()->SetHightTemp(temp);
        Settings::GetInstance()->SetLowTemp(temp - 0.5);
        Settings::GetInstance()->SaveSettings(true);
        m_tftDisplay.fillRect(90, 30, 30, 30, BACKGROUND);
        m_tftDisplay.setTextColor(TEXT_COLOR, BACKGROUND);
        m_tftDisplay.setFreeFont(FSSB18);
        m_tftDisplay.drawString(String(temp, 1), 90, 30, GFXFF);
    });
    m_downButton = std::unique_ptr<Button>(new Button(&m_tftDisplay, m_touch.get(), 195, 75, 165, 15, 225, 15, 0, -30, "-"));
    m_downButton->SetCallback([&]() {
        auto temp = Settings::GetInstance()->GetHighTemp() - 0.5;
        Settings::GetInstance()->SetHightTemp(temp);
        Settings::GetInstance()->SetLowTemp(temp - 0.5);
        Settings::GetInstance()->SaveSettings(true);
        m_tftDisplay.fillRect(90, 30, 30, 30, BACKGROUND);
        m_tftDisplay.setTextColor(TEXT_COLOR, BACKGROUND);
        m_tftDisplay.setFreeFont(FSSB18);
        m_tftDisplay.drawString(String(temp, 1), 90, 30, GFXFF);
    });
    m_tftDisplay.setTextColor(TEXT_COLOR, BACKGROUND);
    m_tftDisplay.setFreeFont(FSS9);
    m_tftDisplay.drawString("Temp curenta: ", 15, 110);
    m_tftDisplay.drawString("Status pompa: ", 15, 158);
    m_tftDisplay.drawString("BT: ", 15, 205);
}

void DisplayGraphics::DrawCurrentTemp()
{
    if (lastTempValue != Settings::GetInstance()->GetCurrTemp())
    {
        lastTempValue = Settings::GetInstance()->GetCurrTemp();
        m_tftDisplay.setTextColor(TEXT_COLOR, BACKGROUND);
        m_tftDisplay.setFreeFont(FSS9);
        m_tftDisplay.drawString(String(Settings::GetInstance()->GetCurrTemp(), 1), 140, 110, GFXFF);
    }
}

void DisplayGraphics::DrawBTAddress()
{
    m_tftDisplay.setTextColor(TEXT_COLOR, BACKGROUND);
    m_tftDisplay.setFreeFont(FSS9);
    m_tftDisplay.drawString(Settings::GetInstance()->GetSensorAddress(), 50, 205, GFXFF);
}

void DisplayGraphics::DrawAnimation(bool t_rotation)
{
    m_animation->SetRotation(t_rotation);
    m_animation->Draw();
}

void DisplayGraphics::DrawBTIcon(bool t_reading)
{
    m_tftDisplay.drawBitmap(170, 250, BTICON, 35, 50, t_reading ? TEXT_COLOR : BACKGROUND);
}

void DisplayGraphics::CheckTouch()
{
    TSPoint p = m_touch->getPoint();
    pinMode(YP, OUTPUT); //restore shared pins
    pinMode(XM, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
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