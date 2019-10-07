#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x5_1.h"
#include <avr/io.h>
#include "navswitch.h"

#define PACER_RATE 500
#define MESSAGE_RATE 30
#define LOOP_RATE 500

tinygl_init(LOOP_RATE);
tinygl_font_set(&font5x5_1);
tinygl_text_speed_set(MESSAGE_RATE);
tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
pacer_init (PACER_RATE);
navswitch_init();

void initialize_game(void)
{
    tinygl_text("Battleships Press S1 to play\0");

    while((PIND & (1<<7)) == 0) {
        pacer_wait();
        tinygl_update();
    }

    tinygl_clear();

}

int set_players(void)
{
    int player_number;
    char player1 = '1';
    char player2 = '2';
    char choice;

    display_character(player1);
    while(1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();

        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            choice = player1;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            choice = player2;
        }
        display_character(choice);

    }

    return 1;

}
