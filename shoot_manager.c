#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "pio.h"
#include "../fonts/font3x5_1.h"
#include "ir_uart.h"
#include "button.h"

#define NUM_COLUMNS 5
#define NUM_ROWS 7

#define LOOP_RATE 500
#define SIZE 35

static uint8_t previous_column = 0;

static tinygl_point_t buffer[SIZE];
static int in = 0;
static int out = 0;

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] = {
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] = {
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


static uint8_t screen[] = {
    0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000
};

static void display_column (uint8_t row_pattern, uint8_t current_column)
{
    pacer_wait();
    pio_output_high(cols[previous_column]);


    for (int current_row = 0; current_row < 7; current_row++) {
        if ((row_pattern >> current_row) & 1) {
            pio_output_low(rows[current_row]);
        } else {
            pio_output_high(rows[current_row]);
        }
    }
    pio_output_low(cols[current_column]);
}

void initialize_led_matrix(void)
{
    pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);

    pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);
}

void set_bit(tinygl_point_t pos)
{
    int column = pos.x;
    int row = pos.y;
    screen[column] |= (1 << row);
    send_pos();
}

void send_pos(tinygl_point_t pos)
{
    char pos_byte = (pos.x << 3) + pos.y;
    ir_uart_putc(pos_byte);
}

void point_add (tinygl_point_t pos)
{
    int old = in;

    tinygl_draw_point (pos, 1);
    buffer[in] = pos;
    in++;
    if (in >= SIZE) {
        in = 0;
    }

    if (in == out) {
        in = old;
    }

}

void point_remove (void)
{
    if (in == out) {
        return;
    }

    tinygl_draw_point (buffer[out], 0);
    out++;
    if (out >= SIZE) {
        out = 0;
    }

}

void move_target_recticle (void)
{
    initialize_led_matrix();
    pacer_wait ();
    uint16_t tick = 0;
    uint8_t current_column = 0;
    tinygl_point_t pos = tinygl_point(2, 3);
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);
    navswitch_init ();
    pacer_init (LOOP_RATE);
    point_add (pos);

    while (1) {

        bool push = 0;
        pacer_wait ();
        tick++;
        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_EAST) && pos.x < 4) {
            pos.x++;
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST) && pos.x > 0) {
            pos.x--;
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && pos.y < 6) {
            pos.y++;
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH) && pos.y > 0) {
            pos.y--;
            push = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            set_bit(pos);
        }

        if (push) {
            push = 0;
            point_remove();
            point_add (pos);
        }


        tinygl_update ();
        display_column (screen[current_column], current_column);
        previous_column = current_column;
        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

    }
}

