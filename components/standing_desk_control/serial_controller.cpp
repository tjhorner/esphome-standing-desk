#include "serial_controller.h"

namespace esphome {
namespace standing_desk_control {

void SerialController::loop() {
  uint8_t b;
  while (this->forward_packets_ && this->available()) {
    this->read_byte(&b);
    this->write_byte(b);
  }
}

void SerialController::recall_preset(uint8_t preset) {
  switch (preset) {
    case 1:
      this->inject_packets(new uint8_t[6]{0xAA, 0xFF, 0x00, 0x48, 0x2A, 0x3E}, 6);
      break;
    default:
      break;
  }
}

void SerialController::inject_packets(const uint8_t *const packets, size_t size) {
  this->forward_packets_ = false;
  this->write_array(packets, size);
  this->forward_packets_ = true;
}

}
}