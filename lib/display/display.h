#if !defined(DISPLAY_H)
#define DISPLAY_H

#include <Arduino.h>
// #include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
// #include <Adafruit_SSD1327.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "EtherArtnet.h"
// #include <U8g2lib.h>

// https://randomnerdtutorials.com/esp32-s3-devkitc-pinout-guide/
#define I2C_ADDRESS 0x3D * 2 // u8g2 only: The Address needs to be multiplied by two (Bitshift by 1 to the left)
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 128
#define ROT_0 0
#define ROT_90 1
#define ROT_180 2
#define ROT_270 3
// #define CLOCK_PIN 9
// #define DATA_PIN 8
#define SPI_MOSI 35 // DIN
#define SPI_MISO 37 // NC - only communication in one way
#define SPI_CLK 36
#define SPI_CS 39
#define SPI_DC 40
#define SPI_RST 41

#define DISPLAY_ON_TIME 200 // 30.000 ms; default value for counter, decreased every 10 ms

#define SELECTION_FRAME_OFFSET 25 // icons are 20x20 px and spaced 5 px apart
#define MAX_MENU_ITEMS_X 5        // 5 menu items per row -> !NEVER 0!
#define MAX_MENU_ITEMS 4

#define MAIN_MENU_DELAY 10

enum DISPLAY_STATUS
{
    ON,
    OFF
};

class OledDisplay
{
    // private data
private:
    // Adafruit_SSD1327 display = Adafruit_SSD1327(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1, 1000000); // I2C ~ 400 ms for bitmap draw
    // Adafruit_SSD1327(uint16_t w, uint16_t h, SPIClass *spi, int8_t dc_pin,
    //                int8_t rst_pin, int8_t cs_pin, uint32_t bitrate = 8000000UL);
    // Adafruit_SSD1327 display = Adafruit_SSD1327(DISPLAY_WIDTH, DISPLAY_HEIGHT, &SPI_1, SPI_DC, SPI_RST, SPI_CS);
    // Adafruit_SSD1327 display = Adafruit_SSD1327(DISPLAY_WIDTH, DISPLAY_HEIGHT, SPI_MOSI, SPI_CLK, SPI_DC, SPI_RST, SPI_CS); // software SPI ~ 130 ms for bitmap draw
    // Adafruit_SH1107 display = Adafruit_SH1107(DISPLAY_WIDTH, DISPLAY_HEIGHT, SPI_MOSI, SPI_CLK, SPI_DC, SPI_RST, SPI_CS);
    // Adafruit_SH1107 display = Adafruit_SH1107(DISPLAY_WIDTH, DISPLAY_HEIGHT, &SPI, SPI_DC, SPI_RST, SPI_CS);
    Adafruit_SH1107 display = Adafruit_SH1107(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1, 1000000);
    // U8G2_SSD1327_WS_128X128_F_4W_SW_SPI display = U8G2_SSD1327_WS_128X128_F_4W_SW_SPI(U8G2_R0, SPI_CLK, SPI_MOSI, SPI_CS, SPI_MISO);

    unsigned long last_update = 0;
    uint16_t display_on_counter = DISPLAY_ON_TIME;
    DISPLAY_STATUS display_on = ON;

    // public functions
public:
    OledDisplay(/* args */);
    ~OledDisplay();

    void init();
    void update();
    void reset_timeout();
    void clear_display();
    void show_main_menu(CON_STATUS con_status, uint8_t menu_item);

    // private functions
private:
    uint8_t calc_center(uint8_t width, uint8_t space = 128);
};

#endif // DISPLAY_H
