//
// Created by 22012 on 2022/8/1.
//

#include "hal.h"

void initTimer()
{
    tim1 = timerBegin(0, 80, true);
    timerAttachInterrupt(tim1, tim1Interrupt, true);
    timerAlarmWrite(tim1, 600000000, true);
    timerAlarmEnable(tim1);
}