#include "rotary_encoder.h"

#define LONG_PRESS_TIME 750
#define ACCERLERATION_SPEED 75

rotary_encoder::rotary_encoder(/* args */)
{
}

rotary_encoder::~rotary_encoder()
{
}

void rotary_encoder::init()
{
    // enc_button.setClickHandler(press_handler);
    enc_button.setClickHandler([&](EncoderButton &btn)
                               { rot_enc.press_handler(btn); });
    enc_button.setDoubleClickHandler([&](EncoderButton &btn)
                                     { rot_enc.double_press_handler(btn); });
    // enc_button.setLongClickHandler(long_press_handler);
    // enc_button.setEncoderHandler(encoder_handler);
    enc_button.setLongClickDuration(500);
    enc_button.setRateLimit(50);
}

void rotary_encoder::update()
{
    enc_button.update();
}

bool rotary_encoder::get_change_status()
{
    return false;
}

BUTTON_STATES rotary_encoder::get_enc_press_state()
{
    BUTTON_STATES temp = button_press_state;

    // if button was pressed,
    // reset button state because it has been read
    if (button_press_state == PRESSED)
    {
        button_press_state = NOT_PRESSED;
    }

    return temp;
}

void rotary_encoder::press_handler(EncoderButton &eb)
{
    button_press_state = PRESSED;
}

void rotary_encoder::double_press_handler(EncoderButton &eb)
{
    Serial.println("Btn double Press");
}

rotary_encoder rot_enc = rotary_encoder();