//
// Created by 22012 on 2022/8/1.
//

#include "page_clock.h"
#include "../app_dispFx.h"

#include "../app_extern.h"
#include "resources/bmp/weather.h"
#include "app/app_getData.h"

#include "../app_typedef.h"

extern time_S time_now, time_display;
extern tmp_S tmp;

void initClockPage()
{
    tmp = gattmp();
    delay(100);

    if (tmp.sta)
    {
        delay(100);

        virtualDisp->setTextColor(virtualDisp->color565(255, 231, 186), virtualDisp->color565(0, 0, 0));
        virtualDisp->setTextSize(1);
        gatwea_img(tmp.wea_img);
        virtualDisp->drawRGBBitmap(20, 90, wea_img_S_temp, 16, 16);
        virtualDisp->setCursor(40, 90);
        virtualDisp->printUTF8(tmp.tem);
        virtualDisp->drawBitmap(59, 92, sheshidu_16_16, 16, 16, virtualDisp->color565(255, 231, 186),
                                virtualDisp->color565(0, 0, 0));


    } else
    {
        tmp_sta = true;
    }
}

void updateTimeStruct()
{
    time_now = gettimenow();
}


void updateTimeUI()
{
    if (!time_display.day.equals(time_now.day))
    {
        time_display.year = time_now.year;
        time_display.month = time_now.month;
        time_display.day = time_now.day;
        time_display.wday = time_now.wday;

        virtualDisp->setTextColor(virtualDisp->color565(255, 231, 186), virtualDisp->color565(0, 0, 0));
        virtualDisp->setTextSize(1);
        virtualDisp->setCursor(6, 5);
        virtualDisp->printUTF8(time_display.year);
        virtualDisp->printUTF8("年");
        virtualDisp->printUTF8(time_display.month);
        virtualDisp->printUTF8("月");
        virtualDisp->setTextSize(3);
        virtualDisp->setCursor(15, 20);
        virtualDisp->printUTF8(time_display.day);
        virtualDisp->setTextSize(1);
        virtualDisp->setCursor(68, 48);
        virtualDisp->printUTF8("日");
        virtualDisp->setCursor(23, 70);
        virtualDisp->printUTF8("星期");
        virtualDisp->printUTF8(time_display.wday);
    }

    if (!time_display.sec.equals(time_now.sec))
    {
        time_display.hour = time_now.hour;
        time_display.min = time_now.min;
        time_display.sec = time_now.sec;

        virtualDisp->setTextSize(1);

        virtualDisp->setTextColor(virtualDisp->color565(255, 231, 186), virtualDisp->color565(0, 0, 0));
        virtualDisp->setCursor(13, 110);
        virtualDisp->printUTF8(time_display.hour);
        virtualDisp->printUTF8(":");
        virtualDisp->printUTF8(time_display.min);
        virtualDisp->printUTF8(":");

        /*1s*/
        printUTF8_fadein(time_display.sec, 67, 110, 1);
        printUTF8_fadeout(time_display.sec, 67, 110, 1);
    }
}


void updateTmpUI()
{
    if (tmp_sta)
    {
        Serial.println("获取天气");
        tmp = gattmp();
        if (tmp.sta)
        {
            delay(200);
            virtualDisp->setTextColor(virtualDisp->color565(255, 231, 186), virtualDisp->color565(0, 0, 0));
            virtualDisp->setTextSize(1);

            //    virtualDisp->setCursor(5,90);
            //    virtualDisp->printUTF8(tmp.wea);

            gatwea_img(tmp.wea_img);
            virtualDisp->drawRGBBitmap(20, 90, wea_img_S_temp, 16, 16);

            virtualDisp->setCursor(40, 90);
            virtualDisp->printUTF8(tmp.tem);

            virtualDisp->drawBitmap(59, 92, sheshidu_16_16, 16, 16, virtualDisp->color565(255, 231, 186),
                                    virtualDisp->color565(0, 0, 0));
            tmp_sta = false;
        } else
        {
            tmp_sta = true;
        }
    }
}












