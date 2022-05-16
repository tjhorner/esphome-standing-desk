#include "standing_desk_height.h"

namespace esphome {
namespace standing_desk_height {

static const char *const TAG = "standing_desk_height";

void StandingDeskHeightSensor::loop() {
  while (this->available() > 0)
  {
    uint8_t byte;
    this->read_byte(&byte);

    // Using a timeout to determine groups of bytes -- all 4 should be sent
    // well within 8ms. Anything outside of that is a different number of bytes
    if(this->data_pos != 0 && millis() > this->first_byte_received + 8) {
      ESP_LOGVV(TAG, "Took too long to receive all 4 bytes; resetting position");
      this->data_pos = 0;
    }

    if (this->data_pos == 0) {
      ESP_LOGVV(TAG, "First byte received %d", millis());
      this->first_byte_received = millis();
    }

    this->data[this->data_pos] = byte;
    ESP_LOGVV(TAG, "%d %d %02X", millis(), this->data_pos, byte);

    if (this->data_pos == 3) {
      ESP_LOGVV(TAG, "got all da bytes %02X %02X %02X %02X", data[0], data[1], data[2], data[3]);

      // rudimentary checking to make sure we have the correct group of bytes
      if (data[0] == 1 && data[1] == 1 && data[2] <= 1)
        this->last_read = ((data[2] << 8) | (data[3] & 0xFF)) / 10.0;
      
      this->data_pos = 0;
    } else {
      this->data_pos++;
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