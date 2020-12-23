#include <RotationAnimation.h>
#include <Defines.h>

RotationAnimation::RotationAnimation(TFT_eSPI *t_tft, int t_x, int t_y, int t_radius)
{
    m_tft = t_tft;
    m_x = t_x;
    m_y = t_y;
    m_radius = t_radius;
    m_drawX = t_x - t_radius;
    m_tft->fillCircle(m_x, m_y, m_radius, BACKGROUND);
    m_tft->drawCircle(m_x, m_y, m_radius, TEXT_COLOR);
}

void RotationAnimation::SetRotation(bool t_rotate)
{
    m_rotate = t_rotate;
}

void RotationAnimation::Draw()
{
    if (m_rotate)
    {
        if (m_iterationX == m_radius * 2)
        {
            m_yMultiplyFactor = -1;
        }
        if (m_iterationX == 0)
        {
            m_yMultiplyFactor = 1;
        }
        m_iterationX += m_yMultiplyFactor;
        const auto drawX = (m_x - m_radius) + m_iterationX;
        const auto drawY = sqrt(static_cast<double>((m_radius * m_radius) - ((drawX - m_x) * (drawX - m_x)))) * m_yMultiplyFactor + m_y;
        m_tft->fillCircle(m_x, m_y, m_radius - 1, BACKGROUND);
        m_tft->drawLine(m_x, m_y, drawX, drawY, TEXT_COLOR);
    }
}