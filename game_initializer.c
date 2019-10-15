/** @file   game_initializer.c
    @author T. Brooker (twb30) & A. Osuna (aos26)
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

/** VARIABLES **/
int player_number;
int players_set = 0;

/** FUNCTIONS **/
/*Function that displays an end game message until the UCFK is reset */
void finish_game(int finish_number)
{
    while(1) {

        if(finish_number == 2) {
            tinygl_text("  YOU LOSE!!!\0");
            for (int i = 0; i < 3000; i++) {
                pacer_wait();
                tinygl_update();
            }
        }

        if(finish_number == 1) {
            tinygl_text("  YOU WIN!!!\0");
            for (int i = 0; i < 3000; i++) {
                pacer_wait();
                tinygl_update();

            }
        }

    }
}

/* Function to Recieve Notification of Player Number */
void scanning (void)
{
    if(ir_uart_read_ready_p ()) {
        char temp;
        temp =ir_uart_getc();

        if(temp == '2') {
            player_number = 0;
            players_set = 1;
        }
    }
}

/* Function to Transmit Player Number to Other UCFK
(returning player number for later use in main code)*/
int set_players(void)
{
    tinygl_text("  BOATS OF WAR! Press S1 to play\0");

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
        tinygl_text("    Welcome P1! Push S1 to play\0");

    } else {
        tinygl_text("    Welcome P2! Push S1 to play\0");
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
