#include <avr/io.h>

#define N F_CPU/1000000

.global start_counter0
start_counter0:
    ; Reset counter
    STS TCNT0, R0; R1 is always 0

    ; Set the prescaler to 1024
    LDI R24, 0b101<<CS00;
    STS TCCR0B, R24;

    RET

.global start_counter1
start_counter1:
    ; Reset counter
    STS TCNT1H, R1; R1 is always 0
    STS TCNT1L, R1;

    ; Set the prescaler to 1024
    LDI R24, 0b101<<CS10;
    STS TCCR1B, R24;

    RET

.global read_timer0
; R24 = TCNT0
read_timer0:
    CLI; Disable interrupts

    ; Read timer
    LDS R24, TCNT0;
    ; R24 is returned as uint8_t

    SEI; Enable interrupts
    RET;

.global read_timer1
; R25:R24 = TCNT1
read_timer1:
    CLI; Disable interrupts

    ; Read timer
    LDS R24, TCNT1L;
    LDS R25, TCNT1H;
    ; R25:R24 is returned as uint16_t

    SEI; Enable interrupts
    RET;


.global timestamp_to_ms0
; Input R24 (timestamp 1), R22 (timestamp 2)
; Output R25:R24 (ms timestamp diff) (R25 probably 0)
timestamp_to_ms0:
    ; Calculate counter differance
    MOV R25, R22;
    SUB R25, R24; R25 = R25 - R24

    ; counts = T*F_CPU/1000/PRESCALER = T*N*1,000/2^10 <=>
    ; T = 2^8*counts/(N*250)
    CLR R24; By puting counts in R25 it is already multiplied by 2^8
    LDI R23, hi8(N*250);
    LDI R22, lo8(N*250);
    CALL div16; R25:R24 = R25:R24 / R23:R22

    RET;

.global busy_wait_ms0
; Input R24 (ms to wait)
busy_wait_ms0:
    PUSH R0;
    PUSH R2;
    PUSH R3;

    ; Calculate number of timer counts to wait
    ; {
    ; To wait T ms the timer should count T*F_CPU/1000/PRESCALER times
    ; Assume F_CPU = N*1,000,000 and PRESCALER = 1024
    ; count = T*125N/2^7 = TN - (3TN >> 7)
    ; The last step is necessary to keep all calculations below 8 bits
    ; This calculation overflows when NT > 85

    ; Calculate T*N
    LDI R22, N;
    MUL R22, R24; R1:R0 = R22*R24
    MOV R2, R0; Save result
    MOV R24, R0;
    MOV R25, R1;

    ; Calculate 3*TN
    CLR R23;
    LDI R22, 3;
    CALL mul16; R25:R24 = R25:R24 * R23:R22

    ; Calculate R25 = (3TN >> 7)
    ; Result should be like this (where a-p is 1 or 0):
    ; R25=abcdefgh R24=ijklmnop -> R25=bcdefghi
    LSL R25; Shift left, R25=bcdefgh0
    LSL R24; Shift left (just to set C flag)
    ADC R25, R1; Add the carry bit R25=bcdefghi (R1 = 0)

    ; Calculate TN - (RTN >> 7) aka R3:R2 - R1:R25 (R1 = 0)
    ; Save result in R2
    SUB R2, R25;
    ; Should do: "SBC R3, R1" but we're no longer intrested in high byte

    ; }

    ; Get current counter value
    CALL read_timer0; R24 = TCNT0

    ; Calculate OCR
    ; R2 = R2 + R24
    ADD R2, R24;

    ; Set OCR
    STS OCR0A, R2;

    ; Wait for output compare match flag
    SBIS TIFR0-0x20, OCF0A; Skip if flag bit is set
    RJMP .-4; Try again

    ; Clear output compare match flag
    SBI TIFR0-0x20, OCF0A;

    POP R3;
    POP R2;
    CLR R1;
    POP R0;
    RET

.global busy_wait_ms1
; Input R25:R24 (ms to wait)
busy_wait_ms1:
    PUSH R0;
    PUSH R2;
    PUSH R3;

    ; Calculate number of timer counts to wait
    ; {
    ; To wait T ms the timer should count T*F_CPU/1000/PRESCALER times
    ; Assume F_CPU = N*1,000,000 and PRESCALER = 1024
    ; count = T*125N/2^7 = TN - (3TN >> 7)
    ; The last step is necessary to keep all calculations below 16 bits
    ; This calculation overflows when NT > 21,845 which is adequate

    ; Calculate T*N
    CLR R23;
    LDI R22, F_CPU/1000000;
    CALL mul16; R25:R24 = R25:R24 * R23:R22
    MOV R2, R24; Save result
    MOV R3, R25;

    ; Calculate 3*TN
    CLR R23;
    LDI R22, 3;
    CALL mul16; R25:R24 = R25:R24 * R23:R22

    ; Calculate R0:R25 = (3TN >> 7)
    ; Result should be like this (where a-p is 1 or 0):
    ; R25=abcdefgh R24=ijklmnop -> R0=0000000a R25=bcdefghi
    CLR R0;
    LSL R25; Shift left, R25=bcdefgh0
    ADC R0, R1; Add the carry bit R0=0000000a (R1 empty)
    LSL R24; Shift left (just to set C flag)
    ADC R25, R1; Add the carry bit R25=bcdefghi

    ; Calculate TN - (RTN >> 7) aka R3:R2 - R0:R25
    ; Save result in R3:R2
    SUB R2, R25;
    SBC R3, R0;

    ; }

    ; Get current counter value
    CALL read_timer1; R25:R24 = TCNT1

    ; Calculate OCR
    ; R3:R2 = R3:R2 + R25:R24
    ADD R2, R24;
    ADC R3, R25;

    ; Set OCR
    STS OCR1AH, R3;
    STS OCR1AL, R2;

    ; Wait for output compare match flag
    SBIS TIFR1-0x20, OCF1A; Skip if flag bit is set
    RJMP .-4; Try again

    ; Clear output compare match flag
    SBI TIFR1-0x20, OCF1A;

    POP R3;
    POP R2;
    CLR R1;
    POP R0;
    RET
