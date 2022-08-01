//
// Created by 22012 on 2022/8/1.
//

#include "page_startup.h"
#include "../app_dispFx.h"
#include "app/app_extern.h"


void keepStatrtupPage()
{
    printUTF8_fadein("LXShow Box", 8, 48, 1);
    printUTF8_fadeout("LXShow Box", 8, 48, 1);

    printUTF8_fadein("LXShow Box", 8, 48, 1);
    for(uint16_t i = 0;i<128;i++){
        virtualDisp->drawFastHLine(0,i,96,virtualDisp->color565(map(i,0,128,0,255),map(i,0,128,0,255),map(i,0,128,0,255)));
        delay(10);
    }
    printUTF8_fadeout("LXShow Box", 8, 48, 1);

    printUTF8_fadein("LXShow Box", 8, 48, 1);
    for(uint16_t i = 0;i<128;i++){
        virtualDisp->drawFastHLine(0,i,96,virtualDisp->color565(map(i,0,128,0,255),map(i,0,128,255,0),map(i,0,128,0,255)));
        delay(10);
    }
    printUTF8_fadeout("LXShow Box", 8, 48, 1);

    printUTF8_fadein("LXShow Box", 8, 48, 1);
    for(uint16_t i = 0;i<128;i++){
        virtualDisp->drawFastHLine(0,i,96,virtualDisp->color565(map(i,0,128,255,0),map(i,0,128,0,255),map(i,0,128,0,255)));
        delay(10);
    }
    printUTF8_fadeout("LXShow Box", 8, 48, 1);

    printUTF8_fadein("LXShow Box", 8, 48, 1);
    for(uint16_t i = 0;i<128;i++){
        virtualDisp->drawFastHLine(0,i,96,virtualDisp->color565(map(i,0,128,0,255),map(i,0,128,0,255),map(i,0,128,255,0)));
        delay(10);
    }
    printUTF8_fadeout("LXShow Box", 8, 48, 1);
}



