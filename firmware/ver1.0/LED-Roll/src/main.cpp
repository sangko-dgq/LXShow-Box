
#include "app/app_typedef.h"
#include "app/pages/page_clock.h"
#include "app/app_tasks.h"
#include "hal/hal.h"

void setup()
{

    Serial.begin(115200);
    initDisplay();
    startTask_keepStatrtupPage();
    initWIFI();
    delay(500);
    initNtpTime();
    initTimer();
    initHomeButton();

    endTask_keepStatrtupPage();

    initClockPage();
    startTask_updateClockPage();
}


void loop()
{
    updateHomeButton();
}// end loop