#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "arduino-mcp2515/mcp2515.h"
#include "systick.hpp"
#include "usart.h"

char D_120_DEG_3_KT_B[] = "\xFF\x70\x0C\x01\x84\x4E\x88\x01\x74\x51\x01\x00\x78\x52\x08\x55\x50\x96";

struct can_frame canMsg;
MCP2515          mcp2515;

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
        if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
            USART_SendBlocking(D_120_DEG_3_KT_B, sizeof(D_120_DEG_3_KT_B) - 1);

        wdt_reset();
    }

    return 0;
}
