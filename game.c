/** @file   game.c
    @authors A.Osuna, T.Brooker
    @date   7 Oct 2019
    @brief Main game file.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "pio.h"
#include "../fonts/font3x5_1.h"
#include "ir_uart.h"

#include "game_initializer.h"
#include "shoot_manager.h"
#include "fleet_manager.h"


void turn_swapper(int player_number)
{
    if(player_number == 1)
    {
        move_target_recticle();
        tinygl_clear();



        while(!ir_uart_read_ready_p ())
        {
            pacer_wait();
        }
        char result = ir_uart_getc();
        if(result == 'M')
        {
            tinygl_text("MISS!\0");
            tinygl_update();
        }
        if (result == 'H')
        {
            tinygl_text("HIT!\0");
            tinygl_update();
        }

    }
    else if(player_number == 0)
    {
        PORTC |= (1 << 7);
        pacer_wait();
        shot_reciever();


    }
}

int main (void)
{
    system_init ();

    int isFinished = 0;
    int player_number = set_players();
    select_fleet();
    while(isFinished < 4)
    {
        turn_swapper(player_number);
        player_number = !player_number;
        pacer_wait();
        tinygl_update();
        isFinished++;
    }
}
