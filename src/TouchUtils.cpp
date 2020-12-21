// #include <TouchUtils.h>
// #include <Defines.h>

// void TouchUtils::CalibrateDisplay(TFT_eSPI *t_tft, TouchScreen *t_ts)
// {
//     t_tft->fillScreen(TFT_BLACK);
//     t_tft->drawLine(15, 0, 15, 15, TFT_CYAN);
//     t_tft->drawLine(15, 15, 0, 15, TFT_CYAN);
//     int curentColor = TFT_BLACK;
//     while (true)
//     {
    

//         TSPoint p = t_ts->getPoint();

//         pinMode(XM, OUTPUT);
//         pinMode(YP, OUTPUT);

//         if (abs(p.z) > 0 && abs(p.z) < MAXPRESSURE 
//             && p.x != -3072 && p.y != -3072 )
//         {
//             Serial.println("pressed");
//             if (curentColor == TFT_BLACK)
//             {
//                 t_tft->fillScreen(TFT_RED);
//                 t_tft->drawCentreString("APASAT", t_tft->width() / 2, t_tft->height() / 2, 3);
//                 curentColor = TFT_RED;
//                 Serial.println("Change color to black");
//                 Serial.print("X first = ");
//                 Serial.print((p.x + 2279) / 9.375);
//                 Serial.print("\tY first = ");
//                 Serial.print(240 - (p.y + 2282)/11.47);
//                 Serial.print("\tPressure = ");
//                 Serial.println(p.z);
//                 break;
//             };
//         }
//         else
//         {
//             if (curentColor == TFT_RED && p.z != 0)
//             {
//                 t_tft->fillScreen(TFT_BLACK);
//                 curentColor = TFT_BLACK;
//                 // Serial.println("Change color tto red");
//                 // Serial.print("X first = ");
//                 // Serial.print(p.x);
//                 // Serial.print("\tY first = ");
//                 // Serial.print(p.y);
//                 // Serial.print("\tPressure = ");
//                 // Serial.println(p.z);
//             }
//         }
//     }
// }