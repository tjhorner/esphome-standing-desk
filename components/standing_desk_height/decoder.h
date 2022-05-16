#pragma once

#include <stdint.h>

namespace esphome {
namespace standing_desk_height {

class Decoder {
  public:
    Decoder() { }
    virtual ~Decoder() { }

    virtual bool put(uint8_t b) = 0;
    virtual float decode() = 0;
};

}
}