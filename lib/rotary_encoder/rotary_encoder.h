#if !defined(ROTARY_ENCODER_H)
#define ROTARY_ENCODER_H

#include <EncoderButton.h>

#define SW_PIN 40
#define CLK_PIN 42
#define DT_PIN 41

enum BUTTON_STATES
{
    PRESSED,
    NOT_PRESSED
};

class rotary_encoder
{
private:
    EncoderButton enc_button = EncoderButton(DT_PIN, CLK_PIN, SW_PIN);

    BUTTON_STATES button_press_state = NOT_PRESSED;
    /* data */
public:
    rotary_encoder(/* args */);
    ~rotary_encoder();

    void init();
    void update();

    bool get_change_status();
    BUTTON_STATES get_enc_press_state();

private:
    void press_handler(EncoderButton &eb);
    void double_press_handler(EncoderButton &eb);
};

// rotary encoder object
// for binding the functions in init to it
extern rotary_encoder rot_enc;
#endif // ROTARY_ENCODER_H
