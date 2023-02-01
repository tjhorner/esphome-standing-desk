#include "omnidesk_decoder.h"

namespace esphome {
namespace standing_desk_height {

bool OmnideskDecoder::put(uint8_t b) {
  switch (state_) {
  case HEIGHT1:
    if (b == 0x02 || b == 0x03 || b == 0x04) {
      buf_[0] = b;
      state_ = HEIGHT2;
      return false;
    }
  default:
    return UpliftDecoder::put(b);
  }
}

}
}
