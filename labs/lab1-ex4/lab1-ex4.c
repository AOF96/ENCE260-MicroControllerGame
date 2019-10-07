#include "button.h"
#include "led.h"
#include "system.h"


int main (void)
{
    system_init ();

    led_init ();
    button_init ();
    int led_bool = 1;

    while (1) {
        if (button_pressed_p ()) {
            if (led_bool == 1) {
                led_off ();
                led_bool = 0;
            } else {
                led_on ();
                led_bool = 1;
            }
        }
    }
}
