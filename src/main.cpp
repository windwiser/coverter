#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "arduino-mcp2515/mcp2515.h"
#include "fastnet.hpp"
#include "model.hpp"
#include "systick.hpp"

static constexpr uint32_t EMISSION_PERIOD_MS = 1000;
static constexpr uint32_t PHASE_SHIFT        = 50;

uint32_t phase_of_emission = 0;
uint32_t p_last_emission   = 0;

void packet_emission_shift_phase() { // TODO rename
    auto new_phase = phase_of_emission;
    new_phase += PHASE_SHIFT;

    if (new_phase > EMISSION_PERIOD_MS) {
        p_last_emission++;
        new_phase -= EMISSION_PERIOD_MS;
    }

    phase_of_emission = new_phase;
}

void packet_emission_update() {
    uint32_t now    = systick_get_ms();
    uint16_t period = now / EMISSION_PERIOD_MS;

    bool model_was_updated = model.evaluateUpdateFlag();
    bool period_has_passed = period > p_last_emission;
    bool phase_is_matching = now % EMISSION_PERIOD_MS >= phase_of_emission;

    if (model_was_updated && period_has_passed && phase_is_matching) {
        sendWindData();
        // TODO led::WindwiserData::toggle();

        p_last_emission = period;
    }
}

struct can_frame canMsg;
MCP2515          mcp2515;

namespace Id {
constexpr uint32_t WindSpeed = 0x000000A0;
constexpr uint32_t WindAngle = 0x000000A2;
} // namespace Id

int main() {

    systick_init();
    USART_Init(10560);

    set_sleep_mode(SLEEP_MODE_IDLE);
    wdt_enable(WDTO_2S);
    sei();

    mcp2515.reset();
    mcp2515.setBitrate(CAN_125KBPS);
    mcp2515.setNormalMode();

    while (1) {
        if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
            switch (canMsg.can_id & CAN_EFF_MASK) {
            case Id::WindSpeed:
                if (canMsg.can_dlc >= 3) {
                    uint16_t wind_speed_cms = (canMsg.data[1] << 8) | canMsg.data[2];
                    model.setWindSpeed(wind_speed_cms);
                }
                break;
            case Id::WindAngle:
                if (canMsg.can_dlc >= 3) {
                    int16_t wind_angle_deg = (canMsg.data[1] << 8) | canMsg.data[2];
                    model.setWindAngle(wind_angle_deg);
                }
                break;
            default:
                break;
            }
        }

        packet_emission_update();
        wdt_reset();
    }

    return 0;
}
