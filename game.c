#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "pio.h"
#include "../fonts/font3x5_1.h"

#include "game_initializer.h"
#include "shoot_manager.h"
#include "fleet_manager.h"

void turn_swapper(int player_number)
{
    if(player_number == 1)
    {
        move_target_recticle();
    }
    if(player_number == 2)
    {
        shot_reciever();
    }
}

int main (void)
{
    system_init ();

    int player_number = set_players();
    select_fleet();

    turn_swapper(player_number);
}
