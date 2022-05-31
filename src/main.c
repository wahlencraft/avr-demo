#include <stdint.h>
#include <avr/io.h>

extern void enable();
extern void toggle();
extern void USART_init();
extern void USART_transmit(uint8_t data);

int main() {
    // Turn on led
    enable();
    toggle();

    // Send Hello World! over USART
    char msg[] = {"Hello World!\n"};
    USART_init();
    char c;
    uint8_t i = 0;
    do {
        c = msg[i++];
        USART_transmit(c);
    } while (c != '\0');

    // Turn of led
    toggle();

    while(1) {}
}
