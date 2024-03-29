#pragma once

#include <stdint.h>
#include "decoder.h"

namespace esphome {
namespace standing_desk_height {

class CTBDecoder : public Decoder {
  protected:
    enum state_t {
      SYNC1,
      SYNC2,
      HEIGHT1,
      HEIGHT2,
      HEIGHT3,
    } state_ = SYNC1;
    uint8_t buf_[3];

  public:
    CTBDecoder() { }
    ~CTBDecoder() { }

    bool put(uint8_t b);
    float decode();
};

}
}
