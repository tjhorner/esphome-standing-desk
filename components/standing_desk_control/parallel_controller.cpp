#include "parallel_controller.h"

namespace esphome {
namespace standing_desk_control {

void ParallelController::setup() { }
void ParallelController::loop() { }

void ParallelController::press_up() {
  if (up_ != nullptr) {
    up_->turn_on();
  }
}

void ParallelController::press_down() {
  if (down_ != nullptr) {
    down_->turn_on();
  }
}

void ParallelController::stop() {
  if (up_ != nullptr) {
    up_->turn_off();
  }
  if (down_ != nullptr) {
    down_->turn_off();
  }
}

void ParallelController::recall_preset(uint8_t preset) {
  switch (preset) {
    case 1:
      press_briefly(bit_1_, bit_2_);
      break;
    case 2:
      press_briefly(bit_4_, nullptr);
      break;
    case 3:
      press_briefly(bit_2_, bit_4_);
      break;
    case 4:
      press_briefly(bit_4_, bit_1_);
      break;
    default:
      break;
  }
}

void ParallelController::set_preset(uint8_t preset) {
  press_briefly(bit_m_, nullptr);
  recall_preset(preset);
}

void ParallelController::press_briefly(output::BinaryOutput *output_1, output::BinaryOutput *output_2) {
  if (output_1 != nullptr) {
    output_1->turn_on();
  }
  if (output_2 != nullptr) {
    output_2->turn_on();
  }

  delay(50);

  if (output_2 != nullptr) {
    output_2->turn_off();
  }
  if (output_1 != nullptr) {
    output_1->turn_off();
  }
}

}
}