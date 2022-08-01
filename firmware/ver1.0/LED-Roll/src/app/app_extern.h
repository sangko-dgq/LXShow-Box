//
// Created by 22012 on 2022/8/1.
//

#ifndef LED_ROLL_APP_EXTERN_H
#define LED_ROLL_APP_EXTERN_H


#include <ESP32-VirtualMatrixPanel-I2S-DMA.h>
extern MatrixPanel_I2S_DMA *dma_display;
extern VirtualMatrixPanel *virtualDisp;

extern const char *ssid;   //wifi名
extern const char *password;//wifi密码

// http://www.tianqiapi.com/ 这个网站免费注册用户 获取appid和appsecret，填入后，调用免费天气API接口。
extern const String cityid;  //城市代码
extern const String appid;    //用户appid
extern const String appsecret;//用户appsecret

//extern time_S time_now, time_display;
//extern tmp_S tmp;


extern String wea_img_S[];
extern uint16_t wea_img_S_temp[];


extern hw_timer_t *tim1;
extern bool tmp_sta;




#endif //LED_ROLL_APP_EXTERN_H
