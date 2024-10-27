#if !defined(ETHERNET_BITMAPS_H)
#define ETHERNET_BITMAPS_H

#include <Arduino.h>

// 'ethernet_connected', 14x13px
const unsigned char BITMAP_ethernet_connected[] PROGMEM = {
    0xff, 0xf8, 0xff, 0xf8, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18,
    0xc0, 0x18, 0xf0, 0x78, 0xf0, 0x78, 0xff, 0xf8, 0x7f, 0xf0};
// 'ethernet_disconnected', 13x13px
const unsigned char BITMAP_ethernet_disconnected[] PROGMEM = {
    0x1f, 0xc0, 0x7f, 0xe0, 0x70, 0x70, 0xfc, 0x78, 0xde, 0x18, 0xdf, 0x18, 0xcf, 0x98, 0xc7, 0xd8,
    0xc3, 0xd8, 0xe1, 0xf8, 0x70, 0x70, 0x3f, 0xe0, 0x1f, 0xc0};

enum {
	ethernet_connected,
	ethernet_disconnected
};

enum {
	WIDTH,
	HEIGHT
};

enum {
	x,
	y
};

#define BITMAP_ETHERNET_LEN 2

int W_H_MATRIX_ETHERNET[BITMAP_ETHERNET_LEN][2] = {
	{14, 13},
	{13, 13}
};

int X_Y_MATRIX_ETHERNET[BITMAP_ETHERNET_LEN][2] = {
	{4, 0},
	{4, 0}
};

const unsigned char* BITMAP_ETHERNET_ARRAY[BITMAP_ETHERNET_LEN] = {
	BITMAP_ethernet_connected,
	BITMAP_ethernet_disconnected
};

#endif // ETHERNET_BITMAPS_H
