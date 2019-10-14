/** @file   game_finalizer.c
    @authors T.Brooker, A.Osuna
    @date   7 Oct 2019
    @brief This file is used when someone wins the game.
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


/** FUNCTIONS **/
/* Function To Display End Of Game Messages Dependent On Win Or Lose*/
void end_game(int end) {
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x5_1);
/*
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);                //One of these does not work???
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
*/
    pacer_init (PACER_RATE);
    navswitch_init();
    ir_uart_init();
    button_init();
    if(end == 1) {
        tinygl_text("YOU WIN! GAME OVER!\0");
    } else {
        tinygl_text("YOU LOSE! GAME OVER!\0");
    }
    tinygl_update();
}
