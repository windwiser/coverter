#include "systick.hpp"

#include <avr/interrupt.h>

#include <stdlib.h>

// --------------------------------------------------------------------------------------------------------------------

uint16_t systick_counter_1hz;
bool     systick_flag_1hz;
uint16_t systick_counter_10hz;
bool     systick_flag_10hz;
uint32_t systick_cntr = 0;

// --------------------------------------------------------------------------------------------------------------------

void systick_init() {
    // 8 000 000 / 64 / 125 = 1000 -> 1 ms tick

    TCCR2 = 0b100; // prescaler: 64
    OCR2  = 125;
    TIMSK |= (1 << OCIE2);
}

bool systick_1hz() {
    bool copy = systick_flag_1hz;

    if (systick_flag_1hz)
        systick_flag_1hz = false;

    return copy;
}

bool systick_10hz() {
    bool copy = systick_flag_10hz;

    if (systick_flag_10hz)
        systick_flag_10hz = false;

    return copy;
}

uint32_t systick_get_ms() {
    return systick_cntr;
}

ISR(TIMER2_COMP_vect) {
    TCNT2 = 0;
    systick_cntr++;

    if (++systick_counter_1hz == 1000) {
        systick_flag_1hz    = true;
        systick_counter_1hz = 0;
    }

    if (++systick_counter_10hz == 100) {
        systick_flag_10hz    = true;
        systick_counter_10hz = 0;
    }
}
