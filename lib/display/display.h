#if !defined(DISPLAY_H)
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "init_loading_btm.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 128

class OledDisplay
{
private:
    Adafruit_SSD1327 display = Adafruit_SSD1327(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1, 1000000);

    uint8_t calc_center(uint8_t width, uint8_t space = 128);

    unsigned long init_loading_last_update = 0;
    uint8_t init_loading_btm_frame = 0;

    uint8_t counter = 0;

public:
    OledDisplay(/* args */);
    ~OledDisplay();

    void init();
    void clear_display();
    void init_loading();
    void main_menu();
};

#endif // DISPLAY_H
