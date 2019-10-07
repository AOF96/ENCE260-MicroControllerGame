/** @file   steer1.c
    @author M.P. Hayes
    @date   28 Aug 2008

    @defgroup steer1 Simple steering program using navswitchs
*/
#include <string.h>
#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "pio.h"
#include "../fonts/font3x5_1.h"

/* Define polling rate in Hz.  */
#define LOOP_RATE 300

#define SIZE 35

static tinygl_point_t buffer[SIZE];
static int in = 0;
static int out = 0;


void point_add (tinygl_point_t pos)
{
    int old = in;

    tinygl_draw_point (pos, 1);
    buffer[in] = pos;
    in++;
    if (in >= SIZE)
        in = 0;
    if (in == out)
        in = old;
}


void point_remove (void)
{
    if (in == out)
        return;

    tinygl_draw_point (buffer[out], 0);
    out++;
    if (out >= SIZE)
        out = 0;
}

void record (tinygl_point_t pos)
{
    int column = pos.x;
    int row = pos.y;


}

int main (void)
{
    uint16_t tick = 0;
    tinygl_point_t pos = tinygl_point (2, 3);

    system_init ();
    DDRC |= (1 << 2);

    pio_config_set (LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);

    pio_config_set (LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);

    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);

    navswitch_init ();

    pacer_init (LOOP_RATE);

    point_add (pos);

    /* Paced loop.  */
    while (1)
    {
        bool push = 0;

        /* Wait for next tick.  */
        pacer_wait ();

        tick++;

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_EAST) && pos.x < 4)
        {
            pos.x++;
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST) && pos.x > 0)
        {
            pos.x--;
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && pos.y < 6)
        {
            pos.y++;
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH) && pos.y > 0)
        {
            pos.y--;
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH))
        {
            PORTC |= (1 << 2);
        } else {
            PORTC &= ~(1<<2);

        }

        if (push)
        {
            push = 0;
            point_remove();
            point_add (pos);
        }

        tinygl_update ();
    }

    return 0;
}
