#pragma once

#include <stdint.h>

class Model
{
  public:
    void setWindSpeed(uint16_t cms) {
        wind_speed_cms = cms;
        speed_is_set   = true;
    }

    uint16_t getWindSpeedCms() {
        return wind_speed_cms;
    }

    void setWindAngle(int16_t degrees) {
        wind_angle_degrees = degrees;
        angle_is_set       = true;
    }

    int16_t getWindAngleDegrees() {
        return wind_angle_degrees;
    }

    bool evaluateUpdateFlag() {
        bool is_updated = speed_is_set && angle_is_set;

        if (is_updated) {
            speed_is_set = false;
            angle_is_set = false;
        }

        return is_updated;
    }

  private:
    uint16_t wind_speed_cms;
    int16_t  wind_angle_degrees;

    bool speed_is_set = false;
    bool angle_is_set = false;
};

Model model;
