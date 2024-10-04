#pragma once

#include <stdint.h>

class Model
{
    // Must set be high enough so emission period can't "starve" the slowest arriving update (wind speed)
    static constexpr uint8_t COUNTDOWN_START_VALUE = 2;

  public:
    void setWindSpeed(uint16_t cms) {
        // ~ 1 Hz update
        wind_speed_cms         = cms;
        speed_update_countdown = COUNTDOWN_START_VALUE;
    }

    uint16_t getWindSpeedCms() {
        return wind_speed_cms;
    }

    void setWindAngle(int16_t degrees) {
        // 5 Hz update
        wind_angle_degrees    = degrees;
        wind_update_countdown = COUNTDOWN_START_VALUE;
    }

    int16_t getWindAngleDegrees() {
        return wind_angle_degrees;
    }

    bool evaluateUpdateFlag() {
        bool is_updated = (speed_update_countdown > 0) && (wind_update_countdown > 0);

        if (is_updated) {
            speed_update_countdown--;
            wind_update_countdown--;
        }

        return is_updated;
    }

  private:
    uint16_t wind_speed_cms;
    int16_t  wind_angle_degrees;

    uint8_t speed_update_countdown = 0;
    uint8_t wind_update_countdown  = 0;
};

Model model;
