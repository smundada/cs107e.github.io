#include "gpio.h"
#include "timer.h"

const int pin = 20;

#define ONE_SECOND 1000
#define DELAY_MORSE (ONE_SECOND/6)

void dot()
{
    gpio_write(pin, 1);
    timer_delay_ms(DELAY_MORSE); // dot
    gpio_write(pin, 0);
    timer_delay_ms(DELAY_MORSE);
}

void dash()
{
    gpio_write(pin, 1);
    timer_delay_ms(DELAY_MORSE * 3); // dash
    gpio_write(pin, 0);
    timer_delay_ms(DELAY_MORSE);
}

void morse_letter(char *code)
{
    // code should be in the form ".--.--."
    while (*code) {
        *code++ == '.' ? dot() : dash();
    }
    timer_delay_ms(DELAY_MORSE * 2); // space between letters (+1 for space after dot/dash)
}

void morse_s()
{
    morse_letter("...");
}

void morse_o()
{
    morse_letter("---");
}

void main(void)
{
    timer_init();
    gpio_init();
    gpio_set_output(pin);

    while (1) {
        // flash SOS
        morse_s();
        morse_o();
        morse_s();
        timer_delay_ms(DELAY_MORSE * 4); // pause between words (+3 from end of last s)
    }
}

