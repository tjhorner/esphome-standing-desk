#pragma once

#include <stdint.h>
#include "decoder.h"

namespace esphome {
namespace standing_desk_height {

class UpliftDecoder : public Decoder {
  private:
    enum state_t {
      SYNC1,   // waiting for 0x01
      SYNC2,   // waiting for 0x01 (second)
      HEIGHT1,  // waiting for first height arg, usually 0x01 but 0x00 if at the bottom
      HEIGHT2,  // waiting for second height arg
    } state_ = SYNC1;
    uint8_t buf_[2];

  public:
    UpliftDecoder() { }
    ~UpliftDecoder() { }

    bool put(uint8_t b);
    float decode();
};

}
}