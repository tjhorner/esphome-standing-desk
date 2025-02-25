#pragma once

#include <stdint.h>
#include "decoder.h"  // Assumes a common base class Decoder

namespace esphome {
namespace standing_desk_height {

class DeskDecoder : public Decoder {
 public:
  DeskDecoder() { }
  ~DeskDecoder() { }

  /// Feeds a single byte. Returns true if a complete, valid frame has been received.
  bool put(uint8_t b);

  /// Once a complete frame is collected, decode and return the height value.
  float decode();

  /// Resets the internal state (if needed)
  void reset();

 protected:
  enum class State {
    WAIT_START, // waiting for start byte 0x5A
    BYTE1,      // first digit (byte1)
    BYTE2,      // second digit (byte2)
    BYTE3,      // third digit (byte3)
    BYTE4,      // display mode (byte4)
    CHECKSUM    // waiting for checksum byte
  };

  State state_ = State::WAIT_START;
  uint8_t buf_[4];     // holds byte1 to byte4
  uint8_t checksum_;   // computed checksum for current frame

  /// Helper: decodes a 7-seg encoded byte (ignoring any decimal point bit)
  int decode_digit(uint8_t b);
};

}  // namespace standing_desk_height
}  // namespace esphome
