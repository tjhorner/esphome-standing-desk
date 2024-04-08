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
    if (b == 0x00 || b == 0x01 || b == 0x02 || b == 0x03 || b == 0x04 || b == 0x05 || b == 0x06 || b == 0x07 || b == 0x08 || b == 0x09) {
      buf_[0] = b;
      state_ = HEIGHT2;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }
  case HEIGHT2:
    if (b == 0x0A || b == 0x0B || b == 0x0C || b == 0x0D || b == 0x0E || b == 0x0F || b == 0x10 || b == 0x11 || b == 0x12 || b == 0x13 || b == 0x00 || b == 0x01 || b == 0x02) {
      buf_[1] = b;
      state_ = HEIGHT3;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }  
  case HEIGHT3:
    if (b == 0x01 || b == 0x02 || b == 0x03 || b == 0x04 || b == 0x06 || b == 0x07 || b == 0x08 || b == 0x09) {
      buf_[2] = b;
      state_ = CRC1;
      return false;
    } else {
      state_ = SYNC1;
      return false;
    }    
  case CRC1:
    buf_[3] = b;
    state_ = CRC2;
    return false;
  case CRC2:
    buf_[4] = b;
    state_ = SYNC1;
    return this->verify_checksum();
  default:
    return false;
  }
  return false;
}

bool CTBDecoder::verify_checksum() {
  uint8_t message[5] = {0xAA, 0xFF, buf_[0], buf_[1], buf_[2]};
  uint16_t crc = CRC::Calculate(message, sizeof(message), crc_parameters);
  return (buf_[3] == (crc & 0xFF)) && (buf_[4] == (crc >> 8));
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
