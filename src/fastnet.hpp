#include "model.hpp"
#include "usart.h"

uint8_t checksum(const uint8_t* ptr, uint8_t len) {
    uint8_t sum = 0;

    for (uint8_t i = 0; i < len; i++)
        sum += *ptr++;

    return (sum ^ 0xff) + 1;
}

void sendWindData() {
    constexpr uint8_t FRAME_LENGTH = 18;

    uint8_t frame[FRAME_LENGTH];

    frame[0] = 0xFF;             // start of frame
    frame[1] = 0x70;             // sender is wind sensor
    frame[2] = FRAME_LENGTH - 6; // data length
    frame[3] = 0x01;             // data command
    frame[4] = checksum(frame, 4);

    // static wind speed, needed for autopilot
    frame[5] = 0x4E;
    frame[6] = 0x88;
    frame[7] = 0x03;
    frame[8] = 0x45;

    int16_t wind_angle = model.getWindAngleDegrees();
    frame[9]           = 0x51;
    frame[10]          = 0x01;
    frame[11]          = wind_angle >> 8; // wind angle
    frame[12]          = wind_angle;      // wind angle

    // raw wind pulses, needed for autopilot
    frame[13] = 0x52;
    frame[14] = 0x08;
    frame[15] = 0x4E;
    frame[16] = 0x34;

    // !! ATTENTION !!
    // contrary to the docs, the last chx must round up to 0xAA rather than 0x100
    frame[FRAME_LENGTH - 1] = checksum(frame + 5, FRAME_LENGTH - 6) + 0xAA;

    USART_SendBlocking(frame, FRAME_LENGTH);
}
