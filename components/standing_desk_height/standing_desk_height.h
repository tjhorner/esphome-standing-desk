#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

struct DeskPacket {
  enum state_t {
    SYNC1,   // waiting for 0x01
    SYNC2,   // waiting for 0x01 (second)
    HEIGHT1,  // waiting for first height arg, usually 0x01 but 0x00 if at the bottom
    HEIGHT2,  // waiting for second height arg
  } state_ = SYNC1;

  uint8_t buf_[2];

  // Process a byte sent by the desk controller. This returns
  // true when we have a complete packet, and false if we've
  // only processed a partial packet so far.
  // AFAIK, the packets sent by the desk controller containing
  // the height are sent as 4 bytes like (0x1, 0x1, (0x0|0x1), NUMBER),
  // and the height is encoded in the last two bytes.
  // Once put returns `true`, call decode() to get the height.
  bool put(uint8_t b) {
    switch (state_) {
    case SYNC1:
      if (b == 0x01) {
        state_ = SYNC2;
        return false;
      } else {
        state_ = SYNC1;
        return false;
      }
    case SYNC2:
      if (b == 0x01) {
        state_ = HEIGHT1;
        return false;
      } else{
        state_ = SYNC1;
        return false;
      }
    case HEIGHT1:
      if (b == 0x00 || b == 0x01) {
        buf_[0] = b;
        state_ = HEIGHT2;
        return false;
      } else {
        state_ = SYNC1;
        return false;
      }
    case HEIGHT2:
      buf_[1] = b;
      state_ = SYNC1;
      return true;
    default:
      return false;
    }
    return false;
  }

  // decode height sent by the desk controller.
  // returns a number from 0 (lowest) to 256
  // (highest). This function can only be called
  // after put() returns true, which indicates
  // that we have enough data. Otherwise it'll
  // return uninitialized garbage.
  float decode() {
    return ((buf_[0] << 8) | (buf_[1] & 0xFF)) / 10.0;
  }
};

namespace esphome {
namespace standing_desk_height {

class StandingDeskHeightSensor : public PollingComponent, public uart::UARTDevice, public sensor::Sensor
{
public:
  void loop() override;
  void update() override;

  float get_last_read();

protected:
  DeskPacket deskPacket;
  float last_read = -1;
  float last_published = -1;
};

}
}