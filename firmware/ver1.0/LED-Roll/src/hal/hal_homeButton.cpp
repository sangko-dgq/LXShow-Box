//
// Created by 22012 on 2022/8/2.
//

#include "hal.h"
#include "../../lib/OneButton/src/OneButton.h"

OneButton homeButton(33, true, true);


static void onHomeButtonSingleClicked()
{
    Serial.printf("SingleClicked\r\n");
}
static void onHomeButtonSDoubleClicked()
{
    Serial.printf("DoubleClick\r\n");

}

static void onHomeButtonPressed()
{
    Serial.printf("Pressed\r\n");
}
static void onHomeButtonPressStarted()
{
    Serial.printf("Press Started\r\n");
}

static void onHomeButtonPressStoped()
{
    Serial.printf("Press Stoped\r\n");
}

void initHomeButton()
{
    homeButton.setDebounceTicks(50);

    homeButton.setClickTicks(500);
    homeButton.attachClick(onHomeButtonSingleClicked);
    homeButton.attachDoubleClick(onHomeButtonSDoubleClicked);

    homeButton.setPressTicks(800);
    homeButton.attachLongPressStart(onHomeButtonPressStarted);
    homeButton.attachLongPressStop(onHomeButtonPressStoped);
    homeButton.attachDuringLongPress(onHomeButtonPressed);
}

void updateHomeButton()
{
    homeButton.tick();
}
