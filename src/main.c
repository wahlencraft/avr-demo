#include <stdint.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "usart.h"
#include "time.h"

extern void enable();
extern void toggle();

int main() {
    stdout = &usart_stdout;
    USART_init();
    start_counter1();
    enable();
    printf("\nNEW\n");
    while(1) {
        printf("Blink ON\n");
        toggle();
        busy_wait_ms(1000);
        printf("Blink OFF\n");
        toggle();
        busy_wait_ms(500);
    }
}

