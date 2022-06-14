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

extern uint32_t mul16(uint16_t n0, uint16_t n1);
extern uint16_t div16(uint16_t numerator, uint16_t denominator);
extern uint32_t div24(uint32_t numerator, uint32_t denominator);
extern uint32_t div32(uint32_t numerator, uint32_t denominator);

void debug_print(uint8_t index) {
    printf("%u: TM=%u, OC=%d TIFR=0x%x\n", index, read_timer1(), OCR1A, TIFR1);
}

int main() {
    stdout = &usart_stdout;
    USART_init();
    start_counter0();
    enable();
    printf("\nNEW\n");

    printf("mul16: %lu\n", mul16(1, 8000));
    printf("div32: %lu\n", div32(18000000, 9000));
    printf("div24: %lu\n", div24(15000000, 3000));
    busy_wait_ms0(32);  // Clear out-stream before next section

    printf("\nTimer 0\n");
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

    printf("\nTimer 1\n");
    start_counter1();
    printf("500 ms\n");
    uint16_t timestamp2 = read_timer1();
    busy_wait_ms1(500);
    uint16_t timestamp3 = read_timer1();
    printf("%u counts: %u ms\n", timestamp3 - timestamp2, timestamp_to_ms1(timestamp2, timestamp3));

    printf("1 ms\n");
    timestamp2 = read_timer1();
    busy_wait_ms1(1);
    timestamp3 = read_timer1();
    printf("%u counts: %u ms\n", timestamp3 - timestamp2, timestamp_to_ms1(timestamp2, timestamp3));

    start_counter1();
    printf("2500 ms\n");
    timestamp2 = read_timer1();
    busy_wait_ms1(2500);
    timestamp3 = read_timer1();
    printf("%u counts: %u ms\n", timestamp3 - timestamp2, timestamp_to_ms1(timestamp2, timestamp3));

    printf("8388 ms\n");  // Max possible time measure @8 MHz
    timestamp2 = read_timer1();
    busy_wait_ms1(8388);
    timestamp3 = read_timer1();
    printf("%u counts: %u ms\n", timestamp3 - timestamp2, timestamp_to_ms1(timestamp2, timestamp3));

    uint8_t count;
    while(1) {
        count = 0;
        printf("Blink HIGH\n");
        while (count++ != 200) {
            toggle();
            busy_wait_ms0(2);
            toggle();
            busy_wait_ms0(8);
        }
        count = 0;
        printf("Blink LOW\n");
        while (count++ != 100) {
            toggle();
            busy_wait_ms0(1);
            toggle();
            busy_wait_ms0(19);
        }
    }
}

