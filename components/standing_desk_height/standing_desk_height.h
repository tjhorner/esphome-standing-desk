#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

#include "decoder.h"
#include "jarvis_decoder.h"
#include "uplift_decoder.h"
#include "omnidesk_decoder.h"

namespace esphome {
namespace standing_desk_height {

class StandingDeskHeightSensor : public sensor::Sensor, public PollingComponent, public uart::UARTDevice
{
public:
  void set_decoder_variant(const std::string &decoder_variant);

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
