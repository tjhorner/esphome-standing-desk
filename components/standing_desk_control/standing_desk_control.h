#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

#include "controller.h"

namespace esphome {
namespace standing_desk_control {

class StandingDeskControlComponent : public Component
{
public:
  Controller *controller_{nullptr};
  void set_controller(Controller *controller) { controller_ = controller; }
};

}
}
