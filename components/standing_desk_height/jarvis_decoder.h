#pragma once

#include <stdint.h>
#include "decoder.h"

namespace esphome {
namespace standing_desk_height {

class JarvisDecoder : public Decoder {
  private:
    enum command_byte {
      // FAKE
      NONE        = 0x00,  // Unused/never seen; used as default for "Uninitialized"

      // CONTROLLER
      HEIGHT      = 0x01,  // Height report; P0=4 (mm?)
      LIMIT_RESP  = 0x20,  // Max-height set/cleared; response to [21];
      REP_MAX     = 0x21,  // Report max height; Response to SET_MAX
      REP_MIN     = 0x22,  // Report min height; Response to SET_MIN
      LIMIT_STOP  = 0x23,  // Min/Max reached
      RESET       = 0x40,  // Indicates desk in RESET mode; Displays "RESET"
      REP_PRESET  = 0x92,  // Moving to Preset location

      // HANDSET
      UNITS       = 0x0E,  // Set units to cm/inches
      MEM_MODE    = 0x19,  // Set memory mode
      COLL_SENS   = 0x1D,  // Set anti-collision sensitivity  (Sent 1x; no repeats)
      SET_MAX     = 0x21,  // Set max height; Sets max-height to current height
      SET_MIN     = 0x22,  // Set min height; Sets min-height to current height
      LIMIT_CLR   = 0x23,  // Clear min/max height
      PROGMEM_1   = 0x03,  // Set memory position 1 to current height
      PROGMEM_2   = 0x04,  // Set memory position 2 to current height
      PROGMEM_3   = 0x25,  // Set memory position 3 to current height
      PROGMEM_4   = 0x26,  // Set memory position 4 to current height
      WAKE        = 0x29,  // Poll message (??) sent when desk doesn't respond to BREAK messages
      CALIBRATE   = 0x91,  // Height calibration (Repeats 2x)
    };

    command_byte cmd = NONE;
    uint8_t checksum = 99;
    uint8_t argc = 0;
    uint8_t argv[5];

    enum state_t {
      SYNC,    // waiting for addr
      SYNC2,   // waiting for addr2
      CMD,     // waiting for cmd
      LENGTH,  // waiting for argc
      // ARGS4,3,2,1   // collecting args
      ARGS = LENGTH + sizeof(argv),   // collecting args
      CHKSUM,  // waiting for checksum
      ENDMSG,  // waiting for EOM
    } state = SYNC;

    void reset(uint8_t ch);

  public:
    JarvisDecoder() { }
    ~JarvisDecoder() { }

    bool put(uint8_t b);
    float decode();
};

}
}