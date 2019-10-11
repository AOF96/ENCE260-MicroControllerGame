#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "navswitch.h"
#include "pio.h"
#include "ir_uart.h"

#include "shoot_manager.h"

#define LOOP_RATE 300


/** VARIABLES **/

static int fleet_number = 0;

static int *display_fleet; //Pointer To Fleet Bitmap That Is To Be Displayed

/* Array Of Potential Fleets */
static int *fleet_options[5];

static int fleet1[] = {0b1000111, 0b1001000, 0b1001000, 0b1001000, 0b1001000};
static int fleet2[] = {0b0111110, 0b0001000, 0b0001001, 0b0001001, 0b0001001};
static int fleet3[] = {0b0000000, 0b0111110, 0b0000001, 0b0111101, 0b0000001};
static int fleet4[] = {0b0000001, 0b1000001, 0b1011101, 0b1000001, 0b1000001};
static int fleet5[] = {0b0000000, 0b0000010, 0b1111010, 0b0000010, 0b0111110};

/* Setting LEDMAT Variables */
static int previous_col = 0;
static int hit_counter = 0;

static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};

static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};



/** FUNCTIONS **/

/* Functions To Continuously Update The LEDMAT Display */

// (Function 1 of 2)
static void display_column(uint8_t row_pattern, uint8_t current_column)
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


// (Function 2 of 2)
void fleet_select_display(int current_column)
{
    // Initialise LED matrix pins
    tinygl_init(LOOP_RATE);

    while(1)
    {
        pacer_wait ();

        display_column (display_fleet[current_column], current_column);

        previous_col = current_column;
        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }
    }
}


void finish_game(void)
{

}
/* Function To Select Players Fleet Layout */
void select_fleet(void)
{
    display_fleet = fleet1;

    fleet_options[0] = fleet1;
    fleet_options[1] = fleet2;
    fleet_options[2] = fleet3;
    fleet_options[3] = fleet4;
    fleet_options[4] = fleet5;

    navswitch_init ();
    pacer_init (LOOP_RATE);

    int current_column = 0;
    while(navswitch_push_event_p (NAVSWITCH_PUSH) !=1) {
        pacer_wait ();

        display_column (display_fleet[current_column], current_column);

        previous_col = current_column;
        current_column++;

        if(current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }

        navswitch_update ();

        if(navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            fleet_number--;
            if(fleet_number < 0) {
                fleet_number = 4;
            }

            display_fleet = fleet_options[fleet_number];
        }

        if(navswitch_push_event_p (NAVSWITCH_NORTH)) {
            fleet_number++;
            if(fleet_number > 4) {
                fleet_number = 0;
            }
            display_fleet = fleet_options[fleet_number];
        }
    }
    tinygl_clear();
    tinygl_update();
}

/* Function for keeping track of enemy hits */
void shot_reciever(void)
{
    int done = 0;
    int enemy_Ypos;
    int enemy_Xpos;

    while(done == 0)
    {
        if (ir_uart_read_ready_p ())
        {
            char enemy_pos = 0;
            char temp = ir_uart_getc();
            enemy_pos = temp;

            enemy_Xpos = enemy_pos >> 3;
            enemy_Ypos = enemy_pos & 0b111;
            if (temp >> 6 == 0 && enemy_Xpos <= 5 && enemy_Ypos <= 7 && enemy_pos != 0)
            {
                if((fleet_options[fleet_number][enemy_Xpos] & (1 << enemy_Ypos)) == 1)
                {
                    hit_counter++;
                    ir_uart_putc('H');
                    //tinygl_text("HIT!\0");
                    //tinygl_update();
                    //PORTC |= (1<<2);
                } else {
                    ir_uart_putc('M');
                    //tinygl_text("MISS!\0");
                    //tinygl_update();
                }
                done = 1;
            }
        }
    }
    if(hit_counter == 12)
    {
        finish_game();
    }

}

