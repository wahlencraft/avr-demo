#include <avr/io.h>

#define USART_BAUDRATE 9600
#define BAUD_GEN (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

.global USART_init
USART_init:
    // Set baud rate
    LDI R16, lo8(BAUD_GEN);
    LDI R17, hi8(BAUD_GEN);
    STS UBRR0H, R17;
    STS UBRR0L, R16;
    // Enable transmitter
    LDI R16, 1<<TXEN0
    STS UCSR0B, R16
    // Set frame format: 8 data bits, 1 stop bit
    LDI R16, (0<<USBS0) | (3<<UCSZ00);
    STS UCSR0C, R16
    RET

// Transmit 1 byte of data
// R26 contains data
.global USART_transmit
USART_transmit:
    // Wait for empty transmit buffer
    LDS R25, UCSR0A;
    SBRS R25, UDRE0;
    RJMP USART_transmit

    // Put data (R24) into buffer, sends data
    STS UDR0, R24
    RET
