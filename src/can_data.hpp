#pragma once

#include "arduino-mcp2515/mcp2515.h"
#include "model.hpp"

namespace Id {
constexpr uint32_t WindSpeed = 0x000000A0;
constexpr uint32_t WindAngle = 0x000000A2;
} // namespace Id

MCP2515 mcp2515;

void can_data_init() {
    mcp2515.reset();
    mcp2515.setBitrate(CAN_125KBPS);
    mcp2515.setNormalMode();
}

void can_data_update() {
    struct can_frame canMsg;

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
}