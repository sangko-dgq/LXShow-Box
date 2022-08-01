//
// Created by 22012 on 2022/8/1.
//

#include "hal.h"

void initWIFI()
{
    Serial.println("*****************************************************");
    Serial.println(" WIFI and TIME ");
    Serial.println("*****************************************************");
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}

void initNtpTime()
{
    Serial.println("Contacting Time Server");

    configTime(3600 * 8, 0, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    struct tm tmstruct;
    delay(2000);
    tmstruct.tm_year = 0;
    getLocalTime(&tmstruct, 5000);
    while (tmstruct.tm_year == 70)
    {
        getLocalTime(&tmstruct, 5000);
        Serial.println("网络对时失败");
        Serial.println(tmstruct.tm_year);
        delay(2000);
    }


    Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1,
                  tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
    Serial.println("");
}