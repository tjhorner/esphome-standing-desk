#include "standing_desk_height.h"

namespace esphome {
namespace standing_desk_height {

static const char *const TAG = "standing_desk_height";

void StandingDeskHeightSensor::set_decoder_variant(const std::string &decoder_variant) {
  if (decoder_variant == "jarvis") {
    this->decoder = new JarvisDecoder();
  } else if (decoder_variant == "uplift") {
    this->decoder = new UpliftDecoder();
  } else if (decoder_variant == "autonomous") {
    this->decoder = new AutonomousDecoder();
  } else {
    ESP_LOGE(TAG, "Unknown decoder variant '%s'", decoder_variant.c_str());
    this->decoder = nullptr;
  }
}

void StandingDeskHeightSensor::loop() {
  while (this->available() > 0)
  {
    uint8_t byte;
    this->read_byte(&byte);

    ESP_LOGVV(TAG, "Reading byte: %d", byte);

    if (this->decoder->put(byte)) {
      float height = this->decoder->decode();
      ESP_LOGVV(TAG, "Got desk height: %f", height);
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