#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x5_1.h"
#include <avr/io.h>
#include "navswitch.h"
#include "ir_uart.h"

#define PACER_RATE 500
#define MESSAGE_RATE 30
#define LOOP_RATE 500

tinygl_init(LOOP_RATE);
tinygl_font_set(&font5x5_1);
tinygl_text_speed_set(MESSAGE_RATE);
tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
pacer_init (PACER_RATE);
navswitch_init();
ir_uart_init ();

void initialize_game(void)
{
    int player_number = 0;
    int sent = 0;
    int recieved = 0;

    tinygl_text("Battleships Press S1 to play\0");

    while((PIND & (1<<7)) == 0) {
        pacer_wait();
        tinygl_update();
    }

    while(recieved_message == 0 && sent == 0)
    {
        if((PIND & (1<<7)) == 1)
        {
            player_number = 1;
            ir_uart_putc (2);
            sent = 1;
        }

        if (ir_uart_read_ready_p ())
        {
            int received;

            received = ir_uart_getc ();
        }

        if received = 2;
            player_number = 2;
        }
    }

    while(1){
        if(player_number == 1)
        {
            tinygl_text("Welcome Player 1");
            pacer_wait();
            tinygl_update();
        }
        else
        {
            tinygl_text("Welcome Player 2");
            pacer_wait();
            tinygl_update();
        }
    }
}
