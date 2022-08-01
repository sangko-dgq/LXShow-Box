//
// Created by 22012 on 2022/8/1.
//

#include "hal.h"

void initDisplay()
{
    Serial.println("*****************************************************");
    Serial.println(" HUN75 ");
    Serial.println("*****************************************************");

    HUB75_I2S_CFG mxconfig(
            PANEL_RES_X,// module width
            PANEL_RES_Y,// module height
            PANEL_CHAIN // chain length
    );
    mxconfig.gpio.e = 32;

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->setBrightness8(60);// range is 0-255, 0 - 0%, 255 - 100%
    // Allocate memory and start DMA display
    if (not dma_display->begin())
        Serial.println("****** !KABOOM! I2S memory allocation failed ***********");

    // create VirtualDisplay object based on our newly created dma_display object
    virtualDisp = new VirtualMatrixPanel((*dma_display), NUM_ROWS, NUM_COLS, PANEL_RES_X, PANEL_RES_Y, SERPENT,
                                         TOPDOWN);
    virtualDisp->fillScreen(virtualDisp->color444(0, 0, 0));
    virtualDisp->setTextColor(virtualDisp->color565(255, 231, 186), virtualDisp->color565(0, 0, 0));

}