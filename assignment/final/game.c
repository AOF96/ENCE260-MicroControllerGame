#include <string.h>
#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "pio.h"
#include "../fonts/font3x5_1.h"
#include "game_initializer.h"


#define LOOP_RATE 300
int select_fleet (void);

static int previous_col = 0;

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

static void display_column (uint8_t row_pattern, uint8_t current_column)
{
    int current_row;
    pio_output_high (cols[previous_col]);

    for(current_row = 0; current_row < 7; current_row++){
        if ((row_pattern >> current_row) & 1)
        {
            pio_output_low (rows[current_row]);
        }
        else
        {
            pio_output_high (rows[current_row]);
        }
    }
    pio_output_low (cols[current_column]);
}

void fleet_select_display (int current_column)
{
    /* Fleet Setup */
    static int *fleet_options[3];

    int fleet1[5] = {0b1000111, 0b1001000, 0b1001000, 0b1001000, 0b1001000};
    int fleet2[5] = {0b0111110, 0b0001000, 0b0001001, 0b0001001, 0b0001001};
    int fleet3[5] = {0b0000000, 0b0111110, 0b0000001, 0b1111001, 0b0000001};


    fleet_options[0] = fleet1;
    fleet_options[1] = fleet2;
    fleet_options[2] = fleet3;

    select_fleet();

    while(1)
    {
        pacer_wait ();

        display_column (*fleet_options[current_column], current_column);

        previous_col = current_column;
        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }
    }
}

int select_fleet (void)
{
    int fleet_number = 0;
    bool push = 0;

    while(push != 1)
    {
        pacer_wait ();

        //tick++;

        navswitch_update ();


        if (navswitch_push_event_p (NAVSWITCH_PUSH))
        {
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        {
            fleet_number--;
            if(fleet_number < 0)
            {
                fleet_number = 2;
            }
            fleet_select_display (fleet_number);
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH))
        {
            fleet_number++;
            if(fleet_number > 2)
            {
                fleet_number = 0;
            }
            fleet_select_display (fleet_number);

        }
    }
    return(fleet_number);
}


int main (void)
{
    //uint16_t tick = 0;
    uint8_t current_column = 0;
    system_init ();
    navswitch_init ();
    pacer_init (LOOP_RATE);

    initialize_game();

    /* TODO: Initialise LED matrix pins.  */
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

    while (1)
    {
        fleet_select_display (current_column);
    }
}
