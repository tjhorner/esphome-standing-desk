#pragma once

#include <stdint.h>
#include <esphome/core/hal.h>
#include <esphome/components/output/binary_output.h>

#include "controller.h"

namespace esphome {
namespace standing_desk_control {

class ParallelController : public Controller {
  public:
    void set_up(output::BinaryOutput *up) { up_ = up; }
    void set_down(output::BinaryOutput *down) { down_ = down; }

    void set_bit_1(output::BinaryOutput *bit_1) { bit_1_ = bit_1; }
    void set_bit_2(output::BinaryOutput *bit_2) { bit_2_ = bit_2; }
    void set_bit_4(output::BinaryOutput *bit_4) { bit_4_ = bit_4; }
    void set_bit_m(output::BinaryOutput *bit_m) { bit_m_ = bit_m; }

    void setup();
    void loop();

    void press_up();
    void press_down();
    void stop();

    void recall_preset(uint8_t preset);
    void set_preset(uint8_t preset);

  protected:
    output::BinaryOutput *up_{nullptr};
    output::BinaryOutput *down_{nullptr};

    output::BinaryOutput *bit_1_{nullptr};
    output::BinaryOutput *bit_2_{nullptr};
    output::BinaryOutput *bit_4_{nullptr};
    output::BinaryOutput *bit_m_{nullptr};

    void press_briefly(output::BinaryOutput *output_1, output::BinaryOutput *output_2);
};

}
}
