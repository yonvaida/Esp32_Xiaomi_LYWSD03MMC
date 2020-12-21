#include <Button.h>
#include <Defines.h>
#include <Fonts.h>

Button::Button(TFT_eSPI *t_tft, TouchScreen *t_ts,
               int t_x1, int t_y1,
               int t_x2, int t_y2,
               int t_x3, int t_y3,
               int t_textXOffset, int t_textYOffset,
               const String t_text)
{
    m_text = t_text;
    m_tft = t_tft;
    m_x1 = t_x1;
    m_x2 = t_x2;
    m_x3 = t_x3;
    m_y1 = t_y1;
    m_y2 = t_y2;
    m_y3 = t_y3;
    m_ts = t_ts;
    m_textX = (max(m_x1, max(m_x2, m_x3)) - min(m_x1, min(m_x2, m_x3))) / 2 + min(m_x1, min(m_x2, m_x3)) + t_textXOffset;
    m_textY = (max(m_y1, max(m_y2, m_y3)) - min(m_y1, min(m_y2, m_y3))) / 2 + min(m_y1, min(m_y2, m_y3)) + t_textYOffset;
    t_tft->setTextColor(TFT_BLACK, TFT_CYAN);
    t_tft->setFreeFont(FSS24);
    t_tft->fillTriangle(t_x1, t_y1, t_x2, t_y2, t_x3, t_y3, TFT_CYAN);
    t_tft->drawCentreString(m_text, m_textX, m_textY, GFXFF);
};

void Button::SetCallback(std::function<void()> t_callback)
{
    m_callback = t_callback;
};
void Button::CheckPoint(int t_x, int t_y)
{
    if (IsInside(t_x, t_y))
    {
        if (!m_pressed)
        {
            m_callback();
            m_tft->setFreeFont(FSS24);
            m_tft->setTextColor(TFT_BLACK, TFT_DARKGREY);
            m_tft->fillTriangle(m_x1, m_y1, m_x2, m_y2, m_x3, m_y3, TFT_DARKGREY);
            m_tft->drawCentreString(m_text, m_textX, m_textY, GFXFF);
            m_pressed = true;
        }
    }
    else
    {
        if (m_pressed)
        {
            m_tft->setTextColor(TFT_BLACK, TFT_CYAN);
            m_tft->setFreeFont(FSS24);
            m_tft->fillTriangle(m_x1, m_y1, m_x2, m_y2, m_x3, m_y3, TFT_CYAN);
            m_tft->drawCentreString(m_text, m_textX, m_textY, GFXFF);
            m_pressed = false;
        }
    }
};
void Button::SetText(const String &t_text)
{
    m_text = t_text;
    if (m_tft)
    {
        m_tft->drawCentreString(m_text, m_x1, m_y2 / 2 - 24, GFXFF);
    }
};

float Button::Area(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool Button::IsInside(int x, int y)
{
    /* Calculate area of triangle ABC */
    float A = Area(m_x1, m_y1, m_x2, m_y2, m_x3, m_y3);

    /* Calculate area of triangle PBC */
    float A1 = Area(x, y, m_x2, m_y2, m_x3, m_y3);

    /* Calculate area of triangle PAC */
    float A2 = Area(m_x1, m_y1, x, y, m_x3, m_y3);

    /* Calculate area of triangle PAB */
    float A3 = Area(m_x1, m_y1, m_x2, m_y2, x, y);

    /* Check if sum of A1, A2 and A3 is same as A */
    return (A == A1 + A2 + A3);
}