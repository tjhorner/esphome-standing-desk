#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

#include "decoders/decoder.h"
#include "decoders/jarvis_decoder.h"

namespace esphome {
namespace standing_desk_height {

class StandingDeskHeightSensor : public PollingComponent, public uart::UARTDevice, public sensor::Sensor
{
public:
  StandingDeskHeightSensor() {
    this->decoder = new JarvisDecoder();
  }

  void loop() override;
  void update() override;
  float get_last_read();

protected:
  Decoder* decoder;
  float last_read = -1;
  float last_published = -1;
};

}
}