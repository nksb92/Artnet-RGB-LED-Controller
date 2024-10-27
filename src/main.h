#include <Arduino.h>
#include <FastLED.h>
#include "display.h"
#include "EtherArtnet.h"
#include "rotary_encoder.h"

#define STEP 3

// https://www.circuitstate.com/tutorials/how-to-write-parallel-multitasking-applications-for-esp32-using-freertos-arduino/
OledDisplay display = OledDisplay();
EtherArtnet artnet = EtherArtnet();

CHSV hsv_value(0, 200, 255);
// CHSV hsv_value(0, 255, 255);
CHSV temp_val(0, 0, 0);
CRGB rgb_val;

unsigned long last_update = 0;
uint16_t update_time = 75;

uint8_t segments = 20;
uint16_t dmx_address = 0; // 1 - 1 // actual 1, but subract one because of data structure
uint8_t data[512];
uint16_t universe = 1;

SemaphoreHandle_t artnet_mutex = NULL;

void loop_0(void *parameters);

void loop_1();