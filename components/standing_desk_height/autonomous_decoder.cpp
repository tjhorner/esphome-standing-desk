#include "autonomous_decoder.h"

namespace esphome {
namespace standing_desk_height {

bool AutonomousDecoder::put(uint8_t b) {
  switch (state_) {
  case SYNC1:
  case SYNC2:
    if (b == 0x98) {
      state_ = static_cast<state_t>(state_ + 1);
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case SYNC3:
  case SYNC4:
    if (b == 0x00 || b == 0x03) {
      state_ = static_cast<state_t>(state_ + 1);
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case HEIGHT1:
    buf_[0] = b;
    state_ = HEIGHT2;
    return false;
  case HEIGHT2:
    buf_[1] = b;
    state_ = SYNC1;
    return buf_[0] == buf_[1];
  default:
    return false;
  }
  return false;
}

float AutonomousDecoder::decode() {
  return buf_[0] / 1.0;
}

}
}