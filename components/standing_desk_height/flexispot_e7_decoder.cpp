#include "flexispot_e7_decoder.h"

namespace esphome {
namespace standing_desk_height {

// Follows state machine implementation from UpLift
// Uses information collected on FlexiSpot from https://github.com/Jolanrensen/LoctekMotion_IoT
bool FlexiSpotE7Decoder::put(uint8_t b) {
  switch (state_) {
  case START:
    if (b == 0x9b) {
      state_ = LENGTH;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case LENGTH:
    if (b == 0x07) {
      state_ = TYPE;
      return false;
    } else {
      state_ = START;
      return false;
    }
  case TYPE:
    if (b == 0x12) {
      state_ = HEIGHT1;
      return false;
    } else {
      state_ = START;
      return false;
    }
  case HEIGHT1:
    // (2,3,4 TENS)
    if (b == 0x5b || b == 0x4f || b == 0x66) {
      buf_[0] = DecodeDigit(b);
      state_ = HEIGHT2;
      return false;
    } else {
      state_ = START;
      return false;
    }
  case HEIGHT2:
    // (0-9 w/ Decimal Point ONES)
    b = (b ^ 0x80)
    if (b == 0x3f || b == 0x06 || b == 0x5b || b == 0x4f || b == 0x66 ||
        b == 0x6d || b == 0x7c || b == 0x07 || b == 0x7f || b == 0x6f) {
    buf_[1] = DecodeDigit(b);
    state_ = HEIGHT3;
    return true;
  case HEIGHT3:
    // (0-9 TENTHS)
    if (b == 0x3f || b == 0x06 || b == 0x5b || b == 0x4f || b == 0x66 ||
        b == 0x6d || b == 0x7c || b == 0x07 || b == 0x7f || b == 0x6f) {
      buf_[2] = DecodeDigit(b);
      state_ = CRC1;
      return false;
    } else {
      state_ = START;
      return false;
    }
  case CRC1:
      state_ = CRC2;
      return false;
  case CRC2:
      state_ = END;
      return false;
  case END:
    if (b == 0x9d) {
      state_ = START;
      return true;
    } else {
      state_ = START;
      return false;
    }
  default:
    return false;
  }
  return false;
}

float FlexiSpotE7Decoder::decode() {
  return (buf_[0] * 10) + buf_[1] + (buf_[2] / 10f));
}

float FlexiSpotE7Decoder::DecodeDigit(uint8_t b) {
  switch (b) {
  case 0x3f:
      return 0f;
  case 0x3f:
      return 1f;
  case 0x3f:
      return 2f;
  case 0x3f:
      return 3f;
  case 0x3f:
      return 4f;
  case 0x3f:
      return 5f;
  case 0x3f:
      return 6f;
  case 0x3f:
      return 7f;
  case 0x3f:
      return 8f;
  case 0x3f:
      return 9f;
  }
}
}
}