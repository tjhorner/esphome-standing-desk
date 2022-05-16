#include "standing_desk_height.h"

namespace esphome {
namespace standing_desk_height {

static const char *const TAG = "standing_desk_height";

void StandingDeskHeightSensor::loop() {
  while (this->available() > 0)
  {
    uint8_t byte;
    this->read_byte(&byte);

    if (this->deskPacket.put(byte)) {
      float height = this->deskPacket.decode();
      ESP_LOGD(TAG, "Got desk packet: %f", height);
      this->last_read = height;
    }
  }
}

void StandingDeskHeightSensor::update() {
  if (this->last_read != this->last_published) {
    publish_state(this->last_read);
    this->last_published = this->last_read;
  }
}

float StandingDeskHeightSensor::get_last_read() {
  return this->last_read;
}

}
}