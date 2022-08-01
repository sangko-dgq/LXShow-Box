//
// Created by 22012 on 2022/8/1.
//

#ifndef LED_ROLL_APP_TYPEDEF_H
#define LED_ROLL_APP_TYPEDEF_H

#include "Arduino.h"

typedef struct
{
    String year, month, day, wday, hour, min, sec;
} time_S;

typedef struct
{
    String city, wea, wea_img, tem, tem_day, tem_night, win, win_speed, win_meter, air;
    bool sta;
} tmp_S;


#endif //LED_ROLL_APP_TYPEDEF_H
