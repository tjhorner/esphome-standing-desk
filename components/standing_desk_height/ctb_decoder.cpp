#include "ctb_decoder.h"

namespace esphome {
namespace standing_desk_height {

bool CTBDecoder::put(uint8_t b) {
  switch (state_) {
  case SYNC1:
    if (b == 0xAA) {
      state_ = SYNC2;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case SYNC2:
    if (b == 0xFF) {
      state_ = HEIGHT1;
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
    state_ = HEIGHT3;
    return false;
  case HEIGHT3:
    buf_[2] = b;
    state_ = SYNC1;
    return true;
  default:
    return false;
  }
  return false;
}

float CTBDecoder::decode() {
  bool shift_decimal_place = false;
  float height = 0.0;

  height += buf_[2] * 100;

  if (buf_[1] >= 10) {
    shift_decimal_place = true;
    height += (buf_[1] - 10) * 10;
  } else {
    height += buf_[1] * 10;
  }

  height += buf_[0];

  if (shift_decimal_place) {
    height /= 10.0;
  }

  return height;
}

}
}