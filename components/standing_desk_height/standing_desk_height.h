#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace standing_desk_height {

class StandingDeskHeightSensor : public PollingComponent, public uart::UARTDevice, public sensor::Sensor
{
public:
  void loop() override;
  void update() override;

  float get_last_read();

protected:
  unsigned long first_byte_received{0};
  size_t data_pos{0};
  uint8_t data[4];
  float last_read = -1;
  float last_published = -1;
};

}
}