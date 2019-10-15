#ifndef FLEET_MANAGER_H
#define FLEET_MANAGER_H

#include "system.h"

/* Function To Select Players Fleet Layout */
int select_fleet (void);

/* Function for keeping track of enemy hits. Calls finish_game() if the
 * current fleet is destroyed */
void shot_reciever(void);

#endif
