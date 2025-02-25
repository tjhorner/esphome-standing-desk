#include "desk_decoder.h"

namespace esphome {
namespace standing_desk_height {

bool DeskDecoder::put(uint8_t b) {
  switch (state_) {
    case State::WAIT_START:
      if (b == 0x5A) {
        state_ = State::BYTE1;
        checksum_ = 0;  // reset checksum accumulator
      }
      break;

    case State::BYTE1:
      buf_[0] = b;
      checksum_ = b;
      state_ = State::BYTE2;
      break;

    case State::BYTE2:
      buf_[1] = b;
      checksum_ += b;
      state_ = State::BYTE3;
      break;

    case State::BYTE3:
      buf_[2] = b;
      checksum_ += b;
      state_ = State::BYTE4;
      break;

    case State::BYTE4:
      buf_[3] = b;
      checksum_ += b;
      state_ = State::CHECKSUM;
      break;

    case State::CHECKSUM:
      // Checksum is (byte1+byte2+byte3+byte4) & 0xff, compare to received b.
      if ((checksum_ & 0xff) == b) {
        // Valid frame received. Prepare for next frame.
        state_ = State::WAIT_START;
        return true;
      } else {
        // Checksum error. Reset state.
        state_ = State::WAIT_START;
      }
      break;
  }
  return false;
}

int DeskDecoder::decode_digit(uint8_t b) {
  // Mask out the high bit (which may indicate the decimal point)
  uint8_t val = b & 0x7F;
  // We use the canonical 7-seg values:
  // 0: 0x3F, 1: 0x06, 2: 0x5B, 3: 0x4F, 4: 0x66,
  // 5: 0x6D, 6: 0x7D, 7: 0x07, 8: 0x7F, 9: 0x6F.
  switch (val) {
    case 0x3F: return 0;
    case 0x06: return 1;
    case 0x5B: return 2;
    case 0x4F: return 3;
    case 0x66: return 4;
    case 0x6D: return 5;
    case 0x7D: return 6;
    case 0x07: return 7;
    case 0x7F: return 8;
    case 0x6F: return 9;
    default:   return -1;  // unknown digit
  }
}

float DeskDecoder::decode() {
  // Decode the three display digits from buf_[0] (first), buf_[1] (second), buf_[2] (third)
  int d1 = decode_digit(buf_[0]);
  int d2 = decode_digit(buf_[1]);
  int d3 = decode_digit(buf_[2]);

  // If any digit is unknown, return an error value (could also throw an exception)
  if (d1 < 0 || d2 < 0 || d3 < 0) {
    return -1.0f;
  }

  // Check if the high bit of the second byte is set.
  // If so, the decimal point is shown (per protocol, e.g. 0xBF instead of 0x3F for digit 0)
  bool has_decimal = (buf_[1] & 0x80) != 0;

  int value;
  float height;
  if (has_decimal) {
    // E.g. digits 7,6,5 -> "76.5"  (first two digits form the integer part)
    value = d1 * 10 + d2;
    height = value + d3 / 10.0f;
  } else {
    // Without a decimal point the number is read as a three-digit integer (e.g. 101 means 101.0)
    value = d1 * 100 + d2 * 10 + d3;
    height = static_cast<float>(value);
  }

  // (buf_[3] is the display mode indicator; here we ignore it, but it could be used for additional logic.)

  return height;
}

void DeskDecoder::reset() {
  state_ = State::WAIT_START;
  checksum_ = 0;
}

}  // namespace standing_desk_height
}  // namespace esphome
