#include "esphome/components/button/button.h"
#include "../standing_desk_control.h"

namespace esphome {
namespace standing_desk_control {

class SetPresetButton : public button::Button, public Parented<StandingDeskControlComponent> {
public:
  void set_preset_number(uint8_t preset) { preset_number_ = preset; }

protected:
  uint8_t preset_number_{0};
  void press_action() { if (this->parent_ != nullptr) this->parent_->controller_->set_preset(preset_number_); };
};

}
}