#pragma once

#include <stdint.h>
#include <esphome/core/hal.h>
#include <esphome/components/uart/uart.h>

#include "controller.h"

namespace esphome {
namespace standing_desk_control {

class SerialController : public Controller, public uart::UARTDevice {
  public:
    void setup();
    void loop();

    void press_up();
    void press_down();
    void stop();

    void recall_preset(uint8_t preset);
    void set_preset(uint8_t preset);

  protected:
    bool forward_packets_{true};

    void inject_packets(const uint8_t *packets, size_t size);
};

}
}
