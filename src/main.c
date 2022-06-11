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

extern uint16_t div16(uint16_t numerator, uint16_t denominator);

void debug_print(uint8_t index) {
    printf("%u: TM=%u, OC=%d TIFR=0x%x\n", index, read_timer1(), OCR1A, TIFR1);
}

int main() {
    stdout = &usart_stdout;
    USART_init();
    start_counter0();
    enable();
    printf("\nNEW\n");

    printf("1 ms\n");
    uint8_t timestamp0 = read_timer0();
    busy_wait_ms0(1);
    uint8_t timestamp1 = read_timer0();
    printf("%u counts: %u ms\n", timestamp1 - timestamp0, timestamp_to_ms0(timestamp0, timestamp1));

    printf("3 ms\n");
    timestamp0 = read_timer0();
    busy_wait_ms0(3);
    timestamp1 = read_timer0();
    printf("%u counts: %u ms\n", timestamp1 - timestamp0, timestamp_to_ms0(timestamp0, timestamp1));

    printf("10 ms\n");
    timestamp0 = read_timer0();
    busy_wait_ms0(10);
    timestamp1 = read_timer0();
    printf("%u counts: %u ms\n", timestamp1 - timestamp0, timestamp_to_ms0(timestamp0, timestamp1));

    printf("17 ms\n");
    timestamp0 = read_timer0();
    busy_wait_ms0(17);
    timestamp1 = read_timer0();
    printf("%u counts: %u ms\n", timestamp1 - timestamp0, timestamp_to_ms0(timestamp0, timestamp1));

    printf("32 ms\n");
    timestamp0 = read_timer0();
    busy_wait_ms0(32);
    timestamp1 = read_timer0();
    printf("%u counts: %u ms\n", timestamp1 - timestamp0, timestamp_to_ms0(timestamp0, timestamp1));

    uint8_t count;
    while(1) {
        count = 0;
        //printf("Blink HIGH\n");
        while (count++ != 200) {
            toggle();
            busy_wait_ms0(2);
            toggle();
            busy_wait_ms0(8);
        }
        count = 0;
        //printf("Blink LOW\n");
        while (count++ != 100) {
            toggle();
            busy_wait_ms0(1);
            toggle();
            busy_wait_ms0(19);
        }
    }
}

