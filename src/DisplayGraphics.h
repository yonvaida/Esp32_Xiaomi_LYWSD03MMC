#pragma once
#include <Arduino.h>
#include <TouchScreen.h>
#include <Button.h>
#include <TFT_eSPI.h>
#include <memory>
// // adjust pressure sensitivity - note works 'backwards'
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// some colours to play with
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

const int XP = 27, XM = 15, YP = 4, YM = 14; 

class DisplayGraphics
{
public:
    DisplayGraphics();
    void DrawCurrentTemp();
    void CheckTouch();
private:
    TFT_eSPI m_tftDisplay;
    std::unique_ptr<TouchScreen> m_touch;
    std::unique_ptr<Button> m_upButton;
    std::unique_ptr<Button> m_downButton;
    int m_lastXValue = 0;
    int m_lastYValue = 0;
};