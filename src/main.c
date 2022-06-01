#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#include "usart.h"

extern void enable();
extern void toggle();

int main() {
    // Define stdout to be usart
    stdout = &usart_stdout;

    // Turn on led
    enable();
    toggle();

    USART_init();

    // Turn of led
    toggle();

    uint8_t i = 0;
    while(1) {
        printf("Hello %d\n", i++);
    }
}
