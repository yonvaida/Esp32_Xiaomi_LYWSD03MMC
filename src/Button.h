#pragma once
#include <functional>
#include <TFT_eSPI.h>
#include <TouchScreen.h>

class Button
{
public:
    Button(TFT_eSPI *t_tft, TouchScreen* t_ts, 
         int t_x1, int t_y1,
         int t_x2, int t_y2,
         int t_x3, int t_y3,
         int t_textXOffset, int t_textYOffset,
         const String t_text);
    ~Button() = default;
    void SetCallback(std::function<void()> t_callback);
    void CheckPoint(int t_x, int t_y);
    void SetText(const String &t_text);

private:
    int m_x1;
    int m_x2;
    int m_x3;
    int m_y1;
    int m_y2;
    int m_y3;
    int m_textX;
    int m_textY;
    String m_text = "";
    TFT_eSPI *m_tft;
    TouchScreen* m_ts;
    std::function<void()> m_callback;
    bool m_pressed = false;

    float Area(int x1, int y1, int x2, int y2, int x3, int y3);
    bool IsInside(int x, int y);
};