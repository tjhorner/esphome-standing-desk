#pragma once

#include <stdint.h>
#include "decoder.h"

namespace esphome {
namespace standing_desk_height {

class FlexiSpotE7Decoder : public Decoder {
  protected:
    enum state_t {
      START,    // waiting for 0x9b (packet start)
      LENGTH,   // waiting for 0x07 (height packet length)
      TYPE,     // waiting for 0x12 (packet type - height)
      HEIGHT1,  // waiting for first height digit (Tens)
      HEIGHT2,  // waiting for second height digit (ONES)
      HEIGHT3,  // waiting for third height digit (.TENTHS)
      CRC1,     // waiting for first CRC value
      CRC2,     // waiting for second CRC value
      END,      // waiting for 0x9D (packet end)
    } state_ = START;
    float buf_[2];

  public:
    FlexiSpotE7Decoder() { }
    ~FlexiSpotE7Decoder() { }

    bool put(uint8_t b);
    float decode();
};

}
}