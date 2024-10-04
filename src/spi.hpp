#pragma once

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#define SPI_CS   2
#define SPI_MOSI 3
#define SPI_MISO 4
#define SPI_SCK  5

#define SPI_DDR  DDRB
#define SPI_PORT PORTB

void spi_init(void) {
    // Set output direction
    SPI_DDR &= ~(1 << SPI_MISO);
    SPI_DDR |= (1 << SPI_CS) | (1 << SPI_MOSI) | (1 << SPI_SCK);

    // Set SPI control registers
    SPCR = 0;
    SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0) | (0 << CPOL) | (0 << CPHA);
}

unsigned char spi_out(unsigned char data) {
    /* send data */
    SPDR = data;
    /* wait for transmission */
    while (!(SPSR & (1 << SPIF)))
        ;
    /* clear the SPIF flag by reading SPDR */
    return SPDR;
    // return data;
}

void spi_disable() {
    SPI_PORT |= (1 << SPI_CS);
}

void spi_enable() {
    SPI_PORT &= ~(1 << SPI_CS);
}
