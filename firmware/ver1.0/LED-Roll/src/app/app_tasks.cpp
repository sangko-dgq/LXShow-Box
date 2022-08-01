//
// Created by 22012 on 2022/8/2.
//
#include "Arduino.h"
#include "app_tasks.h"

#include <freertos/task.h>
#include <freertos/queue.h>
#include "freertos/FreeRTOS.h"

#include "pages/page_startup.h"

#include "../hal/hal.h"

static TaskHandle_t taskHandle_keepStatrtupPage;
static TaskHandle_t taskHandle_updateClockPage;


/*任务本体*/
[[noreturn]]static void task_startupPage(void *argument)
{
    while (true)
    {
        keepStatrtupPage();
        vTaskDelay(100);
    }
}

void startTask_keepStatrtupPage()
{
    xTaskCreate(task_startupPage, "task_startupPage", 2048, nullptr, 2,
                &taskHandle_keepStatrtupPage); //Startup-page need 2048 ,1024 is not enough
}

void endTask_keepStatrtupPage()
{
    virtualDisp->clearScreen();  //important!!!
    vTaskDelete(taskHandle_keepStatrtupPage);
}

/*ClockPage- DataUpdate*/
[[noreturn]] static void task_updateClockPage(void *argument)
{
    while (true)
    {
        updateTimeStruct();
        updateTimeUI();
        updateTmpUI();
        vTaskDelay(0);
    }
}
void startTask_updateClockPage()
{
    xTaskCreate(task_updateClockPage, "task_updateClockPage", 2048, nullptr, 2,
                &taskHandle_updateClockPage); //Startup-page need 2048 ,1024 is not enough
}

void endTask_updateClockPage()
{
    virtualDisp->clearScreen();  //important!!!
    vTaskDelete(taskHandle_keepStatrtupPage);
}
