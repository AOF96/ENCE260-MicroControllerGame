/** @file   game.c
    @author T. Brooker (twb30) & A. Osuna (aos26)
    @date   7 Oct 2019
    @brief Main game file.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "pio.h"
#include "../fonts/font5x5_1.h"
#include "ir_uart.h"
#include "button.h"
#include "game_initializer.h"
#include "shoot_manager.h"
#include "fleet_manager.h"

#define MESSAGE_RATE 20
#define LOOP_RATE 500
#define PACER_RATE 500

#define NUM_COLUMNS 5
#define NUM_ROWS 7
#define SIZE 35


/** VARIABLES **/
int player_number;


/* Function to swap the turns of both players*/
void turn_swapper(int player_number)
{
    if(player_number == 1) {            //Calls targeting function to initiate shooting turn for player
        for (int i = 0; i < 1500; i++) {
            pacer_wait();
        }
        move_target_recticle();
        tinygl_clear();
    }

    if(player_number == 0) {            //Calls targeting function to initiate recieving turn for player
        pacer_wait();
        shot_reciever();
    }
}

int main (void)
{
    system_init ();                     //Initialize the system

    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);

    ir_uart_init();
    button_init();
    navswitch_init();
    pacer_init (PACER_RATE);

    player_number = set_players();
    select_fleet();

    while(1) {
        turn_swapper(player_number);
        player_number = !player_number;
        pacer_wait();
        tinygl_update();
    }
}
