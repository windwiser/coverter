#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "arduino-mcp2515/mcp2515.h"
#include "systick.h"
#include "usart.h"

extern void spi_init();

char D_120_DEG_3_KT_B[] = "\xFF\x70\x0C\x01\x84\x4E\x88\x01\x74\x51\x01\x00\x78\x52\x08\x55\x50\x96";

struct can_frame canMsg;
MCP2515          mcp2515;

int main() {

    systick_init();
    USART_Init(10560);
    spi_init();

    USART_SendBlocking("r", 1);

    set_sleep_mode(SLEEP_MODE_IDLE);
    wdt_enable(WDTO_2S);
    sei();

    if (mcp2515.reset() != MCP2515::ERROR_OK)
        USART_SendBlocking("reset", 4);
    if (mcp2515.setBitrate(CAN_125KBPS) != MCP2515::ERROR_OK)
        USART_SendBlocking("setBitrate", 4);
    if (mcp2515.setNormalMode() != MCP2515::ERROR_OK)
        USART_SendBlocking("setNormalMode", 4);

    while (1) {
        // if (systick_1hz())
        //     USART_SendBlocking("a", 1);

        if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
            USART_SendBlocking("a", 1);

        //     USART_SendBlocking(D_120_DEG_3_KT_B, sizeof(D_120_DEG_3_KT_B) - 1);

        wdt_reset();
    }

    return 0;
}

/*

struct can_frame canMsg;
MCP2515 mcp2515(10);


void setup() {
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" ");
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");

    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      Serial.print(canMsg.data[i],HEX);
      Serial.print(" ");
    }

    Serial.println();
  }
}


*/