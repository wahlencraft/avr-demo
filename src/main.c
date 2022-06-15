#include <stdint.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "usart.h"
#include "time.h"

extern void enable();
extern void toggle();

ISR(TIMER0_OVF_vect) {}

int main() {
    stdout = &usart_stdout;
    USART_init();
    enable();
    printf("\nNEW\n");

    // Start a seperate, disturbing interrupt
    start_counter0();
    TIMSK0 = 1<<TOIE0;

    start_counter1();
    sei();
    while(1) {
        sleep_ms1(1000);
        toggle();
    };
}

