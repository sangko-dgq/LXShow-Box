//
// Created by 22012 on 2022/8/1.
//

#ifndef LED_ROLL_APP_GETDATA_H
#define LED_ROLL_APP_GETDATA_H

#include "config/config.h"
#include "app_typedef.h"

time_S gettimenow();
tmp_S gattmp();
void gatwea_img(String wea_img_T);


void IRAM_ATTR tim1Interrupt();










#endif //LED_ROLL_APP_GETDATA_H
