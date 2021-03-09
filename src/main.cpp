#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "systick.h"
#include "usart.h"

char D_120_DEG_3_KT_B[] = "\xFF\x70\x0C\x01\x84\x4E\x88\x01\x74\x51\x01\x00\x78\x52\x08\x55\x50\x96";

int main() {

    systick_init();
    USART_Init(10560);

    set_sleep_mode(SLEEP_MODE_IDLE);
    wdt_enable(WDTO_2S);
    sei();

    while (1) {
        if (systick_1hz())
            USART_SendBlocking(D_120_DEG_3_KT_B, sizeof(D_120_DEG_3_KT_B) - 1);

        wdt_reset();
    }

    return 0;
}
