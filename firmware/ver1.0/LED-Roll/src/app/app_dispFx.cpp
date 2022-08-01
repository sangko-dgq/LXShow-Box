//
// Created by 22012 on 2022/8/1.
//

#include "app_dispFx.h"
#include "../../lib/Arduino-ColorConverter/src/ColorConverterLib.h"

#include <ESP32-VirtualMatrixPanel-I2S-DMA.h>
MatrixPanel_I2S_DMA *dma_display = nullptr;
VirtualMatrixPanel *virtualDisp = nullptr;

/*
 *
 * in : r,g,b, v(0~100)
 *return : 16bit-rgb
 *
 * */
uint16_t rgb_light(uint8_t r, uint8_t g, uint8_t b, uint8_t v)
{
    double hue, saturation, lighting, value;

    ColorConverter::RgbToHsv(r, g, b, hue, saturation, value);
    ColorConverter::HsvToRgb(hue, saturation, double(map(v, 0, 100, 0, int(value * 100))) / 100, r, g, b);

    return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3);
}

/*fade in 淡入*/
void printUTF8_fadein(String S_param, int16_t x_param, int16_t y_param, uint8_t size_param)
{
    for (int v = 0; v <= 100; v += 10)
    {
        virtualDisp->setTextColor(rgb_light(255, 231, 186, v), virtualDisp->color565(0, 0, 0));
        virtualDisp->setTextSize(size_param);
        virtualDisp->setCursor(x_param, y_param);
        virtualDisp->printUTF8(S_param);
        delay(50);
    }
}
/*fade out 淡出*/
void printUTF8_fadeout(String S_param, int16_t x_param, int16_t y_param, uint8_t size_param)
{
    for (int v = 100; v >= 0; v -= 10)
    {
        virtualDisp->setTextColor(rgb_light(255, 231, 186, v), virtualDisp->color565(0, 0, 0));
        virtualDisp->setTextSize(size_param);
        virtualDisp->setCursor(x_param, y_param);
        virtualDisp->printUTF8(S_param);
        delay(50);
    }
}

