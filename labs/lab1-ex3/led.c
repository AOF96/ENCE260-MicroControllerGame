#include <avr/io.h>
#include "led.h"


/** Turn LED1 on.  */
void led_on (void)
{
    /* TODO!  */
    PORTC |= (1 << 2);
}


/** Turn LED1 off.  */
void led_off (void)
{
    /* TODO!  */
    PORTC &= ~(1<<2);
}


/** Initialise LED1.  */
void led_init (void)
{
    /* TODO!  */
    DDRC |= (1 << 2);
}
