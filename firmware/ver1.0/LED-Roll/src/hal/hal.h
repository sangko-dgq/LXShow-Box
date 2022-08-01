//
// Created by 22012 on 2022/8/1.
//

#ifndef LED_ROLL_HAL_H
#define LED_ROLL_HAL_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <time.h>
#include "resources/bmp/weather.h"

#include "../config/config.h"
#include "../app/app_typedef.h"

#include "../app/app_dispFx.h"
#include "../app/app_getData.h"

#include "../app/pages/page_startup.h"

#include "../app/app_extern.h"
#include "../app/pages/page_clock.h"



void hal_init();


void initDisplay();
void initWIFI();
void initNtpTime();
void initTimer();

void initHomeButton();
void updateHomeButton();


#endif //LED_ROLL_HAL_H
