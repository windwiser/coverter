#pragma once

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

/*

using Cs = GpioOutputB2;
using Int = GpioInputD2;
using Reset = GpioOutputD3;
using Sck = GpioOutputB5;
using Mosi = GpioOutputB3;
using Miso = GpioInputB4;

 */

#define SPI_SS 2 // CSS

#define SPI_MOSI 3
#define SPI_MISO 4
#define SPI_SCK  5

#define SPI_MASK (1 << SPI_SS)
#define SPI_DDR  DDRB
#define SPI_PORT PORTB

void spi_init(void) {
    // REEESEEET!
    DDRD |= (1 << 3);
    PORTD |= (1 << 3);

    // Set output direction
    SPI_DDR &= ~(1 << SPI_MISO);
    SPI_DDR |= (1 << SPI_SS) | (1 << SPI_MOSI) | (1 << SPI_SCK);

    // INterrupt
    // MCUCR = 0x00;
    // GICR |= 0x40;

    // Set SPI control registers
    SPCR = 0;
    SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0) | (0 << CPOL) | (0 << CPHA);
}

void spi_delay(uint16_t val) {

    while (val != 0) {
        _delay_loop_2((F_CPU / 4000) - 7);
        val--;
    }
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
    //GICR |= 0x40;
    // All high = all disabled
    SPI_PORT |= (SPI_MASK);
}

void spi_enable(unsigned char client_id) {
    //spi_disable();
    //GICR &= 0xbf;
    // Lower desired pin
    SPI_PORT &= ~(1 << client_id);
}

#define MCP_ID SPI_SS
/*

void mcp_init() {
    uint8_t i, a1, a2, a3;

    spi_init();
    // reset
    spi_disable();
    spi_delay(10);
    spi_enable(MCP_ID);
    spi_out(MCP_CMD_RESET);
    spi_disable();
    spi_delay(500);
    // wait until response

    // enter in config mode
    mcp_modreg(MCP_CANCTRL, MCP_MASK_MODE, MCP_MODE_CONFIG);
    mcp_readreg(MCP_CANSTAT);
}

#define MCP_CMD_RESET  0xc0
#define MCP_CMD_READ   0x03
#define MCP_CMD_WRITE  0x02
#define MCP_CMD_STATUS 0xA0
#define MCP_CMD_RXSTAT 0xB0
#define MCP_CMD_BITMOD 0x05

uint8_t mcp_readreg(const uint8_t address) {
    uint8_t data;
    spi_enable(MCP_ID);
    spi_out(MCP_CMD_READ);
    spi_out(address);
    data = spi_out(0x00);
    spi_disable();
    return data;
}
void mcp_readregs(const uint8_t address, uint8_t values[], const uint8_t n) {
    uint8_t i;
    spi_enable(MCP_ID);
    spi_out(MCP_CMD_READ);
    spi_out(address);
    for (i = 0; i < n; i++) {
        values[i] = spi_out(0x00);
    }
    spi_disable();
}

void mcp_setreg(const uint8_t address, const uint8_t value) {
    spi_enable(MCP_ID);
    spi_out(MCP_CMD_WRITE);
    spi_out(address);
    spi_out(value);
    spi_disable();
}
void mcp_modreg(const uint8_t address, const uint8_t mask, const uint8_t data) {
    spi_enable(MCP_ID);
    spi_out(MCP_CMD_BITMOD);
    spi_out(address);
    spi_out(mask);
    spi_out(data);
    spi_disable();
}

void mcp_setregs(const uint8_t address, const uint8_t values[], const uint8_t n) {
    uint8_t i;
    spi_enable(MCP_ID);
    spi_out(MCP_CMD_WRITE);
    spi_out(address);
    for (i = 0; i < n; i++) {
        spi_out(values[i]);
    }
    spi_disable();
}

*/
