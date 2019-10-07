#include "system.h"
#include "game_initializer.h"

int main (void)
{
    int player_number;

    system_init ();
    initialize_game();
    player_number = set_players();

    while (1)
    {



    }
}
