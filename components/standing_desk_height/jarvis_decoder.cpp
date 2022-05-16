#include <string.h>
#include "jarvis_decoder.h"

namespace esphome {
namespace standing_desk_height {

#define JARVIS_ADDR 0xF2

void JarvisDecoder::reset(uint8_t ch) {
  state = static_cast<state_t>(SYNC + (ch == JARVIS_ADDR));
  cmd = NONE;
  argc = 0;
  memset(argv, 0U, sizeof(argv));
}

bool JarvisDecoder::put(uint8_t ch) {
  bool complete = false;

  switch (state) {
  case SYNC:
  case SYNC2:
    if (ch != JARVIS_ADDR) {
      reset(ch);
      return false;
    }
    break;

  case CMD:
    cmd = static_cast<command_byte>(checksum = ch);
    break;

  case LENGTH:
    if (ch > sizeof(argv)) {
      reset(ch);
      return false;
    }
    checksum += (argc = ch);
    state = static_cast<state_t>(CHKSUM - ch - 1);
    break;

  default:    // ARGS
    if (state <= LENGTH || state > ARGS) {
      reset(ch);
      return false;
    }
    checksum += (argv[argc - (CHKSUM-state)] = ch);
    break;

  case CHKSUM:
    if (ch != checksum) {
      reset(ch);
      return false;
    }
    complete = true;
    break;

  case ENDMSG:
    reset(ch);
    return false;
  }
  // Common increment for every state
  state = static_cast<state_t>(state + 1);
  if (state < SYNC || state > ENDMSG) {
    reset(ch);
    return false;
  }
  return complete && cmd == HEIGHT;
}

float JarvisDecoder::decode() {
  if (cmd != HEIGHT) return 0.0;
  return ((argv[0] << 8) | (argv[1] & 0xFF)) / 10.0;
}

}
}