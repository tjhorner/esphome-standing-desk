#pragma once

#include <stdint.h>
#include "decoder.h"

namespace esphome {
namespace standing_desk_height {

class AutonomousDecoder : public Decoder {
  private:
    enum state_t {
      SYNC1,   // waiting for 0x98
      SYNC2,   // waiting for 0x98 (second)
      SYNC3,   // waiting for 0x00/0x03
      SYNC4,   // waiting for 0x00/0x03 (second)
      HEIGHT1,
      HEIGHT2,
    } state_ = SYNC1;
    uint8_t buf_[2];

  public:
    AutonomousDecoder() { }
    ~AutonomousDecoder() { }

    bool put(uint8_t b);
    float decode();
};

}
}