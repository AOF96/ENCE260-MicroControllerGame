#ifndef GAME_INITIALIZER_H
#define GAME_INITIALIZER_H

#include "system.h"

/* Function to Transmit Player Number to Other UCFK
(returning player number for later use in main code)*/
int set_players(void);

/*Function that displays an end game message until the UCFK is reset */
void finish_game(int finish_number);
#endif
