#pragma once

#include <avr/io.h>

#ifndef F_CPU
    #error "F_CPU must be defined"
#endif

void USART_Init(unsigned baud) {
    unsigned ubrr = F_CPU / 8 / baud - 1; // U2X

    UBRRL = (unsigned char)ubrr;
    UBRRH = (unsigned char)(ubrr >> 8);

    UCSRA |= 1 << U2X;

    // Enable USART transmitter
    UCSRB = (1 << TXEN);

    // Set frame format: asynchronous, 8bit data, odd parity, 1stop bit
    UCSRC = (1 << URSEL) | (0b11 << UPM0) | (0b11 << UCSZ0);
}

void USART_SendBlocking(const void* ptr, uint8_t len) {
    if (!len)
        return;

    const char* ptr_ = (const char*)ptr;

    while (len--) {
        // Wait for empty transmit buffer
        while (!(UCSRA & (1 << UDRE)))
            ;

        // send
        UDR = *ptr_++;
    }
}

/*

#include "avr/interrupt.h"


void USARTLineRecieved();

char  usart_buffer[128];
char* p_rec     = usart_buffer;
char  connected = 0;


ISR(USART_RXC_vect)
{
    char rec = UDR;

    if (rec == '\n') {
        *p_rec = '\0';
        p_rec  = usart_buffer;
        USARTLineRecieved();
    } else {
        *p_rec = rec;
        p_rec++;
    }
}

ISR(USART_TXC_vect) {
}
*/
