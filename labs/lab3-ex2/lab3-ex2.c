#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 10


void display_character (char character)
{
    char buffer[2];

    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


int main (void)
{
    char character = 'R';

    system_init ();

    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);

    navswitch_init();

    pacer_init (PACER_RATE);

    while(1)
    {
        pacer_wait ();
        tinygl_update ();

        navswitch_update();

        if (navswitch_push_event_p (NAVSWITCH_NORTH)){
            character = 'N';
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST)){
            character = 'E';
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH)){
            character = 'S';
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST)){
            character = 'W';
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)){
            character = 'R';
        }

        display_character (character);
    }
    return 0;
}
