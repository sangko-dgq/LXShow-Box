//
// Created by 22012 on 2022/8/1.
//

#include "app_getData.h"
#include "resources/bmp/weather.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <time.h>


const char *ssid = "CMCC-KP4p";   //wifi名
const char *password = "TqmhtrX4";//wifi密码

// http://www.tianqiapi.com/ 这个网站免费注册用户 获取appid和appsecret，填入后，调用免费天气API接口。
const String cityid = "101270101";  //城市代码
const String appid = "17948967";    //用户appid
const String appsecret = "X9BAj0yf";//用户appsecret


const String wday_z[7] = {"日", "一", "二", "三", "四", "五", "六"};

time_S time_now, time_display;
tmp_S tmp;

hw_timer_t *tim1 = nullptr;
bool tmp_sta = false;

String wea_img_S[9] = {
        "xue",
        "lei",
        "shachen",
        "wu",
        "bingbao",
        "yun",
        "yu",
        "yin",
        "qing",
};

uint16_t wea_img_S_temp[256];


time_S gettimenow()
{
    time_S t;
    struct tm tmstruct;
    getLocalTime(&tmstruct);

    t.year = String((tmstruct.tm_year) + 1900);
    if (((tmstruct.tm_mon) + 1) > 9)
    {
        t.month = String((tmstruct.tm_mon) + 1);
    } else
    {
        t.month = "0" + String((tmstruct.tm_mon) + 1);
    }

    if ((tmstruct.tm_mday) > 9)
    {
        t.day = String(tmstruct.tm_mday);
    } else
    {
        t.day = "0" + String(tmstruct.tm_mday);
    }

    t.wday = wday_z[tmstruct.tm_wday];

    if ((tmstruct.tm_hour) > 9)
    {
        t.hour = String(tmstruct.tm_hour);
    } else
    {
        t.hour = "0" + String(tmstruct.tm_hour);
    }

    if ((tmstruct.tm_min) > 9)
    {
        t.min = String(tmstruct.tm_min);
    } else
    {
        t.min = "0" + String(tmstruct.tm_min);
    }

    if ((tmstruct.tm_sec) > 9)
    {
        t.sec = String(tmstruct.tm_sec);
    } else
    {
        t.sec = "0" + String(tmstruct.tm_sec);
    }
    return t;
}


tmp_S gattmp()
{
    tmp_S t;
    HTTPClient http;
    http.begin(
            "https://www.yiketianqi.com/free/day?appid=" + appid + "&appsecret=" + appsecret + "&unescape=1&cityid=" +
            cityid);//HTTP
    int httpCode = http.GET();
    if (httpCode > 0)
    {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            //Serial.println(payload);
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            JsonObject obj = doc.as<JsonObject>();

            t.city = obj["city"].as<String>();
            t.wea = obj["wea"].as<String>();            //天气情况
            t.wea_img = obj["wea_img"].as<String>();    //天气标识
            t.tem = obj["tem"].as<String>();            //实施温度
            t.tem_day = obj["tem_day"].as<String>();    //白天温度(高温)
            t.tem_night = obj["tem_night"].as<String>();//夜间温度(低温)
            t.win = obj["win"].as<String>();            //风向
            t.win_speed = obj["win_speed"].as<String>();//风力等级
            t.win_meter = obj["win_meter"].as<String>();//风速
            t.air = obj["air"].as<String>();            //空气质量
            t.sta = true;
            Serial.print("城市：" + t.city + "\n" +
                         "天气：" + t.wea + "\n" +
                         "图标：" + t.wea_img + "\n" +
                         "当前温度：" + t.tem + "\n" +
                         "最高温度：" + t.tem_day + "\n" +
                         "最低温度：" + t.tem_night + "\n" +
                         "风向：" + t.win + "\n" +
                         "风力：" + t.win_speed + "\n" +
                         "风速：" + t.win_meter + "\n" +
                         "空气质量：" + t.air + "\n");
        }
    } else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        t.sta = false;
    }
    http.end();
    return t;
}






void gatwea_img(String wea_img_T)
{
    for (uint8_t i; i < 9; i++)
    {
        if (wea_img_S[i].equals(wea_img_T))
        {
            switch (i)
            {
                case 0:
                    memcpy(wea_img_S_temp, xue, sizeof(xue));
                    break;
                case 1:
                    memcpy(wea_img_S_temp, lei, sizeof(lei));
                    break;
                case 2:
                    memcpy(wea_img_S_temp, shachen, sizeof(shachen));
                    break;
                case 3:
                    memcpy(wea_img_S_temp, wu, sizeof(wu));
                    break;
                case 4:
                    memcpy(wea_img_S_temp, bingbao, sizeof(bingbao));
                    break;
                case 5:
                    memcpy(wea_img_S_temp, yun, sizeof(yun));
                    break;
                case 6:
                    memcpy(wea_img_S_temp, yu, sizeof(yu));
                    break;
                case 7:
                    memcpy(wea_img_S_temp, yin, sizeof(yin));
                    break;
                case 8:
                    memcpy(wea_img_S_temp, qing, sizeof(qing));
                    break;
            }
        }
    }
}


void IRAM_ATTR tim1Interrupt()
{//中断服务函数
    tmp_sta = true;
    Serial.println("中断");
}