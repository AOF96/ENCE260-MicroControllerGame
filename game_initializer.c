/** @file   game_initializer.c
    @author T. Brooker, A. Osuna
    @date   Monday 7th October 2019
    @brief  Functions to Initialize Game and Pick Player Numbers.
*/



#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x5_1.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"
#include "ir_uart.h"


#define PACER_RATE 500
#define MESSAGE_RATE 30
#define LOOP_RATE 500

/** VARIABLES **/
int player_number;
int players_set = 0;


/** FUNCTIONS **/
/* Function to Recieve Notification of Player Number */
void scanning (void)
{
    if (ir_uart_read_ready_p ()) {
        char temp;
        temp =ir_uart_getc();
        if (temp == '2') {
            player_number = 2;
            players_set = 1;
        }
    }
}

/* Function to Transmit Player Number to Other UCFK
(returning player number for later use in main code)*/
int set_players(void)
{
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    pacer_init (PACER_RATE);
    navswitch_init();
    ir_uart_init();
    button_init();

    tinygl_text("BOATS OF WAR! Press S1 to play\0");

    while(!players_set) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        button_update();
        scanning();

        if (button_push_event_p(0)) {
            ir_uart_putc('2');
            player_number = 1;
            players_set = 1;
        }
    }

    if (player_number == 1) {
        tinygl_text("Welcome Player 1 Push S1 to Play\0");
    } else {
        tinygl_text("Welcome Player 2 Push S1 to Play\0");
    }
    tinygl_update();
    button_update();

    while(!button_push_event_p(0)) {
        pacer_wait();
        tinygl_update();
        button_update();
    }

    tinygl_clear();
    tinygl_update();
    return player_number;
}
