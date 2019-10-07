#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x5_1.h"
#include <avr/io.h>
#include "navswitch.h"
#include <stdbool.h>
#include "ir_uart.h"
#include <ctype.h>
#include <stdio.h>

#define PACER_RATE 500
#define MESSAGE_RATE 30
#define LOOP_RATE 500

/*
tinygl_init(LOOP_RATE);
tinygl_font_set(&font5x5_1);
tinygl_text_speed_set(MESSAGE_RATE);
tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
pacer_init (PACER_RATE);
navswitch_init();
*/


void initialize_game(void)
{
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    pacer_init (PACER_RATE);
    navswitch_init();

    tinygl_text("Battleships Press S1 to play\0");

    while((PIND & (1<<7)) == 0) {
        pacer_wait();
        tinygl_update();
    }

    tinygl_clear();

}
void display_character (char character)
{
    char buffer[2];

    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

int support_function(int choice)
{
    char ch = 'x';
    ir_uart_putc(choice);

    while (ch == 'x') {
        if (ir_uart_read_ready_p ()) {
            ch = ir_uart_getc ();

        }
    }

    if (choice != ch) {
        return 0;

    } else {
        return 1;
    }
}


int set_players(void)
{

    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    pacer_init (PACER_RATE);
    navswitch_init();

    //int player_number;
    char player1 = '1';
    char player2 = '2';
    char choice = '1';
    int done = 0;

    display_character(player1);
    while(done == 0) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();

        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            choice = player2;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            choice = player1;
        }
        display_character(choice);

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            int result;
            result = support_function(choice);
            (result == 1) ? (done = 1):(done = 0);

        }





    }

    PORTD &= ~(0<<7);
    while((PIND & (1<<7)) == 0) {
        pacer_wait();
        tinygl_update();
        /*
                char str[80];
                sprintf(str, "You are player %d\0", player_number);
        */
        tinygl_text("You are player X\0");
    }

    return choice;
}
