#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "can_data.hpp"
#include "model.hpp"
#include "seatalk.hpp"
#include "systick.hpp"

static constexpr uint32_t EMISSION_PERIOD_MS = 900;
static constexpr uint32_t PHASE_SHIFT        = 50;

// WARNING
// space should be at least 10 ms between packets
// delay does not take first packet into account, so MUST BE TESTED MANUALLY
static constexpr uint32_t SECOND_PACKET_DELAY = 19; // measured: 10.428 ms gap

uint32_t phase_of_emission = 0;
uint32_t p_last_emission   = 0;
uint32_t t_second_packet   = 0;

// can be used for collision detection
// void packet_emission_shift_phase() {
//     auto new_phase = phase_of_emission;
//     new_phase += PHASE_SHIFT;
//
//     if (new_phase > EMISSION_PERIOD_MS) {
//         p_last_emission++;
//         new_phase -= EMISSION_PERIOD_MS;
//     }
//
//     phase_of_emission = new_phase;
//     t_second_packet = 0;
// }

void packet_emission_update() {
    uint32_t now    = systick_get_ms();
    uint16_t period = now / EMISSION_PERIOD_MS;

    bool model_was_updated = model.evaluateUpdateFlag();
    bool period_has_passed = period > p_last_emission;
    bool phase_is_matching = now % EMISSION_PERIOD_MS >= phase_of_emission;

    if (model_was_updated && period_has_passed && phase_is_matching) {
        sendWindAngle();
        // TODO led::WindwiserData::toggle();

        t_second_packet = now + SECOND_PACKET_DELAY;
        p_last_emission = period;
    }

    if ((t_second_packet != 0) && (now >= t_second_packet)) {
        sendWindSpeed();

        t_second_packet = 0;
    }
}

int main() {

    systick_init();
    USART_Init(4800);
    can_data_init();

    set_sleep_mode(SLEEP_MODE_IDLE);
    wdt_enable(WDTO_2S);
    sei();

    while (1) {
        can_data_update();
        packet_emission_update();

        wdt_reset();
    }

    return 0;
}
