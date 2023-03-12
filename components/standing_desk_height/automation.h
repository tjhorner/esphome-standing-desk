#pragma once

#include "standing_desk_height.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace standing_desk_height {

template<typename... Ts> class DetectDecoderAction : public Action<Ts...> {
 public:
  explicit DetectDecoderAction(StandingDeskHeightSensor *a_sdh) : sdh_(a_sdh) {}

  void play(Ts... x) override { this->sdh_->start_decoder_detection(); }

 protected:
  StandingDeskHeightSensor *sdh_;
};

}
}