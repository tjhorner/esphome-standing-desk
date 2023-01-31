#pragma once

#include <stdint.h>
#include "uplift_decoder.h"

namespace esphome {
namespace standing_desk_height {

class OmnideskDecoder : public UpliftDecoder {
  public:
    OmnideskDecoder() { }
    ~OmnideskDecoder() { }

    bool put(uint8_t b);
};

}
}
