//
// Created by 22012 on 2022/8/1.
//

#ifndef _CONFIG_H
#define _CONFIG_H

//爆改车间
//LED屏幕 96*128 RGB屏幕  96*64 32扫 2块拼接

//日历代码
#define PANEL_RES_X 96// Number of pixels wide of each INDIVIDUAL panel module.
#define PANEL_RES_Y 64// Number of pixels tall of each INDIVIDUAL panel module.
#define PIN_E 32

#define NUM_ROWS 2                    // Number of rows of chained INDIVIDUAL PANELS
#define NUM_COLS 1                    // Number of INDIVIDUAL PANELS per ROW
#define PANEL_CHAIN NUM_ROWS *NUM_COLS// total number of panels chained one to another

// Change this to your needs, for details on VirtualPanel pls read the PDF!
#define SERPENT false
#define TOPDOWN false


/*Home Button*/
#define homeButtonPin 33;














#endif //_CONFIG_H
