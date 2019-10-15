/** @file   shoot_manager.c
    @author T. Brooker (twb30) & A. Osuna (aos26)
    @date   Monday 7th October 2019
    @brief  Functions to select an XY coordinate and send it to the other player.
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

#define NUM_COLUMNS 5
#define NUM_ROWS 7
#define SIZE 35

int hits = 0;

static uint8_t previous_column = 0;


/*Bitmap that contains the target recticle and previous shots*/
static int screen[] = {
    0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000
};

/*Displays a bitmap of a target recticle and previous shots */
static void display_screen(uint8_t row_pattern, uint8_t current_column)
{
    for (int current_row = 0; current_row < 7; current_row++) {
        if ((row_pattern >> current_row) & 1) {
            tinygl_draw_point(tinygl_point(current_column, current_row), 1);
        } else {
            tinygl_draw_point(tinygl_point(current_column, current_row), 0);
        }
    }
}

/*Packs and sends shot coordinates to the other player*/
void send_pos(tinygl_point_t pos)
{
    int done = 0;
    char pos_byte = (pos.x << 3) + pos.y;

    ir_uart_putc(pos_byte);
    pacer_wait();
    while (!done) {
        if (ir_uart_read_ready_p()) {
            char response;
            response = ir_uart_getc();
            if (response == 'R') {    //awaits acknowledgement of shot
                done = 1;
            }
        }
    }

    done = 0;
    while(!done) {
        if (ir_uart_read_ready_p()) {
            char result = ir_uart_getc();
            if (result == 'H') {
                hits++;
                tinygl_text("    HIT!\0");
                for (int i = 0; i < 2500; i++) {
                    pacer_wait();
                    tinygl_update();
                }
                if(hits == 9) {
                    finish_game(1);
                }
            } else if(result == 'M') {
                tinygl_text("    MISS!\0");
                for (int i = 0; i < 2500; i++) {
                    pacer_wait();
                    tinygl_update();
                }
            }
            done = 1;
        }
    }
}

/*Sets previous shots in the bitmap and calls send_pos() */
void set_bit(tinygl_point_t pos)
{
    int column = pos.x;
    int row = pos.y;
    screen[column] |= (1 << row);
    send_pos(pos);
}


/*Moves the targeting dot around the screen*/
void move_target_recticle (void)
{
    pacer_wait ();
    uint8_t current_column = 0;
    tinygl_point_t pos = tinygl_point(2, 3);
    tinygl_draw_point (pos, 1);
    while (1) {
        pacer_wait ();
        navswitch_update ();

        display_screen (screen[current_column], current_column);
        if (navswitch_push_event_p (NAVSWITCH_EAST) && pos.x < 4) {
            pos.x++;
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST) && pos.x > 0) {
            pos.x--;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && pos.y < 6) {
            pos.y++;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH) && pos.y > 0) {
            pos.y--;
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {

            set_bit(pos);
            break;
        }

        tinygl_draw_point(pos, 1);


        tinygl_update ();
        previous_column = current_column;
        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

    }
}
