#pragma once

#include <stdint.h>
#include "crc.h"
#include "decoder.h"

namespace esphome {
namespace standing_desk_height {

class CTBDecoder : public Decoder {
  protected:
    const CRC::Parameters<crcpp_uint16, 16> crc_parameters = { 0x6005, 0xFFFF, 0x0000, true, true };

    enum state_t {
      SYNC1,
      SYNC2,
      HEIGHT1,
      HEIGHT2,
      HEIGHT3,
      CRC1,
      CRC2,
    } state_ = SYNC1;
    uint8_t buf_[5];

    bool verify_checksum();

  public:
    CTBDecoder() { }
    ~CTBDecoder() { }

    bool put(uint8_t b);
    float decode();
};

}
}
