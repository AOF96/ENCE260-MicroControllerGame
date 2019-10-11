# File:   Makefile
# Original Author: M. P. Hayes, UCECE
# Changes Made By: T. Brooker & A. Fregoso
# Date:   12 Sep 2010
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@


button.o: ../../drivers/button.c ../../drivers/button.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@


fleet_manager.o: fleet_manager.c ../../drivers/avr/system.h ../../utils/pacer.h ../../utils/tinygl.h ../../utils/font.h ../../drivers/avr/ir_uart.h ../../drivers/navswitch.h ../../drivers/avr/pio.h ../../drivers/button.h fleet_manager.h
	$(CC) -c $(CFLAGS) $< -o $@

game_initializer.o: game_initializer.c ../../drivers/avr/system.h ../../utils/pacer.h ../../utils/tinygl.h ../../utils/font.h ../../drivers/avr/ir_uart.h ../../drivers/button.h game_initializer.h
	$(CC) -c $(CFLAGS) $< -o $@

shoot_manager.o: shoot_manager.c ../../drivers/avr/system.h ../../drivers/navswitch.h ../../utils/pacer.h ../../utils/tinygl.h ../../drivers/avr/ir_uart.h ../../drivers/button.h ../../drivers/avr/pio.h ../../utils/font.h shoot_manager.h
	$(CC) -c $(CFLAGS) $< -o $@

game_finalizer.o: game_finalizer.c ../../drivers/avr/system.h ../../utils/pacer.h ../../utils/tinygl.h ../../utils/font.h ../../drivers/avr/ir_uart.h ../../drivers/button.h game_finalizer.h
	$(CC) -c $(CFLAGS) $< -o $@
# Link: create ELF output file from object files.
game.out: game.o system.o     button.o display.o font.o ir_uart.o ledmat.o navswitch.o pacer.o pio.o prescale.o timer.o timer0.o tinygl.o usart1.o     fleet_manager.o game_initializer.o shoot_manager.o game_finalizer.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start