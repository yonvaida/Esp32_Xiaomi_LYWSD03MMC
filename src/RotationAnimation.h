#include <TFT_eSPI.h>

class RotationAnimation
{
public:
    RotationAnimation(TFT_eSPI* t_tft, int t_x, int t_y, int t_radius);
    ~RotationAnimation() = default;
    void SetRotation(bool t_rotation);
    void Draw();

private:
    bool m_rotate = false;
    TFT_eSPI* m_tft;
    int m_x = 0;
    int m_y = 0;
    int m_radius = 0;
    int m_drawX = 0;
    int m_iterationX = 0;
    int m_yMultiplyFactor = 1;
};