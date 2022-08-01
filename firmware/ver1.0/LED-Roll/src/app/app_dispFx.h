//
// Created by 22012 on 2022/8/1.
//

#ifndef _APP_DISPFX_H
#define _APP_DISPFX_H
#include "Arduino.h"


uint16_t rgb_light(uint8_t r, uint8_t g, uint8_t b, uint8_t v);


void printUTF8_fadein(String S_param, int16_t x_param, int16_t y_param, uint8_t size_param);
void printUTF8_fadeout(String S_param, int16_t x_param, int16_t y_param, uint8_t size_param);






#endif //_APP_DISPFX_H
