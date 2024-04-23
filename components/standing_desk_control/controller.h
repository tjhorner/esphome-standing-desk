#pragma once

#include <stdint.h>

namespace esphome {
namespace standing_desk_control {

class Controller {
  public:
    Controller() { }
    virtual ~Controller() { }

    virtual void setup() = 0;
    virtual void loop() = 0;

    virtual void press_up() = 0;
    virtual void press_down() = 0;
    virtual void stop() = 0;

    virtual void recall_preset(uint8_t preset) = 0;
    virtual void set_preset(uint8_t preset) = 0;
};

}
}