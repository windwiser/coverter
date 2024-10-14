#include "model.hpp"
#include "usart.h"

uint8_t checksum(const uint8_t* ptr, uint8_t len) {
    uint8_t sum = 0;

    for (uint8_t i = 0; i < len; i++)
        sum += *ptr++;

    return (sum ^ 0xff) + 1;
}

void sendWindData() {
    constexpr uint8_t FRAME_LENGTH = 14;

    uint8_t frame[FRAME_LENGTH];

    frame[0] = 0xFF;             // start of frame
    frame[1] = 0x70;             // sender is wind sensor
    frame[2] = FRAME_LENGTH - 6; // data length
    frame[3] = 0x01;             // data command
    frame[4] = checksum(frame, 4);

    // cm/s is already a multiplier of 100
    uint16_t wind_speed_100_knots = model.getWindSpeedCms() * 1.943844;
    frame[5]                      = 0x4E;                      // wind speed
    frame[6]                      = 0x88;                      // format
    frame[7]                      = wind_speed_100_knots >> 8; // wind speed
    frame[8]                      = wind_speed_100_knots;      // wind speed

    int16_t wind_angle = model.getWindAngleDegrees();
    frame[9]           = 0x51;
    frame[10]          = 0x01;
    frame[11]          = wind_angle >> 8; // wind angle
    frame[12]          = wind_angle;      // wind angle

    // !! ATTENTION !!
    // contrary to the docs, the last chx must round up to 0xAA rather than 0x100
    frame[13] = checksum(frame + 5, FRAME_LENGTH - 6) + 0xAA;

    USART_SendBlocking(frame, FRAME_LENGTH);
}
