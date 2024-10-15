#include "display.h"

#define SCREEN_ADDRESS 0x3D
#define WHITE SSD1327_WHITE
#define BLACK SSD1327_BLACK

// -------------------------------------------------------------
// Private Methods

/// @brief calculates the left or top position of a object to center it within a space
/// @param width width or height of the object to be centered
/// @param space the space where the object should be centered in
/// @return top/left coordinate in the space for centering
uint8_t OledDisplay::calc_center(uint8_t width, uint8_t space)
{
    uint8_t position = 0;

    if (width != 0 && space != 0)
    {
        position = space / 2 - width / 2;
    }
    else
    {
        position = 0;
    }

    return position;
}

// -------------------------------------------------------------
// Public Methods
OledDisplay::OledDisplay(/* args */)
{
}

OledDisplay::~OledDisplay()
{
}

void OledDisplay::init()
{
    display.begin(SCREEN_ADDRESS);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setContrast(0x7F);
    display.setFont(&FreeMonoBold9pt7b);
    display.display();
    display.setTextWrap(false);
    init_loading_last_update = millis();
}

void OledDisplay::clear_display()
{
    display.clearDisplay();
    display.display();
}

void OledDisplay::init_loading()
{
    if (millis() - init_loading_last_update >= INIT_LOADING_BTM_FRAME_DELAY)
    {
        Serial.println(init_loading_btm_frame);
        display.clearDisplay();
        display.drawBitmap(32,
                           32,
                           init_loading_btm[init_loading_btm_frame],
                           INIT_LOADING_BTM_WIDTH,
                           INIT_LOADING_BTM_HEIGHT,
                           1);
        init_loading_btm_frame = (init_loading_btm_frame + 1) % INIT_LOADING_BTM_COUNT;
        init_loading_last_update = millis();
        display.display();
    }
}

void OledDisplay::main_menu()
{
    int width = 50;
    int height = 20;

    display.clearDisplay();
    display.setCursor(calc_center(width), calc_center(height) - 20);
    display.printf("Core 0: %d", counter);
    display.drawRoundRect(calc_center(width), calc_center(height), width, height, 5, WHITE);
    display.display();
}
