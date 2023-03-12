#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

#include "decoder.h"
#include "jarvis_decoder.h"
#include "uplift_decoder.h"
#include "omnidesk_decoder.h"

#include "decoder_variant.h"

namespace esphome {
namespace standing_desk_height {

class StandingDeskHeightSensor : public PollingComponent, public uart::UARTDevice, public sensor::Sensor
{
public:
  void set_decoder_variant(DecoderVariant decoder_variant);
  void start_decoder_detection();

  void loop() override;
  void update() override;
  float get_last_read();

protected:
  Decoder* decoder;
  DecoderVariant decoder_variant = DECODER_VARIANT_UNKNOWN;

  float last_read = -1;
  float last_published = -1;

  bool is_detecting = false;
  uint32_t started_detecting_at = 0;

  void try_next_decoder();
};

}
}
