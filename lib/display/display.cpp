#include "display.h"

#include "init_loading_btm.h"
#include "ethernet_bitmaps.h"
#include "main_menu_bitmaps.h"

#define SCREEN_ADDRESS 0x3D

#if defined(SSD1327_WHITE)
#define WHITE SSD1327_WHITE
#define BLACK SSD1327_BLACK
#endif // SSD1327_WHITE

#if defined(SH110X_WHITE)
#define WHITE SH110X_WHITE
#define BLACK SH110X_BLACK
#endif // SH110X_WHITE

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
    // Adafruit

    display.begin(SCREEN_ADDRESS);
    // display.begin();
    display.setRotation(ROT_270);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setContrast(0x7F);
    display.setFont(&FreeMonoBold9pt7b);
    display.display();
    display.setTextWrap(false);

    // U8G2
    // set I2C address, call needs to be before init
    // display.setI2CAddress(I2C_ADDRESS);
    // display.setBusClock(4000000);
    // // init + clear + disables power save mode (set to 0)
    // display.begin();

    // init_loading_last_update = millis();
    // last_update = millis();
}

void OledDisplay::update()
{
    if (millis() - last_update >= 10)
    {
        if (display_on_counter != 0)
        {
            display_on_counter--;
            Serial.printf("Decrease: %d \n", display_on_counter);
            if (display_on_counter == 0)
            {
                Serial.println("Display off");
                clear_display();
                display_on = OFF;
            }
        }
        last_update = millis();
    }
}

void OledDisplay::reset_timeout()
{
    display_on_counter = DISPLAY_ON_TIME;
    display_on = ON;
}

void OledDisplay::clear_display()
{
    display.clearDisplay();
    display.display();
}

void OledDisplay::show_main_menu(CON_STATUS con_status, uint8_t menu_item)
{
    uint8_t x_offset = 0 + SELECTION_FRAME_OFFSET * (menu_item % (MAX_MENU_ITEMS_X)); // 0 -> 0; 1-> 1; ... 5 -> 1; 6 -> 2 ...
    uint8_t y_offset = 0 + SELECTION_FRAME_OFFSET * (menu_item / (MAX_MENU_ITEMS_X)); // 0 - 4 -> 0; 5 - 9 -> 1; 10 - 14 ...

    if (display_on == ON)
    {
        display.clearDisplay();

        display.drawBitmap(X_Y_MATRIX_MAIN_MENU[main_menu][x],
                           X_Y_MATRIX_MAIN_MENU[main_menu][y],
                           BITMAP_MAIN_MENU_ARRAY[main_menu],
                           W_H_MATRIX_MAIN_MENU[main_menu][WIDTH],
                           W_H_MATRIX_MAIN_MENU[main_menu][HEIGHT],
                           WHITE);

        if (con_status == CONNECTED)
        {
            display.drawBitmap(X_Y_MATRIX_ETHERNET[ethernet_connected][x],
                               X_Y_MATRIX_ETHERNET[ethernet_connected][y],
                               BITMAP_ETHERNET_ARRAY[ethernet_connected],
                               W_H_MATRIX_ETHERNET[ethernet_connected][WIDTH],
                               W_H_MATRIX_ETHERNET[ethernet_connected][HEIGHT],
                               WHITE);
        }
        else
        {
            display.drawBitmap(X_Y_MATRIX_ETHERNET[ethernet_disconnected][x],
                               X_Y_MATRIX_ETHERNET[ethernet_disconnected][y],
                               BITMAP_ETHERNET_ARRAY[ethernet_disconnected],
                               W_H_MATRIX_ETHERNET[ethernet_disconnected][WIDTH],
                               W_H_MATRIX_ETHERNET[ethernet_disconnected][HEIGHT],
                               WHITE);
        }

        display.drawBitmap(X_Y_MATRIX_MAIN_MENU[selection_frame][x] + x_offset,
                           X_Y_MATRIX_MAIN_MENU[selection_frame][y] + y_offset,
                           BITMAP_MAIN_MENU_ARRAY[selection_frame],
                           W_H_MATRIX_MAIN_MENU[selection_frame][WIDTH],
                           W_H_MATRIX_MAIN_MENU[selection_frame][HEIGHT],
                           WHITE);

        display.display();
    }
}