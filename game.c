/** @file   game.c
    @authors T. Brooker, A.Osuna
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

/* Function to swap the turns of both players*/
void turn_swapper(int player_number)
{
    if(player_number == 1)              //Calls targeting function to initiate shooting turn for player
    {
        move_target_recticle();
        tinygl_clear();

    }
    if(player_number == 0)              //Calls targeting function to initiate recieving turn for player
    {
        pacer_wait();
        shot_reciever();
    }
}

int main (void)
{
    system_init ();                     //Initialize the system

    int player_number = set_players();
    select_fleet();
    while(1)
    {
        turn_swapper(player_number);
        player_number = !player_number;
        pacer_wait();
        tinygl_update();
    }
}
