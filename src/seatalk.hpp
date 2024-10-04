#include "model.hpp"
#include "usart.h"

struct SeatalkMessage
{
    uint8_t  command;
    uint8_t  size;
    uint8_t* data;

    template<unsigned N>
    SeatalkMessage(uint8_t command, uint8_t (&data)[N])
        : command(command)
        , size(N)
        , data(data)

    {}
};

void send_seatalk(const SeatalkMessage& msg) {
    uint8_t   frame_size = msg.size + 1;
    uint16_t  buffer[frame_size];
    uint16_t* p_data = buffer;

    *p_data++ = (0x100 | msg.command);
    *p_data++ = ((msg.size - 2) | msg.data[0]);

    for (int i = 1; i < msg.size; ++i) {
        *p_data++ = (msg.data[i]);
    }

    USART_SendBlocking(buffer, frame_size);
}

void sendWindAngle() {
    // 10  01  XX  YY  Apparent Wind Angle: XXYY/2 degrees right of bow
    uint16_t angle   = ((model.getWindAngleDegrees() + 360) % 360) * 2;
    uint8_t  data[3] = { //
                        0,
                        static_cast<uint8_t>((angle >> 8) & 0xFF),
                        static_cast<uint8_t>(angle & 0xFF)
    };

    SeatalkMessage seatalkMsg(0x10, data);
    send_seatalk(seatalkMsg);
}

void sendWindSpeed() {
    // 11  01  XX  0Y  Apparent Wind Speed: (XX & 0x7F) + Y/10 Knots
    //     Units flag: XX&0x80=0    => Display value in Knots
    //                 XX&0x80=0x80 => Display value in Meter/Second
    uint16_t       knots_times_10 = static_cast<uint16_t>(model.getWindSpeedCms() * 0.194384449);
    uint8_t        data[3]        = { 0,
                                      static_cast<uint8_t>((knots_times_10 / 10) & 0x7F),
                                      static_cast<uint8_t>(knots_times_10 % 10) };
    SeatalkMessage seatalkMsg(0x11, data);
    send_seatalk(seatalkMsg);
}